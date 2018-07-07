/* HTML-hello.c
 * 
 * build:
 * gcc HTML-hello.c -o HTML-hello.cgi
 *
 */

#include <stdio.h>  /* stdout, stderr, stdin */
#include <stdlib.h>
#include <string.h>



int main(int argc, char const *argv[])
{
	// HTTP head
	//printf("\n")

	printf("\n");
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

	return 0;
}


