/*功能：
 *		1.充值
 *		2.摇号
 *		3.修改管理员密码
 *		4.查看彩民信息(根据uid查找)
 *		5.修改彩民密码(根据uid查找)		
 *		6.查看历史购票信息
 *		7.查看历史中奖记录
 *		8.兑奖
 *		9.模糊搜索
 *		10.余额降序排序
 *内部函数：
 *		void	write_admfile(char *admin_passwd);
 *		bool	admin_login_chk(INPUT *pa);
 *		void	mod_admin_pw(void)	;
 *		void	admin_view_info(pUser puHead);
 *		pUser	getuP_byuid(pUser puHead, int uid, pUser pMod);	
 *		pUser	admin_mod_uname(pUser puHead, int uid, pUser pMod);	
 *		pUser	admin_mod_upass(pUser puHead, int uid, pUser pMod);
 *		void	admin_view_trade(pLott plHead);
 *		int		gene_ram_num(void);
 *		void	write_winner_file(pLott plHead);
 *		int		charge_user(pUser pCurr, int money);
 *		void	cash_award(int win_num, GLO *pglo, pUser puHead, pLott plHead);
 *		pLott	getlP_byissue(pLott pStart, int issue);
 *		void	read_glo_file(GLO *pglo);
 *		void	write_glo_file(GLO *pglo);
 *		pUser	search_user_list(int magic_num, pUser puHead);
 *		pLott	search_lott_list(int magic_num, pLott plres);
 *		void	fuzzy_lookup(int magic_num, pUser puHead, pLott plHead);
 *		void	sort_balance(pUser puHead)；
 *
 *函数功能：
 *		把密码写进data/.admin文件,用于更改管理员密码;
 *		读取admin文件检查密码,admin文件中只有密码,管理员名只能是admin;
 *		隐藏功能,当管理员输入要修改的uid为1000时,才可以修改管理员密码;
  *		管理员查看所有彩民个人信息;
 *		根据UID查找用户,并返回此UID的结点指针,用于修改信息;	
 *		管理员修改用户名字;	
 *		管理员修改用户名字;
 *		管理员查看所有的购票记录;
 *		产中中奖号码
 *		将中奖信息写入到文件
 *		给用户充值
 *		兑奖
 *		写全局文件
 *		读全局文件
 *		模糊搜索->查找用户链
 *		模糊搜索->查找彩票链
 *		按照用户余额降序排序
 */

#include "admin_operate.h"

void write_admfile(char *admin_passwd)
{
	FILE *fpAd = fopen("./data/.admin", "wb");
	if (fpAd == NULL)
	{
		printf("\t\t\tOpen admin file failed.\n");
		exit(1);
	}
	fwrite(admin_passwd, PASS_LEN, 1, fpAd);
	if (fclose(fpAd) != 0)
	{
		printf("\t\t\tClose admin file failed.\n");
		exit(1);
	}
}

bool admin_login_chk(INPUT *pa)
{
	char admin_passwd[PASS_LEN] = {'\0'};

	FILE *fpAd = fopen("./data/.admin", "rb");
	if (fpAd == NULL)
	{
		printf("\t\t\tRead admin file failed.\n");
		exit(1);
	}
	fread(admin_passwd, PASS_LEN, 1, fpAd);
	if (fclose(fpAd) != 0)
	{
		printf("\t\t\tClose admin file failed.\n");
		exit(1);
	}
	
	if (!strcmp(pa->name, "admin"))
	{
		if (!strcmp(pa->passwd, admin_passwd))
			return true;
	}
	else
	{
		return false;
	}
	return false;
}

void mod_admin_pw(void)		//隐藏功能,当输入uid=1000时修改管理员密码
{
	char newpass[PASS_LEN] = {'\0'};
	printf("\t\t\t\033\[;31m\033[1mNOTICE: You are modifing admin passwd.\n\033[0m");
	printf("\t\t\tEnter new passwd: ");
	fgets(newpass, PASS_LEN, stdin);
	fgets_check(newpass, PASS_LEN);
	write_admfile(newpass);
	printf("\t\t\tAdmin passwd is updated.\n");
}

void admin_view_info(pUser puHead)
{
	travers_user_list(puHead);
}

pUser getuP_byuid(pUser puHead, int uid, pUser pMod)
{
	if ( is_ul_empty(puHead) )
	{
		printf("\t\t\tThere is no user.\n");
		return NULL;
	}

	pMod = puHead->next;
	int id = START_UID + 1;
	while (id<uid && pMod != NULL)
	{
		pMod = pMod->next;
		id++;
	}
	if (pMod == NULL)
	{
		printf("\t\t\tWrong uid.");
		return NULL;
	}
	else if (id == uid)
	{
		return pMod;
	}

	return NULL;
}

pUser admin_mod_uname(pUser puHead, int uid, pUser pMod)	//管理员修改用户姓名
{
	
	pMod = getuP_byuid(puHead, uid, pMod);
	if (pMod == NULL)
	{
		return NULL;
	}
	else
	{
		modify_user_name(pMod);
		return pMod;
	}		
}

