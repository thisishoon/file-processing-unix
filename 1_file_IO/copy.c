#include <stdio.h>
  
int main(int argc, char *argv[]){

    char buf[100];

    FILE *fp1, *fp2;
    size_t  size;


    fp1 = fopen(argv[1] , "r");   //읽는 목적으로
    fp2 = fopen(argv[2] , "w");   //쓰는 목적으로 
    //최대 100바이트씩 읽어와서
    while ( 0 < (size=fread(&buf,1,100,fp1))    )  {
        //복사본에 쓰기
        fwrite(&buf, 1, size, fp2);
         
    }   
    fclose(fp1);
    fclose(fp2);
      

    
    
    
    
    
    return 0;
}  
