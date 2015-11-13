#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h >
#include <inet_ntop.h>

#if ((!defined NTDDI_VERSION) || (NTDDI_VERSION < NTDDI_LONGHORN))
const char *inet_ntop(int af, const void *src, char *dest, size_t length)
{
	SOCKADDR_STORAGE address;
	DWORD address_length;
	DWORD string_length;
	int result;

	if (af != AF_INET && af != AF_INET6)
	{
		return NULL;
	}

	if (af == AF_INET)
	{
		struct sockaddr_in* ipv4_address = (struct sockaddr_in*)(&address);
		address_length = sizeof(struct sockaddr_in);
		ipv4_address->sin_family = AF_INET;
		ipv4_address->sin_port = 0;
		memcpy(&ipv4_address->sin_addr, src, sizeof(struct in_addr));
	}
	else // AF_INET6
	{
		struct sockaddr_in6* ipv6_address = (struct sockaddr_in6*)(&address);
		address_length = sizeof(struct sockaddr_in6);
		ipv6_address->sin6_family = AF_INET6;
		ipv6_address->sin6_port = 0;
		ipv6_address->sin6_flowinfo = 0;
		// hmmm
		ipv6_address->sin6_scope_id = 0;
		memcpy(&ipv6_address->sin6_addr, src, sizeof(struct in6_addr));
	}

	string_length = (DWORD)(length);

	result = WSAAddressToStringA((struct sockaddr*)(&address),
		address_length, 0, dest,
		&string_length);

	// one common reason for this to fail is that ipv6 is not installed

	return result == SOCKET_ERROR ? NULL : dest;
}

int inet_pton(int af, const char* src, void* dest)
{
	int result = -1;

	if (af != AF_INET && af != AF_INET6)
	{
		return result;
	}
	else
	{
		SOCKADDR_STORAGE address;
		int address_length = sizeof(SOCKADDR_STORAGE);
		result = WSAStringToAddressA((char*)(src), af, 0,
			(struct sockaddr*)(&address),
			&address_length);

		if (af == AF_INET)
		{
			if (result != SOCKET_ERROR)
			{
				struct sockaddr_in* ipv4_address =(struct sockaddr_in*)(&address);
				memcpy(dest, &ipv4_address->sin_addr, sizeof(struct in_addr));
			}
			else if (strcmp(src, "255.255.255.255") == 0)
			{
				((struct in_addr*)(dest))->s_addr = INADDR_NONE;
			}
		}
		else // AF_INET6
		{
			if (result != SOCKET_ERROR)
			{
				struct sockaddr_in6* ipv6_address = (struct sockaddr_in6*)(&address);
				memcpy(dest, &ipv6_address->sin6_addr, sizeof(struct in6_addr));
			}
		}
	}
	return result == SOCKET_ERROR ? -1 : 1;
}
#endif