LOCAL_PATH:= $(call my-dir)

#
# libutp
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    utp_utils.cpp \
    utp.cpp

LOCAL_CFLAGS:= \
	-DENABLE_STRNATPMPERR \
	-DTARGET_ANDROID \
	-DPOSIX
	
LOCAL_C_INCLUDES:= \
	\

LOCAL_SHARED_LIBRARIES := \
	\

LOCAL_STATIC_LIBRARIES := \
	\


LOCAL_MODULE:=libutp

include $(BUILD_STATIC_LIBRARY)
