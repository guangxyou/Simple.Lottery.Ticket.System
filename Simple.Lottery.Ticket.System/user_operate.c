/*已完成功能：
 *		1.注册
 *		2.购买彩票
 *		3.查看购票历史
 *		4.查看个人信息
 *		2.更改个人信息
 */
/*内部函数：
 *		char	*fgets_check(char *str, int size);
 *		INPUT	*getinput(INPUT *pu);
 *		INPUT	*user_register(pUser puHead, INPUT *pu);
 *		pUser	user_login_chk(pUser puHead, INPUT *pu);
 *		void	user_view_info(pUser pCurr);	
 *		void	modify_user_name(pUser pCurr);	
 *		void	modify_user_passwd(pUser pCurr);
 *		pLott	get_book_input(pLott pbl);
 *		bool	book_lott(pLott plHead, pUser pCurr);
 *		pLott	getlP_byuid(pLott pStart, int uid);
 *		void	user_view_trade(pLott plHead, int uid);
 *
 *函数功能：
 *		检查fgets函数获得的输入,去除字符串中的\n,清缓存;
 *		获取登录或者注册时的输入的name/passwd;
 *		用户注册,返回包含名字和密码的结构体指针pu;
 *		用户登录检查,遍历链表;
 *		用户查看自己的个人信息;
 *		用户修改个人名字;
 *		用户修改个人密码;
  *		用户输入购买的彩票信息(号码/注数);
 *		用户购买彩票(获取输入/计算余额/增加历史信息);
 *		根据UID得到彩民链表中包含此uid的结点;
 *		用户查看自己的购票记录;
 */
 /*用户功能概述：
 * 1.用户注册-->user_register_menu注册界面-->user_register函数-->
 *   (调用getinput函数获取输入并返回包含名字和密码的结构体指针pu，
 *   然后调用insert_tail进行链表尾部插入),调用结束后返回pu指针
 *   -->user_login_check函数直接登录并返回当前用户的结点指针;
 * 2.用户登录-->user_menu登录界面-->user_login_check函数登录,
 *   返回当前用户在彩民链表中的结点指针pCurr,以便下面操作;
 * 3.利用pCurr直接查看个人信息、修改名字、密码;
 */
 
#include "user_operate.h"

char *fgets_check(char *str, int size)
{
	char c;
	int len = strlen(str);
	if (len < size-1)
		str[len-1] = '\0';
	else
	{
		while ((c=getchar())!='\n' && c!=EOF);
		printf("\t\t\tEnter too much,get %d charaters as valid input.\n",size-1);
	}
	return str;
}

INPUT *getinput(INPUT *pu)				//登录或者注册时输入的信息
{
	printf("\n\t\t\t\t\tEnter your name：");
	fgets(pu->name, NAME_LEN, stdin);
	fgets_check(pu->name, NAME_LEN);
	if (!strcmp(pu->name, "quit"))
	{
		strcpy(pu->name, "");
		strcpy(pu->passwd, "");
		return NULL;
	}
	char c;
	char *buffer = getpass("\t\t\t\t\tEnter your passwd：");
	if (strlen(buffer) > PASS_LEN-1)
	{
		while ((c=getchar())!='\n' && c!=EOF);
	}
	strcpy(pu->passwd, buffer);
	return pu;
}

INPUT *user_register(pUser puHead, INPUT *pu)
{
	pu = getinput(pu);
	if (pu == NULL)
		return NULL;
	if ( !strcmp(pu->name, "admin") )
	{
		strcpy(pu->name, "");
		strcpy(pu->passwd, "");
		return NULL;
	}	
	pUser pchk = puHead;;
	while (pchk->next != NULL)
	{
		if (!strcmp(pchk->next->name, pu->name))
		{
			printf("\t\t\t\t\tThe user name is exist.\n");
			usleep(800000);
			return NULL;
		}
		pchk = pchk->next;
	}
	insert_user_tail(puHead, pu->name, pu->passwd, START_FUNDS);
	return pu;
}

