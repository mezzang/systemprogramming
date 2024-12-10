CC = gcc
CFLAGS = -Wall -g 

TARGETS = incprint tw2 tw3 tw4 tw2-rev tw3-rev 


all: $(TARGETS)   #  모든 프로그램 빌드

incprint: incprint.c	
	$(CC) $(CFLAGS) -o incprint incprint.c

tw2: twordcount2.c
	$(CC) $(CFLAGS) -o tw2 twordcount2.c

tw3: twordcount3.c
	$(CC) $(CFLAGS) -o tw3 twordcount3.c

tw4: twordcount4.c
	$(CC) $(CFLAGS) -o tw4 twordcount4.c

tw2-rev: twordcount2-rev.c
	$(CC) $(CFLAGS) -o tw2-rev twordcount2-rev.c

tw3-rev: twordcount3-rev.c
	$(CC) $(CFLAGS) -o tw3-rev twordcount3-rev.c


clean:             # 정리 타겟
	rm -f $(TARGETS)