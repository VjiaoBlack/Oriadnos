/*  Parse-Utilities */

#include <stdio.h>
#include <string.h>
#include "parse_util.h"

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

/* ===================================== parse_util() =============================================
    Splits a line of text into "words", which are strings separated by whitespace.
    LIMITATIONS:
        1. the line must have less than 100 characters.
        2. the maximum number of words is 20
        
    Returns an array of pointers (like argv).
    
    Typical usage:
        char line[1001];
        int i;
        fgets(line, 1000, fp);
        char **list = parse_split(line);
        for (i = 0; list[i] != NULL; ++i)
            printf("word: %s\n", line[i]);
*/
            
char **parse_split(char *line) {
    static char *words[MAX_WORDS+1];
    static char copy[MAX_LINE+1];
    int inside, nwords;
    char *p, c;
    
    if (strlen(line) > MAX_LINE-1) {
        words[0] = NULL;
        printf("Error in parse_split(): line is longer than %d chars\n", MAX_LINE);
        return words;
    }
    
    strcpy(copy,line);
    
    nwords = 0;
    inside = FALSE;
    for (p = line; (c = *p) != '\0'; ++p) {
        if (c == ' ' || c == '\t'  || c == '\n' || c == '\r') {
            if (inside) {
                *p = '\0';
                inside = FALSE;
                if (nwords == MAX_WORDS) {
                    words[MAX_WORDS] = NULL;
                    return words;
                }
            }
        }
        else if (!inside) {
            words[nwords++] = p;
            inside = TRUE;
        }
    }
    words[nwords] = NULL;
    return words;
}

/* =============================== parse_numwords() ===========================================
    Returns the number of words returned by parse_split()
    
    Usage:
        char **words = parse_split(line);
        int nwords = parse_numwords(words);
*/
int parse_numwords(char **words) {
    int i;
    
    for (i = 0; i < MAX_WORDS; ++i)
        if (words[i] == NULL)
            return i;
    return MAX_WORDS;
}

    
    
