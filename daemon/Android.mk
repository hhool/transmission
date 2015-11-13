LOCAL_PATH:= $(call my-dir)

#
# transmission-daemon
#

#include $(CLEAR_VARS)

#LOCAL_SRC_FILES:= \
# daemon.c watch.c

#LOCAL_CFLAGS:= \
#	-DTR_EMBEDDED \
#	-DTARGET_ANDROID

#LOCAL_C_INCLUDES:= \
#	$(LOCAL_PATH)/../ \
#	$(LOCAL_PATH)/../third-party/libevent/include \


#LOCAL_STATIC_LIBRARIES := \
#	libtransmission \
#	libminiupnp \
#	libnatpmp \
#	libdht \
#	libutp \
#	libcurl \
#	libevent \
#	libssl \
#	libcrypto

#LOCAL_LDLIBS := -lz \
#								-llog

#LOCAL_MODULE:=transmission-daemon

#include $(BUILD_EXECUTABLE)


#
# transmission-remote
#

#include $(CLEAR_VARS)

#LOCAL_SRC_FILES:= \
# remote.c

#LOCAL_CFLAGS:= \
#	-DTR_EMBEDDED \
#	-DTARGET_ANDROID


#LOCAL_C_INCLUDES:= \
#	$(LOCAL_PATH)/../ \
#	$(LOCAL_PATH)/../third-party/libevent/include \
#	$(LOCAL_PATH)/../third-party/curl/include \



#LOCAL_LDLIBS := -L$($(SYSROOT)/usr/lib) \
#               -ldl                    \
#               -llog
               
#LOCAL_STATIC_LIBRARIES := \
#	libtransmission \
#	libminiupnp \
#	libnatpmp \
#	libdht \
#	libcurl \
#	libevent \
#	libssl \
#	libcryto \
#	libz 


#LOCAL_MODULE:=transmission-remote

#include $(BUILD_EXECUTABLE)

#
# trdaemon-jni
#

include $(CLEAR_VARS)


base := $(LOCAL_PATH)/aosp-f

LOCAL_SRC_FILES:= \
 daemon.c watch.c daemon-jni.cpp strtoargv.c multithread_android.c

LOCAL_CFLAGS:= \
	-DTR_EMBEDDED \
	-D_FILE_OFFSET_BITS=64 \
	-D_LARGEFILE64_SOURCE \
	-DTARGET_ANDROID

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/../ \
	$(base)/frameworks/base/include \
	$(base)/system/core/include \
	$(base)/dalvik/libnativehelper/include \
	$(LOCAL_PATH)/../third-party/libevent/include \


LOCAL_STATIC_LIBRARIES := \
	libtransmission \
	libminiupnp \
	libnatpmp \
	libdht \
	libutp \
	libcurl \
	libevent \
	libssl \
	libcrypto

LOCAL_LDLIBS := -lz -llog
LOCAL_LDLIBS += -L$(call host-path,$(base)) -landroid_runtime -lnativehelper -lutils -lcutils

LOCAL_CFLAGS += -fPIC -O2 \
				-DTARGET_ANDROID -DPACKAGE_NAME=com_pbi_pvtorrent_PvTorrent -DPACKAGE_DIR=\"com/pbi/pvtorrent/PvTorrent\"
#-DPACKAGE_NAME=com_hs_nas_util_TransmissionUtil -DPACKAGE_DIR=\"com/hs/nas/util/TransmissionUtil\"
LOCAL_MODULE:=trdaemon-jni

include $(BUILD_SHARED_LIBRARY)