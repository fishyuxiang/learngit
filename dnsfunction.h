/*
 * function.h
 *
 *  Created on: 2017年7月31日
 *      Author: yuxiang
 */

#ifndef DNSFUNCTION_H_
#define DNSFUNCTION_H_
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
char daydate[20];

struct dnslist {
	time_t time; //log文件时间字段
	char name[20]; //域名
};
struct resultlist {
	char name[20]; //域名
	int number; //域名访问次数
};
struct dnslist dns[1000000];
struct resultlist res[1000000];
int lines;
int reslines;

typedef struct node {
	char keyname[20];
	int number;
	struct node *lChild, *rChild;
} Node, *BSTree;

void print(){
	for (int i = 0; i < reslines; i++) {
		printf("%s  %d\n", res[i].name, res[i].number);
	}
}

char *daytimetostr(time_t t) {
	struct tm *ptime;
	char *d;
	ptime = localtime(&t);
	if ((1 + ptime->tm_mon) > 9 && ptime->tm_mday > 9) {
		sprintf(daydate, "%d%d%d", (1900 + ptime->tm_year), (1 + ptime->tm_mon),
				ptime->tm_mday);
	} else if ((1 + ptime->tm_mon) < 10 && ptime->tm_mday > 9) {
		sprintf(daydate, "%d0%d%d", (1900 + ptime->tm_year),
				(1 + ptime->tm_mon), ptime->tm_mday);
	} else if ((1 + ptime->tm_mon) > 9 && ptime->tm_mday < 10) {
		sprintf(daydate, "%d%d0%d", (1900 + ptime->tm_year),
				(1 + ptime->tm_mon), ptime->tm_mday);
	} else if ((1 + ptime->tm_mon) < 10 && ptime->tm_mday < 10) {
		sprintf(daydate, "%d0%d0%d", (1900 + ptime->tm_year),
				(1 + ptime->tm_mon), ptime->tm_mday);
	}
	d = daydate;
	return d;
}

char *hourtimetostr(time_t t) {
	struct tm *ptime;
	char *d, hour[6];
	ptime = localtime(&t);
	d = daytimetostr(t);
	if ((ptime->tm_hour) > 9) {
		sprintf(hour, "%d0000", ptime->tm_hour);
	} else {
		sprintf(hour, "0%d0000", ptime->tm_hour);
	}
	strcat(d, hour);
	return d;
}

time_t strtotime1(char datetime[]) //字符串转化为时间
{
	struct tm tm_time;
	time_t unixtime;
	strptime(datetime, "%Y-%m-%d %H:%M:%S", &tm_time);
	unixtime = mktime(&tm_time);
	return unixtime;
}

time_t strtotime2(char datetime[]) //字符串转化为时间
{
	struct tm tm_time;
	time_t unixtime;
	strptime(datetime, "%Y%m%d%H%M%S", &tm_time);
	unixtime = mktime(&tm_time);
	return unixtime;
}

int collectdns(char logname[], time_t beginhour, time_t endhour) //提取log文件中时间，域名字段，保存于dnslist结构体数组dns中
{
	lines = 0;
	FILE *read;
	time_t t;
	char line[512], date[20];
	char *str, name[50] = { };
	strcat(name, "/home/yuxiang/test/log/resolve_");
	strcat(name, logname);
	strcat(name, ".log\0");
	read = fopen(name, "r");
	if (read == NULL) {
		return -1;
	}

	while (fgets(line, 128, read)) {
		str = line;
		str++;
		int j = 0;
		while (*str != ']') {
			date[j] = *str;
			str++;
			j++;
		}
		date[j] = '\0';
		t = strtotime1(date);
		if (t >= beginhour && t < endhour) {
			dns[lines].time = t;
			j = 0;
			for (int k = 0; k < 2; k++) {
				while (*str != ' ')
					str++;
				str++;
			}
			while (*str != ',') {
				dns[lines].name[j] = *str;
				str++;
				j++;
			}
			dns[lines].name[j] = '\0';

			lines++;
		}
		else if (t >= endhour) {
			fclose(read);
			return 1;
		}
	}
	fclose(read);
	return 1;
}

BSTree hourinsertBST(BSTree T, char name[]) { //将1小时内的数据插入二叉树中统计数量
	BSTree S;
	if (!T) {
		S = (Node *) malloc(sizeof(Node));
		S->number = 1;
		strcpy(S->keyname, name);
		S->lChild = NULL;
		S->rChild = NULL;
		return S;
	} else if (strcmp(name, T->keyname) == 0) {
		T->number++;

	} else if (strcmp(name, T->keyname) < 0) {
		T->lChild = hourinsertBST(T->lChild, name);
	} else if (strcmp(name, T->keyname) > 0) {
		T->rChild = hourinsertBST(T->rChild, name);
	}
	return T;
}

