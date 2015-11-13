#ifndef __WIN32_INET_NTOP_H__
#define __WIN32_INET_NTOP_H__

#if ((!defined NTDDI_VERSION) || (NTDDI_VERSION < NTDDI_LONGHORN))

#ifdef __cplusplus
extern "C"{
#endif

const char *inet_ntop(int af, const void *src, char *dest, size_t length);
int inet_pton(int af, const char* src, void* dest);

#ifdef __cplusplus
}
#endif

#endif

#endif //__WIN32_INET_NTOP_H__