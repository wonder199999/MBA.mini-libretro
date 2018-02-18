LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	LOCAL_CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

LOCAL_MODULE    := retro

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -DANDROID_ARM
LOCAL_ARM_MODE := arm
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D__ARM_NEON__ -mfpu=neon
endif
endif

ifeq ($(TARGET_ARCH),x86)
LOCAL_CFLAGS +=  -DANDROID_X86
endif

ifeq ($(TARGET_ARCH),mips)
LOCAL_CFLAGS += -DANDROID_MIPS -D__mips__ -D__MIPSEL__
endif

CORE_ROOT_DIR := ..
CORE_DIR      := $(CORE_ROOT_DIR)/src

include $(CORE_ROOT_DIR)/Makefile.common

LOCAL_STATIC_LIBRARIES := $(OBJECTS)

LOCAL_SRC_FILES  += $(SOURCES_C) $(SOURCES_CXX)
LOCAL_C_INCLUDES = $(INCFLAGS)

LOCAL_CFLAGS += $(DEFS) $(COREDEFS) $(CPUDEFS) $(SOUNDDEFS) $(ASMDEFS) $(DBGDEFS)
LOCAL_CFLAGS += -O3 -std=gnu99 -ffast-math -funroll-loops -Dstricmp=strcasecmp -DINLINE="static inline" -DANDROID $(INCFLAGS)

include $(BUILD_SHARED_LIBRARY)
