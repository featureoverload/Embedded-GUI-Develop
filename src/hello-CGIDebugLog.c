/* hello-CGIDebugLog.c
 * 
 * build:
 * gcc hello-CGIDebugLog.c ../lib/libCGIDebugLogc.a -o hello-CGIDebugLog.cgi
 *
 */

#include <stdio.h>  /* stdout, stderr, stdin */
#include <stdlib.h>
#include <string.h>


int tcdbg_printf(char *message, int msg_len);

int main(int argc, char const *argv[])
{
	char dbg_message[128] = {'\0'};

	snprintf(dbg_message, sizeof(dbg_message), "HTTP head");
	tcdbg_printf(dbg_message, strlen(dbg_message));
	// HTTP head
	//printf("\n")

	printf("\n");

	memset(dbg_message, '\0', sizeof(dbg_message));
	snprintf(dbg_message, sizeof(dbg_message), "HTTP Contents");
	tcdbg_printf(dbg_message, strlen(dbg_message));
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

	memset(dbg_message, '\0', sizeof(dbg_message));
	snprintf(dbg_message, sizeof(dbg_message), "====  CGI END! ======");
	tcdbg_printf(dbg_message, strlen(dbg_message));

	return 0;
}


