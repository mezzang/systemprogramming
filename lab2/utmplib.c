#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT (struct utmp *)NULL
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];      /* storage      */
static int num_recs;                      /* num stored   */
static int cur_rec;                       /* next to go   */
static int fd_utmp = -1;                  /* read from    */

int utmp_reload();

/* utmp_open - open the utmp file and reset everything */
int utmp_open(char *filename) {
    fd_utmp = open(filename, O_RDONLY);   /* open it      */
    cur_rec = num_recs = 0;               /* no recs yet  */
    return fd_utmp;                       /* report       */
}

/* utmp_next - return pointer to next utmp struct */
struct utmp *utmp_next() {
    struct utmp *recp;

    if (fd_utmp == -1)                    /* error?       */
        return NULLUT;
    if (cur_rec == num_recs && utmp_reload() == 0) /* any more? */
        return NULLUT; 
                                             /* get address of next record */
    recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}

/* utmp_reload - read next bunch of records into buffer */
int utmp_reload() {
    int amt_read;

    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE); /* read them in */
    num_recs = amt_read / UTSIZE;                      /* how many did we get? */
    cur_rec = 0;                                       /* reset pointer */
    return num_recs;
}

/* utmp_close - close the utmp file */
void utmp_close() {
    if (fd_utmp != -1)                                  /* don't close if not open */
        close(fd_utmp);
}
