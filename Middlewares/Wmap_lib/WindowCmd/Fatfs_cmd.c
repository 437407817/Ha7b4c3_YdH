/**
  ******************************************************************************
  * @file    Fatfs_cmd.c
  * @author  wmap
  * @version V1.1
  * @date    2025-03-16
  * @brief   应用函数接口
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
  
#include "./WindowCmd/Fatfs_cmd.h"   

#include "ff.h"
#include "ffconf.h"
#include "diskio.h"	
 #include "./sys/sysio.h"

//******************************



/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs            (C)ChaN, 2018    */
/*----------------------------------------------------------------------*/

#include <string.h>
//#include "STM32F100.h"
//#include "uart_stm32f1.h"
#include "rtc_stm32fx.h"
#include "xprintf.h"


//#include "./usart/bsp_usart.h"
#include "./usart/bsp_usart_shell.h"
#include "sd_diskio.h"
#include "sdram_diskio.h"
#include "flash_diskio.h"
#include "./fatfs.h"

extern void disk_timerproc (void);


char Line[256];				/* Console input buffer */
BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */

FATFS FatFs[FF_VOLUMES];				/* File system object for each logical drive */
FIL File[2];				/* File objects *///多个驱动公用一个文件file
DIR Dir;					/* Directory object */
FILINFO Finfo;

volatile UINT Timer;
typedef uint64_t		QWORD;	/* 64-bit unsigned */


/*---------------------------------------------*/
/* 1kHz timer process                          */
/*---------------------------------------------*/

//void SysTick_Handler (void)
//{
//	static uint16_t led;


//	Timer++;	/* Increment performance counter */

//	if (++led >= 500) {
//		led = 0;
//		GPIOC_ODR ^= _BV(9)|_BV(8);		/* Flip Green/Blue LED state */
//	}

//	disk_timerproc();	/* Disk timer process */
//}



/*---------------------------------------------------------*/
/* User provided RTC function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called back     */
/* from FatFs module.                                      */

#if !FF_FS_NORTC && !FF_FS_READONLY
DWORD get_fattime (void)
{
	RTCTIME rtc;

	/* Get local time */
	if (!rtc_gettime(&rtc)) return 0;

	/* Pack date and time into a DWORD variable */
	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}
#endif


/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */
/*--------------------------------------------------------------------------*/

static FRESULT scan_files (
	char* path,		/* Pointer to the path name working buffer */
	UINT* n_dir,
	UINT* n_file,
	QWORD* sz_file
)
{
	DIR dirs;
	FRESULT res;
	BYTE i;


	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		i = strlen(path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (Finfo.fattrib & AM_DIR) {
				(*n_dir)++;
				*(path+i) = '/'; strcpy(path+i+1, Finfo.fname);
				res = scan_files(path, n_dir, n_file, sz_file);
				*(path+i) = '\0';
				if (res != FR_OK) break;
			} else {
			/*	xprintf("%s/%s\n", path, fn); */
				(*n_file)++;
				*sz_file += Finfo.fsize;
			}
		}
	}

	return res;
}



static
void put_rc (FRESULT rc)
{
	const char *str =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
		"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0" "INVALID_PARAMETER\0";
	FRESULT i;
if(rc>20){

xprintf("rc=%u is too max\n", (UINT)rc);
	return;
}
	for (i = 0; i != rc && *str; i++) {
		while (*str++) ;
	}
	xprintf("rc=%u FR_%s\n", (UINT)rc, str);
}


static
const char HelpMsg[] =
	"[Disk contorls]\n"
	" di <pd#> - Initialize disk {:di 1}\n"
	" dd [<pd#> <lba>] - Dump a secrtor {:dd 1 0}\n"
	" ds <pd#> - Show disk status {:ds 1}\n"
	"[Buffer controls]\n"
	" bd <ofs> - Dump working buffer {:bd 10}\n"
	" be <ofs> [<data>] ... - Edit working buffer {:be 10 255/:be 10}\n"
	" br <pd#> <lba> [<count>] - Read disk into working buffer\n"
	" bw <pd#> <lba> [<count>] - Write working buffer into disk\n"
	" bf <val> - Fill working buffer\n"
	"[File system controls]\n"
