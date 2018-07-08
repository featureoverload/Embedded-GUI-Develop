/* CGIDebugLogc.c
 * Author : Joseph Lin
 * E-mail : joseph.lin@aliyun.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> /* should notice the sys/ directory. */

#include "CGIDebugLogc.h"

const char *serverIP = "127.0.0.1"; // localhost.
const int serverPort = 21919;       // the default DebugLog server port. 

const int RET_SUCCESS = 0;
const int RET_FAILURE = -1;

/*
 *  msg_len: should not include '\0'; the actually length.
 */
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


