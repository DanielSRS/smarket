/**
 * Code from:
 * 
 * https://stackoverflow.com/questions/4014827/how-can-i-compare-strings-in-c-using-a-switch-statement
*/

#ifndef __SWITCHS_H__
#define __SWITCHS_H__

#include <string.h>
#include <regex.h>
#include <stdbool.h>

/** Begin a switch for the string x */
#define SWITCH(x) \
    { char *ss__sw = (x); bool ss__done = false; bool ss__cont = false; \
        regex_t ss__regex; regcomp(&ss__regex, ".*", 0); do {

/** Check if the string matches the cases argument (case sensitive) */
#define CASE(x)    } if ( ss__cont || !strcmp ( ss__sw, x ) ) \
                        { ss__done = true; ss__cont = true;

/** Check if the string matches the icases argument (case insensitive) */
#define ICASE(x)    } if ( ss__cont || !strcasecmp ( ss__sw, x ) ) { \
                        ss__done = true; ss__cont = true;

/** Check if the string matches the specified regular expression using regcomp(3) */
#define RCASE(x,flags) } regfree ( &ss__regex ); if ( ss__cont || ( \
                              0 == regcomp ( &ss__regex, x, flags ) && \
                              0 == regexec ( &ss__regex, ss__sw, 0, NULL, 0 ) ) ) { \
                                ss__done = true; ss__cont = true;

/** Default behaviour */
#define DEFAULT    } if ( !ss__done || ss__cont ) {

/** Close the switchs */
#define END } while ( 0 ); regfree(&ss__regex); }

#endif // __SWITCHS_H__