" fi <ld#> [<mount>]- Force initialized the volume. -> first-{:fi} -> second-{:fi 1 0}\n"
	" fq <pd#>- umount the volume {:fq 1}\n"
	" fs <ld#> - Select Current volume And Show volume status{:fs 1}\n"
	" fl [<path>] - Show a directory\n"
	" fo <mode> <file> - Open a file  {:fo 7 new.txt}{:fo 0x13 file.txt -FA_READ:0x01 FA_WRITE:0x02 FA_OPEN_ALWAYS:0x10}\n"
	" fc - Close the file\n"
	" fe <position> - Move fp in normal seek  {:fe 0}\n"
	" fd <len> - Read and dump the file  {:fd 16}\n"
	" fr <len> - Read the file\n"
	" fw <len> <val> - Write to the file\n"
	" fpc <achar> - Write a character to the file  {:fpc z}\n"
	" fps <string> - Write String to the file  {:fps zzz}\n"
	" fn <org.name> <new.name> - Rename an object\n"
	" fu <name> - Unlink(delete) an object\n"
	" fv - Truncate the file at current fp\n"
	" fk <name> - Create a directory{:fk dir0}\n"
	" fa <atrr> <mask> <object name> - Change attribute of an object\n"
	" ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp of an object\n"
	" fx <src.file> <dst.file> - Copy a file\n"
	" fg <path> - Change current directory {:fg dir0}  jump up dir{:fg /}\n"
	" fff - Show current directory num {:fff }\n"
	" ffs - Show selected volume\n"
	" ffa - test all\n"
	" ff(0-n) -test n\n"	
	" fb <name> - Set volume label\n"
	" fm <fs type> <au size> <align> <n_fats> <n_root> - Create filesystem{:fm FM_FAT32 4096 1 2 512}\n"
	" fz [<len>] - Change/Show R/W length for fr/fw/fx command\n"
	"[Misc commands]\n"
	" md[b|h|w] <addr> [<count>] - Dump memory\n"
	" mf <addr> <value> <count> - Fill memory\n"
	" me[b|h|w] <addr> [<value> ...] - Edit memory\n"
	" t [<year> <mon> <mday> <hour> <min> <sec>] - Set/Show RTC\n"
	" . - exit\n"
	"\n";



