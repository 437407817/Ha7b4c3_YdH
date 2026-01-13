#ifndef __IIC_CMD_H
#define	__IIC_CMD_H


#include "stm32fxxx.h"


typedef uint16_t		WORD;	/* 16-bit unsigned */
typedef uint32_t		DWORD;	/* 32-bit unsigned */
typedef uint64_t		QWORD;	/* 64-bit unsigned */
typedef WORD			WCHAR;	/* UTF-16 code unit */
typedef DWORD FSIZE_t;
typedef DWORD LBA_t;


typedef struct {
//	FFOBJID	obj;			/* Object identifier (must be the 1st member to detect invalid object pointer) */
//	BYTE	flag;			/* File status flags */
//	BYTE	err;			/* Abort flag (error code) */
  	FSIZE_t	fptr;			/* File read/write pointer (Zeroed on file open) */
	  WORD  fposition;
		WORD  remainingQuantity;
	  WORD  cnt;
//	DWORD	clust;			/* Current cluster of fpter (invalid when fptr is 0) */
//	LBA_t	sect;			/* Sector number appearing in buf[] (0:invalid) */
//#if !FF_FS_READONLY
//	LBA_t	dir_sect;		/* Sector number containing the directory entry (not used at exFAT) */
//	BYTE*	dir_ptr;		/* Pointer to the directory entry in the win[] (not used at exFAT) */
//#endif
//#if FF_USE_FASTSEEK
//	DWORD*	cltbl;			/* Pointer to the cluster link map table (nulled on open, set by application) */
//#endif
//#if !FF_FS_TINY
//	BYTE	buf[FF_MAX_SS];	/* File private data read/write window */
//#endif
} IIC_FIL;















void IIC_window_cmd(void);























#endif /* __TEST_ALL_H */
