#ifndef ADMIN_OPER_H
#define	ADMIN_OPER_H

#include "common.h"
#include "user_operate.h"
#include "user_list.h"

#define	PATHGLO		"./data/global.dat"

typedef struct w_histoty
{
	char name[NAME_LEN];
	int issue;
	int lott_num;
	int uid;
	int win_amount;
}WINHIS;

//内部声明
void	write_admfile(char *admin_passwd);
bool	admin_login_chk(INPUT *pa);
void	mod_admin_pw(void)	;
void	admin_view_info(pUser puHead);
pUser	getuP_byuid(pUser puHead, int uid, pUser pMod);	
pUser	admin_mod_uname(pUser puHead, int uid, pUser pMod);	
pUser	admin_mod_upass(pUser puHead, int uid, pUser pMod);
pUser	admin_del_user(pUser puHead, int uid);
void	admin_view_trade(pLott plHead);
int		gene_ram_num(void);
void	write_glo_file(GLO *pglo);
void 	read_glo_file(GLO *pglo);
int		charge_user(pUser pCurr, int money);
pLott	getlP_byissue(pLott pStart, int issue);
void	cash_award(int win_num, GLO *pglo, pUser puHead, pLott plHead);
void	write_winner_file(pLott plHead, int win_num);
void	print_winner_his(WINHIS *his);
void	read_winner_file(void);
void	fuzzy_lookup(int magic_num, char *magic_str, bool search_flag, pUser puHead, pLott plHead);
void	sort_balance(pUser puHead);

//外部调用
//用户登录时临时保存用户名/密码的结构体INPUT		//user_operate.h
//void travers_user_list(pUser puHead);		//user_list.c
//void travers_lott_list(pLott plHead);		//lottery_list.c

extern void	divise_line(void);				//mainsys.h
extern void	before_print_info(void);
extern void	before_print_trade(int print_flag);

#endif
