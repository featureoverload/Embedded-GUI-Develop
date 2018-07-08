/*
 *
 */

#include "web.h"
//#include "CGIDebugLogc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	char *msg = "Hello CGI Debug Log!\n";
	tcdbg_printf(msg, strlen(msg));

	return 0;
}

