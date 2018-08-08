/**
 * file name: getEnvir.c
 * Author   : Joseph Lin
 * E-mail   : joseph.lin@aliyun.com
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "CGIDebugLogc.h"
#include "flate.h"

/**
 * functions define:
 */


/**
 *  Global variables:
 */
int doDebug = 1;

/* ./var/www/cgi-bin/ -> ../ ==> ./var/www/environment-variables.html */
#define HTML_PATH  "../environment-variables.html"  

#define  HEADER_ZONE 		"header_env_zone"
#define  JAVASCRIPT_ZONE 	"javascript_env_zone"
#define  FORM_ZONE 			"form_env_zone"


int show_head( char *tplName );
int show_javascript( char *tplName );
int show_form( char *tplName );

int showHTML();



/**
 * running logical:
 */
int cgiMain(int argc, char *argv[])
{
	CGIPrint("\nrunning cgiMain()\n");

	// CGIPrint("\n --- now try to get Browser Request METHOD ----");

	// char method[30] = {0};
	// if(getenv("REQUEST_METHOD")) {
	// 	strcpy(method, getenv("REQUEST_METHOD"));
	// }
	// CGIPrint("[Debug] > Request METHOD is: %s", method);


	// if(strcmp(method,"POST") == 0) { //REQUEST_METHOD = POST
	// 	return 1;
	// }

	CGIPrint( "HTTP head" );
	// HTTP head
	//printf("\n")

	printf("\n");

	CGIPrint( "HTTP Contents 👇");
	showHTML();

	CGIPrint( "====  CGI END! ======");

	return 0;
}


int show_head( char *tplName ){
	
	CGIPrint( "\n running show_head()\n");

	templateSetFile(tplName);			CGIPrint ("\t templateSetFile(tplName); finish! \n");

	templateSetVar( HEADER_ZONE, "");	CGIPrint ("\t templateSetVar( HEADER_ZONE , \"\"); finish\n");


	///// database
	// char menuType[32] = {0};
	// tcapi_get("WebCurSet_Entry", "menuType", menuType);
	// templateSetVar("menuType", menuType);
	//templateSetVar("", "");

	templatePrint();		CGIPrint("\t templatePrint(); finish\n");
	templateFreeMem();		CGIPrint ( "\t templateFreeMem(); finish\n");

	return 0;
}

int showHTML(){
	show_head(HTML_PATH);
	show_javascript(HTML_PATH);
	show_form(HTML_PATH);
	return 0;
}

int show_javascript( char *tplName ){
	templateSetFile( tplName );
	templateSetVar ( JAVASCRIPT_ZONE, "");

	//// database
	// char pwd[128] = {0};
	// tcapi_get("Info_devDefInf", "Password0", pwd);
	// templateSetVar("Password0", pwd);
	//templateSetVar("", "");


	templatePrint  ();
	templateFreeMem();
}


int show_form( char *tplName ){
	templateSetFile( tplName );
	templateSetVar ( FORM_ZONE, "");

	//// database	
	// char menuType[32] = {0};

	// tcapi_get("WebCurSet_Entry", "menuType", menuType);
	// templateSetVar("menuType", menuType);
	// HTTP contents

	templateSetVar("v_port", getenv("SERVER_PORT") );	    		//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "SERVER_PORT",     getenv("SERVER_PORT") );
	templateSetVar("v_serverName", getenv("SERVER_NAME") );			//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "SERVER_NAME",     getenv("SERVER_NAME") );
	templateSetVar("v_reqHost", getenv("HTTP_HOST") );				//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "HTTP_HOST",       getenv("HTTP_HOST") );
	templateSetVar("v_reqMethod", getenv("REQUEST_METHOD") );		//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "REQUEST_METHOD",  getenv("REQUEST_METHOD") );
	templateSetVar("v_serverProtocol", getenv("SERVER_PROTOCOL") );	//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "SERVER_PROTOCOL", getenv("SERVER_PROTOCOL") );
	templateSetVar("v_serverSoftware", getenv("SERVER_SOFTWARE") );	//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "SERVER_SOFTWARE", getenv("SERVER_SOFTWARE") );
	templateSetVar("v_userAgent", getenv("HTTP_USER_AGENT") );		//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "HTTP_USER_AGENT", getenv("HTTP_USER_AGENT") );
	templateSetVar("v_gatewayInterface", getenv("GATEWAY_INTERFACE") );	//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "GATEWAY_INTERFACE ", getenv("GATEWAY_INTERFACE ") );
	templateSetVar("v_PATH", getenv("PATH") );						//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "PATH",            getenv("PATH") );
	templateSetVar("v_scriptName", getenv("SCRIPT_NAME") );			//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "SCRIPT_NAME",     getenv("SCRIPT_NAME") );
	templateSetVar("v_remoteAddr", getenv("REMOTE_ADDR") );			//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "REMOTE_ADDR",     getenv("REMOTE_ADDR") );
	templateSetVar("v_libPATH", getenv("LD_LIBRARY_PATH") );		//printf( "\t\t<li><span style='color:green'>%-30s </span> : %s </li>\n", "LD_LIBRARY_PATH",     getenv("LD_LIBRARY_PATH") );



	templatePrint();
	templateFreeMem();
}
