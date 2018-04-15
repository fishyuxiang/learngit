/*
 * function.h
 *
 *  Created on: 2018年3月7日
 *      Author: yuxiang
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

#define CODESIZE 30


struct vehiclelist{
	int vehicle_id;
	int timesum;
	int milesum;
	int timeavg;
	int mileavg;
	int allnumber;
	int number0;
	int number1;
	int ispass;
};

struct codelist{
	int item_code;
	struct vehiclelist vehicle[10];
	int vehiclenumber;
};

typedef struct studentnode {
	int studentid;
	int age;
	int sex;
	struct codelist code[CODESIZE]; //训练项目
	int codenumber;
	struct studentnode *lChild, *rChild;
} Node, *BSTree;

struct result {
	int age;
	int sex;
	int code;
	int vehicle;
	int time;
	int mile;
	int pass;
};


BSTree insertBST(BSTree T, int studentid,int age, int sex,int code,int vehicle_id,int time,int mile,
		int pass) {
	BSTree S;
	if (!T) {
		S = (Node *) malloc(sizeof(Node));
		S->studentid=studentid;
		S->age=age;
		S->sex=sex;
		S->code[0].item_code=code;
		S->code[0].vehicle[0].vehicle_id=vehicle_id;
		S->codenumber=1;
		S->code[0].vehiclenumber=1;
		S->code[0].vehicle[0].allnumber=1;
		S->code[0].vehicle[0].timesum=time;
		S->code[0].vehicle[0].timeavg=time;
		S->code[0].vehicle[0].milesum=mile;
		S->code[0].vehicle[0].mileavg=mile;
		S->code[0].vehicle[0].ispass=pass;
		if(pass==0)
			S->code[0].vehicle[0].number0=1;
		else
			S->code[0].vehicle[0].number1=1;


		S->lChild = NULL;
		S->rChild = NULL;
		return S;
	} else if (T->studentid==studentid) {
		int i;
		for(i=0;i<T->codenumber;i++)
		{
			if(T->code[i].item_code==code){
				int j;
				for(j=0;j<T->code[i].vehiclenumber;j++)
				{
					if(T->code[i].vehicle[j].vehicle_id==vehicle_id){
						T->code[i].vehicle[j].allnumber++;
						T->code[i].vehicle[j].milesum=T->code[i].vehicle[j].milesum+mile;
						T->code[i].vehicle[j].timesum=T->code[i].vehicle[j].timesum+time;
						T->code[i].vehicle[j].mileavg=T->code[i].vehicle[j].milesum/T->code[i].vehicle[j].allnumber;
						T->code[i].vehicle[j].timeavg=T->code[i].vehicle[j].timesum/T->code[i].vehicle[j].allnumber;
						if(pass==0)
							T->code[i].vehicle[j].number0++;
						else
							T->code[i].vehicle[j].number1++;
						if(T->code[i].vehicle[j].number0>T->code[i].vehicle[j].number1)
							T->code[i].vehicle[j].ispass=0;
						else
							T->code[i].vehicle[j].ispass=1;
						break;
					}
				}
				if(j==T->code[i].vehiclenumber){
					T->code[i].vehicle[j].vehicle_id=vehicle_id;

					T->code[i].vehiclenumber++;
					T->code[i].vehicle[j].allnumber=1;
					T->code[i].vehicle[j].timesum=time;
					T->code[i].vehicle[j].timeavg=time;
					T->code[i].vehicle[j].milesum=mile;
					T->code[i].vehicle[j].mileavg=mile;
					T->code[i].vehicle[j].ispass=pass;
					if(pass==0)
						T->code[0].vehicle[0].number0=1;
					else
						T->code[0].vehicle[0].number1=1;
				}
				break;

			}

		}
		if (i == T->codenumber) {
			//printf("%s    %d\n","hahah",i);
			T->code[i].item_code=code;
			//printf("%s    %d\n","babababbabab",T->code[i].item_code);
			T->code[i].vehicle[0].vehicle_id=vehicle_id;

			T->code[i].vehiclenumber=1;
			T->code[i].vehicle[0].allnumber=1;
			T->code[i].vehicle[0].timesum=time;
			T->code[i].vehicle[0].timeavg=time;
			T->code[i].vehicle[0].milesum=mile;
			T->code[i].vehicle[0].mileavg=mile;
			T->code[i].vehicle[0].ispass=pass;
			if(pass==0)
				T->code[i].vehicle[0].number0=1;
			else
				T->code[i].vehicle[0].number1=1;
			T->codenumber++;
		}
	} else if (T->studentid>studentid) {
		T->lChild = insertBST(T->lChild, studentid,age, sex,code,vehicle_id,time,mile,pass);
	} else if (T->studentid<studentid) {
		T->rChild = insertBST(T->rChild, studentid,age, sex,code,vehicle_id,time,mile,pass);
	}
	return T;
}


BSTree createBST(BSTree T)
{

	FILE *read;
	char line[512],student[6],age[3],sex[2],code[6],vehicle[5],time[5],mile[5],pass[2];
	int st,a,s,c,v,t,m,p;
	char *str, name[100] = { };
	strcat(name, "/home/yuxiang/test/log/1.log\0");
	read = fopen(name, "r");
	if (read == NULL) {
		printf("read error!\n");
		return NULL;
	}

	while (fgets(line,100, read)) {
		str = line;
		//str++;
		int j = 0;
		while (*str != ' ') {
			student[j] = *str;
			str++;
			j++;
		}
		student[j] = '\0';
		st=atoi(student);
		//printf("%s\n",student);

		j = 0;
		while (*str ==' ')
			str++;
		while (*str != '-') {
					age[j] = *str;
					str++;
					j++;
				}
		age[j] = '\0';
		a=atoi(age);
		//printf("%s\n",age);

		for (int k = 0; k < 2; k++) {
			while (*str != ' ')
				str++;
			str++;
			}

		j=0;
		while (*str != ' ') {
			sex[j] = *str;
			str++;
			j++;
		}
		sex[j] = '\0';
		s=atoi(sex);
		//printf("%s\n",sex);

		while (*str ==' ')
			str++;

		j=0;
		while (*str != ' ') {
					code[j] = *str;
					str++;
					j++;
				}

		code[j] = '\0';
		c=atoi(code);
		//printf("%s\n",code);

		while (*str ==' ')
			str++;

		j=0;
		while (*str != ' ') {
			vehicle[j] = *str;
			str++;
			j++;
			}

		vehicle[j] = '\0';
		v=atoi(vehicle);
		//printf("%s\n",vehicle);

		while (*str ==' ')
			str++;

		j=0;
		while (*str != ' ') {
			time[j] = *str;
			str++;
			j++;
		}

		time[j] = '\0';
		t=atoi(time);
		//printf("%s\n",time);

		while (*str ==' ')
			str++;

		j=0;
		while (*str != ' ') {
			mile[j] = *str;
			str++;
			j++;
		}


		mile[j] = '\0';
		m=atoi(mile);
		//printf("%s\n",mile);

		while (*str ==' ')
			str++;

		j=0;
		while (*str != ' ') {
			pass[j] = *str;
			str++;
			j++;
		}
		pass[j] = '\0';
		p=atoi(pass);
		//printf("%s\n",pass);

		/*printf("%d ",st);
		printf("%d ",2017-a);
		printf("%d ",s);
		printf("%d ",c);
		printf("%d ",v);
		printf("%d ",t);
		printf("%d ",m);
		printf("%d \n",p);
		printf("%s \n","aaaaaaaaaaaaaaa");*/

		/*st=atoi(student);
		a=2017-atoi(age);
		s=atoi(sex);
		c=atoi(code);
		v=atoi(vehicle);
		t=atoi(time);
		m=atoi(mile);
		p=atoi(pass);*/
		/*printf("%s ",student);
				printf("%s ",age);
				printf("%s ",sex);
				printf("%s ",code);
				printf("%s ",vehicle);
				printf("%s ",time);
				printf("%s ",mile);
				printf("%s \n",pass);*/

		//printf("%d %d %d %d %d %d %d \n",atoi(student),2017-atoi(age), atoi(sex),atoi(code),atoi(vehicle),atoi(time),atoi(mile),atoi(pass));
		//printf("%s %s %s %s %s %s %s \n",student,age, sex,code,vehicle,time,mile,pass);
		T=insertBST(T,st,a, s,c,v,t,m,p);

	}
	fclose(read);
	printf("read success!\n");
	return T;
}

