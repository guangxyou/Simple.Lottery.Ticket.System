#ifndef MAINSYS_H
#define MAINSYS_H

#include "common.h"
#include "main_menu.h"
#include "user_list.h"
#include "lottery_list.h"
#include "user_operate.h"
#include "admin_operate.h"

//内部函数声明
void	main_menu(void);			//主界面
void	sys_reset(void);			//重置所有信息
void	admin_func(INPUT *pu);		//管理员功能
void	user_func(INPUT *pu, pUser pCurr, char login_flag);	//用户功能

void	press_any_key(void);
void	divise_line(void);
void	mod_choice(void);
void	searching(void);
bool	operate_choice(int *num);
void	before_print_info(void);
void	before_print_trade(int print_flag);

//外部函数调用
//pUser	creat_void_user_list(pUser puHead);				//user_list.c
//void	save_user_info(pUser puHead);
//void	load_user_info(pUser puHead);

//pLott	creat_void_lott_list(pLott plHead);				//lott_list.c
//void	save_user_trade(pLott plHead);
//void	load_user_trade(pLott plHead);

//INPUT	*getinput(INPUT *pu);							//user_operate.c
//INPUT	*user_register(pUser puHead, INPUT *pu);
//pUser	*user_login_chk(pUser puHead, INPUT *pu);
//void	user_view_info(pUser pCurr);	
//void	modify_user_name(pUser pCurr);	
//void	modify_user_passwd(pUser pCurr);

//bool	book_lott(pLott plHead, pUser pCurr);
//void	user_view_trade(pLott plHead, int uid);

			
//bool	admin_login_chk(INPUT *pa);						//admin_operate.c
//void	mod_admin_pw(void);
//void	admin_view_info(pUser puHead);
//pUser	admin_mod_uname(pUser puHead, int uid, pUser pMod);	
//pUser	admin_mod_upass(pUser puHead, int uid, pUser pMod);
//void	admin_view_trade(pLott plHead);

//void	admin_menu(void);								//main_menu.c
//void	admin_menu(void);
//void	user_menu(void);
//void	user_register_menu(void);

#endif
