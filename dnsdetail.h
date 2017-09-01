/*
 * dnsdetail.h
 *
 *  Created on: 2017年8月10日
 *      Author: yuxiang
 */

#ifndef DNSDETAIL_H_
#define DNSDETAIL_H_

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

#define ROUTESIZE 30
#define NAMESIZE 128
#define ROUTENAMESIZE 40


char daydate[20];


struct dnsdetaillist {
	time_t time; //log文件时间字段
	char name[NAMESIZE]; //域名
	char route[ROUTENAMESIZE]; //线路类型
};

struct routelist{
	char routename[ROUTENAMESIZE];
	int number;
};

struct resultdetaillist {
	char time[15]; //时间
	char name[NAMESIZE]; //域名
	struct routelist rou[ROUTESIZE]; //线路访问次数
	int routenumber;
};



struct dnsdetaillist dnslist[3000000];
struct resultdetaillist reslist[100000];
int detaillines;
int detailreslines;

typedef struct denode {
	char keyname[NAMESIZE];
	char time[15];
	struct routelist rou[ROUTESIZE]; //线路访问次数
	int routenumber;
	struct denode *lChild, *rChild;
} detailNode, *detailBSTree;


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


int collectdnsdetail(char logname[], time_t beginhour, time_t endhour) //提取log文件中时间，域名,线路类型字段，保存于dnsdetaillist结构体数组dnslist中
{
	detaillines = 0;
	FILE *read;
	time_t t;
	char line[512], date[20];
	char *str, name[100] = { };
	strcat(name, "/home/yuxiang/test/resolve_");
	strcat(name, logname);
	strcat(name, ".log\0");
	read = fopen(name, "r");
	if (read == NULL) {
		return -1;
	}

	while (fgets(line, 512, read)) {
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
		printf("time:%ld\n",t);
		if (t >= beginhour && t < endhour) {
			dnslist[detaillines].time = t;
			j = 0;
			for (int k = 0; k < 2; k++) {
				while (*str != ' ')
					str++;
				str++;
			}
			while (*str != ',') {
				dnslist[detaillines].name[j] = *str;
				str++;
				j++;
			}
			dnslist[detaillines].name[j] = '\0';
			j = 0;
			for (int k = 0; k < 2; k++) {
				while (*str != ' ')
					str++;
				str++;
			}
			while (*str != ',') {
				dnslist[detaillines].route[j] = *str;
				str++;
				j++;
			}
			dnslist[detaillines].route[j] = '\0';
			detaillines++;
			printf("detaillines:%d\n",detaillines);
		} else if (t >= endhour) {
			fclose(read);
			printf("t >= endhour,detaillines:%d\n",detaillines);
			return 1;
		}
	}
	fclose(read);
	return 1;
}

detailBSTree detailhourinsertBST(detailBSTree T, char time[], char name[],
		char route[]) { //将1小时内的数据插入二叉树中统计数量
	detailBSTree S;
	if (!T) {
		S = (detailNode *) malloc(sizeof(detailNode));
		strcpy(S->rou[0].routename,route);
		S->rou[0].number=1;
		S->routenumber=1;
		strcpy(S->keyname, name);
		strcpy(S->time, time);
		S->lChild = NULL;
		S->rChild = NULL;
		return S;
	} else if (strcmp(name, T->keyname) == 0) {
		int i;
		for(i=0;i<T->routenumber;i++)
		{
			if(strcmp(route, T->rou[i].routename) == 0){
				T->rou[i].number++;
				break;
			}
		}
		if (i == T->routenumber) {
			strcpy(T->rou[T->routenumber].routename, route);
			T->rou[T->routenumber].number=1;
			T->routenumber++;
		}
	} else if (strcmp(name, T->keyname) < 0) {
		T->lChild = detailhourinsertBST(T->lChild, time, name, route);
	} else if (strcmp(name, T->keyname) > 0) {
		T->rChild = detailhourinsertBST(T->rChild, time, name, route);
	}
	return T;
}

detailBSTree detailhourcreateBST(detailBSTree T) { //创建二叉树，统计的是一个小时的数据
	char *timestr;
	for (int i = 0; i < detaillines; i++) {
		timestr = hourtimetostr(dnslist[i].time);
//		printf("%s %s %s\n", timestr, dnslist[i].name, dnslist[i].route); //输出测试
		T = detailhourinsertBST(T, timestr, dnslist[i].name, dnslist[i].route);
	}
	return T;
}

