#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smsh.h"

char *next_cmd(char *prompt, FILE *fp)
{
    char *buf;
    int bufspace = 0;
    int pos = 0;
    int c;
    
    printf("%s", prompt);
    
    buf = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    
    while((c = getc(fp)) != EOF) {
        // 앞쪽 공백 건너뛰기
        if (pos == 0 && isspace(c))
            continue;
            
        if(pos + 1 >= bufspace) {
            buf = erealloc(buf, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
        }
        
        // 세미콜론이나 개행문자를 만났을 때
        if(c == ';' || c == '\n') {
            // 뒤쪽 공백 제거
            while(pos > 0 && isspace(buf[pos-1]))
                pos--;
            break;
        }
        
        buf[pos++] = c;
    }
    
    if(c == EOF && pos == 0) {
        free(buf);
        return NULL;
    }
    
    // 뒤쪽 공백 제거
    while(pos > 0 && isspace(buf[pos-1]))
        pos--;
        
    buf[pos] = '\0';
    
    // 빈 명령어 처리
    if(pos == 0) {
        free(buf);
        return next_cmd(prompt, fp);  // 다음 명령어 읽기
    }
    
    return buf;
}


#define is_delim(x) ((x) == ' ' || (x) == '\t')

char ** splitline(char *line)
{
    char *newstr();
    char **args;
    int spots = 0;
    int bufspace = 0;
    int argnum = 0;
    char *cp = line;
    char *start;
    int len;

    if(line == NULL)
        return NULL;
    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ/sizeof(char *);
    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ/sizeof(char *);

    while (*cp != '\0')
    {
        while (is_delim(*cp))
        {
            cp++;
        }
        if(*cp == '\0')
                break;
        if(argnum +1 >= spots){
            args = erealloc(args, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/sizeof(char*));
        }

        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp)))
        {
            len++;

        }
        args[argnum++] = newstr(start, len);

        
    }
    args[argnum] = NULL;
    return args;
}

char *newstr(char *s, int l)
{
    char *rv = emalloc(l + 1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char **list)
{
    char **cp = list;
    while (*cp)
        free(*cp++);
    free(list);
}

void *emalloc(size_t n)
{
    void *rv;
    if ((rv = malloc(n)) == NULL)
        fatal("out of memory", "", 1);
    return rv;
}

void *erealloc(void *p, size_t n)
{
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", 1);
    return rv;
}