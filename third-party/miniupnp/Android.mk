LOCAL_PATH:= $(call my-dir)

#
# libminiupnp
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    connecthostport.c \
		igd_desc_parse.c \
		minisoap.c \
		minissdpc.c \
		miniupnpc.c \
		miniwget.c \
		minixml.c \
		portlistingparse.c \
		receivedata.c \
		upnpcommands.c \
		upnpreplyparse.c

LOCAL_CFLAGS:= \
	-DNDEBUG \
	-DTARGET_ANDROID

LOCAL_C_INCLUDES:= \
	\

LOCAL_SHARED_LIBRARIES := \
	\

LOCAL_STATIC_LIBRARIES := \
	\


LOCAL_MODULE:=libminiupnp

include $(BUILD_STATIC_LIBRARY)
