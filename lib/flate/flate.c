/**************************************************************************
 *                                                                        *
 * Flate library 2.0.1 (Fast Template)                                    *
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
#include <ctype.h>
#include <time.h>

#include "flate.h"


#define VARSZ 100 /* max size of a variable name */
#define MAXFILENAMESZ 1024

#define MAXFIELDSIZE 50 /* max size of a field name in a form */
#define MAXVALSIZE 8192 /* max size of data for a field */
#define MAXCOOKIESIZE 4096


enum {
    FLATE_HTML,
    FLATE_VAR,
    FLATE_BEGINTABLE,
    FLATE_ENDTABLE,
    FLATE_BEGINZONE,
    FLATE_ENDZONE,
    FLATE_BEGINGENTABLE,
    FLATE_ENDGENTABLE,
    FLATE_INCLUDEFILE
};


static int tempcompare(const void *a, const void *b) {
    return(strcmp(((st_ptr *)a)->name, ((st_ptr *)b)->name));
}


static int tempcompname(const void *a, const void *b) {
    return(strcmp((char *)a, ((st_ptr *)b)->name));
}


/*
 * seeking function: looks for a value in a shortcut table
 * (tempVar, tempZone, tempTable)
 */
static void *tempsearch(const void *elem, const void *base, size_t nbelem,
                        size_t size, int (*comparefunc)(const void *, const void *),
                        size_t *nb) {
    void *ptr;
    int pos;


    *nb = 0;
    ptr = bsearch(elem, base, nbelem, size, comparefunc);
    if (!ptr)
        return(NULL);
    pos = ((char *)ptr - (char *)base) / size;
    /* go back to first entry of the sorted table */
    while (pos > 0
            && comparefunc(elem, (void *)((char *)base + (pos - 1 ) *size)) == 0)
        pos--;
    /* computes number of entries in the table */
    while (pos + *nb < nbelem
            && comparefunc(elem, (void *)((char *)base + (pos + *nb) * size)) == 0)
        (*nb)++;

    return((void *)(((char *)base) + pos * size));
}


