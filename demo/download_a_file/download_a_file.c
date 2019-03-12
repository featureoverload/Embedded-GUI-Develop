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

#define STATUS_READY       0
#define ERR_DOWNLOAD_FAIL -1

int handler_display(int *);
int handler_submit(void);

int cgiMain(){
	int ret = 0;
	int stat = STATUS_READY;

	typedef enum Act {ACT_INIT=0, DISPLAY, SUBMIT, Invalid} Act;
	Act act = ACT_INIT;
	int client_act = 0;

	cgiFormInteger("act", &client_act, (int)DISPLAY);
	act = (Act)client_act;

	switch(act){
		case SUBMIT:
			stat = handler_submit();
			return 0;
		case DISPLAY:
			handler_display(&stat);
			break;
		case Invalid:
		default:
			break;
	}
	return ret;
}


int handler_display(int *stat_p){
	int ret = 0;

	if (*stat_p<0){
		// pass
	}

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


#define FILENAME "index.html"
#define FILEPATH "/var/www/html/"

int handler_submit(void){
	int ret = ERR_DOWNLOAD_FAIL;

	/*
	char cmd[256] = {'\0'};
	snprintf(cmd, 255, "%s", "/bin/save_file");
	ret = system(cmd);
	if (ret!=0){
		// DPRINT();
		return ret;
	}
	*/
	ret = 0;

	printf("Pragma: no-cache\n");
	printf("Cache-control: no-cache\n");
	printf("Content-type: application/octet-stream\n");
	printf("Content-Transfer-Encoding: binary\n");

	printf("Content-Disposition: attachment; filename=\"%s\"\n\n",
		   FILENAME);
	fflush(stdout);

	FILE *fp = NULL;
	char file_fullpath[256] = {'\0'};
	snprintf(file_fullpath, 255, "%s%s", FILEPATH, FILENAME);
	if ((fp=fopen(file_fullpath, "rb"))==NULL){
		// log error
		return ERR_DOWNLOAD_FAIL;
	}

	struct stat _fstat;
	if ( stat(file_fullpath, &_fstat) == -1 ){
		// log error;
		return ERR_DOWNLOAD_FAIL;
	}

	char *download_buffer = NULL;
	long long read_size = 0;
	download_buffer = (char*)malloc(sizeof(char) * ((long long)_fstat.st_size + 1));
	read_size = fread(download_buffer, sizeof(char), (long long)_fstat.st_size, fp);
	download_buffer[(long long)_fstat.st_size] = '\0';
	if (read_size!=(long long)_fstat.st_size){
		// log err;
		free(download_buffer);
		download_buffer = NULL;
		return ERR_DOWNLOAD_FAIL;
	}

	if (write(STDOUT_FILENO, download_buffer, (long long)_fstat.st_size) != (long long)_fstat.st_size){
		// log error
		return ERR_DOWNLOAD_FAIL;
	}

	fflush(stdout);

	free(download_buffer);
	fclose(fp);
	return ret;
}