pUser user_login_chk(pUser puHead, INPUT *pu)
{
	if ( is_ul_empty(puHead) )
	{
		printf("\t\t\tUser list is empty.\n");
		return NULL;
	}
	char flag = 0;
	pUser pchk = puHead->next;
	
	while (pchk!=NULL && !flag)				//遍历链表检查用户是否存在，存在返回当前指针				
	{
		if (!strcmp(pu->name, pchk->name) && !strcmp(pu->passwd, pchk->passwd))
		{
			flag = 1;
			break;
		}
		pchk = pchk->next;
	}

	if ( flag )
		return pchk;
	else
		return NULL;	
}

void user_view_info(pUser pCurr)
{
	print_user_info(pCurr);
}

void modify_user_name(pUser pCurr)
{
	char newname[NAME_LEN];
	printf("\n\t\t\tInput you new name: ");
	fgets(newname, NAME_LEN, stdin);
	fgets_check(newname, NAME_LEN);	
	strcpy(pCurr->name, newname);
}

void modify_user_passwd(pUser pCurr)
{
	char passwd[PASS_LEN];
	printf("\n\t\t\tInput you new passwd: ");
	fgets(passwd, PASS_LEN, stdin);
	fgets_check(passwd, PASS_LEN);
	strcpy(pCurr->passwd, passwd);
}

/*购买彩票：1.获得期号，输入彩票信息;2.金额减少，余额判断;3.增加购买历史信息*/

pLott get_book_input(pLott pbl)			//pointer book lottery
{
	printf("\n\t\t\tEnter number(1-20):\t");
	do
	{
		scanf("%d",&pbl->lott_num);
		if (pbl->lott_num>20 || pbl->lott_num<1)
		{
			printf("\t\t\tReinput number(1-20):\t");
		}
	}while (pbl->lott_num>20 || pbl->lott_num<1);
	printf("\t\t\tEnter multiple:\t\t");
	scanf("%d",&pbl->multiple);
	pbl->book_amount = pbl->multiple * BASE1;
	return pbl;
}

bool book_lott(pLott plHead, pUser pCurr)
{
	LOTT bl;
	pLott pbl = &bl;
	get_book_input(pbl);
	if ( pbl->book_amount > pCurr->balance )
	{
		return false;
	}
	pCurr->balance -= pbl->book_amount;
	pbl->issue = plHead->issue;
	pbl->sweep_flag = false;
	pbl->win_flag = false;
	insert_lott_tail(plHead, pbl->issue, pCurr->uid, pCurr->name, 
	pbl->lott_num, pbl->multiple, pbl->sweep_flag, pbl->win_flag);
	return true;
}

pLott getlP_byuid(pLott pStart, int uid)		//查找包含特定UID的结点
{
	pLott pS = pStart;
	while (pS != NULL)
	{
		if (pS->uid == uid)
		{
			return pS;
		}
		pS = pS->next;
	}
	return NULL;
}

void user_view_trade(pLott plHead, int uid)		//输出特定UID购买的彩票历史信息
{
	if ( is_ll_empty(plHead) )
	{
		printf("\t\t\tThere is no trade.\n");
		return ;
	}
	pLott pStart = plHead->next;
	while (pStart != NULL)
	{
		pStart = getlP_byuid(pStart, uid);
		if (pStart != NULL)
		{
			printf(" \t\t\t%d\t  %d\t %d\t",pStart->issue, 
											pStart->lott_num, 
											pStart->multiple);
			if (pStart->multiple == 1)
				printf(" ");
			printf("%dyuan\t",pStart->multiple * BASE1);
			if (!pStart->sweep_flag)
			{
				printf("     NO\t\t   NO");
			}
			else if (pStart->sweep_flag && pStart->win_flag)
			{
				printf("     YES\t   \033[;31mYES	\033[0m");	//颜色控制
			}
			else
			{
				printf("     YES\t   NO");
			}
			printf("\n");

			pStart = pStart->next;
		}
	}
}