void fatfs_window_cmd(void)
{
	static const char *ft[] = {"", "FAT12", "FAT16", "FAT32", "exFAT"};
	static const char days[] = "Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";
	char *ptr, *ptr2;
	long p1, p2, p3;
	BYTE res, b, drv = 0;
	UINT s1, s2, cnt, blen = sizeof Buff, acc_files, acc_dirs;
	DWORD ofs = 0, sect = 0, blk[FF_VOLUMES], dw;
	QWORD acc_size;
	FATFS *fs;
	RTCTIME rtc;
  char path[20];
//	char parentpath[20];
//	char SDRAMpath[4],FLASHpath[4], SDpath[4];
//	char fatfsPath[4];/* SD逻辑驱动器路径 */
	char diskpath[20];
	/* Enable LED indicators on the STM32VL Discovery board */
//	__enable_peripheral(IOPCEN);
//	__gpio_conf_bit(GPIOC, 8, OUT_PP);	/* PC8:Blue LED */
//	__gpio_conf_bit(GPIOC, 9, OUT_PP);	/* PC9:Green LED */
//	GPIOC_BSRR = _BV(8);				/* Blue LED on */

	/* Enable SysTick timer as 1 ms interval timer */
//	SYST_RVR = 24000000 / 1000 - 1;
//	SYST_CVR = 24000000 / 1000 - 1;
//	SYST_CSR = 0x07;

	/* Enable UART1 and attach it to xprintf module for console */
//	uart1_init(115200);
	xdev_out(USART_Shell_SendChar);
	xdev_in(USART_Shell_ReceiveChar);
	#if USE_SERIAL_IT
	xputs("USE_SERIAL_IT is open ,please close!\n");
	#else
		xputs("STM32Fxxx fatfs monitor\n");
	
	#endif
	xputs(FF_USE_LFN ? "LFN Enabled" : "LFN Disabled");
//	xprintf(", Code page: %u\n", FF_CODE_PAGE);

	/* Initiazlize RTC */
//	if (rtc_initialize()) {
//		rtc_gettime(&rtc);
//		xprintf("Current time is %u/%u/%u %2u:%02u:%02u.\n", rtc.year, rtc.month, rtc.mday, rtc.hour, rtc.min, rtc.sec);
//	} else {
//		xputs("RTC is not available.\n");
//	}
//(FATFS_LinkDriver(&SDRAMDISK_Driver, RAMpath) == 0) && (FATFS_LinkDriver(&SD_Driver, SDpath) == 0))
//FATFS_LinkDriver(&SDRAMDISK_Driver, SDRAMpath);
//FATFS_LinkDriver(&FLASHDISK_Driver, FLASHpath);
//FATFS_LinkDriver(&SD_Driver, SDpath);



	for (;;) {
		xputc('>');
		xgets(Line, sizeof Line);

		ptr = Line;
		switch (*ptr++) {
		case '?' :	/* Show Command List */
			xputs(HelpMsg);
			break;

		case 'm' :	/* Memory dump/fill/edit */
			switch (*ptr++) {
			case 'd' :	/* md[b|h|w] <address> [<count>] - Dump memory */
				switch (*ptr++) {
				case 'w': p3 = 4; break;
				case 'h': p3 = 2; break;
				default: p3 = 1;
				}
				if (!xatoi(&ptr, &p1)) break;
				if (!xatoi(&ptr, &p2)) p2 = 128 / p3;
				for (ptr = (char*)p1; p2 >= 16 / p3; ptr += 16, p2 -= 16 / p3)
					put_dump(ptr, (DWORD)ptr, 16 / p3, p3);
				if (p2) put_dump((BYTE*)ptr, (UINT)ptr, p2, p3);
				break;
			case 'f' :	/* mf <address> <value> <count> - Fill memory */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				while (p3--) {
					*(BYTE*)p1 = (BYTE)p2;
					p1++;
				}
				break;
			case 'e' :	/* me[b|h|w] <address> [<value> ...] - Edit memory */
				switch (*ptr++) {	/* Get data width */
				case 'w': p3 = 4; break;
				case 'h': p3 = 2; break;
				default: p3 = 1;
				}
				if (!xatoi(&ptr, &p1)) break;	/* Get start address */
				if (xatoi(&ptr, &p2)) {	/* 2nd parameter is given (direct mode) */
					do {
						switch (p3) {
						case 4: *(DWORD*)p1 = (DWORD)p2; break;
						case 2: *(WORD*)p1 = (WORD)p2; break;
						default: *(BYTE*)p1 = (BYTE)p2;
						}
						p1 += p3;
					} while (xatoi(&ptr, &p2));	/* Get next value */
					break;
				}
				for (;;) {				/* 2nd parameter is not given (interactive mode) */
					switch (p3) {
					case 4: xprintf("%08X 0x%08X-", p1, *(DWORD*)p1); break;
					case 2: xprintf("%08X 0x%04X-", p1, *(WORD*)p1); break;
					default: xprintf("%08X 0x%02X-", p1, *(BYTE*)p1);
					}
					ptr = Line; xgets(ptr, sizeof Line);
					if (*ptr == '.') break;
					if ((BYTE)*ptr >= ' ') {
						if (!xatoi(&ptr, &p2)) continue;
						switch (p3) {
						case 4: *(DWORD*)p1 = (DWORD)p2; break;
						case 2: *(WORD*)p1 = (WORD)p2; break;
						default: *(BYTE*)p1 = (BYTE)p2;
						}
					}
					p1 += p3;
				}
				break;
			}
			break;

		case 'd' :	/* Disk I/O layer controls */
			switch (*ptr++) {
			case 'd' :	/* dd [<pd#> <sect>] - Dump secrtor */
				if (!xatoi(&ptr, &p1)) {
					p1 = drv; p2 = sect;
				} else {
					if (!xatoi(&ptr, &p2)) break;
				}
				drv = (BYTE)p1; sect = p2;
				res = disk_read(drv, Buff, sect, 1);
				if (res) { xprintf("rc=%d\n", (WORD)res); break; }
				xprintf("PD#:%u LBA:%lu\n", drv, sect++);
				for (ptr=(char*)Buff, ofs = 0; ofs < 0x200; ptr += 16, ofs += 16)
					put_dump((BYTE*)ptr, ofs, 16, 1);
				break;

			case 'i' :	/* di <pd#> - Initialize disk */
				if (!xatoi(&ptr, &p1)) break;
				xprintf("rc=%d\n", (WORD)disk_initialize((BYTE)p1));
				break;

			case 's' :	/* ds <pd#> - Show disk status */
				if (!xatoi(&ptr, &p1)) break;
				if (disk_ioctl((BYTE)p1, GET_SECTOR_COUNT, &p2) == RES_OK)
					{ xprintf("Drive size: %lu sectors\n", p2); }
				if (disk_ioctl((BYTE)p1, GET_BLOCK_SIZE, &p2) == RES_OK)
					{ xprintf("Block size: %lu sectors\n", p2); }
				if (disk_ioctl((BYTE)p1, MMC_GET_TYPE, &b) == RES_OK)
					{ xprintf("Media type: %u\n", b); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CSD, Buff) == RES_OK)
					{ xputs("CSD:\n"); put_dump(Buff, 0, 16, 1); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CID, Buff) == RES_OK)
					{ xputs("CID:\n"); put_dump(Buff, 0, 16, 1); }
				if (disk_ioctl((BYTE)p1, MMC_GET_OCR, Buff) == RES_OK)
					{ xputs("OCR:\n"); put_dump(Buff, 0, 4, 1); }
				if (disk_ioctl((BYTE)p1, MMC_GET_SDSTAT, Buff) == RES_OK) {
					xputs("SD Status:\n");
					for (s1 = 0; s1 < 64; s1 += 16) put_dump(Buff+s1, s1, 16, 1);
				}
				break;

			case 'c' :	/* Disk ioctl */
				switch (*ptr++) {
				case 's' :	/* dcs <pd#> - CTRL_SYNC *///强制将缓存中的数据刷新到存储设备
					if (!xatoi(&ptr, &p1)) break;
					xprintf("rc=%d\n", disk_ioctl((BYTE)p1, CTRL_SYNC, 0));
					break;
				case 'e' :	/* dce <pd#> <s.lba> <e.lba> - CTRL_TRIM */
//					if (!xatoi(&ptr, &p1) || !xatoi(&ptr, (long*)&blk[0]) || !xatoi(&ptr, (long*)&blk[1])) break;
					    if (!xatoi(&ptr, &p1)) { break;};
									for (BYTE i = 0; i < FF_VOLUMES; i++) {if (!xatoi(&ptr, (long*)&blk[i])) { break;}};	
					xprintf("rc=%d\n", disk_ioctl((BYTE)p1, CTRL_TRIM, blk));
					break;
				}
				break;
			}
			break;

		case 'b' :	/* Buffer controls */
			switch (*ptr++) {
			case 'd' :	/* bd <ofs> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				for (ptr=(char*)&Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr+=16, ofs+=16)
					put_dump((BYTE*)ptr, ofs, 16, 1);
				break;

			case 'e' :	/* be <ofs> [<data>] ... - Edit R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (xatoi(&ptr, &p2)) {
					do {
						Buff[p1++] = (BYTE)p2;
					} while (xatoi(&ptr, &p2));
					break;
				}
				for (;;) {
					xprintf("%04X %02X-", (WORD)(p1), (WORD)Buff[p1]);
					xgets(Line, sizeof Line);
					ptr = Line;
					if (*ptr == '.') break;
					if (*ptr < ' ') { p1++; continue; }
					if (xatoi(&ptr, &p2))
						Buff[p1++] = (BYTE)p2;
					else
						xputs("???\n");
				}
				break;

			case 'r' :	/* br <pd#> <lba> [<num>] - Read disk into R/W buffer */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf("rc=%u\n", (WORD)disk_read((BYTE)p1, Buff, p2, p3));
				break;

			case 'w' :	/* bw <pd#> <lba> [<num>] - Write R/W buffer into disk */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				if (!xatoi(&ptr, &p3)) p3 = 1;
				xprintf("rc=%u\n", (WORD)disk_write((BYTE)p1, Buff, p2, p3));
				break;

			case 'f' :	/* bf <val> - Fill working buffer */
				if (!xatoi(&ptr, &p1)) break;
				memset(Buff, (BYTE)p1, sizeof Buff);
				break;

			}
			break;

		case 'f' :	/* FatFS API controls */
			switch (*ptr++) {

			case 'i' :	/* fi [<opt>]- Initialize logical drive */
			  if (!xatoi(&ptr, &p1)) {
				p1=0; 
					
				put_rc(f_mount(&FatFs[0], "", (BYTE)p1));
				//xprintf("f_mount1=%d\n",p1);
					break;
				}
			   snprintf(path, sizeof(path),  "%lu:", p1);
//			   FATFS_LinkDriver(&SD_Driver, fatfsPath);
				if (!xatoi(&ptr, &p2)) p2 = 0;
				//put_rc(f_mount(&FatFs, "", (BYTE)p2));
				xprintf("f_mount=%d %d %s\n",p1, p2,(char*)path);
			  put_rc(f_mount(&FatFs[p1], path, (BYTE)p2));
				break;
			
			case 'q' :	/* fi [<opt>]- Initialize logical drive */
				if (!xatoi(&ptr, &p1)) 
					{
					//p1 = 0;
			  put_rc(f_unmount(""));//put_rc(f_unmount(""));
					//xprintf("f_unmount");
						break;
					}

					snprintf(path, sizeof(path),  "%lu:", p1);
					//xprintf("upath= %d  %s",p1,(char*)path);
					put_rc(f_unmount(path));//put_rc(f_unmount(path));
				break;
			
			
			case 's' :	/* fs [<path>] - Show volume status */
				while (*ptr == ' ') ptr++; 
				xprintf("upath= %s\n",(char*)ptr);
			  if (!xatoi(&ptr, &p2))p2=0;
			snprintf(diskpath, sizeof(diskpath),  "%u:", (unsigned  )(p2));

			f_chdrive(diskpath);//切换当前的工作驱动器
			xprintf("diskpath= %s\n",(char*)diskpath);fs=NULL;p1=NULL;
//				res = f_getfree(ptr, (DWORD*)&p1, &fs);
				res = f_getfree(diskpath, (DWORD*)&p1, &fs);
				if (res) { put_rc(res); break; }
				xprintf("FAT type = %s\n", ft[fs->fs_type]);
				xprintf("Bytes/Cluster = %lu\n", (DWORD)fs->csize * 512);
				xprintf("Number of FATs = %u\n", fs->n_fats);
				if (fs->fs_type < FS_FAT32) xprintf("Root DIR entries = %u\n", fs->n_rootdir);
				xprintf("Sectors/FAT = %lu\n", fs->fsize);
				xprintf("Number of clusters = %lu\n", (DWORD)fs->n_fatent - 2);
				xprintf("Volume start (lba) = %lu\n", fs->volbase);
				xprintf("%lu KiB total drive space. %lu KiB available.\n", ((fs->n_fatent - 2) * fs->csize)/2,(p1 * fs->csize)/2);
				xprintf("FAT start (lba) = %lu  DIR start (lba,clustor) = %lu\n", fs->fatbase, fs->dirbase);
				xprintf("Data start (lba) = %lu\n\n", fs->database);
#if FF_USE_LABEL
				res = f_getlabel(diskpath, (char*)Buff, (DWORD*)&p2);
				if (res) { put_rc(res); break; }
				xprintf(Buff[0] ? "Volume name is %s\n" : "No volume label\n", (char*)Buff);
				xprintf("Volume S/N is %04X-%04X\n", (DWORD)p2 >> 16, (DWORD)p2 & 0xFFFF);
#endif
				acc_size = acc_files = acc_dirs = 0;
				xprintf("...");
				res = scan_files(diskpath, &acc_dirs, &acc_files, &acc_size);
				if (res) { put_rc(res); break; }
				xprintf("\r%u files, %llu bytes.\n%u folders.\n"
						"%lu KiB total disk space.\n%lu KiB available.\n",
						acc_files, acc_size, acc_dirs,
						(fs->n_fatent - 2) * (fs->csize / 2), (DWORD)p1 * (fs->csize / 2)
				);
				break;

			case 'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == ' ') ptr++;
			xprintf("param path = %s  ",(char*)ptr);
			
			res = f_getcwd(path, sizeof path);
			
//			  if (!xatoi(&ptr, &p2))p2=0;
//			snprintf(path, sizeof(path),  "%s%s", diskpath,ptr);
//				res = f_opendir(&Dir, ptr);
			xprintf("current path -- %s\n",(char*)path);
			
			res = f_opendir(&Dir, ptr);
				if (res) { put_rc(res); break; }
				acc_size = acc_dirs = acc_files = 0;
				for(;;) {
					res = f_readdir(&Dir, &Finfo);
//					xprintf("res= %d %d ",(res),Finfo.fname[0]);
					if ((res != FR_OK) || !Finfo.fname[0]) {
					
					break;}
					if (Finfo.fattrib & AM_DIR) {
						acc_dirs++;
					} else {
						acc_files++; acc_size += Finfo.fsize;
					}
					xprintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s\n",
							(Finfo.fattrib & AM_DIR) ? 'D' : '-',
							(Finfo.fattrib & AM_RDO) ? 'R' : '-',
							(Finfo.fattrib & AM_HID) ? 'H' : '-',
							(Finfo.fattrib & AM_SYS) ? 'S' : '-',
							(Finfo.fattrib & AM_ARC) ? 'A' : '-',
							(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
							(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
							Finfo.fsize, Finfo.fname);
				}
				xprintf("%4u File(s),%10llu bytes total\n%4u Dir(s)", acc_files, acc_size, acc_dirs);
//				res = f_getfree(ptr, &dw, &fs);
				res = f_getfree(path, &dw, &fs);
				if (res == FR_OK) {
					xprintf(", %10llu bytes free\n", (QWORD)dw * fs->csize * 512);
				} else {
					put_rc(res);
				}
				break;

			case 'o' :	/* fo <mode> <file> - Open a file */
				if (!xatoi(&ptr, &p1)) break;
				while (*ptr == ' ') ptr++;
			
//			snprintf(path, sizeof(path),  "%s%s", diskpath,ptr);	//34		
//				put_rc(f_open(&File[0], path, (BYTE)p1));
			put_rc(f_open(&File[0], ptr, (BYTE)p1));
//				xprintf("mode= %d path= %s err=%d\r\n",p1,(char*)path,(File[0].err));
				break;

			case 'c' :	/* fc - Close a file */
				put_rc(f_close(&File[0]));
				break;

			case 'e' :	/* fe - Seek file pointer */
				if (!xatoi(&ptr, &p1)) break;
				res = f_lseek(&File[0], p1);
				put_rc(res);
				if (res == FR_OK) {
					xprintf("fptr=%lu(0x%lX)\n", File[0].fptr, File[0].fptr);
				}
				break;

			case 'd' :	/* fd <len> - read and dump file from current fp */
				if (!xatoi(&ptr, &p1)) break;
				ofs = File[0].fptr;
				while (p1) {
					if ((UINT)p1 >= 16) { cnt = 16; p1 -= 16; }
					else 				{ cnt = p1; p1 = 0; }
					res = f_read(&File[0], Buff, cnt, &cnt);
					if (res != FR_OK) { put_rc(res); break; }
					if (!cnt) break;
					put_dump(Buff, ofs, cnt, 1);
					ofs += 16;
				}
				break;

			case 'r' :	/* fr <len> - read file */
				if (!xatoi(&ptr, &p1)) break;
				p2 = 0;
				Timer = 0;
				while (p1) {
					if ((UINT)p1 >= blen) {
						cnt = blen; p1 -= blen;
					} else {
						cnt = p1; p1 = 0;
					}
					res = f_read(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				xprintf("%lu bytes read with %lu kB/sec.\n", p2, Timer ? (p2 / Timer) : 0);
				break;

			case 'w' :	/* fw <len> <val> - write file */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)){xprintf("fw not digital "); break;}
				memset(Buff, (BYTE)p2, blen);
				p2 = 0;
				Timer = 0;
				while (p1) {
					if ((UINT)p1 >= blen) {
						cnt = blen; p1 -= blen;
					} else {
						cnt = p1; p1 = 0;
					}
//					xprintf("File g err=%d   \n",(File[0].err));
					res = f_write(&File[0], Buff, cnt, &s2);
					
//					xprintf("File  h err=%d   \n",(File[0].err));
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				xprintf("%lu bytes written with %lu kB/sec.\n", p2, Timer ? (p2 / Timer) : 0);
				break;
			case 'p' :	
				switch (*ptr++) 
					{/* fp <len> <val> - write file */
					case 'c':while (*ptr == ' ') ptr++;xprintf("rc=%d  put_rc:%c \n",f_putc(*ptr,&File[0]),*ptr);break;
					case 's':while (*ptr == ' ') ptr++;xprintf("rc=%d\n",f_puts(ptr,&File[0]));break;
				  }   //put_rc(f_open(&File[0], ptr, (BYTE)p1));
			case 'n' :	/* fn <org.name> <new.name> - Change name of an object */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				put_rc(f_rename(ptr, ptr2));
				break;

			case 'u' :	/* fu <name> - delete/Unlink an object */
				while (*ptr == ' ') ptr++;
//			snprintf(path, sizeof(path),  "%s%s", diskpath,ptr);
			
			xprintf("delete -- %s\n",path);
				put_rc(f_unlink(ptr));
				break;

			case 'v' :	/* fv - Truncate file */
				put_rc(f_truncate(&File[0]));
				break;

			case 'k' :	/* fk <name> - Create a directory */
				while (*ptr == ' ') ptr++;
//			snprintf(path, sizeof(path),  "%s%s", diskpath,ptr);//34
//			xprintf("f_mkdir= %s\n",path);
//				put_rc(f_mkdir(path));
			put_rc(f_mkdir(ptr));
				break;

			case 'a' :	/* fa <atrr> <mask> <name> - Change attribute of an object */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_chmod(ptr, p1, p2));
				break;

			case 't' :	/* ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp of an object */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.fdate = ((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31);
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.ftime = ((p1 & 31) << 11) | ((p2 & 63) << 5) | ((p3 >> 1) & 31);
				put_rc(f_utime(ptr, &Finfo));
				break;

			case 'x' : /* fx <src.name> <dst.name> - Copy a file */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				xprintf("Opening \"%s\"", ptr);
				res = f_open(&File[0], ptr, FA_OPEN_EXISTING | FA_READ);
				xputc('\n');
				if (res) {
					put_rc(res);
					break;
				}
				xprintf("Creating \"%s\"", ptr2);
				res = f_open(&File[1], ptr2, FA_CREATE_ALWAYS | FA_WRITE);
				xputc('\n');
				if (res) {
					put_rc(res);
					f_close(&File[0]);
					break;
				}
				xprintf("Copying file...");
				Timer = 0;
				p1 = 0;
				for (;;) {
					res = f_read(&File[0], Buff, blen, &s1);
					if (res || s1 == 0) break;   /* error or eof */
					res = f_write(&File[1], Buff, s1, &s2);
					p1 += s2;
					if (res || s2 < s1) break;   /* error or disk full */
				}
				xprintf("\n%lu bytes copied with %lu kB/sec.\n", p1, p1 / Timer);
				f_close(&File[0]);
				f_close(&File[1]);
				break;
#if FF_FS_RPATH
			case 'g' :	/* fg <path> - Change current directory */
				while (*ptr == ' ') ptr++;
						xprintf("upath= %s  ",(char*)ptr);
//			  if (!xatoi(&ptr, &p2))p2=0;
//			snprintf(path, sizeof(path),  "%s/%s", diskpath,ptr);//34
//				res = f_opendir(&Dir, ptr);
//			xprintf("upath2= %s\n",(char*)path);
				res=f_chdir(ptr);//进入子文件夹
				if(res){put_rc(res);}else{



					
			res = f_getcwd(path, sizeof path);
			if (res){put_rc(res);}else {
			xprintf("current path -- %s \n", path);

					
			};


				
			}break;

			
				

			case 'f' :	/* fq - Show current dir path */
					switch (*ptr++) {
#if FF_FS_RPATH >= 2						
					case 'f':

					res = f_getcwd(path, sizeof path);if (res){put_rc(res);}else {xprintf("current path -- %s\n", path);}break;//获取当前工作目录的路径
#endif					
					case 's':xprintf("diskpath -- %s\n",(char*)diskpath);break;//获取当前工作盘号
					
					case '0':put_rc(f_getfree("0:", (DWORD*)&p1, &fs));xprintf("FAT start (lba) = %lu  DIR start (lba,clustor) = %lu\n", fs->fatbase, fs->dirbase);
//					SYSTEM_DEBUG_ARRAY_MESSAGE((FATFS)((FatFs)),sizeof(FATFS),"0=");
					break;
					case '1':put_rc(f_getfree("1:", (DWORD*)&p1, &fs));xprintf("FAT start (lba) = %lu  DIR start (lba,clustor) = %lu\n", fs->fatbase, fs->dirbase);
//					SYSTEM_DEBUG_ARRAY_MESSAGE(FatFs[0],sizeof(FATFS),"1=");
					break;
						case 'a':ffa();
//					SYSTEM_DEBUG_ARRAY_MESSAGE(FatFs[0],sizeof(FATFS),"1=");
					break;
				  } break;
				
#endif
#if FF_USE_LABEL
			case 'b' :	/* fb <name> - Set volume label */
				while (*ptr == ' ') ptr++;
				put_rc(f_setlabel(ptr));
				break;
#endif	/* FF_USE_LABEL */
#if FF_USE_MKFS
			case 'm' :	/* fm [<fs type> [<au size> [<align> [<n_fats> [<n_root>]]]]] - Create filesystem */
				{
					MKFS_PARM opt, *popt = 0;

					if (xatoi(&ptr, &p2)) {
						memset(&opt, 0, sizeof opt);
						popt = &opt;
						popt->fmt = (BYTE)p2;
						if (xatoi(&ptr, &p2)) {
							popt->au_size = p2;
							if (xatoi(&ptr, &p2)) {
								popt->align = p2;
								if (xatoi(&ptr, &p2)) {
									popt->n_fat = (BYTE)p2;
									if (xatoi(&ptr, &p2)) {
										popt->n_root = p2;
									}
								}
							}
						}
					}
					xprintf("The volume will be formatted. Are you sure? (Y/n)=");
					xgets(Line, sizeof Line);
					if (Line[0] == 'Y') put_rc(f_mkfs(path, popt, Buff, sizeof Buff));
					break;
				}
#endif	/* FF_USE_MKFS */
			case 'z' :	/* fz [<size>] - Change/Show R/W length for fr/fw/fx command */
				if (xatoi(&ptr, &p1) && p1 >= 1 && p1 <= (long)sizeof Buff)
					blen = p1;
				xprintf("blen=%u\n", blen);
				break;
			}
			break;

		case 't' :	/* t [<year> <mon> <mday> <hour> <min> <sec>] - Set/Show RTC */
			if (xatoi(&ptr, &p1)) {
				rtc.year = (WORD)p1;
				xatoi(&ptr, &p1); rtc.month = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.mday = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.hour = (BYTE)p1;
				xatoi(&ptr, &p1); rtc.min = (BYTE)p1;
 				if (!xatoi(&ptr, &p1)) break;
				rtc.sec = (BYTE)p1;
				rtc_settime(&rtc);
			}
			rtc_gettime(&rtc);
			xprintf("%u/%u/%u %s %02u:%02u:%02u\n", rtc.year, rtc.month, rtc.mday, &days[rtc.wday*4], rtc.hour, rtc.min, rtc.sec);
			break;
		case '.' :xprintf("exit");return;
		}
	}
}
















/*********************************************END OF FILE**********************/
