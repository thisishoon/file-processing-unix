#include <stdio.h>
#include <stdlib.h>
#include "record.h"


int main(int argc, char **argv)
{

if(argc != 2) exit(1);

int recordNum = atoi(argv[1]);

FILE* fstream;
fstream = fopen("students.dat", "w+");

char studentTemp[250];

for(int i=0; i<recordNum; i++){
int k=0;
int j;

for(j=0; j<10; j++){
studentTemp[k] = (rand() % 10) + '0';
k++;
}
for(j=0; j<30; j++){
studentTemp[k] = (rand() % 10) + '0';
k++;
}
for(j=0; j<70; j++){
studentTemp[k] = (rand() % 10) + '0';
k++;
}
for(j=0; j<30; j++){
studentTemp[k] = (rand() % 10) + '0';
k++;
}
for(j=0; j<40; j++){ 
studentTemp[k] = (rand() % 10) + '0';
k++;     
}         
for(j=0; j<70; j++){
studentTemp[k] = (rand() % 10) + '0';
k++;
}            
      
fwrite(studentTemp,250,1,fstream);

}       
    
return 0;
}   
