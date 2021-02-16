SRCS := $(addprefix ../../,$(SRCS))
SRCS += ledGDbus.c
SRCS += ledPlatformCalix.c

CFLAGS += -I../../include
CFLAGS += -I../../src
CFLAGS += -I.
CFLAGS += -DLED_PLATFORM_CALIX
CFLAGS += $(DBUS_GLIB_CFLAGS)
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