pUser admin_mod_upass(pUser puHead, int uid, pUser pMod)	//管理员修改用户密码
{
	pMod = getuP_byuid(puHead, uid, pMod);
	if (pMod == NULL)
	{
		return NULL;
	}
	else
	{
		modify_user_passwd(pMod);
		return pMod;
	}
}

pUser admin_del_user(pUser puHead, int uid)
{
	pUser pDel = NULL;
	pUser pre = puHead;
	pDel = getuP_byuid(puHead, uid, pDel);

	if (pDel == NULL)
		return puHead;

	while (pre->next != pDel)
	{
		pre = pre->next;
	}
	
	pre->next = pDel->next;
	free(pDel);
	pDel = NULL;

	return puHead;
}

void admin_view_trade(pLott plHead)
{
	travers_lott_list(plHead);
}

int gene_ram_num(void)			//产生随机中奖号码
{
	int num = 0;
	srand(time(NULL));
	num = rand() % 3 + 1;
	return num;
}

void write_glo_file(GLO *pglo)			//写全局变量文件，每当开奖后写一次
{
	FILE *fpGlo = fopen(PATHGLO, "wb");
	if (fpGlo == NULL)
	{
		printf("\t\t\tOpen global file failed.\n");
		exit(1);
	}
	fwrite(pglo, sizeof(GLO), 1, fpGlo);
	if (fclose(fpGlo) != 0)
	{
		printf("\t\t\tClose global file failed.\n");
		exit(1);
	}
}

void read_glo_file(GLO *pglo)			//读全局变量文件，程序运行开始后运行
{
	FILE *fpGlo = fopen(PATHGLO, "rb");
	if (fpGlo == NULL)
	{
		printf("\t\t\tOpen global file failed.\n");
		exit(1);
	}
	fread(pglo, sizeof(GLO), 1, fpGlo);
	if (fclose(fpGlo) != 0)
	{
		printf("\t\t\tClose global file failed.\n");
		exit(1);
	}
}

/*兑奖，查找当期号码，号码为中奖号的用户，计算奖池剩余金额，给用户发放奖金*/
pLott getlP_byissue(pLott pStart, int issue)	//查找包含特定issue的结点并返回结点指针
{
	pLott pS = pStart;
	while (pS != NULL)
	{
		pS->sweep_flag = true;				//开奖标志位
		if (pS->issue == issue)
		{
			return pS;
		}
		pS = pS->next;
	}
	return NULL;
}

int charge_user(pUser pCurr, int money)		//管理员给用户充值/兑奖
{
	pCurr->balance += money;
	return money;
}

void cash_award(int win_num, GLO *pglo, pUser puHead, pLott plHead)		//开奖，计算奖池金额，兑奖
{
	if ( is_ll_empty(plHead) )
	{
		printf("\t\t\tThere is no trade.\n");
		return ;
	}
	int issue = pglo->g_issue;
	int book_amount = 0;		//购买彩票所用的金额
	int win_amount = 0;			//奖金=注数*PERAWD
	pUser pCurr = NULL;
	pLott pStart = plHead->next;
	
	while (pStart != NULL)
	{
		pStart = getlP_byissue(pStart, issue);				//查找期号并返回当前结点
		if (pStart != NULL)
		{
			book_amount = pStart->multiple * BASE1;			//计算购买彩票的金额
			pglo->g_jackpot += book_amount;					//奖池金额更新，金额增加
			if (pStart->lott_num == win_num)
			{
				pStart->win_flag = true;					//中奖标志
				win_amount = pStart->multiple * PERAWD;		//中奖金额计算
				pglo->g_jackpot -= win_amount;				//奖池金额更新，金额减少
				//查找到uid返回结点指针，兑奖用到
				pCurr = getuP_byuid(puHead, pStart->uid, pCurr);
				if (pCurr == NULL)
				{
					printf("\t\t\t\033[;31mUnexpected error!\n\033[0m");
					return ;
				}
				charge_user(pCurr, win_amount);				//给用户兑奖
			}
			pStart = pStart->next;
		}

	}
	pglo->g_issue += 1;					//开奖完成之后期号要加1
	plHead->issue = pglo->g_issue;		//更新彩票链表头中的期号
	write_glo_file(pglo);
	write_winner_file(plHead, win_num);			//将中奖信息写入文件
}

