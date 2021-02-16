// Copyright (c) 2020, SmartThings. All Rights Reserved.

#ifndef __LED_NCP_SOCKET_H__
#define __LED_NCP_SOCKET_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define INVALID_SOCKET -1

typedef struct ledNcpSocket_s
{
  int fd;
} ledNcpSocket_t;

bool ledNcpSocket_setup(ledNcpSocket_t *ncpSocket);
void ledNcpSocket_destroy(ledNcpSocket_t *ncpSocket);
int32_t ledNcpSocket_write(ledNcpSocket_t *ncpSocket,
                           uint8_t *pBuf,
                           size_t nBuf);
int32_t ledNcpSocket_writeWithRetry(ledNcpSocket_t *ncpSocket,
                                    uint8_t *pBuf,
                                    uint32_t nBuf,
                                    int const maxRetry);
int32_t ledNcpSocket_read(ledNcpSocket_t *ncpSocket, uint8_t *pBuf, size_t nBuf);

#endif // __LED_NCP_SOCKET_H__