void inordertraverse(BSTree T) { //
	if (T != NULL) {
		inordertraverse(T->lChild);
		FILE *fp;
		char name[100] = { }, buffer[128];
		strcat(name, "/home/yuxiang/test/log/2.log\0");

		fp = fopen(name, "a+");
		if (fp == NULL)
			printf("read error!\n");
		fseek(fp, 0, SEEK_END);

		for(int i=0;i<T->codenumber;i++){
			for(int j=0;j<T->code[i].vehiclenumber;j++){
				sprintf(buffer, "%d %d %d %d %d %d %d \n", 2017-T->age,T->sex,T->code[i].item_code,T->code[i].vehicle[j].vehicle_id,T->code[i].vehicle[j].timeavg,T->code[i].vehicle[j].mileavg,T->code[i].vehicle[j].ispass);
				fwrite(buffer, strlen(buffer), 1, fp);
						}
					}
		fclose(fp);
		//printf("%d %d %d %d %d %d %d \n", 2017-T->age,T->sex,T->code[i].item_code,T->code[i].vehicle[j].vehicle_id,T->code[i].vehicle[j].timeavg,T->code[i].vehicle[j].mileavg,T->code[i].vehicle[j].ispass);
		inordertraverse(T->rChild);
	}
}


#endif /* FUNCTION_H_ */
