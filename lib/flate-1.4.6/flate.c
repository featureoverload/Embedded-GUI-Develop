/**************************************************************************
 *                                                                        *
 * Flate library 1.4.6 (Fast Template)                                    *
 * Copyright (C) 2013 Fabien Menemenlis (nihilist@dead-inside.org)        *
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
#ifdef _USE_FETCH_
#include <sys/param.h>
#include <fetch.h>
#endif

#include "flate.h"


#define VARSZ 256


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

typedef struct tempUnit {
    struct tempUnit *next;
    struct tempUnit *prev;
    char *data;
    char *name;
    int tdata;
    int type;
    int alloc;
}
tempUnit;

typedef struct {
    char *name;
    tempUnit *unit;
}
st_ptr;


static tempUnit *UnitInit;
static st_ptr *tempVar, *tempZone, *tempTable;
static int tempnbVar, tempnbZone, tempnbTable;

static char *tempBuf;
static char *curzone;
static char *curtable;


int tempcompare(const void *a, const void *b) {
    return(strcmp(((st_ptr *)a)->name, ((st_ptr *)b)->name));
}


int tempcompname(const void *a, const void *b) {
    return(strcmp((char *)a, ((st_ptr *)b)->name));
}


/*
 * seeking function: looks for a value in a shortcut table
 * (tempVar, tempZone, tempTable)
 */
void *tempsearch(const void *elem, const void *base, size_t nbelem,
                 size_t size, int (*comparefunc)(const void *, const void *),
                 int *nb) {
    void *ptr;
    int pos;


    *nb = 0;
    ptr = bsearch(elem, base, nbelem, size, comparefunc);
    if (!ptr)
        return(NULL);
    pos = ((char *)ptr - (char *)base) / size;
    /* go back to first entry of the sorted table */
    while (pos > 0
            && comparefunc(elem, base + (pos - 1 ) *size) == 0)
        pos--;
    /* computes number of entries in the table */
    while (pos + *nb < nbelem
            && comparefunc(elem, base + (pos + *nb) * size) == 0)
        (*nb)++;

    return((void *)(((char *)base) + pos * size));
}


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


#ifdef _USE_FETCH_
/*
 * you'd better be sure there's no buffer overflow possible with this
 * library before using this feature...
 */
