#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define	NAME_LEN	25
#define	PASS_LEN	13
#define	PATH_LOTT		"./data/lottery_info.dat"
#define	PATH_USER		"./data/user_info.dat"
#define	PATH_WIN		"./data/winner.dat"

typedef struct global
{
	int g_issue;		//期号
	float g_jackpot;	//奖池余额
}GLO;

#endif
