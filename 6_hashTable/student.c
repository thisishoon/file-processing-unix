#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
FILE *fp, *fp2;
//
// 학생 레코드 파일로부터  레코드 번호에 해당하는 레코드를 읽어 레코드 버퍼에 저장한다.
//
void readStudentRec(FILE *fp, char *recordbuf, int rn)
{
    fseek(fp, rn*STUDENT_RECORD_SIZE, SEEK_SET);
    fread(recordbuf, STUDENT_RECORD_SIZE, 1, fp);
}

//
// Hash file로부터 rn의 레코드 번호에 해당하는 레코드를 읽어 레코드 버퍼에 저장한다.
//
void readHashRec(FILE *fp, char *recordbuf, int rn)
{
    fseek(fp2, 4+HASH_RECORD_SIZE*rn, SEEK_SET);
    fread(recordbuf, 10, 1, fp2);
}
//
// Hash file에 rn의 레코드 번호에 해당하는 위치에 레코드 버퍼의 레코드를 저장한다.
//
void writeHashRec(FILE *fp, const char *recordbuf, int rn)
{
    fseek(fp2, 4+HASH_RECORD_SIZE*rn, SEEK_SET);
    fwrite(recordbuf, 10, 1, fp2);
    
}

//
// n의 크기를 갖는 hash file에서 주어진 학번 키값을 hashing하여 주소값(레코드 번호)를 리턴한다.
//
int hashFunction(const char *sid, int n)
{
    int last=(int)strlen(sid);
    int ASKII_1=sid[last-2];
    int ASKII_2=sid[last-1];
    int hashing=(ASKII_1*ASKII_2)%n;
    return hashing;
}

//
// n의 크기를 갖는 hash file을 생성한다.
// Hash file은 fixed length record 방식으로 저장되며, 레코드의 크기는 14 바이트이다.
// (student.h 참조)
//
void makeHashfile(int n)
{
    // Hash file을 생성할 때 이 파일의 맨앞부분에 4바이트 헤더를 둔다.
    // 여기에는 hash file의 크기 n을 저장한다. 이것은 search()와 (필요하면) delete()를     // 위한 것이다.

    fseek(fp2, 0, SEEK_SET);
    fwrite(&n, 4, 1, fp2);
    fseek(fp, 0, SEEK_END);
    int sz=(int)(ftell(fp)/STUDENT_RECORD_SIZE);
  
    for(int i=0; i<sz; i++){
        char idbuf[11];
        memset(idbuf, '\0', 11);
        
        fseek(fp, 120*i, SEEK_SET);
        fread(idbuf, 10, 1, fp);
        int hashing=hashFunction(idbuf, n);
       
        for(int j=hashing; j<n+hashing; j++){
            char readbuf[10];
            readHashRec(fp2, readbuf, j%n);
            if(readbuf[0]==0 || readbuf[0]=='*'){ //비어있거나 삭제되있으면
                writeHashRec(fp2, idbuf, j%n);
                fseek(fp2, 4+14*(j%n)+10, SEEK_SET);
                fwrite(&i, 4, 1, fp2);
                break;
            }
        }
    }
}

//
// 주어진 학번 키값을 hash file에서 검색한다.
// 그 결과는 주어진 학번 키값이 존재하는 hash file에서의 주소(레코드 번호)와 search length이다.
// 검색한 hash file에서의 주소는 rn에 저장하며, 이때 hash file에 주어진 학번 키값이
// 존재하지 않으면 rn에 -1을 저장한다. (search()는 delete()에서도 활용할 수 있음)
// search length는 함수의 리턴값이며, 검색 결과에 상관없이 search length는 항상 계산되어야 한다.
//
int search(const char *sid, int *rn)
{
    int n;
    fseek(fp2, 0, SEEK_SET);
    fread(&n, 4, 1, fp2);
    
    int hasing=hashFunction(sid, n);
    int search_length=0;
    
    for(int i=hasing; i<n+hasing; i++){
       char idbuf[11];
        memset(idbuf, '\0', 11);
        readHashRec(fp2, idbuf, i%n);
        search_length++;
        if(strcmp(idbuf, sid)==0){   //찾았을 때
            
            *rn=i%n;
            
            return search_length;
        }
        else{
            if(idbuf[0]=='\0'){    //찾다가 공백을 만났을 떄
                *rn=-1;
                return search_length;
            }
         
        }
    }
    *rn=-1;
    return search_length;
}

//
// Hash file에서 주어진 학번 키값과 일치하는 레코드를 찾은 후 해당 레코드를 삭제 처리한다.
// 이때 학생 레코드 파일에서 레코드 삭제는 필요하지 않다.
//
void delete(const char *sid)
{
    int rn;
    if(search(sid,&rn)!=-1){
        fseek(fp2, 4+14*rn, SEEK_SET);
        fwrite("*", 1, 1, fp2);
    }
}

//
// rn은 hash file에서의 레코드 번호를, sl은 search length를 의미한다.
//
void printSearchResult(int rn, int sl)
{
    printf("%d %d\n", rn, sl);
}


int main(int argc, char *argv[])
{
    // 학생레코드파일은 student.h에 정의되어 있는 STUDENT_FILE_NAME을,
    // hash file은 HASH_FILE_NAME을 사용한다.
    // 검색 기능을 수행할 때 출력은 반드시 주어진 printSearchResult() 함수를 사용한다.
    fp=fopen(RECORD_FILE_NAME,"rb");
    /*
    fp2=fopen(HASH_FILE_NAME,"r+b");
    if(fp2==NULL)    fp2=fopen(HASH_FILE_NAME,"w+b");
     */
    if(argc==3){
        if(argv[1][1]=='c'){
            fp2=fopen(HASH_FILE_NAME,"w+b");
            int hash_table_size=atoi(argv[2]);
            fseek(fp2, 0, SEEK_SET);
            for(int i=0; i<4+HASH_RECORD_SIZE*hash_table_size; i++){
                char dummy=0;
                fwrite(&dummy, 1, 1, fp2);
            }
            makeHashfile(hash_table_size);
        }
        else if(argv[1][1]=='s'){
            fp2=fopen(HASH_FILE_NAME,"r+b");
            int rn;
            int sl=search(argv[2], &rn);
            printSearchResult(rn, sl);
            
        }
        else if(argv[1][1]=='d'){
            fp2=fopen(HASH_FILE_NAME,"r+b");
            delete(argv[2]);
        }
    }
    
    
    fclose(fp); fclose(fp2);
}
