/**
 * file name: echo2ternimal.c
 * author   : joseph 
 * e-mail   : joseph.lin@aliyun.com
 *
 *
 */


/**
 *  Exit code:
 *  219: command or USER Enter error
 *  210: device or file operate error
 *  215: Unknow error
 *  209: function call error.
 */

/**
 *  Head files
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
//#include <tlpi_hdr.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

#include "become_daemon.h"
#include "tlpi_hdr.h"
	

#include <unistd.h>          /* getcwd() need this! */
/**** APUE function *****/
#include <errno.h>
#include <limits.h>

#ifdef PATH_MAX
	static long pathmax = PATH_MAX;
#else
	static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;
/* If PATH_MAX is indeterminate, no guarantee this is adequate */
#define PATH_MAX_GUESS 1024

char *path_alloc(size_t *sizep){
	char    *ptr;
	size_t  size;

	if ( posix_version == 0 )
		posix_version = sysconf(_SC_VERSION);

	if ( xsi_version == 0 )
		xsi_version = sysconf( _SC_XOPEN_VERSION );

	if ( pathmax == 0 )	{ /* first time through */
		errno = 0;
		if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
			if (errno == 0)
				pathmax = PATH_MAX_GUESS; /* it’s indeterminate */
			else{
				fprintf(stderr, "pathconf error for _PC_PATH_MAX");
				exit(209);
			}
		} else {
			pathmax++; /* add one since it’s relative to root */
		}
	}
	/*
	* Before POSIX.1-2001, we aren’t guaranteed that PATH_MAX includes
	* the terminating null byte. Same goes for XPG3.
	*/
	if ((posix_version < 200112L) && (xsi_version < 4))
		size = pathmax + 1;
	else
		size = pathmax;
	if ((ptr = malloc(size)) == NULL){
		fprintf(stderr, "malloc error for pathname");
		exit(209);
	}
	if (sizep != NULL)
		*sizep = size;

	return(ptr);
}
/**** end APUE  ********/

/**
 *  Global Variables
 */

///// josutils.h
// typedef enum { TRUE = 0, FALSE } Boolean_t;	/* defined in tlpi_hdr.h */

/* EXIT_SUCCESS was defined on stdlib.h */
typedef enum EXIT_CODE { 
	BUILDIN_FUNC_ERR = 209,
	CMD_ERR = 210,
	FIEL_IO_ERR = 219,
	COMMON_ERR = 1,
	FUNC_ERR = -1,
	SERIOUS_ERR = 911
 } EXIT_CODE_t;

//// END josutils.h  ///////

int doDebug = FALSE;




/**
 *  Funcitons define
 */
//int run(int argc, char *argv[]);
int run(int argc, char *argv[], char *ptr);


int main( int argc, char *argv[] )
{
	#if 0
	if ( argc != 3 ){
		fprintf ( stderr, "usage: $ %s <device> <text>\n", argv[0] );
		fprintf ( stderr, "\t like: $ %s /dev/ttyS0 \"this content will show on termial of /dev/ttyS0\"\n", argv[0]);
		exit(219);
	}
	#else
	if ( argc < 2 ){
		fprintf ( stderr, "usage: $ %s <TTY device> [option...]\n", argv[0] );
		fprintf ( stderr, "\t like: $ %s /dev/ttyS0\n", argv[0]);
		fprintf ( stderr, "\n\t [Option...]: Enter the path which is CGIDebugLogd.py file exist on. \n" 
						  "\t this program will default open CGIDebugLogd.py on the same PATH with \"%s\"! \n\n", argv[0]);
		exit(CMD_ERR);
	}
	#endif


		char   *ptr=NULL;
		size_t size;
	
		ptr = path_alloc(&size);                         /* APUE funciton. */
		if (getcwd(ptr, size) == NULL){
			fprintf ( stderr, "getcwd() failed!\n");
			exit(BUILDIN_FUNC_ERR);
		}

	if (doDebug){
		fprintf (stdout, "cwd = %s\n", /*current work path*/ptr);
	}


	becomeDaemon(0);

	//run(argc, argv);
	run(argc, argv, ptr);

	exit(0);
}

