LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
				buffer.c \
				bufferevent.c \
				bufferevent_filter.c \
				bufferevent_openssl.c \
				bufferevent_pair.c \
				bufferevent_ratelim.c \
				bufferevent_sock.c \
				epoll.c \
				evdns.c \
				event.c \
				event_tagging.c \
				evmap.c \
				evrpc.c \
				evthread.c \
				evthread_pthread.c \
				evutil.c \
				evutil_rand.c \
				http.c \
				listener.c \
				log.c \
				poll.c \
				select.c \
				signal.c \
				strlcpy.c


LOCAL_CFLAGS := -DHAVE_CONFIG_H -DTARGET_ANDROID -DARC4RANDOM_NOADDRANDOM
LOCAL_C_INCLUDES := $(LOCAL_PATH)/compat \
										$(LOCAL_PATH)/include \
										$(LOCAL_PATH)/../../external/openssl/include

LOCAL_MODULE:= libevent

LOCAL_COPY_HEADERS_TO :=
LOCAL_COPY_HEADERS := \
	event.h \
	evhttp.h \
	evdns.h \
	evrpc.h \
	evutil.h \
	event-config.h

include $(BUILD_STATIC_LIBRARY)
