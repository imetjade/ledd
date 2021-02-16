SRCS := $(addprefix ../../,$(SRCS))
SRCS += ../../src/linkv2/ledLinkv2Cmd.c
SRCS += ../../src/linkv2/ledNcpSocket.c
SRCS += ../../src/linkv2/ledPlatformLinkV2.c
SRCS += ledGDbusAPI.c
SRCS += ledPlatformSamsungTV.c

CFLAGS += -I../../include
CFLAGS += -I../../src
CFLAGS += -I../../src/linkv2
CFLAGS += -I.
CFLAGS += -DLED_PLATFORM_SAMSUNG_TV
CFLAGS += $(DBUS_GLIB_CFLAGS)
LDFLAGS += -llinkv2cmd -lbuttcmd
LDFLAGS += -lgio-2.0 -lgmodule-2.0
LDFLAGS += $(DBUS_GLIB_LDFLAGS)
LDFLAGS += -lpcre -lffi -lmount -lblkid -lz -luuid

OBJS = $(SRCS:.c=.o)
TARGET = ledd

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: clean
