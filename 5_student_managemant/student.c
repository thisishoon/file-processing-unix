#include <stdio.h>
#include "student.h"
#include <string.h>
#include <stdlib.h>
short head;
short records;
FILE *fp, *fp2;

// 학생 레코드 파일에 레코드를 저장하기 전에 구조체 STUDENT에 저장되어 있는 데이터를 레코드 형태로 만든다.
void pack(char *recordbuf, const STUDENT *s){
    int i=0;
    for(int j=0; (*s).id[j]!='\0'; j++){
        recordbuf[i]=(*s).id[j];
        i++;
    }
    recordbuf[i]='|';  i++;
    for(int j=0; (*s).name[j]!='\0'; j++){
        recordbuf[i]=(*s).name[j];
        i++;
    }
    recordbuf[i]='|';  i++;
    for(int j=0; (*s).dept[j]!='\0'; j++){
        recordbuf[i]=(*s).dept[j];
        i++;
    }
    recordbuf[i]='|';  i++;
    for(int j=0; (*s).year[j]!='\0'; j++){
        recordbuf[i]=(*s).year[j];
        i++;
    }
    recordbuf[i]='|';  i++;
    for(int j=0; (*s).addr[j]!='\0'; j++){
        recordbuf[i]=(*s).addr[j];
        i++;
    }
    recordbuf[i]='|';  i++;
    for(int j=0; (*s).phone[j]!='\0'; j++){
        recordbuf[i]=(*s).phone[j];
        i++;
    }
    recordbuf[i]='|';  i++;
    for(int j=0; (*s).email[j]!='\0'; j++){
        recordbuf[i]=(*s).email[j];
        i++;
    }
    recordbuf[i]='|';  i++;
}



// 학생 레코드 파일로부터 레코드를 읽어 온 후 구조체 STUDENT로 변환한다.
void unpack(const char *recordbuf, STUDENT *s){
    int i = 0;
    int j = 0;
    
    for(j = 0; j < sizeof((*s).id); j++){
        if(recordbuf[i] == '|'){
            (*s).id[j] = '\0';
            i++;
            break;
        }
        (*s).id[j] = recordbuf[i];
        i++;
    }
    for(j = 0; j < sizeof((*s).name); j++){
        if(recordbuf[i] == '|'){
            (*s).name[j] = '\0';
            i++;
            break;
        }
        (*s).name[j] = recordbuf[i];
        i++;
    }
    for(j = 0; j < sizeof((*s).dept); j++){
        if(recordbuf[i] == '|'){
            (*s).dept[j] = '\0';
            i++;
            break;
        }
        (*s).dept[j] = recordbuf[i];
        i++;
    }
    for(j = 0; j < sizeof((*s).year); j++){
        if(recordbuf[i] == '|'){
            (*s).year[j] = '\0';
            i++;
            break;
        }
        (*s).year[j] = recordbuf[i];
        i++;
    }
    for(j = 0; j < sizeof((*s).addr); j++){
        if(recordbuf[i] == '|'){
            (*s).addr[j] = '\0';
            i++;
            break;
        }
        (*s).addr[j] = recordbuf[i];
        i++;
    }
    for(j = 0; j < sizeof((*s).phone); j++){
        if(recordbuf[i] == '|'){
            (*s).phone[j] = '\0';
            i++;
            break;
        }
        (*s).phone[j] = recordbuf[i];
        i++;
    }
    for(j = 0; j < sizeof((*s).email); j++){
        if(recordbuf[i] == '|'){
            (*s).email[j] = '\0';
            i++;
            break;
        }
        (*s).email[j] = recordbuf[i];
        i++;
    }
}


