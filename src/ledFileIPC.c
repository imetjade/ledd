// Copyright (c) 2020 SmartThings. All Rights Reserved.

#include "ledFileIPC.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ledPlatform.h"
#include "log.h"

bool ledFileIPC_setup(ledFileIPC_t *self, const char *filePath)
{
  // Set umask to allow setting proper write permissions for group/all
  umask(000);
  // Make a new named pipe that anyone can read or write to.
  if (-1 == mkfifo(filePath, 0666))
  {
    if (errno != EEXIST)
    {
      LOG_ERROR("Error creating Named Pipe - %d/%s", errno, strerror(errno));
      LOG_ERROR("  Continuing execution optimistically");
    }
  }
  // Set umask back to normal
  umask(022);

  self->fd = open(filePath, O_RDWR | O_NONBLOCK);
  if (self->fd < 0)
  {
    LOG_ERROR("Error opening file: %s", strerror(errno));
    return false;
  }

  LOG_INFO("Opened %s: fd: %d", filePath, self->fd);
  return true;
}
