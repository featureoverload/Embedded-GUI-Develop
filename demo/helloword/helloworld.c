/*****************************
 *
******************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "cgic.h"


int cgiMain(){
	int ret = 0;

	// cgiWriteEnvironment("/CHANGE/THIS/PATH/capcgi.dat");
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<h1>Hello World</h1>");

	fflush(stdout);

	return ret;
}
