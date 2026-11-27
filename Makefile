# Minimal devkitPro 3DS homebrew Makefile
ifeq ($(strip $(DEVKITARM)),)
$(error "DEVKITARM is not set")
endif

TARGET := NOVA_3DS
BUILD := build
SOURCES := source
DATA :=
INCLUDES := include

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS := -g -Wall -O2 -mword-relocations -ffunction-sections -fdata-sections $(ARCH)
CFLAGS += $(INCLUDE) -D__3DS__
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS := -g $(ARCH)
LDFLAGS := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS := -lctru -lm
LIBDIRS := $(CTRULIB)

include $(DEVKITARM)/3ds_rules
