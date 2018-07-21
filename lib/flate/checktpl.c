/**************************************************************************
 *                                                                        *
 * Template Check 1.0 for Flate library                                   *
 * Copyright (C) 2001 Fabien Menemenlis (nihilist@dead-inside.org)        *
 *                                                                        *
 * This library is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU Lesser General Public             *
 * License as published by the Free Software Foundation; either           *
 * version 2.1 of the License, or (at your option) any later version.     *
 *                                                                        *
 * This library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 * Lesser General Public License for more details.                        *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with this library; if not, write to the Free Software    *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,                  *
 * MA  02111-1307  USA                                                    *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define TNAME 100
#define MAXFILENAMESZ 1024


enum {
    TEMP_HTML,
    TEMP_VAR,
    TEMP_BEGINTABLE,
    TEMP_ENDTABLE,
    TEMP_BEGINZONE,
    TEMP_ENDZONE,
    TEMP_BEGINGENTABLE,
    TEMP_ENDGENTABLE,
    TEMP_INCLUDEFILE
};


struct st_tpl {
    char *nomchamp;
    struct st_tpl *next;
}
*tempVar, *tempZone, *tempTable, *curtempVar, *curtempZone, *curtempTable;
int tempnbVar, tempnbZone, tempnbTable;


char *includefile(char *path, int *filesize) {
    FILE *f;
    char *buf;


    if ((f = fopen(path, "r")) == NULL) {
        return(NULL);
    }

    fseek(f, 0, SEEK_END);
    *filesize = ftell(f);
    rewind(f);
    buf = (char *)malloc(*filesize);
    if (!buf) {
        fclose(f);
        return(NULL);
    }
    fread(buf, 1, *filesize, f);
    fclose(f);

    return(buf);
}


void usage(void) {
    fprintf(stderr, "checktpl [-p root path] <template file to check>\n");
    exit(1);
}


int main(int argc, char *argv[]) {
    char tmp[TNAME + 1], nom[TNAME + 1];
    FILE *ftpl;
    int tplsize;
    int debut, pos, tempdebut;
    int type;
    int i;
    char *tempBuf;
    char *filebuf;
    int filesize;
    int c;
    char *rootpath = NULL;
    char cfilename[MAXFILENAMESZ + 1];
    
    
    while ((c = getopt(argc, argv, "p:")) != -1) {
        switch (c) {
            case 'p':
                rootpath = strdup(optarg);
                break;
            default:
                usage();
                break;
        }
    }
    argc -= optind;
    argv += optind;

    if (argc != 1) {
        usage();
    }

    if ((ftpl = fopen(argv[0], "r")) == NULL) {
        perror(argv[1]);
        return(1);
    }

    fseek(ftpl, 0, SEEK_END);
    tplsize = ftell(ftpl);
    rewind(ftpl);
    tempBuf = (char *)malloc(sizeof(char) * tplsize);
    fread(tempBuf, 1, tplsize, ftpl);
    fclose(ftpl);

    tempnbVar = tempnbZone = tempnbTable = 0;
    tempVar = tempZone = tempTable = NULL;

    /* give an error if ENDZONE/ENDTABLE happens before BEGINZONE/BEGINTABLE */
    /* give an error if BEGINZONE/BEGINTABLE happens with no
       BEGINZONE/BEGINTABLE after */
    /* check when there are multiple instances with the same name that there
       is a corresponding [BEGIN|END][ZONE|TABLE] */
    /* give a warning when a #UNKNOWN_COMMAND is encountered */
    /* report statistics like variable/zones/tables counts, file size */
    /* give an warning when a BEGIN|[ZONE|TABLE] encounters a END|[ZONE|TABLE]
       with a different name "this is probably not what you want" */

    tempdebut = 0;
    debut = pos = 0;
    while (pos < tplsize) {
        if (tempBuf[pos] == '#') {
            if (tempBuf[pos + 1] == '#') {
                /* the encountered code till there is pure html */
                tempdebut += pos - debut;

                /* from now on we're in the variable name */
                i = 0;
                pos += 2;
                while (pos + i < tplsize
                        && tempBuf[pos + i] != '#'
                        && i < TNAME)
                    i++;
                memcpy(tmp, tempBuf + pos, i);
                tmp[i] = '\0';

                tempnbVar++;
                pos += i + 1;
                debut = pos + 1;
            }
        }
        /* comments */
        else if (tempBuf[pos] == '<'
                 && tempBuf[pos + 1] == '!'
                 && tempBuf[pos + 2] == '-'
                 && tempBuf[pos + 3] == '-') {
            /* the encountered code till there is pure html */
            tempdebut += pos - debut;
            debut = pos;

            pos += 4;
            i = 0;
            while (pos < tplsize && isspace(tempBuf[pos]))
                pos ++;
            while (pos < tplsize && tempBuf[pos] != '-'
                    && tempBuf[pos] != '>'
                    && !isspace(tempBuf[pos])
                    && i < TNAME) {
                tmp[i++] = tempBuf[pos];
                pos++;
            }
            tmp[i] = '\0';
            while (pos < tplsize && isspace(tempBuf[pos]))
                pos++;
            i = 0;
            while (pos < tplsize
                    && tempBuf[pos] != '-'
                    && tempBuf[pos] != '>'
                    && !isspace(tempBuf[pos])
                    && i < TNAME) {
                nom[i++] = tempBuf[pos];
                pos++;
            }
            nom[i] = '\0';
            while (pos < tplsize && tempBuf[pos] != '>')
                pos++;

            type = -1;

            if (strcmp(tmp, "#INCLUDEFILE") == 0) {
                type = TEMP_INCLUDEFILE;
                if (rootpath) {
                    snprintf(cfilename, MAXFILENAMESZ, "%s/%s", rootpath, nom);
                    filebuf = includefile(cfilename, &filesize);
                } else
                    filebuf = includefile(nom, &filesize);
                if (filebuf) {
                    pos++;
                    tplsize += filesize - (pos - debut);
                    tempBuf = (char *)realloc(tempBuf, tplsize);
                    memmove(tempBuf + debut + filesize, tempBuf + pos, tplsize - pos - filesize);
                    memcpy(tempBuf + debut, filebuf, filesize);
                    free(filebuf);
                    continue;
                } else {
                    printf("ERROR: could not open file %s given in INCLUDEFILE.\n", nom);
                    return(1);
                }
            } else if (strcmp(tmp, "#BEGINTABLE") == 0)
                type = TEMP_BEGINTABLE;
            else if (strcmp(tmp, "#ENDTABLE") == 0)
                type = TEMP_ENDTABLE;
            else if (strcmp(tmp, "#BEGINZONE") == 0)
                type = TEMP_BEGINZONE;
            else if (strcmp(tmp, "#ENDZONE") == 0)
                type = TEMP_ENDZONE;

            if (type != -1) {
                switch (type) {
                case TEMP_BEGINTABLE:
                    curtempTable = (struct st_tpl *)malloc(sizeof(struct st_tpl));
                    curtempTable->next = tempTable;
                    curtempTable->nomchamp = strdup(nom);
                    tempTable = curtempTable;
                    tempnbTable++;
                    break;
                case TEMP_ENDTABLE:
                    curtempTable = tempTable;
                    if (curtempTable) {
                        if (strcmp(curtempTable->nomchamp, nom)) {
                            printf("ERROR: ENDTABLE %s is right after BEGINTABLE %s.\n",
                                   nom, curtempTable->nomchamp);
                            return(1);
                        }
                    } else {
                        printf("ERROR: ENDTABLE %s without BEGINTABLE\n", nom);
                        return(1);
                    }
                    curtempTable = tempTable->next;
                    free(tempTable->nomchamp);
                    free(tempTable);
                    tempTable = curtempTable;
                    break;
                case TEMP_BEGINZONE:
                    curtempZone = (struct st_tpl *)malloc(sizeof(struct st_tpl));
                    curtempZone->next = tempZone;
                    curtempZone->nomchamp = strdup(nom);
                    tempZone = curtempZone;
                    tempnbZone++;
                    break;
                case TEMP_ENDZONE:
                    curtempZone = tempZone;
                    if (curtempZone) {
                        if (strcmp(curtempZone->nomchamp, nom)) {
                            printf("ERROR: ENDZONE %s is right after BEGINZONE %s.\n",
                                   nom, curtempZone->nomchamp);
                            return(1);
                        }
                    } else {
                        printf("ERROR: ENDZONE %s without BEGINZONE\n", nom);
                        return(1);
                    }
                    curtempZone = tempZone->next;
                    free(tempZone->nomchamp);
                    free(tempZone);
                    tempZone = curtempZone;
                    break;
                }
            } else {
                /* the whole block is taken for html */
                tempdebut += pos + 1 - debut;
            }
            debut = pos + 1;
        }
        pos++;
    }

    if (tempTable) {
        printf("ERROR: table %s not closed.\n", tempTable->nomchamp);
        return(1);
    }
    if (tempZone) {
        printf("ERROR: zone %s not closed.\n", tempZone->nomchamp);
        return(1);
    }

    free(tempBuf);

    printf("%s: all ok\n", argv[0]);
    printf("%d variables\n%d zones\n%d tables\n", tempnbVar, tempnbZone,
           tempnbTable);

    return(0);
}
