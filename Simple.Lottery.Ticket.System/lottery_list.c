/*彩票链表 包括 期号,uid,name,彩票号码,注数,购买彩票金额,是否开奖,是否中奖
 *内部函数：
 *		pLott creat_void_lott_list(void);
 *		bool is_ll_empty(pLott plHead);
 *		void print_user_trade(pLott p);
 *		void travers_lott_list(pLott plHead);
 *		void insert_lott_tail(pLott plHead, int issue, int uid, 
        char *name, int lott_num, int multiple, bool sweep_flag, bool win_flag);
 *		void clean_user_file(char *path);
 *		void save_user_trade(pLott plHead);
 *		void load_user_trade(pLott plHead);
 *函数功能：
 *		1.创建一个彩票空链表;
 *		2.判断链表是否为空;
 *		3.打印一个用户的购买彩票信息；
 *		4.遍历所有结点并输出所有用户信息；
 *		5.向尾部插入新结点;
 *		6.将彩票信息保存到文件；
 *		7.将文件中的彩票信息重新加载到彩票链表里；
 */

#include "lottery_list.h"	

pLott creat_void_lott_list(void)			//创建一个只有头指针的空链表
{	
	pLott pHead = (pLott) malloc (sizeof(LOTT));
	pHead->next = NULL;
	return pHead;
}

bool is_ll_empty(pLott plHead)
{
	if (plHead->next == NULL)
		return true;
	else
		return false;
}

void print_user_trade(pLott p)
{
	printf("\t\t\t%d",p->issue);
	printf("  %d    %s",p->uid, p->name);
	int i = 8 - strlen(p->name);
	while (i--)
		printf(" ");		
	printf("%d       %d  ",p->lott_num, p->multiple);
	if (p->multiple == 1)
		printf(" ");
	printf("  %dyuan",p->multiple * BASE1);
	if (!p->sweep_flag)
	{
		printf("    NO\t   NO");
	}
	else if (p->sweep_flag && p->win_flag)
	{
		printf("   YES\t   \033[;31mYES\033[0m");	//颜色控制
	}
	else
	{
		printf("   YES\t   NO");
	}
	printf("\n");
}

void travers_lott_list(pLott plHead)
{
	if ( is_ll_empty(plHead) )
	{
		printf("\t\t\tThe lottery list is empty.\n");
		return ;
	}

	pLott p = plHead->next;
	while (p != NULL)
	{
		print_user_trade(p);
		p = p->next;
	}
}

void insert_lott_tail(pLott plHead, int issue, int uid,
	 char *name, int lott_num, int multiple, bool sweep_flag, bool win_flag)		
{
	pLott pNew = NULL;
	pNew = (pLott) malloc (sizeof(LOTT));

	if ( is_ll_empty(plHead) )
	{
		plHead->next = pNew;
	}
	else
	{
		pLott pTail = plHead->next;
		while (pTail->next != NULL)
		{
			pTail = pTail->next;
		}
		pTail->next = pNew;
	}

	pNew->issue = issue;
	pNew->uid = uid;
	strcpy(pNew->name, name);
	pNew->lott_num = lott_num;
	pNew->multiple = multiple;
	pNew->book_amount = multiple * BASE1;
	pNew->sweep_flag = sweep_flag;
	pNew->win_flag = win_flag;
	pNew->next = NULL;
}

void save_user_trade(pLott plHead)
{
	if ( is_ll_empty(plHead) )
	{
		return ;
	}
	
	FILE *fpTr = fopen(PATH_LOTT, "wb");
	if (fpTr == NULL)
	{
		printf("\t\t\tOpen trade.dat failed.\n");
		exit(1);
	}

	pLott pWt = plHead->next;
	while (pWt != NULL)
	{
		fwrite(pWt, sizeof(LOTT), 1, fpTr);
		pWt = pWt->next;
	}

	if (fclose(fpTr) != 0)
	{
		printf("\t\t\tClose trade_info failed.\n");
		exit(1);
	}
}

void load_user_trade(pLott plHead)
{
	FILE *fpTr = fopen(PATH_LOTT, "rb");
	if (fpTr == NULL)
	{
		printf("\t\t\tOpen trade.dat failed.\n");
		exit(1);
	}

	pLott p = NULL;
	p = (pLott) malloc (sizeof(LOTT));
	int cnt = fread(p, sizeof(LOTT), 1, fpTr);	//先读取一次测试文件是否为空
	if (cnt == 0)
	{
		fclose(fpTr);
		return ;
	}
	insert_lott_tail(plHead, p->issue, p->uid, p->name, 
	p->lott_num, p->multiple, p->sweep_flag, p->win_flag);

	while (1)
	{
		fread(p, sizeof(LOTT), 1, fpTr);
		if ( feof(fpTr) )
			break;
		insert_lott_tail(plHead, p->issue, p->uid, p->name, 
		p->lott_num, p->multiple, p->sweep_flag, p->win_flag);
	}

	if (fclose(fpTr) != 0)
	{
		printf("\t\t\tClose trade_info failed.\n");
		exit(1);
	}
	free(p);
	p = NULL;
}

void free_lott_list(pLott plHead)
{
	if ( is_ll_empty(plHead) )
	{
		free(plHead);
		return ;
	}

	pLott ptmp = NULL;
	pLott pFree = plHead->next;
	while (pFree != NULL)
	{
		ptmp = pFree->next;
		free(pFree);
		pFree = ptmp;
	}
	pFree = NULL;
	ptmp = NULL;
}