// 학생 레코드 파일로부터  레코드 번호에 해당하는 레코드를 읽어서 레코드 버퍼에 저장한다.
void readRecord(FILE *fp, char *recordbuf, int rn){
    short now, next, length;
    fseek(fp2, 0, SEEK_SET);
    fread(&records, sizeof(records), 1, fp2);
    
    if(records!=rn){
        fseek(fp2, 2*rn, SEEK_SET);
        fread(&now, sizeof(now), 1, fp2);
        fread(&next, sizeof(next), 1, fp2);
        length=next-now;
        fseek(fp, 2+now, SEEK_SET);
        fread(recordbuf, length, 1, fp);
    }
    else{
        fseek(fp2, 2*rn, SEEK_SET);
        fread(&now, sizeof(now), 1, fp2);
        fseek(fp,0,SEEK_END);
        next=(short)ftell(fp)-2;
        length=next-now;
        fseek(fp, 2+now, SEEK_SET);
        fread(recordbuf, length, 1, fp);
    }
    
}


//알고리즘: 저장
//학생 레코드 파일에서 삭제 레코드의 존재 여부를 검사한 후 삭제 레코드가 존재하면 이 공간에
//새로운 레코드를 저장하며, 만약 삭제 레코드가 존재하지 않거나 조건에 부합하는 삭제 레코드가
//존재하지 않으면  파일의 맨마지막에 저장한다.
void add(FILE *fp, const STUDENT *s){
    char addbuf[MAX_RECORD_SIZE]="";
    pack(addbuf, s);
    
    
    fseek(fp, 0, SEEK_SET);
    fread(&head, sizeof(head), 1, fp);
    
    short prev_offset=0;
    short cur, next, size;
    if(head!=-1){
        cur=head;
        while(1){
            
            fseek(fp, 2+1+cur ,SEEK_SET);
            fread(&next,sizeof(next),1,fp);
            fread(&size,sizeof(size),1,fp);
            if(size>=strlen(addbuf)){  //first fit success
                fseek(fp, 2+cur, SEEK_SET);
                fwrite(addbuf, (int)strlen(addbuf), 1, fp);
                fseek(fp, prev_offset, SEEK_SET);   //head의 연결 리스트 수정
                fwrite(&next, sizeof(next), 1, fp);
                return;
            }
            else{    //first fit fail
                fseek(fp, 2+1+cur, SEEK_SET);
                prev_offset=ftell(fp);
                
                cur=next;
                
                if(cur==-1) {
                    
                    break;
                }
            }
            
        }
    }
    
    
    
    fseek(fp2, 0, SEEK_SET);
    fread(&records,sizeof(records),1,fp2);
    records++;
    fseek(fp2, 0, SEEK_SET);
    fwrite(&records, sizeof(records), 1, fp2);
    
    fseek(fp, 0, SEEK_END);
    short location=(short)ftell(fp)-2;
    fseek(fp2,0,SEEK_END);
    fwrite(&location, sizeof(location), 1, fp2);
    fwrite(addbuf, (int)strlen(addbuf), 1, fp);
}

//알고리즘: 검색
//학생 레코드 파일에서 sequential search 기법으로 주어진 학번 키값고 일치하는 레코드를 찾는다.
//출력은 만드시 주어진 print 함수를 사용한다.
//검색 레코드가 존재하면 레코드 번호 rn을, 그렇지 않으면 -1을 리턴한다.
int search(FILE *fp, const char *keyval){
    
    STUDENT s;
    
    fseek(fp2, 0, SEEK_SET);
    fread(&records, sizeof(records), 1, fp2);
    
    for(int i=1; i<=records; i++){
        char searchbuf[MAX_RECORD_SIZE]="";
        readRecord(fp, searchbuf, i);
        unpack(searchbuf, &s);
        if(strcmp(s.id, keyval)==0){
            return i;
        }
    }
    
    return -1;
}

