

#ifndef __SQLITE_WRAP1_C__
#define __SQLITE_WRAP1_C__

#include "sqlite_config.h"


#ifdef __RE_USING_ADITIONAL_SQLITE_LIBRARY__


#ifdef __RE_OS_BADA__

// there's no ftruncate function in Bada OS so we need to add one
int ftruncate(int fd, long length)
{
	long cursize;
	
	if((cursize = lseek(fd, 0L, 2)) == -1) return -1;
	
	if(cursize < length)
	{
		if(lseek(fd, length - 1, 0) == -1 || write(fd, "", 1) == -1) return -1;
		return 0;
	}
	else if(cursize > length)
	{
		/*
		 struct flock fl;
		 fl.l_whence = 0;
		 fl.l_len = 0;
		 fl.l_start = length;
		 fl.l_type = F_WRLCK;
		 return (fcntl(fd, F_FREESP, &fl));
		 */
		return 0;
	}
	return 0;
}

#endif /* __RE_OS_BADA__ */



#include "sqlite/sqlite3.c"

#endif

#endif

