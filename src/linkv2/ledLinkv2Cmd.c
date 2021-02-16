// Copyright (c) 2020, SmartThings. All Rights Reserved.

#include "ledLinkv2Cmd.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "log.h"

#define MAX_PACKET_SIZE 1024
#define MAX_SEND_RETRIES 10
#define BLINKPATTERN_MS 500
#define LED_BLINK_COUNT (0xff)

static int requestWashCommand(ledLinkv2Cmd_t *ledLinkv2Cmd,
                              uint8_t *outPkt,
                              uint32_t outPktLen)
{
  int ret = ledNcpSocket_writeWithRetry(&ledLinkv2Cmd->ncpSocket, outPkt,
                                        outPktLen, MAX_SEND_RETRIES);
  if (ret < 0)
  {
    LOG_ERROR("Failed to write on socket");
    return ret;
  }

  linkv2Cmd_t cmdType = 0x0;
  outPktLen = ledNcpSocket_read(&ledLinkv2Cmd->ncpSocket, outPkt, outPktLen);
  linkv2GetCmd(ledLinkv2Cmd->session, &cmdType, outPkt, outPktLen);
  if (cmdType == LINKV2_CMD_NACK)
  {
    uint16_t reason;
    linkv2ParseNack(ledLinkv2Cmd->session, outPkt, outPktLen, &cmdType,
                    &reason);
    LOG_ERROR("requestWashCommand Failed = %d", reason);
    return -1;
  }
  return ret;
}

bool ledLinkv2Cmd_setup(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledNcpSocket_setup(&ledLinkv2Cmd->ncpSocket);

  ledLinkv2Cmd->session = linkv2CmdInit();
  if (ledLinkv2Cmd->session == NULL)
  {
    LOG_ERROR("Failed to init linkv2Cmd");
    ledNcpSocket_destroy(&ledLinkv2Cmd->ncpSocket);
    return false;
  }

  return true;
}

void ledLinkv2Cmd_destroy(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledNcpSocket_destroy(&ledLinkv2Cmd->ncpSocket);
  linkv2CmdDeinit(ledLinkv2Cmd->session);
}

static int _prepareSetColorSolidRequest(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                        bool red,
                                        bool green,
                                        bool blue,
                                        uint8_t *outPkt,
                                        uint32_t *outPktLen)
{
  if (!red && !green && !blue)
  {
    LOG_INFO("Turn Off all led");
    return linkv2PrepareTurnOffLedRequest(ledLinkv2Cmd->session, outPkt,
                                          outPktLen);
  }
  LOG_INFO("Turn On Red=%d, Green=%d, Blue=%d", red, green, blue);
  return linkv2PrepareTurnOnLedRequest(ledLinkv2Cmd->session, red, green, blue,
                                       outPkt, outPktLen);
}

void ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                bool red,
                                bool green,
                                bool blue)
{
  uint8_t outPkt[MAX_PACKET_SIZE] = {0};
  uint32_t outPktLen = MAX_PACKET_SIZE;
  int ret = _prepareSetColorSolidRequest(ledLinkv2Cmd, red, green, blue, outPkt,
                                         &outPktLen);
  if (ret != LINKV2_CMD_OK)
  {
    LOG_ERROR("Failed to prepare linkv2Cmd. result: %d errno: %s", ret,
              strerror(errno));
    return;
  }

  ret = requestWashCommand(ledLinkv2Cmd, outPkt, outPktLen);
  if (ret < 0)
  {
    LOG_ERROR("Failed to request linkv2Cmd. result: %d errno: %s", ret,
              strerror(errno));
  }
}

static void _setColorBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                  bool red,
                                  bool green,
                                  bool blue,
                                  const linkv2LedStrobePattern_t pattern)
{
  uint8_t outPkt[MAX_PACKET_SIZE] = {0};
  uint32_t outPktLen = MAX_PACKET_SIZE;
  int ret =
    linkv2PrepareStrobeLedRequest(ledLinkv2Cmd->session, red, green, blue,
                                  BLINKPATTERN_MS, LED_BLINK_COUNT, pattern,
                                  outPkt, &outPktLen);
  if (ret != LINKV2_CMD_OK)
  {
    LOG_ERROR("Failed to prepare linkv2Cmd");
    return;
  }
  LOG_INFO("Red: %d, Green: %d, Blue %d, freq: %d, count %d, Pattern %d", red,
           green, blue, BLINKPATTERN_MS, LED_BLINK_COUNT, pattern);

  ret = requestWashCommand(ledLinkv2Cmd, outPkt, outPktLen);
  if (ret < 0)
  {
    LOG_ERROR("Failed to request linkv2Cmd");
  }
}

void ledLinkv2Cmd_setColorBlink(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                bool red,
                                bool green,
                                bool blue)
{
  _setColorBlinkPattern(ledLinkv2Cmd, red, green, blue,
                        LINKV2_LED_STROBE_SIMPLE);
}

void ledLinkv2Cmd_setColorBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd,
                                       bool red,
                                       bool green,
                                       bool blue)
{
  _setColorBlinkPattern(ledLinkv2Cmd, red, green, blue,
                        LINKV2_LED_STROBE_PATTERN);
}

void ledLinkv2Cmd_offSolid(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd, false, false, false);
}

void ledLinkv2Cmd_setRedSolid(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd, true, false, false);
}

void ledLinkv2Cmd_setGreenSolid(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd, false, true, false);
}

void ledLinkv2Cmd_setBlueSolid(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd, false, false, true);
}

void ledLinkv2Cmd_setYellowSolid(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorSolid(ledLinkv2Cmd, true, true, false);
}

void ledLinkv2Cmd_offBlink(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorBlink(ledLinkv2Cmd, false, false, false);
}

void ledLinkv2Cmd_setRedBlink(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorBlink(ledLinkv2Cmd, true, false, false);
}

void ledLinkv2Cmd_setGreenBlink(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorBlink(ledLinkv2Cmd, false, true, false);
}

void ledLinkv2Cmd_setBlueBlink(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorBlink(ledLinkv2Cmd, false, false, true);
}

void ledLinkv2Cmd_setRedGreenBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorBlinkPattern(ledLinkv2Cmd, true, true, false);
}

void ledLinkv2Cmd_setRGBBlinkPattern(ledLinkv2Cmd_t *ledLinkv2Cmd)
{
  ledLinkv2Cmd_setColorBlinkPattern(ledLinkv2Cmd, true, true, true);
}
