LOCAL_PATH:= $(call my-dir)

#
# libtransmission
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
				ConvertUTF.c \
				JSON_parser.c \
				announcer-http.c \
				announcer-udp.c \
				announcer.c \
				bandwidth.c \
				bencode.c \
				bitfield.c \
				blocklist.c \
				cache.c \
				clients.c \
				completion.c \
				crypto.c \
				fdlimit.c \
				handshake.c \
				history.c \
				inout.c \
				json.c \
				list.c \
				magnet.c \
				makemeta.c \
				metainfo.c \
				natpmp.c \
				net.c \
				peer-io.c \
				peer-mgr.c \
				peer-msgs.c \
				platform.c \
				port-forwarding.c \
				ptrarray.c \
				resume.c \
				rpc-server.c \
				rpcimpl.c \
				session.c \
				stats.c \
				torrent-ctor.c \
				torrent-magnet.c \
				torrent.c \
				tr-dht.c \
				tr-getopt.c \
				tr-lpd.c \
				tr-udp.c \
				tr-utp.c \
				trevent.c \
				upnp.c \
				utils.c \
				verify.c \
				web.c \
				webseed.c \
				wildmat.c \
				android/arm/ftruncate64.S  \
				android/arm/pwrite64.S  \
				android/arm/pread64.S


LOCAL_CFLAGS:= \
	-D__TRANSMISSION__ \
	-DTARGET_ANDROID \
	-DHAVE_ZLIB \
	-DHAVE_MINIUPNP_16 \
	-DWITH_UTP \
	-D_LARGEFILE64_SOURCE \
	-D_FILE_OFFSET_BITS=64 \
	-DWEB_DIR=\"pvtorrent/web\" \
    -DPACKAGE_DATA_DIR=\"com.pbi.pvtorrent\" \
    #-DTR_LIGHTWEIGHT=1 \
	#-DWEB_DIR=\"nas/web\" \
	#-DPACKAGE_DATA_DIR=\"com.hs.nas\" \
    #-DREQUIRE_SESSION_ID

LOCAL_C_INCLUDES:= \
	$(SYSROOT)/usr/include \
	$(LOCAL_PATH)/../third-party \
	$(LOCAL_PATH)/../third-party/libevent/include \
	$(LOCAL_PATH)/../third-party/libnatpmp \
	$(LOCAL_PATH)/../third-party/libutp \
	$(LOCAL_PATH)/../third-party/curl/include \
	$(LOCAL_PATH)/../external/openssl \
	$(LOCAL_PATH)/../external/openssl/include \
	

LOCAL_STATIC_LIBRARIES := \
	libminiupnp \
	libnatpmp \
	libdht \
	libutp \
	libcurl \
	libevent \
	libssl \
	libcrypto

	
LOCAL_LDLIBS := -lz \
								-llog
	


LOCAL_MODULE:=libtransmission

include $(BUILD_STATIC_LIBRARY)