int queryhourdnsdetail(char txtname[], char doname[]) //查询某天24小时某域名各线路访问数据
{
	FILE *read;
	char name[100] = { }, line[512], *str, dn[NAMESIZE];
	strcat(name, "/home/yuxiang/test/log/detailhour_");
	strcat(name, txtname);
	strcat(name, ".log\0");
	read = fopen(name, "r");
	if (read == NULL)
		return -1;
	while (fgets(line, 512, read)) {
		str = line;
		int j = 0;
		while (*str != ',') {
			dn[j] = *str;
			str++;
			j++;
		}
		dn[j] = '\0';
		str++;
		if (strcmp(dn, doname) == 0) {
			for(int i=0;i<4;i++){
				printf("%c",*str);
				str++;
			}
			printf(".");
			for (int i = 0; i < 2; i++) {
				printf("%c", *str);
				str++;
			}
			printf(".");
			for (int i = 0; i < 2; i++) {
				printf("%c", *str);
				str++;
			}
			printf("-");
			for (int i = 0; i < 2; i++) {
				printf("%c", *str);
				str++;
			}
			printf(":");
			for (int i = 0; i < 2; i++) {
				str++;
			}
			printf("%s", str);
		}
	}
	fclose(read);
	return 1;
}

void querydetail(char doname[], char begintime[], char endtime[]) {
	time_t begin, end, temptime;
	int t;
	char *be, *en, bt[15] = { }, et[15] = { };
	be = begintime;
	en = endtime;
	strcat(bt, be);
	strcat(bt, "000000\0");
	strcat(et, en);
	strcat(et, "000000\0");
	begin = strtotime2(bt);
	end = strtotime2(et);
	t = (end - begin) / (3600 * 24) + 1;
	for (int i = 0; i < t; i++) {
		//	printf("%s\n", be);
		queryhourdnsdetail(be, doname);
		temptime = strtotime2(bt) + 3600 * 24 * (i + 1);
		be = daytimetostr(temptime);
	}
}

void detailinordertraverse(detailBSTree T) { //遍历二叉树，将结果存入结构体数组reslist[]中
	if (T != NULL) {
		detailinordertraverse(T->lChild);
		strcpy(reslist[detailreslines].time, T->time);
		strcpy(reslist[detailreslines].name, T->keyname);
		reslist[detailreslines].routenumber=T->routenumber;
		for (int i = 0; i < T->routenumber; i++) {
			strcpy(reslist[detailreslines].rou[i].routename,
					T->rou[i].routename);
			reslist[detailreslines].rou[i].number = T->rou[i].number;

//			printf("detailinordertraverse:%s  %s  %s  %d\n",reslist[detailreslines].time,reslist[detailreslines].name,
//					reslist[detailreslines].rou[i].routename,reslist[detailreslines].rou[i].number);
		}

		detailreslines++;
		detailinordertraverse(T->rChild);
	}
}

int writetodetailhourtxt(char txtname[]) //将每小时统计的数据写入文件保存
{
	FILE *fp;
	char name[100] = { }, buffer[256];
	strcat(name, "/home/yuxiang/test/log/detailhour_");
	strcat(name, txtname);
	strcat(name, ".log\0");
	fp = fopen(name, "a+");
	if (fp == NULL)
		return -1;
	fseek(fp, 0, SEEK_END);
	for (int i = 0; i < detailreslines; i++) {
//		printf("i:%d  %d\n",i,reslist[i].routenumber);
		for (int j = 0; j < reslist[i].routenumber; j++) {
//			printf("j:%d\n",j);
			sprintf(buffer, "%s,%s,%s,%d\n", reslist[i].name, reslist[i].time,
					reslist[i].rou[j].routename, reslist[i].rou[j].number);
			fwrite(buffer, strlen(buffer), 1, fp);

//			printf("writetodetailhourtxt:%s %s %s %d\n", reslist[i].name,
//					reslist[i].time, reslist[i].rou[j].routename,
//					reslist[i].rou[j].number);
		}
	}
	fclose(fp);
	return 1;
}

void writedetailrecord() {
	while (1) {
		char *day;
		time_t begintime, endtime;
		endtime = time(NULL);
		printf("%ld\n",endtime);
		if (endtime % 3600 == 0) {
			begintime = endtime - 3600;
			day = daytimetostr(begintime);
			collectdnsdetail(day, begintime, endtime);
			detailBSTree T;
			T = NULL;
			T = detailhourcreateBST(T);
			detailreslines = 0;
			detailinordertraverse(T);
			day = daytimetostr(begintime);
			writetodetailhourtxt(day);
		}
		sleep(1);
	}
}

#endif /* DNSDETAIL_H_ */