#if 0
int run ( int argc, char *argv[] )
{
	FILE *dbgLogFd = NULL;
	if ( (dbgLogFd=fopen("/tmp/debug.log", "w")) == NULL ){
		exit(210);
	}

	if ( argc != 3 ){
		fprintf ( dbgLogFd, "usage: $ %s <device> <text>\n", argv[0] );
		fprintf ( dbgLogFd, "\t like: $ %s /dev/ttyS0 \"this content will show on termial of /dev/ttyS0\"\n", argv[0]);
		exit(219);
	}

	fprintf ( dbgLogFd, "hello echo2ternimal\n"); 

	fprintf ( dbgLogFd, "\n==== start open %s ======\n", argv[1]);
	int    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	mode_t filePerms = S_IRUSR | S_IWUSR  | S_IRGRP | S_IWGRP |
					   S_IROTH | S_IWOTH;							/* rw-rw-rw- */
	//int tmnlFd = open( argv[1], openFlags, filePerms );					// ~~write only~~
	int tmnlFd = open( argv[1], O_WRONLY);
	if ( tmnlFd == -1 ){
		fprintf ( dbgLogFd, "open %s fail\n", argv[1]);
		exit(210);
	}
	fprintf (dbgLogFd, "      open %s finished!     \n", argv[1]);

	// 获取 argv[2] 的长度
	ssize_t numEcho = strlen(argv[2]);
	if (doDebug)
		fprintf( dbgLogFd, "\nlength of argv[2] is: %d\n", (int)numEcho);
	
	int CNT=0;
	while(doDebug){
	// 写入到 terminal 中去，
	int numWrt = write(tmnlFd, argv[2], numEcho);
	// 判断写入到 terminal 中去的长度是否符合 argv[2] 的长度
	if ( numWrt != (int)numEcho ){
		fprintf(dbgLogFd, "%s \n", "wite fail!" );
		exit(210);
	}

	sleep(3);
	CNT++;
	if (CNT==10){
		doDebug=0;
	}

	}//end while

	if (close(tmnlFd)==-1){
		fprintf(dbgLogFd, "close tmnlFd fail\n" );
		exit(210);
	}
	fclose(dbgLogFd);

	return 0;
}

#else
	#if 0 /* for github-Eebedded_GUI_Develop dir struct */
		#define CGIDEBUGLOG_SRC_RELATIVE_PATH "/src/CGIDebugLogd.py"
	#else
		/* code struct:
		 *  WorkPath/
		 *     |---- bin/
		 *     |      |---- CGIDebugLogd.py
		 *     |      `---- daemonEcho2*
		 *     |---- var/
		 *     `---- etc/
		 */
		#define CGIDEBUGLOG_SRC_RELATIVE_PATH "/bin/CGIDebugLogd.py"
		/* $ cd WorkPath
		 * $ ./bin/daemonEcho2 `tty`
		 * pwd=> /home/USER/<path>/WorkPath
		 * argv[0] => ./bin/daemonEcho2
		 * absolute path = pwd + argv[0]
		 * -[o] fix absolute path.
		 */
	#endif
int run ( int argc, char *argv[], char *ptr )
{
	char  IsExeclError = EXIT_SUCCESS;
	char program_path[218] = {'\0'};
	/*
	 * 经过 daemon 化之后， work path 变成了 “/”; 后面“APP 规范化”之后， 会修正的。
	 */
	/// argc could not < 2
	if( argc = 2) { // default CGIDebugLogd.py path. 
		snprintf ( program_path, sizeof(program_path), "%s%s", ptr, CGIDEBUGLOG_SRC_RELATIVE_PATH);
	}else { // Specfaction path.
		snprintf( program_path, sizeof(program_path), "%s", argv[2] ); // 0:programName 1:TTY  2:PATH
		// - [o] joseph, check CGIDebugLogd.py exist on the path.
		// ...code...
	}

	/*
	 * APP 规范化， 配置 log 路径。
	 */
	FILE *pErrorFile=NULL;
	pErrorFile = fopen( "/tmp/debug.log", "a+" );
	if ( pErrorFile == NULL )   exit(1);
	fprintf(pErrorFile, "\nprogram_path: %s\n", program_path );
	fclose(pErrorFile);

	/*
	 * equal to ==>  $ python3 CGIDebugLogd.py /dev/ttyS0 
	 */
	// IsExeclError = execl( "/usr/bin/python3", "/usr/bin/python3", program_path, argv[1], argv[2],NULL);
	IsExeclError = execl( "/usr/bin/python3", "/usr/bin/python3", program_path, argv[1], NULL);
	if (IsExeclError == -1 ){
		exit(SERIOUS_ERR);
	}

	return EXIT_SUCCESS;
}
#endif



