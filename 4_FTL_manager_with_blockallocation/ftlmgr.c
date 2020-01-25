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
    freeBlock = BLOCKS_PER_DEVICE-1;
    return;
}


void ftl_read(int lsn, char *sectorbuf)
{
    int lbn = lsn / NONBUF_PAGES_PER_BLOCK;
    int offset = lsn % NONBUF_PAGES_PER_BLOCK;
    int pbn=mappingTable[lbn].pbn;
    int ppn= mappingTable[lbn].pbn * PAGES_PER_BLOCK + offset;
    int last=pbn*PAGES_PER_BLOCK + PAGES_PER_BLOCK - 1;
    
    char readbuf[PAGE_SIZE];
    
    if(pbn==-1)
        return;
    
    else{
        //BUFFER영역
        for(int i=0; i<BUF_PAGES_PER_BLOCK; i++){
            dd_read(last-i, readbuf);
            if(readbuf[SECTOR_SIZE]==offset){
                memcpy(sectorbuf, readbuf, SECTOR_SIZE);
                return;
            }
        }
        
        //NONBUFFER영역
        dd_read(ppn, readbuf);
        memcpy(sectorbuf, readbuf, SECTOR_SIZE);
        return;
        
        
        
        
    }
}


void ftl_write(int lsn, char *sectorbuf)
{
    int lbn = lsn / NONBUF_PAGES_PER_BLOCK;
    int offset = lsn % NONBUF_PAGES_PER_BLOCK;
    if(mappingTable[lbn].pbn == -1 ) mappingTable[lbn].pbn = lbn;
    int pbn = mappingTable[lbn].pbn;
    int ppn=( pbn * PAGES_PER_BLOCK) + offset ;
    int start=lbn*NONBUF_PAGES_PER_BLOCK;
    
    
    char readbuf[PAGE_SIZE];
    char writebuf[PAGE_SIZE];
    
    memcpy(writebuf,sectorbuf,SECTOR_SIZE);
    writebuf[SECTOR_SIZE]=offset;
    
    dd_read(ppn, readbuf);
    //NonOverwriting
    if(readbuf[SECTOR_SIZE]== -1){
        dd_write(ppn, writebuf);
        
    }
    
    //OverWriting
    else{
        //Buffer 에 공간이 존재한다면
        for(int i=0; i<BUF_PAGES_PER_BLOCK; i++){
            dd_read(pbn*PAGES_PER_BLOCK+NONBUF_PAGES_PER_BLOCK+i, readbuf);
            
            if(readbuf[SECTOR_SIZE]==EOF){
                dd_write(pbn*PAGES_PER_BLOCK+NONBUF_PAGES_PER_BLOCK+i, writebuf);
                return;
            }
        }
        
        //Buffer에 공간이 없으면 복사
        for(int i=0; i<NONBUF_PAGES_PER_BLOCK; i++){
            
            
            if(i!=offset){
                char ftl_readbuf[SECTOR_SIZE];
                char ftl_writebuf[PAGE_SIZE];
                ftl_read(start+i,ftl_readbuf);
                if(ftl_readbuf[0]!=-1 && ftl_readbuf[1]!=-1 && ftl_readbuf[2]!=-1){
                memcpy(ftl_writebuf, ftl_readbuf, SECTOR_SIZE);
                ftl_writebuf[SECTOR_SIZE]=i;
                
                dd_write(freeBlock*PAGES_PER_BLOCK+i, ftl_writebuf);
                }
            }
            
            //쓰려고 하는 값
            else if(i==offset){
                dd_write(freeBlock*PAGES_PER_BLOCK+offset, writebuf);
            }
        }
        
        int temp=pbn;
        mappingTable[lbn].pbn=freeBlock;
        freeBlock=temp;
        
        dd_erase(freeBlock);
        
        
    }
    
    return;
}


