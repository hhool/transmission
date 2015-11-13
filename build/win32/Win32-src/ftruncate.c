#include <unistd.h>
int _ftruncate(int __fd, off_t __length)
{
  return _chsize_s(__fd,__length);//_chsize (__fd, __length);
}

int _ftruncate64(int __fd, long long __length)
{
	return _chsize_s(__fd,__length);
}