void write_winner_file(pLott plHead, int win_num)
{
	if ( is_ll_empty(plHead) )
	{
		return ;
	}
	
	FILE *fpWin = fopen(PATH_WIN, "wb");
	if (fpWin == NULL)
	{
		printf("\t\t\tOpen winner file failed.\n");
		exit(1);
	}
	
	WINHIS history, *his;
	his = &history;
	
	pLott pFind = plHead->next;
	int issue = START_ISSUE-1;
	int tmpissue = 0;
	while (pFind != NULL)
	{
		tmpissue = pFind->issue;
		while ( pFind!=NULL && tmpissue==pFind->issue )		//一期彩票有未中奖的,检索链表一直到下一期
		{
			if ( pFind->win_flag )
			{
				his->issue = pFind->issue;
				his->lott_num = pFind->lott_num;
				his->uid = pFind->uid;
				strcpy(his->name, pFind->name);
				his->win_amount = pFind->multiple * PERAWD;
				fwrite(his, sizeof(WINHIS), 1, fpWin);
			}
			issue = his->issue;
			pFind = pFind->next;
		}
		if (pFind!=NULL && pFind->issue>issue)				//如果此期彩票没有人中奖，写入相关信息.
		{
			while (pFind->issue > issue)		//某期彩票没有人买彩票,需要循环操作
			{
				issue++;
				his->issue = issue;
				his->lott_num = win_num;
				his->uid = 0;
				strcpy(his->name, "");
				his->win_amount = 0;
				fwrite(his, sizeof(WINHIS), 1, fpWin);
			}
		}
		if (pFind != NULL )
			pFind = pFind->next;
	}

	if (fclose(fpWin) != 0)
	{
		printf("\t\t\tClose winner file failed.\n");
		exit(1);
	}
}

void print_winner_his(WINHIS *his)
{
	printf("\t\t\t%d\t    %d\t\t%d\t%s"
	      ,his->issue, his->lott_num, his->uid, his->name);
	int i = 8 - strlen(his->name);
	while (i--)
		printf(" ");
	printf("     %dyuan",his->win_amount);
	printf("\n");
}

void read_winner_file(void)			//读取中奖者文件并输出
{
	FILE *fpWin = fopen(PATH_WIN, "rb");
	if (fpWin == NULL)
	{
		printf("\t\t\tOpen winner file failed.\n");
		exit(1);
	}

	WINHIS history, *his;
	his = &history;
	int cnt = 0;
	cnt = fread(his, sizeof(WINHIS), 1, fpWin);

	if (cnt != 0)
	{
		printf("\t\t\tIssue\tWin number\tUID\tNAME\tWinning amount\n");
		print_winner_his(his);
		while (1)
		{
			fread(his, sizeof(WINHIS), 1, fpWin);
			if ( feof(fpWin) )
				break;
			print_winner_his(his);
		}
	}

	if (fclose(fpWin) != 0)
	{
		printf("\t\t\tClose winner file failed.\n");
		exit(1);
	}
}

pUser search_user_list(int magic_num, pUser puHead)			//模糊查找
{
	pUser p = puHead;
	while (p != NULL)
	{
		if (p->uid == magic_num)
		{
			return p;
		}
		p = p->next;
	}

	return NULL;
}

pLott search_lott_list(int magic_num, pLott plres)
{
	pLott p = plres;
	while (p != NULL)
	{
		if (p->issue == magic_num)
		{
			return p;
		}
		p = p->next;
	}

	return NULL;
}

void fuzzy_lookup(int magic_num, char *magic_str, bool search_flag, pUser puHead, pLott plHead)
{
	pUser pures = NULL;
	pLott plres = NULL;
	if ( search_flag )
	{
		if (magic_num>START_UID && magic_num<START_ISSUE)
		{
			pures = search_user_list(magic_num, puHead);
			if (pures != NULL)
			{
				divise_line();
				before_print_info();
				user_view_info(pures);
				divise_line();
				before_print_trade(1);	//用户打印模式
				user_view_trade(plHead, pures->uid);
				divise_line();
			}
		}
		if (magic_num >= START_ISSUE)
		{
			divise_line();
			before_print_trade(0);
			while (plres!=NULL && (plres=search_lott_list(magic_num,plres))!=NULL)
			{
				print_user_trade(plres);
				plres = plres->next;
			}
			divise_line();
		}
	}
	else
	{
		pures = puHead;
		while (pures->next != NULL)
		{
			if (!strcmp(pures->next->name, magic_str))
			{
				divise_line();
				before_print_info();
				user_view_info(pures->next);
				divise_line();
				break;
			}
			pures = pures->next;
		}
		plres = plHead;
		divise_line();
		before_print_trade(0);
		while (plres->next != NULL)
		{
			if (!strcmp(plres->next->name, magic_str))
			{
				print_user_trade(plres->next);
			}
			plres = plres->next;
		}
		divise_line();
	}
}

void sort_balance(pUser puHead)	//根据余额进行降序排序	
{
	save_user_info(puHead);
	pUser ptmp = puHead;		//保存头指针地址,load之前再赋给puHead
	pUser ps = NULL;
	pUser pre = NULL;			//保留前指针,释放结点用
	pUser pMax = NULL;
	while (puHead->next != NULL)
	{
		ps = puHead->next;
		pre = puHead;
		pMax = ps;
		while (ps->next != NULL)
		{
			if (pMax->balance < ps->next->balance)
			{
				pMax = ps->next;
				pre = ps;
			}
			ps = ps->next;
		}
		user_view_info(pMax);
		if (pMax->next != NULL)
			pre->next = pMax->next;
		else
			pre->next = NULL;
		free(pMax);
	}
	puHead = ptmp;
	load_user_info(puHead);
}
