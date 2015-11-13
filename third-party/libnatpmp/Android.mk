LOCAL_PATH:= $(call my-dir)

#
# libnatpmp
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    getgateway.c \
    natpmp.c \
    wingettimeofday.c

LOCAL_CFLAGS:= \
	-DENABLE_STRNATPMPERR \
	-DTARGET_ANDROID \
	
LOCAL_C_INCLUDES:= \
	\

LOCAL_SHARED_LIBRARIES := \
	\

LOCAL_STATIC_LIBRARIES := \
	\


LOCAL_MODULE:=libnatpmp

include $(BUILD_STATIC_LIBRARY)
