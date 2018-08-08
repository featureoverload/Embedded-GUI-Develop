NOTE: For a version of the FLATE Library suitable for using with the FastCGI
FCGX (fcgiapp.h) API, checkout the fcgx branch.

-- Andrew Clayton

FLATE Library (Fast Template 2.0.1)
(c) Fabien MENEMENLIS (nihilist@dead-inside.org)

This program is released under the LGPL License. For copying information
see the file COPYING.


1. Description

FLATE is a library used to handle HTML template files outside C code as well as
basic CGI input/output such as retrieving variables from HTML forms or cookies.
Template files can be manipulated from the C code with various functions to
change variables and display or hide part of the HTML code. The best way to
sum it up would be that printf("<html>hello</html>"); is now history.


2. Installation

Simply do a "make" in the root directory of the archive. The resulting library
should be linked with your C cgi:
gcc cgi.c -o cgi -L/path/to/Flate -I/path/to/Flate -lflate


3. Changes

Flate 2.0 breaks compatibility with version 1.4 as it can now handle more than
a template file in memory at a time. Perl support has also been removed due to
my lack of interest for this language.


4. Documentation

4.1. HTML Code

A few extensions have been added to the HTML language so that you can easily
modify the code from your C application.
##variable## is a text zone that will be replaced by the value you want when you
call flateSetVar(f, "variable", "value");
<!-- #BEGINZONE myzone --><html code goes here><!-- #ENDZONE myzone --> is a
zone that will be hidden until you call flateSetVar(f, "myzone", "");
<!-- #BEGINTABLE mytable --><html code><!-- #ENDTABLE mytable --> is a zone
that will be displayed as many times as you call the function
flateDumpTableLine(f, "mytable");


4.2 Sample HTML code

<html>
<h1>
  ##title##
</h1>
<body>
<p>

<!-- let's imagine you either want to show on your page a sun if it's daytime
     or a moon if it's nighttime -->
<!-- #BEGINZONE sunzone -->
  <img src="sun.png">
<!-- #ENDZONE sunzone -->
<!-- #BEGINZONE moonzone -->
  <img src="moon.png">
<!-- #ENDZONE moonzone -->

<!-- the following will display a table with 2 columns and will be fed by
     the CGI -->
<p>
The following events are recorded in your database:
<table>
<!-- #BEGINTABLE dbline -->
<tr>
  <td>
    ##number##
  </td>
  <td>
    ##value##
  </td>
</tr>
<!-- #ENDTABLE dbline -->
</table>
</body>
</html>


You can nest tables inside tables, variables and zones inside other zones etc.

You can use the tool "checktpl" to check if your HTML template is valid
(when the code gets complex it's easy to mispell a zone or table name: this
would result in the page not being displayed correctly, or even not at all).

You can also include external template files using
<!-- #INCLUDEFILE /path/to/otherfile.html -->
Note that that path is relative to the SERVER_ROOT variable set in your
Apache/web server environment (this wasn't true with Flate 1.4).


4.3. Template functions

You first need to include "flate.h" in your program.
Each template in memory will be allocated in a Flate structure that you have
to declare:
Flate *f = NULL;
It must be set to NULL, or the function freeing the structure will be called,
leading to a crash if *f points to a random pointer.

flateSetFile(&f, "template.html");
  will load the file into memory and initialize the f template.

flateSetVar(f, "variable", "value");
  will set ##variable## in the HTML template to "value".

flateSetVar(f, "myzone", "");
  will display the block between
    <!-- #BEGINZONE myzone --> <!-- #ENDZONE myzone -->

flateDumpTableLine(f, "dbline");
  will print the zone between #BEGINTABLE and #ENDTABLE, with the variables
  set before. Once printed, the variables are set to NULL again, you can
  reuse flateSetVar(f, "variable", "value"); to set them for the next
  flateDumpTableLine(f, "dbline");

Once you're done, you can use:
flatePrint(f, "text/html");
  this will output the whole page (the result) to stdout with the given content
type.
You can also populate a buffer with char *buf = flatePage(f);
this will dump the output in a buffer pointed by buf. You need to free(buf);
after using this function.

flateFreeMem(f);
  will free all memory used by the f template.


4.4. Form reading

Flate 2.0 can easily retrieve variables from HTML Forms passed with the GET or
POST method indifferently.
You must first declare a FlateForm structure:
FlateForm *form = NULL;

You have then two methods to pass the variables you want to retrieve:

- add them one by one with the flateAddForm() function:
  form = firstfield = flateAddForm(form, "variable", 50, isalnum);
  "variable" is the variable name in the HTML Form, 50 the maximum length of
  the string and isalnum a function called to check the validity of the
  characters passed in the form, or NULL if you don't want to check the user
  input.
  You would add another variable with
  form = secondfield = flateAddForm(form, "second", 30, NULL);
  (this is a linked list so you need to point flateAddForm() to "form" every
  time you add a variable)

- pass a single string with all the variables:
  char *s;
  s = strdup("variable,50:second,30");
  form = flateSetForm(s);
  free(s);
  Note that the s string will be modified by flateSetForm() so you cannot do
  a simple char *s = "variable,50:second,30"; as the string declared this way
  is read only.

Once you have set the variables you want to retrieve, call flateReadForm(form);
to populate the internal buffers with the HTML form values.

To access the variables values, if you have used flateAddForm() you can
directly access firstfield->value for the first variable or second->value for
the second: they will contain the variables passed with the GET or POST methods
(as a char * type) or NULL if the form does not contain these variable names.
If you have used flateSetForm() you can get the variable value with
char *val = flateGetForm(form, "variable");


4.5. Cookies manipulation

Cookies manipulation is really basic but I included these functions for people
like me who always forget about the format of HTTP cookies:

flateSetCookie(Flate *f, char *name, char *value, char *domain, time_t expires);
expires is in seconds or 0 for a session cookie or -1 to remove it
cookies are set in the HTTP headers so they will be set when flatePrint() is
called.

flateGetCookie(char *value, int valuesz, char *cookie, check_function);
will retrieve the cookie named in "cookie" inside the "value" buffer if
characters matches the check_function() or NULL when no checking is wanted.
Example:
char cookie[50 + 1];
flateGetCookie(cookie, 50, "mycookie", isdigit);

Note that flateSetCookie() is bound to a Flate structure, so you have to call
flateSetFile() with a template file first. It might be a problem if you don't
want to print anything after you have set a cookie and instead need to do
a redirect, but I've not come accross a better API idea yet.


5. Suggestions and bug reports

You can reach me at the following address: nihilist <at> dead-inside.org if
you have any question or want to report a bug.

Fabien MENEMENLIS
 
