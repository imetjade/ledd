// Copyright (c) 2020, SmartThings. All Rights Reserved.

#ifndef __LED_LINKV2CMD_H__
#define __LED_LINKV2CMD_H__

#include <stdbool.h>
#include <stdint.h>

#include "ledNcpSocket.h"
#include "linkv2Cmd.h"

typedef struct ledLinkv2Cmd_s
{
  ledNcpSocket_t ncpSocket;
  linkv2Session_t *session;
} ledLinkv2Cmd_t;

bool ledLinkv2Cmd_setup(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_destroy(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                bool red,
                                bool green,
                                bool blue);
void ledLinkv2Cmd_setColorBlink(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                bool red,
                                bool green,
                                bool blue);
void ledLinkv2Cmd_setColorBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                       bool red,
                                       bool green,
                                       bool blue);
void ledLinkv2Cmd_offSolid(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setRedSolid(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setGreenSolid(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setBlueSolid(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setYellowSolid(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_offBlink(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setRedBlink(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setGreenBlink(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setBlueBlink(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setRedGreenBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd);
void ledLinkv2Cmd_setRGBBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd);

#endif // __LED_LINKV2CMD_H__
