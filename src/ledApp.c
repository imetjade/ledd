// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledApp.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "log.h"

#define LED_POLL_TIMEOUT_MS (2000)

static ledApp_handle_t ledApp_handle;

static void signalHandler_setup(void);

static const char *_getEnvDefault(char const *key, char const *def)
{
  const char *res = getenv(key);

  return res ? res : def;
}

ledApp_handle_t *ledApp_getHandle(void)
{
  return &ledApp_handle;
}

static void ledApp_init(void)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();

  signalHandler_setup();

  ledApp->exitCode = 0;
#if defined(LED_PLATFORM_CALIX)
#include "ledPlatformCalix.h"
  ledPlatformCalix_init(&ledApp->platform);
#elif defined(LED_PLATFORM_LINKV2)
#include "ledPlatformGenericLinkV2.h"
  ledPlatformGenericLinkV2_init(&ledApp->platform);
#elif defined(LED_PLATFORM_SAMSUNG_TV)
#include "ledPlatformSamsungTV.h"
  ledPlatformSamsungTV_init(&ledApp->platform);
#endif
}

static bool ledApp_canIgnoreExitSignal(void)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();

  return ledPlatform_canIgnoreExitSignal(&ledApp->platform);
}

static void handleExitSignal(int sig, siginfo_t *siginfo, void *ignore)
{
  LOG_DEBUG("ExitSignalHandler: sig: %d: code: %d", sig, siginfo->si_code);
  if (ledApp_canIgnoreExitSignal())
  {
    LOG_INFO("Resetting, do not terminate");
    return;
  }
  ledApp_close(sig);
  exit(1);
}

static void signalHandler_setup(void)
{
  struct sigaction act;
  act.sa_flags = SA_RESTART | SA_SIGINFO;
  act.sa_sigaction = handleExitSignal;
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGQUIT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);
  sigaction(SIGTSTP, &act, NULL);
}

static void _onLedSignalFDChanged(int fd, void *data)
{
  uint8_t ledSignal;
  int ret = read(fd, &ledSignal, sizeof ledSignal);
  if (ret == 0)
  {
    LOG_ERROR("ledSignalIPC file is EOF. Terminate.");
    ledApp_close(0);
  }
  else if (ret != sizeof ledSignal)
  {
    LOG_ERROR(
      "Failed to read ledSignal from ledSignalIPC file. errno %d, ret %d",
      errno, ret);
    return;
  }

  ledApp_handle_t *ledApp = data;
  ledPlatform_setLedSignal(&ledApp->platform, ledSignal);
}

static void _onHubStatusFDChanged(int fd, void *data)
{
  uint8_t hubStatus;
  int ret = read(fd, &hubStatus, sizeof hubStatus);
  if (ret == 0)
  {
    LOG_ERROR("hubStatusIPC file is EOF. Terminate.");
    ledApp_close(0);
  }
  else if (ret != sizeof hubStatus)
  {
    LOG_ERROR(
      "Failed to read hubStatus from hubStatusIPC file. errno %d, ret %d",
      errno, ret);
    return;
  }

  ledApp_handle_t *ledApp = data;
  ledPlatform_setHubStatus(&ledApp->platform, hubStatus);
}

static bool ledApp_setup(void)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();
  char filePath[512];

  snprintf(filePath, sizeof filePath, "%s/%s",
           _getEnvDefault("HUB_TMP_PATH", "/tmp"), LED_STATE_FIFO_NAME);
  if (!ledFileIPC_setup(&ledApp->ledSignalIPC, filePath))
  {
    return false;
  }

  snprintf(filePath, sizeof filePath, "%s/%s",
           _getEnvDefault("HUB_TMP_PATH", "/tmp"), LED_HUB_STATUS_FIFO_NAME);
  if (!ledFileIPC_setup(&ledApp->hubStatusIPC, filePath))
  {
    return false;
  }

  ledPoll_init(&ledApp->poll);
  if (!ledPoll_setup(&ledApp->poll))
  {
    return false;
  }
  if (!ledPoll_addFD(&ledApp->poll, ledApp->ledSignalIPC.fd,
                     _onLedSignalFDChanged, ledApp))
  {
    LOG_ERROR("Failed to add LED Signal IPC fd to poll.");
    return false;
  }
  if (!ledPoll_addFD(&ledApp->poll, ledApp->hubStatusIPC.fd,
                     _onHubStatusFDChanged, ledApp))
  {
    LOG_ERROR("Failed to add Hub Status IPC fd to poll.");
    return false;
  }

  if (!ledPlatform_setup(&ledApp->platform))
  {
    return false;
  }

  return true;
}

static void ledApp_run(void)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();

  ledPoll_run(&ledApp->poll);
}

void ledApp_close(int exitCode)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();

  ledPoll_quit(&ledApp->poll);
  ledApp->exitCode = exitCode;
}

static void ledApp_destroy(void)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();

  ledPoll_destroy(&ledApp->poll);
  ledPlatform_destroy(&ledApp->platform);
}

int main(int argc, char **argv)
{
  ledApp_handle_t *ledApp = ledApp_getHandle();

  ledApp_init();
  if (!ledApp_setup())
  {
    ledApp_destroy();
    return 1;
  }
  ledApp_run();
  ledApp_destroy();

  return ledApp->exitCode;
}
