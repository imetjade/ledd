SRCS := $(addprefix ../../,$(SRCS))
SRCS += ../../src/linkv2/ledLinkv2Cmd.c
SRCS += ../../src/linkv2/ledNcpSocket.c
SRCS += ../../src/linkv2/ledPlatformLinkV2.c
SRCS += ledPlatformGenericLinkV2.c

CFLAGS += -I../../include
CFLAGS += -I../../src
CFLAGS += -I../../src/linkv2
CFLAGS += -I.
CFLAGS += -DLED_PLATFORM_LINKV2
LDFLAGS += -llinkv2cmd

OBJS = $(SRCS:.c=.o)
TARGET = ledd

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: clean