//알고리즘: 삭제
//학생 파일에서 주어진 학번 키값과 일치하는 레코드를 찾은 후 해당 레코드를 삭제 처리한다.
void delete(FILE *fp, const char *keyval){
    
    
    fseek(fp, 0, SEEK_SET);
    fread(&head, sizeof(head), 1, fp);
    fseek(fp2,0,SEEK_SET);
    fread(&records,sizeof(head),1,fp2);
    
    short delete_number=search(fp, keyval);
    short delete_offset;
    short next_offset;
    short size;
    
    
    
    if(delete_number!=-1){
        
        fseek(fp2, 2*delete_number, SEEK_SET);
        fread(&delete_offset,sizeof(delete_offset),1,fp2);
        if(delete_number!=records) fread(&next_offset,sizeof(next_offset),1,fp2);
        else{
            fseek(fp, 0, SEEK_END);
            next_offset=ftell(fp)-2;
        }
        size=next_offset-delete_offset;
        //printf("-d 삭제된 레코드의 크기는 %d\n",size);
        fseek(fp, 2+delete_offset, SEEK_SET);
        fwrite("*", 1, 1, fp);
        fwrite(&head,sizeof(head),1,fp);
        fwrite(&size, sizeof(size), 1, fp);
        fseek(fp, 0, SEEK_SET);
        fwrite(&delete_offset, sizeof(delete_offset), 1, fp);
        
    }
    else{
        
    }
    
}

void printRecord(const STUDENT *s, int n);


int main(int argc, char *argv[])
{
    
    fp=fopen(RECORD_FILE_NAME, "r+b");
    fp2=fopen(INDEX_FILE_NAME, "r+b");
    if(fp==NULL && fp2==NULL){
        fp=fopen(RECORD_FILE_NAME, "w+b");
        fp2=fopen(INDEX_FILE_NAME, "w+b");
        head=-1;
        records=0;
        fwrite(&head, sizeof(head), 1, fp);
        fwrite(&records, sizeof(records),1,fp2);
    }
    
    
    
    STUDENT s;
    if(argc==9){
        strcpy(s.id, argv[2]);
        strcpy(s.name, argv[3]);
        strcpy(s.dept, argv[4]);
        strcpy(s.year, argv[5]);
        strcpy(s.addr, argv[6]);
        strcpy(s.phone, argv[7]);
        strcpy(s.email, argv[8]);
        add(fp, &s);
    }
    
    else if(argc==3){
        if(argv[1][1]=='d'){//delete
            delete(fp, argv[2]);
        }
        else if(argv[1][1]=='s'){//search
            int search_number=search(fp, argv[2]);
            char searchbuf[MAX_RECORD_SIZE]="";
            if(search_number!=-1){
                readRecord(fp, searchbuf, search_number);
                unpack(searchbuf, &s);
                printRecord(&s, 1);
            }
        }
    }
    
    /*
     fseek(fp, 0, SEEK_SET);
     short temp;
     fread(&temp,2,1,fp);  printf("head: %d\n",temp);
     
     char record[1000];
     fread(record,1,1000,fp);
     printf("records: %s\n",record);
     
     short index;
     fseek(fp2,0,SEEK_SET);
     printf("index: ");
     while(fread(&index,2,1,fp2)>0){
     printf("%d  ",index);
     }
     
     fseek(fp, 0, SEEK_END);
     short file_size=ftell(fp);
     printf("size: %d\n",file_size);
     */
    fclose(fp);
    fclose(fp2);
}

void printRecord(const STUDENT *s, int n)
{
    int i;
    
    for(i=0; i<n; i++)
    {
        printf("%s|%s|%s|%s|%s|%s|%s\n", s[i].id, s[i].name, s[i].dept, s[i].year, s[i].addr, s[i].phone, s[i].email);
    }
}

// ./a.out -a "20152445" "jihoon" "CS" "3" "seoul" "01051548279" "jihoon522@naver.com"  59
// ./a.out -a "20151216" "heyz" "design" "5" "seoul" "01099455227" "khj0528khj@" 53
// ./a.out -a "20152448" "sumin" "media" "4" "seoul" "01012345678" "soooming"  50
// ./a.out -a "2015" "JH" "X" "3" "SU" "010" "1@1.com"      27


// ./a.out -a "20151234" "Gildong hong" "Computer Science" "3" "Dongjak-ku,Seoul" "010-828-0567" "gdhong@ssu.ac.kr"                 88

// ./a.out -a "12341234" "choi-minsung" "global media" "3" "seoul-dongjak-ku" "8285" "fuck@ssu.ac.kr"   74

// ./a.out -a "11111111" "moooo" "moooo" "4" "seoul-seoul" "01012345678" "soooming" 56

