// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledPoll.h"

#include <errno.h>
#include <glib-unix.h>

#include "log.h"

void ledPoll_init(ledPoll_t *ledPoll)
{
  memset(ledPoll, 0, sizeof(ledPoll_t));
}

bool ledPoll_setup(ledPoll_t *ledPoll)
{
  ledPoll->mainLoop = g_main_loop_new(NULL, FALSE);
  return true;
}

void ledPoll_destroy(ledPoll_t *ledPoll)
{
  for (int i = 0; i < ledPoll->nfds; i++)
  {
    g_source_unref(ledPoll->sources[i]);
  }
  g_main_loop_unref(ledPoll->mainLoop);
}

static gboolean _onFDSourceChanged(gint fd,
                                   GIOCondition condition,
                                   gpointer user_data)
{
  ledPollFD_t *ledPollFD = user_data;

  if ((condition & G_IO_IN) && ledPollFD->fn)
  {
    ledPollFD->fn(fd, ledPollFD->data);
  }
  return TRUE;
}

bool ledPoll_addFD(ledPoll_t *ledPoll, gint fd, ledPollFn_t fn, void *data)
{
  if (ledPoll->nfds >= LED_POLL_MAX)
  {
    LOG_ERROR("The poll fd array is full. The maximum length is %d",
              LED_POLL_MAX);
    return false;
  }

  ledPoll->fds[ledPoll->nfds].fd = fd;
  ledPoll->fds[ledPoll->nfds].fn = fn;
  ledPoll->fds[ledPoll->nfds].data = data;
  ledPoll->sources[ledPoll->nfds] = g_unix_fd_source_new(fd, G_IO_IN);
  g_source_set_callback(ledPoll->sources[ledPoll->nfds],
                        (GSourceFunc)_onFDSourceChanged,
                        &(ledPoll->fds[ledPoll->nfds]), NULL);
  g_source_attach(ledPoll->sources[ledPoll->nfds], NULL);
  ledPoll->nfds++;
  return true;
}

void ledPoll_run(ledPoll_t *ledPoll)
{
  g_main_loop_run(ledPoll->mainLoop);
}

void ledPoll_quit(ledPoll_t *ledPoll)
{
  g_main_loop_quit(ledPoll->mainLoop);
}