BSTree hourcreateBST(BSTree T) { //创建二叉树，统计的是一个小时的数据
	for (int i = 0; i < lines; i++) {
		printf("%s\n", dns[i].name); //输出测试
		T = hourinsertBST(T, dns[i].name);
	}
	return T;
}

BSTree dayinsertBST(BSTree T, char name[], int num) { //将一天内统计的数据插入二叉树中再统计求和
	BSTree S;
	if (!T) {
		S = (Node *) malloc(sizeof(Node));
		S->number = num;
		strcpy(S->keyname, name);
		S->lChild = NULL;
		S->rChild = NULL;
		return S;
	} else if (strcmp(name, T->keyname) == 0) {
		T->number = T->number + num;

	} else if (strcmp(name, T->keyname) < 0) {
		T->lChild = dayinsertBST(T->lChild, name, num);
	} else if (strcmp(name, T->keyname) > 0) {
		T->rChild = dayinsertBST(T->rChild, name, num);
	}
	return T;
}

BSTree daycreateBST(BSTree T) { //创建二叉树，统计的是一天的数据
	for (int i = 0; i < reslines; i++) {
//		printf("%s\n", res[i].name); //输出测试
		T = dayinsertBST(T, res[i].name, res[i].number);
	}
	return T;
}


int collectdaydns(char txtname[]) //收集某天统计的域名访问数据
{
	reslines = 0;
	int number = 0;
	FILE *read;
	char name[25] = { }, line[32], *str, numbuf[12];
	strcat(name, "/home/yuxiang/test/log/day_");
	strcat(name, txtname);
	strcat(name, ".log\0");
	read = fopen(name, "r");
	if (read == NULL)
		return -1;
	while (fgets(line, 32, read)) {
		str = line;
		int j = 0;
		while (*str != ' ') {
			res[reslines].name[j] = *str;
			str++;
			j++;
		}
		res[reslines].name[j] = '\0';
		j = 0;
		str++;
		while (*str != '\n') {
			numbuf[j] = *str;
			str++;
			j++;
		}
		numbuf[j] = '\0';
		number = atoi(numbuf);
		res[reslines].number = number;
		reslines++;
	}
	fclose(read);
	return 1;
}

int collecthourdns(char txtname[]) //收集某天24小时统计的域名访问数据
{
	reslines = 0;
	int number = 0;
	FILE *read;
	char name[50] = { }, line[32], *str, numbuf[12];
	strcat(name, "/home/yuxiang/test/log/hour_");
	strcat(name, txtname);
	strcat(name, ".log\0");
	read = fopen(name, "r");
	if (read == NULL)
		return -1;
	while (fgets(line, 32, read)) {
		str = line;
		int j = 0;
		while (*str != ' ') {
			res[reslines].name[j] = *str;
			str++;
			j++;
		}
		res[reslines].name[j] = '\0';
		j = 0;
		str++;
		while (*str != '\n') {
			numbuf[j] = *str;
			str++;
			j++;
		}
		numbuf[j] = '\0';
		number = atoi(numbuf);
		res[reslines].number = number;
		reslines++;
	}
	fclose(read);
	return 1;
}

BSTree createBST(char begintime[], char endtime[], BSTree T) {
	time_t begin, end, temptime,now;
	int t;
	char *be, *en, bt[15] = { }, et[15] = { },*nowstr;
	be = begintime;
	en = endtime;
	strcat(bt, be);
	strcat(bt, "000000\0");
	strcat(et, en);
	strcat(et, "000000\0");
	begin = strtotime2(bt);
	end = strtotime2(et);
	now =time(NULL);
	nowstr=daytimetostr(now);
	t = (end - begin) / (3600 * 24) + 1;
	if (strcmp(nowstr, endtime) > 0) {
		for (int i = 0; i < t; i++) {
//		    printf("%s\n",be);
			collectdaydns(be);
			for (int j = 0; j < reslines; j++) {
//		    	printf("%s\n", res[j].name); //输出测试
				T = dayinsertBST(T, res[j].name, res[j].number);
			}
			temptime = strtotime2(bt) + 3600 * 24 * (i + 1);
			be = daytimetostr(temptime);
		}
	}
	else if (strcmp(nowstr, endtime) == 0){
		for (int i = 0; i < t-1; i++) {
			//		    printf("%s\n",be);
			printf("%d\n", collectdaydns(be));
			for (int j = 0; j < reslines; j++) {
				//		    	printf("%s\n", res[j].name); //输出测试
				T = dayinsertBST(T, res[j].name, res[j].number);
			}
			temptime = strtotime2(bt) + 3600 * 24 * (i + 1);
			be = daytimetostr(temptime);
		}
		printf("%d\n", collecthourdns(be));
		for (int j = 0; j < reslines; j++) {
			//		    	printf("%s\n", res[j].name); //输出测试
			T = dayinsertBST(T, res[j].name, res[j].number);
		}
	}
	return T;
}

