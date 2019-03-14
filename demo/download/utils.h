#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

/****************************
 *  CGI Debug Log solution  *
 ****************************/
// #define __EMBEDDED_PLATFORM /* 仅在嵌入式平台在编译运行时注释掉这一行。 */
#ifndef __EMBEDDED_PLATFORM
#include <stdarg.h>

#include <arpa/inet.h>
#include <sys/socket.h> /* should notice the sys/ directory. */

int tcdbg_printf(char *message, int msg_len);
void CGI_Println(const char *fmt, ...);

#else
#define CGI_Println(fmt, arg...) do { \
	FILE *fp; \
	if((fp=fopen(DEVICE,"a"))==NULL)\
	{\
		printf("open failed "); fclose(fp);\
	}\
	fprintf(fp, "%s[%d]. "fmt,__FUNCTION__, __LINE__, ##arg); \
	fprintf(fp, "\n"); fclose(fp);\
} while(0)
#endif



#define STATUS_READY       0
#define ERR_DOWNLOAD_FAIL -1


int handler_submit(void);



#endif /* ndef __UTILS_H__ */