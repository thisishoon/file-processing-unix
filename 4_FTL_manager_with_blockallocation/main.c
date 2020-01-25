
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blkmap.h"

FILE *devicefp;

int write_cnt;
int erase_cnt;



void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);

int main(int argc, char *argv[])
{
    char *blockbuf;
    char sectorbuf[SECTOR_SIZE];
    int lsn, i;
    
    devicefp = fopen("flashmemory", "w+b");
    if(devicefp == NULL)
    {
        printf("file open error\n");
        exit(1);
    }
    
    blockbuf = (char *)malloc(BLOCK_SIZE);
    memset(blockbuf, 0xFF, BLOCK_SIZE);
    
    for(i = 0; i < BLOCKS_PER_DEVICE; i++)
    {
        fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
    }
    
    free(blockbuf);
    
    ftl_open();    // ftl_read(), ftl_write() 호출하기 전에 이 함수를 반드시 호출해야 함
    
    
    ftl_write(0,"a0");
    ftl_write(1,"a1");
    ftl_write(2,"a2");
    ftl_write(3,"a3");
    ftl_write(0,"a4");
    ftl_write(1,"a5");
    
    ftl_write(4,"b0");
    ftl_write(4,"b1");
    ftl_write(4,"b2");
    ftl_write(7,"b3");
    ftl_write(4,"b4");
    ftl_write(5,"b5");
    
    ftl_write(3, "aa");
    
    ftl_write(12, "12");
    ftl_write(3, "aaa");
    ftl_write(3, "cc");
    ftl_write(0, "a");
    
   
    
    int j;
    char pagebuf[PAGE_SIZE];
    char buf[4];
    
    printf("\n>>>>>>>>>>>>>> UPDATE <<<<<<<<<<<<<<\n\n");
    for(i=0;i<6;i+=2){
        for(j=0;j<6;j++){
            memset(pagebuf,'\0',sizeof(pagebuf));
            memset(buf,'\0',sizeof(buf));
            dd_read((i*6)+j,pagebuf);
            if(pagebuf[0]==-1 && pagebuf[1]==-1 && pagebuf[2]==-1){
                printf("[ NULL ]");
            }else{
                strncpy(buf,pagebuf,4);
                buf[3]='\0';
                printf("[ %s %d ]",buf,pagebuf[SECTOR_SIZE]);
            }
            printf("              ");
            
            memset(pagebuf,'\0',sizeof(pagebuf));
            memset(buf,'\0',sizeof(buf));
            dd_read((i+1)*6+j,pagebuf);
            if(pagebuf[0]==-1 && pagebuf[1]==-1 && pagebuf[2]==-1){
                printf("[ NULL ]\n");
            }else{
                strncpy(buf,pagebuf,4);
                buf[3]='\0';
                printf("[ %s %d ]\n",buf,pagebuf[SECTOR_SIZE]);
            }
        }
        printf("\n");
    }
    printf("===================================\n");
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(0,sectorbuf);
    printf("lbn[0] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(1,sectorbuf);
    printf("lbn[1] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(2,sectorbuf);
    printf("lbn[2] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(3,sectorbuf);
    printf("lbn[3] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(4,sectorbuf);
    printf("lbn[4] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(5,sectorbuf);
    printf("lbn[5] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(6,sectorbuf);
    if(sectorbuf[0]!=-1)
    printf("lbn[6] : %s\n",sectorbuf);
    
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    ftl_read(7,sectorbuf);
    printf("lbn[7] : %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    
    ftl_read(12,sectorbuf);
    printf("lbn[12]: %s\n",sectorbuf);
    memset(sectorbuf,'\0',sizeof(sectorbuf));
    printf("===================================\n");

  
  
    fclose(devicefp);
    
    return i;
}