static char *includefile(char *path, int *filesize) {
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


/*
 * read and parse template file in memory
 * *templte must be initialed with NULL
 */
Flate *flateSetFile(Flate **tmplte, char *filename) {
    FILE *f;
    int pos, size, start;
    char *tempBuf;
    int tempstart;
    char cfilename[MAXFILENAMESZ + 1];
    char tmp[VARSZ + 1], name[VARSZ + 1];
    int i;
    int nbvar;
    int type;
    tempUnit *Unit;
    char *filebuf;
    int filesize;
    int tocopy;
    char *rootdir;


    /* if we have a DOCUMENT_ROOT environment variable, add it in front of filename, unless a - is used as a first character */
    if (filename[0] == '-')
        filename++;
    else {
        rootdir = getenv("DOCUMENT_ROOT");
        if (rootdir) {
            snprintf(cfilename, MAXFILENAMESZ, "%s/%s", rootdir, filename);
            filename = cfilename;
        }
    }

#ifdef DEBUG
    fprintf(stderr, "FlateSetFile: %s\n", filename);
    fflush(stderr);
#endif

    if ((f = fopen(filename, "r")) == NULL) {
#ifdef DEBUG
        fprintf(stderr, "*** error opening file\n");
        fflush(stderr);
#endif

        return(NULL);
    }

    flateFreeMem(*tmplte);
    *tmplte = (Flate *)malloc(sizeof(Flate));
    memset(*tmplte, 0, sizeof(Flate));
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    tempBuf = (char *)malloc(sizeof(char) * (size + 1));
    tempBuf[size] = '\0';
    fread(tempBuf, 1, size, f);
    fclose(f);

    (*tmplte)->tempnbVar = (*tmplte)->tempnbZone
                           = (*tmplte)->tempnbTable = 0;
    (*tmplte)->tempVar = (*tmplte)->tempZone = (*tmplte)->tempTable = NULL;
    (*tmplte)->UnitInit = (tempUnit *)malloc(sizeof(tempUnit));
    memset((*tmplte)->UnitInit, 0, sizeof(tempUnit));
    Unit = (*tmplte)->UnitInit;
    nbvar = 0;
    tempstart = 0;
    start = pos = 0;
    while (pos < size) {
        if (tempBuf[pos] == '#') {
            if (tempBuf[pos + 1] == '#') {
                /* the encountered code till there is pure html */
                memmove(tempBuf + tempstart, tempBuf + start,
                        pos - start);
                i = 0;
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->data = (char *)malloc(pos - start);
                memcpy(Unit->data, tempBuf + tempstart, pos - start);
                Unit->tdata = pos - start;
                Unit->type = FLATE_HTML;
                Unit = Unit->next;
                tempstart += pos - start;

                /* from now on we're in the variable name */
                pos += 2;
                while (pos + i < size
                        && tempBuf[pos + i] != '#'
                        && i < VARSZ)
                    i++;
                memcpy(tmp, tempBuf + pos, i);
                tmp[i] = '\0';

                /* saves the unit containing the variable */
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->next->prev = Unit;
                Unit->type = FLATE_VAR;
                Unit->name = strdup(tmp);
                /* adds the variable in the shortcut table */
                (*tmplte)->tempVar =
                    (st_ptr *)realloc((*tmplte)->tempVar,
                                      sizeof(st_ptr) * ((*tmplte)->tempnbVar + 1));
                (*tmplte)->tempVar[(*tmplte)->tempnbVar].name = strdup(tmp);
                (*tmplte)->tempVar[(*tmplte)->tempnbVar].unit = Unit;
                (*tmplte)->tempnbVar++;

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
            Unit->type = FLATE_HTML;
            Unit = Unit->next;
            tempstart += pos - start;
            start = pos;

            pos += 4;
            i = 0;
            while (isspace(tempBuf[pos]))
                pos ++;
            while (pos < size
                    && tempBuf[pos] != '-'
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
            while (pos < size
                    && tempBuf[pos] != '-'
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
                type = FLATE_INCLUDEFILE;
                if (rootdir) {
                    snprintf(cfilename, MAXFILENAMESZ, "%s/%s", rootdir, name);
                    filebuf = includefile(cfilename, &filesize);
                } else {
                    filebuf = includefile(name, &filesize);
                }

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
                type = FLATE_BEGINTABLE;
            else if (strcmp(tmp, "#ENDTABLE") == 0)
                type = FLATE_ENDTABLE;
            else if (strcmp(tmp, "#BEGINZONE") == 0)
                type = FLATE_BEGINZONE;
            else if (strcmp(tmp, "#ENDZONE") == 0)
                type = FLATE_ENDZONE;

            if (type != -1) {
                /* saves the unit containing the variable */
                Unit->next = (tempUnit *)malloc(sizeof(tempUnit));
                memset(Unit->next, 0, sizeof(tempUnit));
                Unit->type = type;
                Unit->next->prev = Unit;
                if (Unit->type != FLATE_HTML)
                    Unit->name = strdup(name);
                switch (type) {
                case FLATE_BEGINTABLE:
                    (*tmplte)->tempTable
                    = (st_ptr *)realloc((*tmplte)->tempTable,
                                        sizeof(st_ptr)
                                        * ((*tmplte)->tempnbTable + 1));
                    (*tmplte)->tempTable[(*tmplte)->tempnbTable].name = strdup(name);
                    (*tmplte)->tempTable[(*tmplte)->tempnbTable].unit = Unit;
                    (*tmplte)->tempnbTable++;
                    break;
                case FLATE_BEGINZONE:
                    (*tmplte)->tempZone
                    = (st_ptr *)realloc((*tmplte)->tempZone,
                                        sizeof(st_ptr)
                                        * ((*tmplte)->tempnbZone + 1));
                    (*tmplte)->tempZone[(*tmplte)->tempnbZone].name = strdup(name);
                    (*tmplte)->tempZone[(*tmplte)->tempnbZone].unit = Unit;
                    (*tmplte)->tempnbZone++;
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
                Unit->tdata = pos + 1 - start;
                Unit->type = FLATE_HTML;
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
        Unit->type = FLATE_HTML;
        tempstart += pos - start;
    }

    free(tempBuf);

    qsort((*tmplte)->tempVar, (*tmplte)->tempnbVar, sizeof(st_ptr), tempcompare);
    qsort((*tmplte)->tempZone, (*tmplte)->tempnbZone, sizeof(st_ptr), tempcompare);
    qsort((*tmplte)->tempTable, (*tmplte)->tempnbTable, sizeof(st_ptr), tempcompare);

    return(*tmplte);
}


/*
 * set cookie "name" with "value" for the given "domain" and will expire in "expires" seconds
 * set "expires" to 0 for a session cookie, -1 to remove it
 */
void flateSetCookie(Flate *tmplte, char *name, char *value, char *domain, time_t expires) {
    char bufc[MAXCOOKIESIZE + 1];
    char timebuf[100 + 1];
    time_t t;
    struct tm *tm;
    int sz;
    
    
    snprintf(bufc, MAXCOOKIESIZE, "Set-cookie: %s=%s; path=/; domain=%s; ", name, value, domain);
    if (expires == -1) {
        strncat(bufc, "expires=Sat, 01-Jan-1980 00:00:00 GMT", MAXCOOKIESIZE);
    } else if (expires != 0) {
        t = time(NULL) + expires;
        tm = gmtime(&t);
        strftime(timebuf, 100, "%a, %d-%b-%Y %H:%M:%S GMT", tm);
        strncat(bufc, "expires=", MAXCOOKIESIZE);
        strncat(bufc, timebuf, MAXCOOKIESIZE);
    }
//    strncat(bufc, "; HttpOnly", MAXCOOKIESIZE); /* some will probably see as a "gotcha" once the dude in charge for the HTML/Javascript code tell them something is not working, but trust me you probably want to activate this if you want to avoid kiddies collecting your users cookies */
    sz = strlen(bufc);
    if (tmplte->cookies) {
        tmplte->cookies = (char *)realloc(tmplte->cookies, tmplte->cookiessz + sz + 1 + 1);
        tmplte->cookies[tmplte->cookiessz] = '\n';
        memcpy(tmplte->cookies + tmplte->cookiessz + 1, bufc, sz + 1);
        tmplte->cookiessz += sz + 1;
    } else {
        tmplte->cookies = (char *)malloc(sz + 1);
        memcpy(tmplte->cookies, bufc, sz + 1);
        tmplte->cookiessz = sz;
    }
}


/*
 * retrieves "cookie" in "value"
 * returns 0 if OK
 *         1 if no cookie at all
 *         2 if no cookie by the name "cookie" found
 *         3 if cookie value contains unwanted characters
 */
int flateGetCookie(char *value, int valuesz, char *cookie, int (*checkfunc)(int c)) {
    char *ptr;
    char buf[MAXCOOKIESIZE + 1];
    int sz;
    
    
    ptr = getenv("HTTP_COOKIE");
    if (!ptr) {
        value[0] = '\0';
        return(1);
    }
    
    while (*ptr != '\0') {
        while (isspace(*ptr) || *ptr == ';')
            ptr++;
        sz = 0;
        while (sz < MAXCOOKIESIZE && ptr[sz] != '\0' && !isspace(ptr[sz]) && ptr[sz] != ';' && ptr[sz] != '=') {
            buf[sz] = ptr[sz];
            sz++;
        }
        buf[sz] = '\0';
        
        if (strcasecmp(buf, cookie)) { /* according to RFC 2965, cookie names are case insensitive */
            ptr += sz;
            while (*ptr != '\0' && *ptr != ';')
                ptr++;
            continue;
        }
        
        ptr += sz;
        while (isspace(*ptr))
            ptr++;
        
        if (*ptr != '=') {
            while (*ptr != '\0' && *ptr != ';')
                ptr++;
            continue;
        }
        ptr++;
        while (isspace(*ptr))
            ptr++;
        
        sz = 0;
        while (sz < valuesz && ptr[sz] != '\0' && !isspace(ptr[sz]) && ptr[sz] != ';') {
            if (checkfunc && !checkfunc(ptr[sz])) { /* check for unwanted characaters */
                value[0] = '\0';
                return(3);
            }
            value[sz] = ptr[sz];
            sz++;
        }
        value[sz] = '\0';
        
        return(0);
    }
    return(2);    
}


/*
 * assigns a value in the template to a variable, or marks a zone for display
 */
void flateSetVar(Flate *tmplte, char *fld, char *val) {
    tempUnit *Unit;
    st_ptr *rec;
    size_t i, nb;


    if (val == NULL)
        return;
    rec = (st_ptr *)tempsearch(fld, tmplte->tempVar, tmplte->tempnbVar,
                               sizeof(st_ptr), tempcompname, &nb);
    if (rec) {
        for (i = 0; i < nb; i++) {
            Unit = rec[i].unit;
            if (Unit->data)
                free(Unit->data);
            Unit->data = strdup(val);
            Unit->tdata = strlen(val);
        }
    }

    rec = (st_ptr *)tempsearch(fld, tmplte->tempZone, tmplte->tempnbZone,
                               sizeof(st_ptr), tempcompname, &nb);
    if (rec) {
        for (i = 0; i < nb; i++) {
            Unit = rec[i].unit;
            Unit->tdata = 1;
        }
    }
}


static void flateDumpUnit(tempUnit *UnitP, tempUnit *Unit) {
    tempUnit *UnitN;


    if (!(Unit->type == FLATE_HTML || Unit->type == FLATE_VAR)
            || (Unit->type == FLATE_VAR && Unit->data == NULL))
        return;

    UnitN = (tempUnit *)malloc(sizeof(tempUnit));
    memset(UnitN, 0, sizeof(tempUnit));
    UnitN->data = (char *)malloc(Unit->tdata);
    memcpy(UnitN->data, Unit->data, Unit->tdata);
    UnitN->tdata = Unit->tdata;
    UnitN->type = FLATE_HTML;
    UnitN->prev = UnitP->prev;
    if (UnitP->prev)
        UnitP->prev->next = UnitN;
    UnitN->next = UnitP;
    UnitP->prev = UnitN;
}


void flateDumpTableLine(Flate *tmplte, char *ligne) {
    tempUnit *Unit, *UnitS;
    tempUnit *UnitTableS = NULL, *UnitN, *UnitO, *UnitD;
    int pr;
    size_t i, nb;
    st_ptr *rec;


    rec = (st_ptr *)tempsearch(ligne, tmplte->tempTable, tmplte->tempnbTable,
                               sizeof(st_ptr), tempcompname, &nb);
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
            UnitN->type = FLATE_BEGINGENTABLE;

            UnitS = Unit;
            Unit = Unit->next;

            /*
             * we go through all the units till the corresponding ENDTABLE
             */
            while (Unit
                    && !(Unit->type == FLATE_ENDTABLE
                         && strcmp(Unit->name, ligne) == 0)) {
                if (Unit->type == FLATE_BEGINGENTABLE) {
                    UnitTableS = Unit;
                } else if (Unit->type == FLATE_ENDGENTABLE) {
                    /*
                     * frees the encountered generated unit
                     */
                    UnitD = UnitTableS->prev;
                    UnitD->next = Unit->next;
                    Unit->next->prev = UnitD;
                    UnitN = UnitTableS;
                    while (UnitN != Unit) {
                        /* freeing the units */
                        if (UnitN->name)
                            free(UnitN->name);
                        if (UnitN->data)
                            free(UnitN->data);
                        UnitO = UnitN;
                        UnitN = UnitN->next;
                        free(UnitO);
                    }
                    /* and we also have to free the ENDGENTABLE unit */
                    free(UnitN);
                    Unit = UnitD;
                } else if (Unit->type == FLATE_BEGINZONE
                           && !tmplte->curzone
                           && Unit->tdata != 1) {
                    free(tmplte->curzone);
                    tmplte->curzone = strdup(Unit->name);
                    pr = 0;
                } else if (Unit->type == FLATE_ENDZONE
                           && tmplte->curzone
                           && strcmp(Unit->name, tmplte->curzone) == 0) {
                    pr = 1;
                    free(tmplte->curzone);
                    tmplte->curzone = NULL;
                } else if (Unit->type == FLATE_BEGINTABLE) {
                    tmplte->curtable = strdup(Unit->name);
                    Unit = Unit->next;
                    while (Unit
                            && !(Unit->type == FLATE_ENDTABLE
                                 && strcmp(Unit->name, tmplte->curtable) == 0)) {
                        Unit = Unit->next;
                    }
                    free(tmplte->curtable);
                    tmplte->curtable = NULL;
                } else {
                    if (pr) {
                        flateDumpUnit(UnitS, Unit);
                    }
                    /*
                     * if the unit is a variable, reinitialize it once copied
                     */
                    if (Unit->type == FLATE_VAR) {
                        free(Unit->data);
                        Unit->data = NULL;
                        Unit->tdata = 0;
                    } else if (Unit->type == FLATE_BEGINZONE) {
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
            UnitN->type = FLATE_ENDGENTABLE;
        }
    }
}


/*
 * prints all template parts to standard output
 */
void flatePrint(Flate *tmplte, char *type) {
    tempUnit *Unit;
    char *ptr;
    int pr;


    if (tmplte->cookies) {
        puts(tmplte->cookies);
    }
    if (type && type[0] != '\0') {
        printf("Content-type: %s\n\n", type);
    }
    
    Unit = tmplte->UnitInit;
    pr = 1;
    while (Unit) {
        if (Unit->type == FLATE_BEGINTABLE) {
            tmplte->curtable = strdup(Unit->name);
            Unit = Unit->next;
            while (Unit
                    && !(Unit->type == FLATE_ENDTABLE
                         && strcmp(Unit->name, tmplte->curtable) == 0)) {
                Unit = Unit->next;
            }
            free(tmplte->curtable);
            tmplte->curtable = NULL;
        } else if (Unit->type == FLATE_BEGINZONE && !tmplte->curzone
                   && Unit->tdata != 1) {
            pr = 0;
            free(tmplte->curzone);
            tmplte->curzone = strdup(Unit->name);
        } else if (Unit->type == FLATE_ENDZONE
                   && tmplte->curzone
                   && strcmp(Unit->name, tmplte->curzone) == 0) {
            pr = 1;
            free(tmplte->curzone);
            tmplte->curzone = NULL;
        } else if (pr && (Unit->type == FLATE_HTML || Unit->type == FLATE_VAR)
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


/*
 * same as flatePrint() but in a newly allocated buffer
 */
char *flatePage(Flate *tmplte) {
    char *tBufp;
    int sizep = 0;

    tempUnit *Unit;
    int pr;


    tBufp = NULL;
    Unit = tmplte->UnitInit;
    pr = 1;
    while (Unit) {
        if (Unit->type == FLATE_BEGINTABLE) {
            tmplte->curtable = strdup(Unit->name);
            Unit = Unit->next;
            while (Unit
                    && !(Unit->type == FLATE_ENDTABLE
                         && strcmp(Unit->name, tmplte->curtable) == 0)) {
                Unit = Unit->next;
            }
            free(tmplte->curtable);
            tmplte->curtable = NULL;
        } else if (Unit->type == FLATE_BEGINZONE && !tmplte->curzone
                   && Unit->tdata != 1) {
            pr = 0;
            free(tmplte->curzone);
            tmplte->curzone = strdup(Unit->name);
        } else if (Unit->type == FLATE_ENDZONE
                   && tmplte->curzone
                   && strcmp(Unit->name, tmplte->curzone) == 0) {
            pr = 1;
            free(tmplte->curzone);
            tmplte->curzone = NULL;
        } else if (pr && (Unit->type == FLATE_HTML || Unit->type == FLATE_VAR)
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


/*
 * template clean up
 */
void flateFreeMem(Flate *tmplte) {
    tempUnit *Unit, *UnitO;
    int i;


    if (!tmplte)
        return;
    if (tmplte->cookies) {
        free(tmplte->cookies);
        tmplte->cookies = NULL;
    }
    Unit = tmplte->UnitInit;
    while (Unit) {
        if (Unit->name)
            free(Unit->name);
        if (Unit->data)
            free(Unit->data);
        UnitO = Unit;
        Unit = Unit->next;
        free(UnitO);
    }
    tmplte->UnitInit = NULL;
    for (i = 0; i < tmplte->tempnbVar; i++)
        free(tmplte->tempVar[i].name);
    for (i = 0; i < tmplte->tempnbZone; i++)
        free(tmplte->tempZone[i].name);
    for (i = 0; i < tmplte->tempnbTable; i++)
        free(tmplte->tempTable[i].name);
    if (tmplte->tempVar) {
        free(tmplte->tempVar);
        tmplte->tempVar = NULL;
    }
    if (tmplte->tempZone) {
        free(tmplte->tempZone);
        tmplte->tempZone = NULL;
    }
    if (tmplte->tempTable) {
        free(tmplte->tempTable);
        tmplte->tempTable = NULL;
    }
    tmplte->tempnbVar = tmplte->tempnbZone = tmplte->tempnbTable = 0;

    if (tmplte->curzone) {
        free(tmplte->curzone);
        tmplte->curzone = NULL;
    }
    if (tmplte->curtable) {
        free(tmplte->curtable);
        tmplte->curtable = NULL;
    }
    if (tmplte->tempBuf) {
        free(tmplte->tempBuf);
        tmplte->tempBuf = NULL;
    }
    free(tmplte);
}


static int flateHexchartoi(char c) {
    c = toupper(c);
    if (c >= '0' && c <= '9')
        return(c - '0');
    if (c >= 'A' && c <= 'F')
        return(c - 'A' + 10);
    return(0);
}


/* add a new field to look for in a form */
FlateForm *flateAddForm(FlateForm *field, char *name, int sz, int (*checkfunc)(int c)) {
    FlateForm *nform;
    
    
    nform = (FlateForm *)malloc(sizeof(FlateForm));
    nform->next = field;
    nform->name = name;
    nform->sz = sz;
    nform->checkfunc = checkfunc;
    nform->value = NULL;
    return(nform);
}


/*
 * set all fields to look for with a string
 * NOTE: the string will be modified, thus you can't pass a static string
 */
FlateForm *flateSetForm(char *s) {
    char *ptr;
    char *optr, *ptrs;
    FlateForm *n = NULL;
    
    
    ptr = strtok(s, ":");
    while (ptr) {
        ptrs = ptr;
        optr = strsep(&ptrs, ",");
        if (!optr)
            break;
        n = flateAddForm(n, optr, atoi(ptrs), NULL);
        ptr = strtok(NULL, ":");
    }
    return(n);
}


/*
 * parse HTTP strings with the field arguments in form (as populated with flateAddForm())
 */
int flateReadForm(FlateForm *form) {
    enum {
        RF_FIELDNAME, RF_VALUE, RF_HEXCHAR
    };
    enum {
        RF_METHOD_POST, RF_METHOD_GET
    };

    FlateForm *cform = NULL;
    char buffer[MAXVALSIZE + 1];
    char field[MAXFIELDSIZE + 1];
    char *qptr, *ptr, c;
    int hexchar;
    int hasunk = 0; /* if we have encountered an unknown field, return the information to the caller */
    int method;
    int sz, rsz;
    int pos;
    int state, pstate;
    int done = 0;
    int hexc = 0; /* used to parse hexa encoding of special characters (%20 in string) */
    

    /* are we retrieving POST or GET ? */
    if ((ptr = (char *)getenv("REQUEST_METHOD")) == NULL)
        return(3);
    if (!strcmp(ptr, "GET")) {
        method = RF_METHOD_GET;
        if ((qptr = (char *)getenv("QUERY_STRING")) == NULL)
            return(3);
        sz = strlen(qptr);
    } else if (!strcmp(ptr, "POST")) {
        method = RF_METHOD_POST;
        if ((qptr = (char *)getenv("CONTENT_LENGTH")) == NULL)
            return(3);
        sz = atoi(qptr);
    } else {
        return(3);
    }

    /* parse string */
    rsz = 0;
    pos = 0;
    state = RF_FIELDNAME; /* first char of query string is a field name */
    while (!done) {
        if (hexc == 2) {
            hexc = 0; /* encoding is on 2 characters, reset counter */
            switch (state) {
                case RF_FIELDNAME:
                    if (rsz < MAXFIELDSIZE)
                        field[rsz++] = c;
                    break;
                case RF_VALUE:
                    if (rsz < MAXVALSIZE)
                        buffer[rsz++] = c;
                    break;
            }
        } else {
            switch (method) {
                case RF_METHOD_POST:
                    c = getc(stdin);
                    if (feof(stdin))
                        done = 1;
                    break;
                case RF_METHOD_GET:
                    c = *(qptr++);
                    break;
            }
            if (pos == sz)
                done = 1;
            pos++;
            
            if (done || c == '&') { /* end of data for this field */
                buffer[rsz] = '\0';
                if (cform) {
                    if (rsz > cform->sz) {
                        return(2);
                    }
                    if (cform->checkfunc) {
                        ptr = buffer;
                        while (*ptr) {
                            if (!(cform->checkfunc(*ptr)))
                                break;
                            ptr++;
                        }
                        if (*ptr == '\0') /* if we made it up there it's all good */
                            cform->value = strdup(buffer);
                    } else
                        cform->value = strdup(buffer);
                }
                rsz = 0;
                state = RF_FIELDNAME;
            } else if (c == '=') { /* end of field name */
                field[rsz] = '\0';
                rsz = 0;
                cform = form;
                while (cform) {
                    if (!strcmp(field, cform->name)) {
                        break;
                    }
                    cform = cform->next;
                }
                state = RF_VALUE;
                if (cform == NULL) {
                    hasunk = 1;
                }
            } else if (c == '%') { /* 7 bit encoding of a 8 bit character */
                pstate = state; /* save state so that we come back to it once we have decoded the non 7 bit character */
                hexc = 0;
                state = RF_HEXCHAR;
            } else { /* other character */
                if (c == '+') /* space */
                    c = ' ';
                if (hexc == 2)
                    hexc = 0;
                switch (state) {
                    case RF_FIELDNAME:
                        if (rsz < MAXFIELDSIZE)
                            field[rsz++] = c;
                        break;
                    case RF_VALUE:
                        if (rsz < MAXVALSIZE)
                            buffer[rsz++] = c;
                        break;
                    case RF_HEXCHAR:
                        hexc++;
                        if (hexc == 1) {
                            hexchar = flateHexchartoi(c) << 4;
                        } else if (hexc == 2) {
                            hexchar |= flateHexchartoi(c);
                            c = (char)hexchar;
                            state = pstate; /* we have decoded the character, return to previous state and keep on parsing */                            
                        }
                        break;
                }
            }
        }
    }
    if (hasunk)
        return(1);
    return(0);
}


/*
 * return a field value according to a given field name
 */
char *flateGetForm(FlateForm *form, char *name) {
    FlateForm *cform;
    
    
    cform = form;
    while (cform) {
        if (!strcmp(cform->name, name))
            return(cform->value);
        cform = cform->next;
    }
    return(NULL);
}


void flateDestroyForm(FlateForm **form) {
    FlateForm *cform, *nform;
    
    
    cform = *form;
    while (cform) {
        nform = cform->next;
        free(cform);
        cform = nform;
    }
    *form = NULL;
}

