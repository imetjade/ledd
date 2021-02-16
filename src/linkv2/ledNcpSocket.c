// Copyright (c) 2020, SmartThings. All Rights Reserved.

#include "ledNcpSocket.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "log.h"

#define NCP_SOCKET_FILENAME "zigbeeipc.socket"
#define MAX_CONNECT_RETRIES 10

static const char *_getEnvDefault(char const *key, char const *def)
{
  const char *res = getenv(key);

  return res ? res : def;
}

bool ledNcpSocket_setup(ledNcpSocket_t *ncpSocket)
{
  struct sockaddr_un sockAddr;
  int retries = 0, res = -1;

  ncpSocket->fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (ncpSocket->fd < 0)
  {
    LOG_ERROR("Socket creation failed %d", errno);
    return false;
  }

  memset(&sockAddr, 0, sizeof(sockAddr));
  sockAddr.sun_family = AF_UNIX;
  char ncpSocketPath[108];
  snprintf(ncpSocketPath, sizeof ncpSocketPath, "%s/%s",
           _getEnvDefault("HUB_TMP_PATH", "/tmp"), NCP_SOCKET_FILENAME);
  strncpy(sockAddr.sun_path, ncpSocketPath, sizeof(sockAddr.sun_path) - 1);

  while (res != 0 && retries < MAX_CONNECT_RETRIES)
  {
    res =
      connect(ncpSocket->fd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
    if (res == 0)
    {
      LOG_INFO("Socket connected");
      return true;
    }

    // Prevent ledd from overloading the CPU
    sleep(1);
    retries++;
  }

  LOG_ERROR("Socket connect failed due to %d", errno);
  close(ncpSocket->fd);
  ncpSocket->fd = INVALID_SOCKET;
  return false;
}

void ledNcpSocket_destroy(ledNcpSocket_t *ncpSocket)
{
  if (ncpSocket->fd == INVALID_SOCKET)
  {
    return;
  }
  close(ncpSocket->fd);
  ncpSocket->fd = INVALID_SOCKET;
}

int32_t ledNcpSocket_write(ledNcpSocket_t *ncpSocket, uint8_t *pBuf, size_t nBuf)
{
  return write(ncpSocket->fd, pBuf, nBuf);
}

int32_t ledNcpSocket_writeWithRetry(ledNcpSocket_t *ncpSocket,
                                    uint8_t *pBuf,
                                    uint32_t nBuf,
                                    int const maxRetry)
{
  int32_t ret = -1, retries = 0;

  /*
   * If initially ncp-host socket isn't connected the file descriptor
   * would be set to -1. Or in other words if the Socket setup has
   * failed during initialization or during previous write the socket
   * fd will be -1. So in this case if we try to write we will be getting
   * EBADF (Bad file descriptor). Therefore we need to first check if
   * socket connection is proper or not. If it is not proper we would
   * try to reestablish connection with ncp-host socket and increase
   * the count of retry by 1. Each setup call will take about 10 seconds
   * to establish the connection with ncp-host. So the total time in worst
   * case can be upto 100 second wait here.
   *
   * If write fails for any other error apart from EPIPE we wouldn't retry
   * but we would return. In case of EPIPE the current session will be destroyed
   * and LED module will try to re-establish the socket connection again.
   */

  while (ret < 0 && retries < maxRetry)
  {
    if (ncpSocket->fd == -1)
    {
      ledNcpSocket_setup(ncpSocket);
    }
   else
   {
      ret = ledNcpSocket_write(ncpSocket, pBuf, nBuf);
      if (ret > 0)
      {
        LOG_INFO("Socket write was successful");
        return ret;
      }
      if (errno != EAGAIN && errno != EWOULDBLOCK)
      {
        if (errno != EPIPE)
        {
          LOG_ERROR("Error encountered while trying to write LED status %d",
                    errno);
          return ret;
        }
        ledNcpSocket_destroy(ncpSocket);
        ledNcpSocket_setup(ncpSocket);
      }
   }

    sleep(1);
    retries++;
  }

  if (ret < 0)
  {
    LOG_ERROR("Retries %d", retries);
  }
  return ret;
}

int32_t ledNcpSocket_read(ledNcpSocket_t *ncpSocket, uint8_t *pBuf, size_t nBuf)
{
  return recv(ncpSocket->fd, pBuf, nBuf, 0);
}
