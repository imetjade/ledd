// Copyright (c) 2020 SmartThings. All Rights Reserved.

#ifndef __LED_POLL_H__
#define __LED_POLL_H__

#include <glib.h>
#include <stdbool.h>

#define LED_POLL_MAX 5

typedef void (*ledPollFn_t)(int fd, void *data);

typedef struct ledPollFD_s
{
  gint fd;
  ledPollFn_t fn;
  void *data;
} ledPollFD_t;

typedef struct ledPoll_s
{
  GMainLoop *mainLoop;
  GSource *sources[LED_POLL_MAX];
  ledPollFD_t fds[LED_POLL_MAX];
  int nfds;
} ledPoll_t;

void ledPoll_init(ledPoll_t *ledPoll);
bool ledPoll_setup(ledPoll_t *ledPoll);
void ledPoll_destroy(ledPoll_t *ledPoll);
bool ledPoll_addFD(ledPoll_t *ledPoll, gint fd, ledPollFn_t fn, void *data);
void ledPoll_run(ledPoll_t *ledPoll);
void ledPoll_quit(ledPoll_t *ledPoll);

#endif /*__LED_POLL_H__*/
