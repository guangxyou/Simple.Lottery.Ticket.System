#ifndef	USER_OPER_H
#define	USER_OPER_H

#include "common.h"
#include "user_list.h"
#include "lottery_list.h"

typedef struct input	//临时保存登录时输入的name/passwd
{
	char name[NAME_LEN];
	char passwd[PASS_LEN];
}INPUT;

//内部函数声明
char	*fgets_check(char *str, int size);
INPUT	*getinput(INPUT *pu);
INPUT	*user_register(pUser puHead, INPUT *pu);
pUser	user_login_chk(pUser puHead, INPUT *pu);
void	user_view_info(pUser pCurr);	
void	modify_user_name(pUser pCurr);	
void	modify_user_passwd(pUser pCurr);

pLott	get_book_input(pLott pbl);
bool	book_lott(pLott plHead, pUser pCurr);
pLott	getlP_byuid(pLott pStart, int uid);
void	user_view_trade(pLott plHead, int uid);

//外部调用user_list.c
//bool is_ul_empty(pUser puHead);
//void insert_user_tail(pUser puHead, char *name, char *passwd, int balance);
//void print_user_info(pUser p);

//外部调用lott_list.c
//bool is_ll_empty(pLott plHead);
//void insert_lott_tail(pLott plHead, int issue, int uid, 
//	char *name, int lott_num, int multiple, bool sweep_flag, bool win_flag);

#endif
