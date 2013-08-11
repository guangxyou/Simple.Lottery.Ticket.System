#ifndef LOTT_LIST_H
#define LOTT_LIST_H

#include "common.h"

#define	START_ISSUE		1301
#define	BASE1			5		//每注彩票5元
#define	PERAWD			20

typedef struct lott
{
	bool sweep_flag;		//0没有开奖，1已开奖
	bool win_flag;			//0没有中奖，1中奖
	char name[NAME_LEN];	//购买彩票用户的姓名
	int issue;				//彩票期号
	int uid;				//购买彩票用户的UID
	int lott_num;			//彩票号码
	int multiple;			//注数
	int book_amount;		//购买彩票的金额=multiple*BASE1
	
	struct lott *next;
}LOTT, *pLott;

//内部函数
pLott	creat_void_lott_list(void);
bool	is_ll_empty(pLott plHead);
void	print_user_trade(pLott p);
void	travers_lott_list(pLott plHead);
void 	insert_lott_tail(pLott plHead, int issue, int uid, 
        char *name, int lott_num, int multiple, bool sweep_flag, bool win_flag);
void	save_user_trade(pLott plHead);
void	load_user_trade(pLott plHead);
void	free_lott_list(pLott plHead);

#endif
