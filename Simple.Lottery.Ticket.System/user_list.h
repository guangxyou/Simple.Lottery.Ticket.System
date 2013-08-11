#ifndef USER_LIST_H
#define USER_LIST_H

#include "common.h"

#define	START_UID		1000
#define	START_FUNDS		100

typedef struct user
{
	int uid;
	char name[NAME_LEN];
	char passwd[PASS_LEN];
	float balance;
	struct user *next;
}USER, *pUser;

//内部函数声明
pUser	creat_void_user_list(void);
bool	is_ul_empty(pUser puHead);
void	print_user_info(pUser p);
void	travers_user_list(pUser puHead);
void	insert_user_tail(pUser puHead, char *name, char *passwd, int balance);
void	clean_file(char *path);
void	save_user_info(pUser puHead);
void	load_user_info(pUser puHead);
void	free_user_list(pUser puHead);

#endif
