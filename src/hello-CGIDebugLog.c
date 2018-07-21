/* hello-CGIDebugLog.c
 * 
 * build:
 * gcc hello-CGIDebugLog.c ../lib/libCGIDebugLogc.a -o hello-CGIDebugLog.cgi
 *
 */

#include <stdio.h>  /* stdout, stderr, stdin */
#include <stdlib.h>
#include <string.h>

// #define __EMBEDDED_PLATFORM /* 仅在嵌入式平台在编译运行时注释掉这一行。 */
#ifndef __EMBEDDED_PLATFORM
	#include <stdarg.h>

	int tcdbg_printf(char *message, int msg_len);

	void CGI_DEBUG_LOG(const char *fmt, ...){
		va_list ap;
		va_start(ap, fmt);

	char dbg_message[128] = {'\0'};
		/*ret =*/ vsprintf(dbg_message, fmt, ap);

	tcdbg_printf(dbg_message, strlen(dbg_message));

		va_end(ap);
	}
#else
	#define CGI_DEBUG_LOG( fmt, ...) /* other code... */
#endif


int main(int argc, char const *argv[])
{

	CGI_DEBUG_LOG( "HTTP head" );
	// HTTP head
	//printf("\n")

	printf("\n");

	CGI_DEBUG_LOG( "HTTP Contents");
	// HTTP contents
	printf("<!DOCTYPE html>\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<meter charset=\"utf-8\"></meter>\n");
	printf("<title>Hello</title>\n");
	printf("</head>\n");

	printf("<body>\n");
	printf("\t<p>Hello HTML</p>\n");
	printf("\t<p>This is CGI program.</p>\n");
	printf("</body>\n");
	printf("</html>\n");

	CGI_DEBUG_LOG( "====  CGI END! ======");

	return 0;
}


