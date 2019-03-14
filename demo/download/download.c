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

#include "cgic.h"

#include "utils.h"


int handler_display(int *);


int cgiMain(){
	CGI_Println("cgiMain running...");
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

	char *html_file = "cgi_download.html";

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
