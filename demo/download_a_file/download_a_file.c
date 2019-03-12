/*****************************
 * Build:
 *   $ make all
 * 
 * Install:
 *   $ sudo make install
 *
 * Clean:
 *   $ make clean
 *   $ sudo rm -f /var/www/cgi-bin/*download_a_file.*
 * 
 * Note:
 *   you may need to run:
 *   $ sudo ln -s /usr/include/x86_64-linux-gnu/sys/ /usr/include/sys
 *   ### /\ example of Ubuntu 64bit /\
 * 
******************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "cgic.h"

#include <sys/stat.h>
#include <sys/types.h>


int cgiMain(){
	int ret = 0;

	// cgiWriteEnvironment("/CHANGE/THIS/PATH/capcgi.dat");
	cgiHeaderContentType("text/html");

	char *html_file = "cgi_download_a_file.html";

	FILE *fp = NULL;
	if ((fp = fopen(html_file, "r")) == NULL){
		// LOG ERROR;
		return 1;
	}

	struct stat _fstat;
	if ( stat(html_file, &_fstat) == -1 ){
		// log error;
		return 1;
	}
	// char html[10240] = {'\0'};
	char *html = NULL;
	long long read_size = 0;
	html = (char*)malloc(sizeof(char) * ((long long)_fstat.st_size + 1));
	read_size = fread(html, sizeof(char), (long long)_fstat.st_size, fp);
	html[(long long)_fstat.st_size] = '\0';
	if (read_size!=(long long)_fstat.st_size){
		// log err;
		free(html);
		html = NULL;
		return 1;
	}

	fprintf(cgiOut, "%s", html);

	fflush(stdout);


	free(html);
	fclose(fp);
	return ret;
}
