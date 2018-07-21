/**************************************************************************
 *                                                                        *
 * Flate library (Fast Template)                                          *
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

#ifndef _LIB_FLATE_H_
#define _LIB_FLATE_H_

#include <time.h>

typedef struct tempUnit {
    struct tempUnit *next;
    struct tempUnit *prev;
    char *data;
    char *name;
    int tdata;
    int type;
} tempUnit;

typedef struct {
    char *name;
    tempUnit *unit;
} st_ptr;

typedef struct {
    tempUnit *UnitInit;
    st_ptr *tempVar, *tempZone, *tempTable;
    int tempnbVar, tempnbZone, tempnbTable;
    char *curzone, *curtable;
    char *tempBuf;
    char *cookies;
    int cookiessz;
} Flate;

typedef struct s_form {
    char *name;
    char *value;
    int sz;
    int (*checkfunc)(int c);
    struct s_form *next;
} FlateForm;

extern Flate *flateSetFile(Flate **tmplte, char *filename);
extern void flateSetCookie(Flate *tmplte, char *name, char *value, char *domain, time_t expires);
extern int flateGetCookie(char *value, int valuesz, char *cookie, int (*checkfunc)(int c));
extern void flateSetVar(Flate *tmplte, char *fld, char *val);
extern void flateDumpTableLine(Flate *tmplte, char *line);
extern void flatePrint(Flate *tmplte, char *type);
extern char *flatePage(Flate *tmplte);
extern void flateFreeMem(Flate *tmplte);
extern FlateForm *flateAddForm(FlateForm *field, char *name, int sz, int (*checkfunc)(int c));
extern int flateReadForm(FlateForm *form);
extern FlateForm *flateSetForm(char *s);
extern char *flateGetForm(FlateForm *form, char *name);
extern void flateDestroyForm(FlateForm **form);

#endif