void inordertraverse(BSTree T) { //遍历二叉树，将结果存入结构体数组res[]中
	if (T != NULL) {
		inordertraverse(T->lChild);
		strcpy(res[reslines].name, T->keyname);
		res[reslines].number = T->number;
		reslines++;
		inordertraverse(T->rChild);
	}
}

int writetohourtxt(char txtname[]) //将每小时统计的数据写入文件保存
{
	FILE *fp;
	char name[50] = { }, buffer[32];
	strcat(name, "/home/yuxiang/test/log/hour_");
	strcat(name, txtname);
	strcat(name, ".log\0");
	fp = fopen(name, "a+");
	if (fp == NULL)
		return -1;
	fseek(fp, 0, SEEK_END);
	for (int i = 0; i < reslines; i++) {
		sprintf(buffer, "%s %d\n", res[i].name, res[i].number);
		fwrite(buffer, strlen(buffer), 1, fp);
//		printf("%s    \n",buffer);
	}
	fclose(fp);
	return 1;
}

int writetodaytxt(char txtname[]) //将每天统计的数据写入文件保存
{
	FILE *fp;
	char name[25] = { }, buffer[32];
	strcat(name, "/home/yuxiang/test/log/day_");
	strcat(name, txtname);
	strcat(name, ".log\0");
	fp = fopen(name, "a+");
	if (fp == NULL)
		return -1;
	for (int i = 0; i < reslines; i++) {
		sprintf(buffer, "%s %d\n", res[i].name, res[i].number);
		fwrite(buffer, strlen(buffer), 1, fp);
//		printf("%s    \n",buffer);
	}
	fclose(fp);
	return 1;
}

void writerecord() {
	while (1) {
		char *day;
		time_t begintime, endtime;
		endtime = time(NULL);
		if (endtime / 3600 == 0 && endtime / (24 * 3600) != 0) {
			begintime = endtime - 3600;
			day = daytimetostr(endtime);
			printf("%d\n", collectdns(day, begintime, endtime));
			BSTree T;
			T = NULL;
			T = hourcreateBST(T);
			printf("\n");
			inordertraverse(T);
			writetohourtxt(day);
		}

		if (endtime / (24 * 3600) == 0) {
			begintime = endtime - 3600;
			day = daytimetostr(begintime);
			collectdns(day, begintime, endtime);
			BSTree Th, Td;
			Th = NULL;
			Th = hourcreateBST(Th);
			printf("\n");
			reslines = 0;
			inordertraverse(Th);
			writetohourtxt(day);
			collecthourdns(day);
			printf("\n");
			print();
			Td = NULL;
			Td = daycreateBST(Td);
			printf("\n");
			reslines = 0;
			inordertraverse(Td);
			print();
			writetodaytxt(day);
		}

	}
}

void swapnumber(int *a, int *b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void swapname(char *a, char *b) {
	char t[20];
	strcpy(t, a);
	strcpy(a, b);
	strcpy(b, t);
}

void quicksort(int begin, int end)	//对res结构体数组进行快速排序，(n*logn)
{
	int i, j;

	if (begin < end) {
		i = begin + 1;
		j = end;

		while (i < j) {
			if (res[i].number < res[begin].number) {
				swapnumber(&(res[i].number), &(res[j].number));
				swapname(res[i].name, res[j].name);
				j--;
			} else {
				i++;
			}
		}

		if (res[i].number <= res[begin].number)
			i--;
		swapnumber(&(res[begin].number), &(res[i].number));
		swapname(res[begin].name, res[i].name);
		quicksort(begin, i);
		quicksort(j, end);
	}
}

void show(int n) {
	for (int i = 0; i < n; i++) {
		printf("%d %s %d\n",i+1,res[i].name, res[i].number);
	}
}

#endif /* DNSFUNCTION_H_ */
