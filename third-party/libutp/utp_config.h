#define CCONTROL_TARGET (100 * 1000) // us
#define RATE_CHECK_INTERVAL 10000 // ms
#define DYNAMIC_PACKET_SIZE_ENABLED false
#define DYNAMIC_PACKET_SIZE_FACTOR 2
// This should return the global number of bytes sent, used for determining dynamic
// packet size based on rate
#define REMIND_ME_A(a) #a
#define REMIND_ME_B(a) __FILE__  "(" REMIND_ME_A(a) ")" 
#define REMIND_ME(a) message(REMIND_ME_B(__LINE__)a)

#pragma REMIND_ME("WARNING IMPLEMENT IN LIBTRANSMISSION")
uint64 UTP_GetGlobalUTPBytesSent(const struct sockaddr *remote, socklen_t remotelen) { return 0; }

enum bandwidth_type_t {
	payload_bandwidth, connect_overhead,
	close_overhead, ack_overhead,
	header_overhead, retransmit_overhead
};

#ifdef WIN32
#define I64u "%I64u"
#else
#define I64u "%Lu"
#endif
#ifdef WIN32
#define snprintf _snprintf
#endif

#ifdef	WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#undef  malloc
#undef  calloc
#define malloc(x) _malloc_dbg(x,_NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(x,y) _calloc_dbg(x,y,_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
/*
#include <event2/buffer.h>

#define g_log_utp 1
#define g_log_utp_verbose 1
void utp_log(char const* fmt, ...)
{
	
	va_list           args;
	char              timestr[64];
	struct evbuffer * buf = evbuffer_new( );

	
	va_start( args, fmt );
	evbuffer_add_vprintf( buf, fmt, args );
	va_end( args );
	
	OutputDebugString((LPCSTR)evbuffer_pullup(buf, -1 ) );
	OutputDebugString("\n");
	evbuffer_free( buf );
	
};*/
#define g_log_utp 0
#define g_log_utp_verbose 0
void utp_log(char const* fmt, ...)
{
	/*
	printf("[%u] ", UTP_GetMilliseconds());
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	va_end(vl);
	puts("");
	fflush(stdout);
	*/
};
