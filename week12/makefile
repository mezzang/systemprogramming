CC = gcc          
CFLAGS = -Wall -g 

TARGETS = rls rlsd2 timeclnt timeserv tinybc

all: $(TARGETS)   #  모든 프로그램 빌드

rls: rls.c
	$(CC) $(CFLAGS) -o rls rls.c

rlsd2: rlsd2.c
	$(CC) $(CFLAGS) -o rlsd2 rlsd2.c

timeclnt: timeclnt.c
	$(CC) $(CFLAGS) -o timeclnt timeclnt.c

timeserv: timeserv.c
	$(CC) $(CFLAGS) -o timeserv timeserv.c

tinybc: tinybc.c
	$(CC) $(CFLAGS) -o tinybc tinybc.c

clean:             # 정리 타겟
	rm -f $(TARGETS)