/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2015.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 37 */

/* test_become_daemon.c

   Test our becomeDaemon() function.
*/
#if 0
#include "become_daemon.h"
#include "./include/tlpi_hdr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     /* STDIN_FILENO - 0, STDOUT_FILENO - 1, STDERR_FILENO - 2; 
                        *  reference: <APUE> ch3 &3.2
                        * */
#include <fcntl.h>      /* int open(); */

#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    int fd_out = open("/dev/tty", O_RDWR);
    printf("fd_out = %d\n", fd_out);
    if (fd_out == -1)
    { /* 'fd' -1 open fail */

        exit(1);
    }
    char buff[128] = {'\0'};
    snprintf(buff, sizeof(buff), "hello world\n");
    write(fd_out, buff, strlen(buff));
    close(fd_out);

    #if 0
     becomeDaemon(0);
    #else
    int flags = 0;
    int maxfd, fd;

    switch (fork()) { /* Become background process */
        case -1:
            return -1;
        case 0:
            break; /* Child falls through... */
        default:
            _exit(EXIT_SUCCESS); /* while parent terminates */
    }

    if (setsid() == -1) /* Become leader of new session */
        return -1;

    switch (fork()) { /* Ensure we are not session leader */
        case -1:
            return -1;
        case 0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0))
        umask(0); /* Clear file mode creation mask */

    if (!(flags & BD_NO_CHDIR))
        chdir("/"); /* Change to root directory */

    if (!(flags & BD_NO_CLOSE_FILES)) { /* Close all open files */
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1)          /* Limit is indeterminate... */
            maxfd = BD_MAX_CLOSE; /* so take a guess */

        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS)) {
        close(STDIN_FILENO); /* Reopen standard fd's to /dev/null */

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO) /* 'fd' should be 0 */
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }
    #endif
    /* Normally a daemon would live forever; we just sleep for a while */
    sleep ( 5 );

    fd_out = open("/dev/tty", O_RDWR);
    //printf("fd = %d\n", fd_out);
    if (fd_out == -1)
    { /* 'fd' -1 open fail */
        // 当初在写 web 开发的库的时候，有试过 *.cgi 进程打开 tty 向某个终端输出， 但是失败了，
        // 这里出现的现象一定是一样的，因为都是 daemon 进程， 所以刚刚没有想到这一点是我的错！
        // 这里应该通过写文件 log 的方式来输出信息。至于想要输出到 tty？ 还有待学习研究。
        exit(1);
    }
    snprintf ( buff, sizeof(buff), "hello daemon\n" );
    write(fd_out, buff, strlen(buff));
    close(fd_out);

    sleep((argc > 1) ? getInt(argv[1], GN_GT_0, "sleep-time") : 20);



    exit(EXIT_SUCCESS);
}

#else
#include "become_daemon.h"
#include "tlpi_hdr.h"
	
    void run();

	int
	main(int argc, char *argv[])
	{
	    becomeDaemon(0);
	
	    /* Normally a daemon would live forever; we just sleep for a while */
        run();
	   // sleep((argc > 1) ? getInt(argv[1], GN_GT_0, "sleep-time") : 20);
	
	    exit(EXIT_SUCCESS);

    }

    void run(){
        FILE *pf = fopen("/tmp/foo.daemon", "at+");
        if (pf == NULL)
        {
            exit(1);
        }
        char buff[256] = {'\0'};
        while (1)
        {
            memset(buff, '\0', sizeof(buff));
            fgets(buff, sizeof(buff), pf); // move fseek piont to the end of file.
            fprintf(pf, "heheda\n");

            sleep(5);
        }
        fclose(pf);
    }
#endif