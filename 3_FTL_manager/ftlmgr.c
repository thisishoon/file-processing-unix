#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"
int dd_read(int ppn, char *pagebuf);
int dd_write(int ppn, char *pagebuf);
int dd_erase(int pbn);

extern FILE *devicefp;
int freeBlock;

struct mapping{
    int lbn;
    int pbn;
};
struct mapping mappingTable[DATABLKS_PER_DEVICE];

void ftl_open()
{
    for(int i=0; i < DATABLKS_PER_DEVICE; i++){
        mappingTable[i].pbn = -1;
    }
    freeBlock = 31;
    return;
}


void ftl_read(int lsn, char *sectorbuf)
{
    int lbn = lsn / PAGES_PER_BLOCK;
    int offset = lsn % PAGES_PER_BLOCK;
    int pbn=mappingTable[lbn].pbn;
    int ppn= mappingTable[lbn].pbn * PAGES_PER_BLOCK + offset;
    char readbuf[PAGE_SIZE];
    
    if(pbn==-1)
        return;
    else{
        dd_read(ppn, readbuf);
        if(readbuf[SECTOR_SIZE]==-1){
            return;
        }
        else{
            memcpy(sectorbuf,readbuf,SECTOR_SIZE);
        }
    }
    return;
}


void ftl_write(int lsn, char *sectorbuf)
{
    int lbn = lsn / PAGES_PER_BLOCK;
    int offset = lsn % PAGES_PER_BLOCK;
    if(mappingTable[lbn].pbn == -1 ) mappingTable[lbn].pbn = lbn;
    int pbn = mappingTable[lbn].pbn;
    int ppn=( pbn * PAGES_PER_BLOCK) + offset ;
    char readbuf[PAGE_SIZE];
    char writebuf[PAGE_SIZE];
    
    memcpy(writebuf,sectorbuf,SECTOR_SIZE);
    writebuf[SECTOR_SIZE]=1;
    
    dd_read(ppn, readbuf);
    
    if(readbuf[SECTOR_SIZE]== EOF){
        dd_write(ppn, writebuf);
    }
    
    else{
        for(int i=0; i<PAGES_PER_BLOCK; i++){
            if(i!=offset){
                dd_read(pbn * PAGES_PER_BLOCK + i, readbuf);
                if(readbuf[SECTOR_SIZE]==1){
                dd_write(freeBlock * PAGES_PER_BLOCK + i, readbuf);
                memset(readbuf,0xff,SECTOR_SIZE);
                }
            }
            else if(i==offset){
                dd_write(freeBlock*PAGES_PER_BLOCK+i, writebuf);
            }
        }
        int temp;
        temp=pbn;
        mappingTable[lbn].pbn=freeBlock;
        freeBlock=temp;
        dd_erase(freeBlock);
    }
    return;
}