int templateSetFileURL(char *filename) {
    FILE *f;
    char *ptr;
    int pos, size, start;
    int tempstart;
    char tmp[VARSZ + 1], name[VARSZ + 1];
    int i;
    int nbvar;
    int type;
    tempUnit *Unit;
    struct url_stat stat;
    char *filebuf;
    int filesize;
    int tocopy;


    if ((f = fetchXGetURL(filename, &stat, "")) == NULL) {
        return(-1);
    }

    templateFreeMem();
    size = stat.size;
    tempBuf = (char *)malloc(sizeof(char) * (size + 1));
    fread(tempBuf, 1, size, f);
    tempBuf[size] = '\0';
    fclose(f);

    tempnbVar = tempnbZone = tempnbTable = 0;
    tempVar = tempZone = tempTable = NULL;
    UnitInit = (tempUnit *)malloc(sizeof(tempUnit));
    memset(UnitInit, 0, sizeof(tempUnit));
    Unit = UnitInit;
    nbvar = 0;
    tempstart = 0;
    start = pos = 0;
    while (pos < size) {
        if (tempBuf[pos] == '#') {
            if (tempBuf[pos + 1] == '#') {
                /* the encountered code till there is pure html */
                memmove(tempBuf + tempstart, tempBuf + start, pos - start);
                i = 0;
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->data = (char *)malloc(pos - start);
                memcpy(Unit->data, tempBuf + tempstart, pos - start);
                Unit->tdata = pos - start;
                Unit->type = TEMP_HTML;
                Unit = Unit->next;
                tempstart += pos - start;

                /* from now on we're in the variable name */
                pos += 2;
                while (tempBuf[pos + i] != '\0'
                        && tempBuf[pos + i] != '#'
                        && i < VARSZ)
                    i++;
                memcpy(tmp, tempBuf + pos, i);
                tmp[i] = '\0';

                /* saves the unit containing the variable */
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->type = TEMP_VAR;
                Unit->name = strdup(tmp);
                /* adds the variable in the shortcut table */
                tempVar = (st_ptr *)realloc(tempVar, sizeof(st_ptr) * (tempnbVar + 1));
                tempVar[tempnbVar].name = strdup(tmp);
                tempVar[tempnbVar].unit = Unit;
                tempnbVar++;

                Unit = Unit->next;
                nbvar++;
                pos += i + 1;
                start = pos + 1;
            }
        }
        /* comments */
        else if (tempBuf[pos] == '<'
                 && tempBuf[pos + 1] == '!'
                 && tempBuf[pos + 2] == '-'
                 && tempBuf[pos + 3] == '-') {
            /* the encountered code till there is pure html */
            memmove(tempBuf + tempstart, tempBuf + start, pos - start);
            i = 0;
            Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
            memset(Unit->next, 0, sizeof(tempUnit));
            Unit->next->prev = Unit;
            Unit->data = (char *)malloc(pos - start);
            memcpy(Unit->data, tempBuf + tempstart, pos - start);
            Unit->tdata = pos - start;
            Unit->type = TEMP_HTML;
            Unit = Unit->next;
            tempstart += pos - start;
            start = pos;

            pos += 4;
            i = 0;
            while (isspace(tempBuf[pos]))
                pos++;
            while (pos < size && tempBuf[pos] != '-'
                    && tempBuf[pos] != '>'
                    && !isspace(tempBuf[pos])
                    && i < VARSZ) {
                tmp[i++] = tempBuf[pos];
                pos++;
            }
            tmp[i] = '\0';
            while (isspace(tempBuf[pos]))
                pos++;
            i = 0;
            while (pos < size && tempBuf[pos] != '-'
                    && tempBuf[pos] != '>'
                    && !isspace(tempBuf[pos])
                    && i < VARSZ) {
                name[i++] = tempBuf[pos];
                pos++;
            }
            name[i] = '\0';
            while (pos < size && tempBuf[pos] != '>')
                pos++;

            type = -1;
            if (strcmp(tmp, "#INCLUDEFILE") == 0) {
                type = TEMP_INCLUDEFILE;
                filebuf = includefile(name, &filesize);
                if (filebuf) {
                    pos++;
                    tocopy = size - pos;
                    size += filesize - (pos - start);
                    tempBuf = (char *)realloc(tempBuf, size);
                    memmove(tempBuf + start + filesize, tempBuf + pos, tocopy);
                    memcpy(tempBuf + start, filebuf, filesize);
                    free(filebuf);
                    pos = start;
                    continue;
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
                /* saves the unit containing the variable */
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->type = type;
                Unit->next->prev = Unit;
                if (Unit->type != TEMP_HTML)
                    Unit->name = strdup(name);
                switch (type) {
                case TEMP_BEGINTABLE:
                    tempTable = (st_ptr *)realloc(tempTable,
                                                  sizeof(st_ptr) * (tempnbTable + 1));
                    tempTable[tempnbTable].name = strdup(name);
                    tempTable[tempnbTable].unit = Unit;
                    tempnbTable++;
                    break;
                case TEMP_BEGINZONE:
                    tempZone = (st_ptr *)realloc(tempZone,
                                                 sizeof(st_ptr) * (tempnbZone + 1));
                    tempZone[tempnbZone].name = strdup(name);
                    tempZone[tempnbZone].unit = Unit;
                    tempnbZone++;
                    break;
                }
                Unit = Unit->next;
            } else {
                /* the whole unit is taken for html */
                memmove(tempBuf + tempstart, tempBuf + start, pos + 1 - start);
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->data = (char *)malloc(pos + 1 - start);
                memcpy(Unit->data, tempBuf + tempstart, pos + 1 - start);
                Unit->type = TEMP_HTML;
                Unit = Unit->next;
                tempstart += pos + 1 - start;
            }
            start = pos + 1;
        }
        pos++;
    }
    if (tempstart < size) {
        memmove(tempBuf + tempstart, tempBuf + start, pos - start);
        Unit->next = NULL;
        Unit->data = (char *)malloc(pos - start);
        memcpy(Unit->data, tempBuf + tempstart, pos - start);
        Unit->tdata = pos - start;
        Unit->type = TEMP_HTML;
        tempstart += pos - start;
    }
    free(tempBuf);
    tempBuf = NULL;

    qsort(tempVar, tempnbVar, sizeof(st_ptr), tempcompare);
    qsort(tempZone, tempnbZone, sizeof(st_ptr), tempcompare);
    qsort(tempTable, tempnbTable, sizeof(st_ptr), tempcompare);

    return(0);
}
#endif


int templateSetFile(char *filename) {
    FILE *f;
    char *ptr;
    int pos, size, start;
    int tempstart;
    char tmp[VARSZ + 1], name[VARSZ + 1];
    int i;
    int nbvar;
    int type;
    tempUnit *Unit;
    char *filebuf;
    int filesize;
    int tocopy;


#ifdef DEBUG

    fprintf(stderr, "templateSetFile: %s\n", filename);
    fflush(stderr);
#endif

    if ((f = fopen(filename, "r")) == NULL) {
#ifdef DEBUG
        fprintf(stderr, "*** error opening file\n");
        fflush(stderr);
#endif

        return(-1);
    }

    templateFreeMem();
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    tempBuf = (char *)malloc(sizeof(char) * (size + 1));
    fread(tempBuf, 1, size, f);
    tempBuf[size] = '\0';
    fclose(f);

    tempnbVar = tempnbZone = tempnbTable = 0;
    tempVar = tempZone = tempTable = NULL;
    UnitInit = (tempUnit *)malloc(sizeof(tempUnit));
    memset(UnitInit, 0, sizeof(tempUnit));
    Unit = UnitInit;
    nbvar = 0;
    tempstart = 0;
    start = pos = 0;
    while (pos < size) {
        if (tempBuf[pos] == '#') {
            if (tempBuf[pos + 1] == '#') {
                /* the encountered code till there is pure html */
                memcpy(tempBuf + tempstart, tempBuf + start, pos - start);
                i = 0;
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->data = (char *)malloc(pos - start);
                memcpy(Unit->data, tempBuf + tempstart, pos - start);
                Unit->tdata = pos - start;
                Unit->type = TEMP_HTML;
                Unit = Unit->next;
                tempstart += pos - start;

                /* from now on we're in the variable name */
                pos += 2;
                while (tempBuf[pos + i] != '\0'
                        && tempBuf[pos + i] != '#'
                        && i < VARSZ)
                    i++;
                memcpy(tmp, tempBuf + pos, i);
                tmp[i] = '\0';

                /* saves the unit containing the variable */
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->type = TEMP_VAR;
                Unit->name = strdup(tmp);
                /* adds the variable in the shortcut table */
                tempVar = (st_ptr *)realloc(tempVar, sizeof(st_ptr) * (tempnbVar + 1));
                tempVar[tempnbVar].name = strdup(tmp);
                tempVar[tempnbVar].unit = Unit;
                tempnbVar++;

                Unit = Unit->next;
                nbvar++;
                pos += i + 1;
                start = pos + 1;
            }
        }
        /* comments */
        else if (tempBuf[pos] == '<'
                 && tempBuf[pos + 1] == '!'
                 && tempBuf[pos + 2] == '-'
                 && tempBuf[pos + 3] == '-') {
            /* the encountered code till there is pure html */
            memcpy(tempBuf + tempstart, tempBuf + start, pos - start);
            i = 0;
            Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
            memset(Unit->next, 0, sizeof(tempUnit));
            Unit->next->prev = Unit;
            Unit->data = (char *)malloc(pos - start);
            memcpy(Unit->data, tempBuf + tempstart, pos - start);
            Unit->tdata = pos - start;
            Unit->type = TEMP_HTML;
            Unit = Unit->next;
            tempstart += pos - start;
            start = pos;

            pos += 4;
            i = 0;
            while (isspace(tempBuf[pos]))
                pos ++;
            while (pos < size && tempBuf[pos] != '-'
                    && tempBuf[pos] != '>'
                    && !isspace(tempBuf[pos])
                    && i < VARSZ) {
                tmp[i++] = tempBuf[pos];
                pos++;
            }
            tmp[i] = '\0';
            while (isspace(tempBuf[pos]))
                pos++;
            i = 0;
            while (pos < size && tempBuf[pos] != '-'
                    && tempBuf[pos] != '>'
                    && !isspace(tempBuf[pos])
                    && i < VARSZ) {
                name[i++] = tempBuf[pos];
                pos++;
            }
            name[i] = '\0';
            while (pos < size && tempBuf[pos] != '>')
                pos++;

            type = -1;

            if (strcmp(tmp, "#INCLUDEFILE") == 0) {
                type = TEMP_INCLUDEFILE;
                filebuf = includefile(name, &filesize);
                if (filebuf) {
                    pos++;
                    tocopy = size - pos;
                    size += filesize - (pos - start);
                    tempBuf = (char *)realloc(tempBuf, size);
                    memmove(tempBuf + start + filesize, tempBuf + pos, tocopy);
                    memcpy(tempBuf + start, filebuf, filesize);
                    free(filebuf);
                    pos = start;
                    continue;
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
                /* saves the unit containing the variable */
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->type = type;
                Unit->next->prev = Unit;
                if (Unit->type != TEMP_HTML)
                    Unit->name = strdup(name);
                switch (type) {
                case TEMP_BEGINTABLE:
                    tempTable = (st_ptr *)realloc(tempTable,
                                                  sizeof(st_ptr) * (tempnbTable + 1));
                    tempTable[tempnbTable].name = strdup(name);
                    tempTable[tempnbTable].unit = Unit;
                    tempnbTable++;
                    break;
                case TEMP_BEGINZONE:
                    tempZone = (st_ptr *)realloc(tempZone,
                                                 sizeof(st_ptr) * (tempnbZone + 1));
                    tempZone[tempnbZone].name = strdup(name);
                    tempZone[tempnbZone].unit = Unit;
                    tempnbZone++;
                    break;
                }
                Unit = Unit->next;
            } else {
                /* the whole unitk is taken for html */
                memcpy(tempBuf + tempstart, tempBuf + start, pos + 1 - start);
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->data = (char *)malloc(pos + 1 - start);
                memcpy(Unit->data, tempBuf + tempstart, pos + 1 - start);
                Unit->tdata = pos + 1 - start;
                Unit->type = TEMP_HTML;
                Unit = Unit->next;
                tempstart += pos + 1 - start;
            }
            start = pos + 1;
        }
        pos++;
    }
    if (tempstart < size) {
        memcpy(tempBuf + tempstart, tempBuf + start, pos - start);
        Unit->next = NULL;
        Unit->data = (char *)malloc(pos - start);
        memcpy(Unit->data, tempBuf + tempstart, pos - start);
        Unit->tdata = pos - start;
        Unit->type = TEMP_HTML;
        tempstart += pos - start;
    }
    free(tempBuf);
    tempBuf = NULL;

    qsort(tempVar, tempnbVar, sizeof(st_ptr), tempcompare);
    qsort(tempZone, tempnbZone, sizeof(st_ptr), tempcompare);
    qsort(tempTable, tempnbTable, sizeof(st_ptr), tempcompare);

    return(0);
}


void templateSetVar(char *fld, char *val) {
    tempUnit *Unit;
    st_ptr *rec;
    int i, nb;


    if (val == NULL)
        return;
    rec = tempsearch(fld, tempVar, tempnbVar, sizeof(st_ptr), tempcompname,
                     &nb);
    if (rec) {
        for (i = 0; i < nb; i++) {
            Unit = rec[i].unit;
            if (Unit->data)
                free(Unit->data);
            Unit->data = strdup(val);
            Unit->tdata = strlen(val);
            Unit->alloc = 1;
        }
    }

    rec = tempsearch(fld, tempZone, tempnbZone, sizeof(st_ptr), tempcompname,
                     &nb);
    if (rec) {
        for (i = 0; i < nb; i++) {
            Unit = rec[i].unit;
            Unit->tdata = 1;
        }
    }
}


void templateDumpUnit(tempUnit *UnitP, tempUnit *Unit) {
    tempUnit *UnitN;


    if (!(Unit->type == TEMP_HTML || Unit->type == TEMP_VAR)
            || Unit->type == TEMP_VAR && Unit->data == NULL)
        return;

    UnitN = (tempUnit *)malloc(sizeof(tempUnit));
    memset(UnitN, 0, sizeof(tempUnit));
    UnitN->data = (char *)malloc(Unit->tdata);
    memcpy(UnitN->data, Unit->data, Unit->tdata);
    UnitN->tdata = Unit->tdata;
    UnitN->alloc = 1;
    UnitN->type = TEMP_HTML;
    UnitN->prev = UnitP->prev;
    if (UnitP->prev)
        UnitP->prev->next = UnitN;
    UnitN->next = UnitP;
    UnitP->prev = UnitN;
}


void templateDumpTableLine(char *ligne) {
    tempUnit *Unit, *UnitS;
    tempUnit *UnitTableS, *UnitN, *UnitO, *UnitD;
    int pr;
    int i, nb;
    st_ptr *rec;


    rec = tempsearch(ligne, tempTable, tempnbTable, sizeof(st_ptr), tempcompname,
                     &nb);
    if (rec) {
        pr = 1;

        for (i = 0; i < nb; i++) {
            Unit = rec[i].unit;

            /*
             * insert a mark "beginning of generated code"
             */
            UnitN = (tempUnit *)malloc(sizeof(tempUnit));
            memset(UnitN, 0, sizeof(tempUnit));
            UnitN->next = Unit;
            UnitN->prev = Unit->prev;
            Unit->prev->next = UnitN;
            Unit->prev = UnitN;
            UnitN->type = TEMP_BEGINGENTABLE;

            UnitS = Unit;
            Unit = Unit->next;

            /*
             * we go through all the unitks till the corresponding ENDTABLE
             */
            while (Unit
                    && !(Unit->type == TEMP_ENDTABLE
                         && strcmp(Unit->name, ligne) == 0)) {
                if (Unit->type == TEMP_BEGINGENTABLE) {
                    UnitTableS = Unit;
                } else if (Unit->type == TEMP_ENDGENTABLE) {
                    /*
                     * frees the encountered generated unitks
                     */
                    UnitD = UnitTableS->prev;
                    UnitD->next = Unit->next;
                    Unit->next->prev = UnitD;
                    UnitN = UnitTableS;
                    while (UnitN != Unit) {
                        /* freeing the unitks */
                        if (UnitN->name)
                            free(UnitN->name);
                        if (UnitN->alloc && UnitN->data)
                            free(UnitN->data);
                        UnitO = UnitN;
                        UnitN = UnitN->next;
                        free(UnitO);
                    }
                    /* and we also have to free the ENDGENTABLE unitk */
                    free(UnitN);
                    Unit = UnitD;
                } else if (Unit->type == TEMP_BEGINZONE
                           && !curzone
                           && Unit->tdata != 1) {
                    curzone = strdup(Unit->name);
                    pr = 0;
                } else if (Unit->type == TEMP_ENDZONE
                           && curzone
                           && strcmp(Unit->name, curzone) == 0) {
                    pr = 1;
                    free(curzone);
                    curzone = NULL;
                } else if (Unit->type == TEMP_BEGINTABLE)	{
                    curtable = strdup(Unit->name);
                    Unit = Unit->next;
                    while (Unit
                            && !(Unit->type == TEMP_ENDTABLE
                                 && strcmp(Unit->name, curtable) == 0)) {
                        Unit = Unit->next;
                    }
                    free(curtable);
                } else {
                    if (pr) {
                        templateDumpUnit(UnitS, Unit);
                    }
                    /*
                     * if the unitk is a variable, reinitialize it once copied
                     */
                    if (Unit->type == TEMP_VAR) {
                        free(Unit->data);
                        Unit->data = NULL;
                        Unit->tdata = 0;
                        Unit->alloc = 0;
                    } else if (Unit->type == TEMP_BEGINZONE) {
                        Unit->tdata = 0;
                    }
                }
                if (Unit)
                    Unit = Unit->next;
            }
            /*
             * insert a mark "end of generated code"
             */
            UnitO = rec[i].unit;
            UnitN = (tempUnit *)malloc(sizeof(tempUnit));
            memset(UnitN, 0, sizeof(tempUnit));
            UnitN->next = UnitO;
            UnitN->prev = UnitO->prev;
            UnitO->prev->next = UnitN;
            UnitO->prev = UnitN;
            UnitN->type = TEMP_ENDGENTABLE;
        }
    }
}


void templatePrint() {
    tempUnit *Unit;
    char *ptr;
    int pr;


    Unit = UnitInit;
    pr = 1;
    while (Unit) {
        if (Unit->type == TEMP_BEGINTABLE) {
            curtable = strdup(Unit->name);
            Unit = Unit->next;
            while (Unit
                    && !(Unit->type == TEMP_ENDTABLE
                         && strcmp(Unit->name, curtable) == 0)) {
                Unit = Unit->next;
            }
            free(curtable);
        } else if (Unit->type == TEMP_BEGINZONE && !curzone && Unit->tdata != 1) {
            pr = 0;
            curzone = strdup(Unit->name);
        } else if (Unit->type == TEMP_ENDZONE
                   && curzone
                   && strcmp(Unit->name, curzone) == 0) {
            pr = 1;
            free(curzone);
            curzone = NULL;
        } else if (pr && (Unit->type == TEMP_HTML || Unit->type == TEMP_VAR)
                   && Unit->data) {
            ptr = (char *)malloc(Unit->tdata + 1);
            memcpy(ptr, Unit->data, Unit->tdata);
            ptr[Unit->tdata] = '\0';
            fputs(ptr, stdout);
            free(ptr);
        }
        if (Unit)
            Unit = Unit->next;
    }
}


char *templatePage() {
    char *tBufp;
    int sizep = 0;

    tempUnit *Unit;
    int pr;


    tBufp = NULL;
    Unit = UnitInit;
    pr = 1;
    while (Unit) {
        if (Unit->type == TEMP_BEGINTABLE) {
            curtable = strdup(Unit->name);
            Unit = Unit->next;
            while (Unit
                    && !(Unit->type == TEMP_ENDTABLE
                         && strcmp(Unit->name, curtable) == 0)) {
                Unit = Unit->next;
            }
            free(curtable);
        } else if (Unit->type == TEMP_BEGINZONE && !curzone && Unit->tdata != 1) {
            pr = 0;
            free(curzone);
            curzone = strdup(Unit->name);
        } else if (Unit->type == TEMP_ENDZONE
                   && curzone
                   && strcmp(Unit->name, curzone) == 0) {
            pr = 1;
            free(curzone);
            curzone = NULL;
        } else if (pr && (Unit->type == TEMP_HTML || Unit->type == TEMP_VAR)
                   && Unit->data) {
            tBufp = (char *)realloc(tBufp, sizep + Unit->tdata + 1);
            memcpy(tBufp + sizep, Unit->data, Unit->tdata);
            tBufp[sizep + Unit->tdata] = '\0';
            sizep += Unit->tdata;
        }
        if (Unit)
            Unit = Unit->next;
    }
    return(tBufp);
}


void templateFreeMem() {
    tempUnit *Unit, *UnitO;
    int i;


    Unit = UnitInit;
    while (Unit) {
        if (Unit->name)
            free(Unit->name);
        if (Unit->alloc && Unit->data)
            free(Unit->data);
        UnitO = Unit;
        Unit = Unit->next;
        free(UnitO);
    }
    UnitInit = NULL;
    for (i = 0; i < tempnbVar; i++)
        free(tempVar[i].name);
    for (i = 0; i < tempnbZone; i++)
        free(tempZone[i].name);
    for (i = 0; i < tempnbTable; i++)
        free(tempTable[i].name);
    if (tempVar) {
        free(tempVar);
        tempVar = NULL;
    }
    if (tempZone) {
        free(tempZone);
        tempZone = NULL;
    }
    if (tempTable) {
        free(tempTable);
        tempTable = NULL;
    }
    tempnbVar = tempnbZone = tempnbTable = 0;

    if (curzone) {
        free(curzone);
        curzone = NULL;
    }
}
