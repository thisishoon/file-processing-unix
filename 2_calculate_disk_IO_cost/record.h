#ifndef _RECORD_H_
#define _RECORD_H_

//student record size = 250 bytes
typedef struct _Student
{
	char id[11];		//�й�: 10 bytes
	char name[31];		//�̸�: 30 bytes
	char address[71];	//�ּ�: 70 bytes
	char univ[31];		//�б�: 30 bytes
	char dept[41];		//�а�: 40 bytes
	char others[71];	//�׿�: 70 bytes
} Student;

#endif
