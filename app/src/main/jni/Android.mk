LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := Cube
LOCAL_ARM_MODE := arm

LOCAL_CFLAGS := -Wno-error=format-security -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fpermissive -w -Werror -s -std=c++17
LOCAL_CPPFLAGS += -fno-rtti -fno-exceptions -fms-extensions -Wno-error=c++11-narrowing

FILE_LIST += $(wildcard $(LOCAL_PATH)/app/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/imgui/*.c*)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS := -Wl -llog -landroid -lEGL -lGLESv2

include $(BUILD_SHARED_LIBRARY)
