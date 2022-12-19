CC=gcc
CFLAGS=-Wall

all: samples txt2epub tokenring

samples: 
    @$(CC) samples.c $(CFLAGS) -o samples

txt2epub: 
    @$(CC) txt2epub.c $(CFLAGS) -o txt2epub

tokenring: 
    @$(CC) tokenring.c $(CFLAGS) -o tokenring

clean:
    @rm -rf samples txt2epub tokenring
