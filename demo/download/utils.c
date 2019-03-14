/**********
 *
 **********/

#include "utils.h"

/****************************
 *  CGI Debug Log solution  *
 ****************************/
// #define __EMBEDDED_PLATFORM /* 仅在嵌入式平台在编译运行时注释掉这一行。 */
#ifndef __EMBEDDED_PLATFORM
	const char *serverIP = "127.0.0.1"; // localhost.
	const int serverPort = 21919;       // the default DebugLog server port. 

	const int RET_SUCCESS = 0;
	const int RET_FAILURE = -1;

	int tcdbg_printf(char *message, int msg_len){
		int sock = socket(PF_INET, SOCK_STREAM, 0);
		if ( sock==-1 )	return RET_FAILURE;

		struct sockaddr_in serv_addr;
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(serverIP); // IP: server ip
		serv_addr.sin_port = htons( serverPort );

		if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
			return RET_FAILURE;

		int send_len = write(sock, message, msg_len);

		close(sock);

		return RET_SUCCESS;
	}


	void CGI_Println(const char *fmt, ...){
		va_list ap;
		va_start(ap, fmt);

		char dbg_message[128] = {'\0'};
		/*ret =*/ vsprintf(dbg_message, fmt, ap);

		tcdbg_printf(dbg_message, strlen(dbg_message));

		va_end(ap);
	}
#endif


int dump_http_head(char *filename){
	printf("Pragma: no-cache\n");
	printf("Cache-control: no-cache\n");
	printf("Content-type: application/octet-stream\n");
	printf("Content-Transfer-Encoding: binary\n");

	printf("Content-Disposition: attachment; filename=\"%s\"\n\n",
		   filename);
	fflush(stdout);

	return 0;
}

int dump_http_FileStream(char *file_fullpath){
	FILE *fp = NULL;

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

	return 0;
}

int prepare_download_file(){
	/*
	char cmd[256] = {'\0'};
	snprintf(cmd, 255, "tar zcvf %s -C /tmp/ heheda.txt", file_fullpath);

	// char tmp[256] = {'\0'};
	// snprintf(tmp, 255, "echo '%s' > /dev/pts/5", cmd);
	// if((ret=system(tmp)) != 0){
	if((ret=call_cmd("/bin/cat", 1, 1, "/tmp/heheda.txt")) != 0){
		return ret;
	}

	ret = system(cmd);
	if (ret!=0){
		return ret;
	}
	sleep(1);
	*/
	return 0;
}


// #define FILENAME "all_pids.tar.gz"
// #define FILEPATH "/tmp/"

// Test:
#define FILENAME "test.png"
#define FILEPATH "/tmp/"

int handler_submit(void){
	int ret = ERR_DOWNLOAD_FAIL;
	char file_fullpath[256] = {'\0'};

	snprintf(file_fullpath, 255, "%s%s", FILEPATH, FILENAME);
	CGI_Println("file_fullpath: %s", file_fullpath);

	if ((ret = prepare_download_file()) == 0){
		CGI_Println("prepare_download_file => ret: %d", ret);
		if ((ret = dump_http_head(FILENAME)) == 0){
			CGI_Println("dump_http_head => ret: %d", ret);
			ret = dump_http_FileStream(file_fullpath);
		}
		// exit(ret);
	}
	CGI_Println("handler_submit return ret: %d", ret);
	return ret;
}
