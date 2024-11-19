//smsh.h
//쉘 프로그램의 헤더파일
#define YES 1
#define NO 0

char *next_cmd();
char **splitline(char*);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void*, size_t);
void fatal(char*, char*, int);
int execute(char **);
int process(char **);
