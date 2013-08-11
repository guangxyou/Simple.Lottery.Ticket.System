/*彩民用户链表，包含uid,name,passwd,balance.
 *内部函数：
 *		pUser creat_void_list(void);
 *		bool is_ul_empty(pUser puHead);
 *		void print_user_info(pUser p);
 *		void travers_user_list(pUser puHead);
 *		void insert_user_tail(pUser puHead, char *name, char *passwd, int balance);
 *		void clean_user_file(char *path);
 *		void save_user_info(pUser puHead);
 *		void load_user_info(pUser puHead);
 *函数功能：
 *		1.创建一个空链表，头指针中uid值代表uid起始值1000，
 *		  以后创建新结点uid等于此uid值加上结点个数(遍历链表时得到)；
 *		2.判断链表是否为空；
 *		3.打印一个结点的所有信息；
 *		4.遍历所有结点并输出所有用户信息；
 *		5.向尾部插入新结点，必须提供name和passwd；
 *		6.清空一个文件，必须提供文件路径；
 *		7.将彩民链表里的个人信息保存到文件；
 *		8.将文件中的彩民个人信息重新加载到彩民链表里；
 */

#include "user_list.h"

pUser creat_void_user_list(void)	//创建一个只有头指针的空链表
{
	pUser pHead = (pUser) malloc (sizeof(USER));
	pHead->uid = START_UID;				//头指针的uid表示uid起始值1000
	pHead->next = NULL;
	return pHead;
}

bool is_ul_empty(pUser puHead)
{
	if (puHead->next == NULL)
		return true;
	else
		return false;
}

void print_user_info(pUser p)
{
	printf("\t\t\t%d\t\t%s",p->uid, p->name);
	if (strlen(p->name) < 8)
		printf("\t\t");
	else
		printf("\t");
	printf("%s\t\t%.2f\n",p->passwd, p->balance);
}

void travers_user_list(pUser puHead)			//遍历链表
{
	if ( is_ul_empty(puHead) )
	{
		printf("\t\t\tUser list is empty.\n");
		return ;
	}

	pUser p = puHead->next;
	while (p != NULL)
	{
		print_user_info(p);
		p = p->next;
	}
}

void insert_user_tail(pUser puHead, char *name, char *passwd, int balance)	//链表尾部插入
{
	int cnt = 1;		//结点个数，尾部插入时加上START_UID作为新结点的uid
	pUser pNew = NULL;
	pNew = (pUser) malloc (sizeof(USER));
	
	if ( is_ul_empty(puHead) )
	{
		puHead->next = pNew;
	}
	else
	{
		pUser pTail = puHead->next;
		while (pTail->next != NULL)
		{
			pTail = pTail->next;
			cnt++;
		}
		cnt++;
		pTail->next = pNew;
	}

	pNew->uid = START_UID + cnt;
	strcpy(pNew->name, name);
	strcpy(pNew->passwd, passwd);
	pNew->balance = balance;
	pNew->next = NULL;
}

void clean_file(char *path)			//清空文件，测试用
{
	FILE *fp = fopen(path, "wb");
	if (fp == NULL)
	{
		printf("\t\t\tOpen %s failed.\n",path);
		exit(1);
	}
	fwrite("", 0, 0, fp);
	if (fclose(fp) != 0)
	{
		printf("\t\t\tClose %s failed.\n",path);
		exit(1);
	}
	printf("\t\t\tClean %s succeed.\n",path);
}

void save_user_info(pUser puHead)
{
	if ( is_ul_empty(puHead) )
	{
		return ;
	}
	
	FILE *fpUser = fopen(PATH_USER, "wb");
	if (fpUser == NULL)
	{
		printf("\t\t\tOpen %s failed.\n",PATH_USER);
		exit(1);
	}
	
	pUser pWt = puHead->next;
	while (pWt != NULL)
	{
		fwrite(pWt, sizeof(USER), 1, fpUser);
		pWt = pWt->next;
	}

	if (fclose(fpUser) != 0)
	{
		printf("\t\t\tClose %s failed.\n",PATH_USER);
		exit(1);
	}
}

void load_user_info(pUser puHead)
{
	FILE *fpUser = fopen(PATH_USER, "rb");
	if (fpUser == NULL)
	{
		printf("\t\t\tOpen %s failed.\n",PATH_USER);
		exit(1);
	}

	pUser ptmp = NULL;
	ptmp = (pUser) malloc (sizeof(USER));
	int cnt = fread(ptmp, sizeof(USER), 1, fpUser);	//先读取一次测试文件是否为空
	if (cnt == 0)
	{
		fclose(fpUser);
		free(ptmp);
		return ;
	}
	insert_user_tail(puHead, ptmp->name, ptmp->passwd, ptmp->balance);

	while (1)
	{
		fread(ptmp, sizeof(USER), 1, fpUser);
		if ( feof(fpUser) )
			break;
		insert_user_tail(puHead, ptmp->name, ptmp->passwd, ptmp->balance);
	}

	if (fclose(fpUser) != 0)
	{
		printf("\t\t\tClose %s failed.\n",PATH_USER);
		exit(1);
	}
	free(ptmp);
	ptmp = NULL;
}

void free_user_list(pUser puHead)
{
	if ( is_ul_empty(puHead) )
	{
		free(puHead);
		return ;
	}

	pUser ptmp = NULL;
	pUser pFree = puHead->next;
	while (pFree != NULL)
	{
		ptmp = pFree->next;
		free(pFree);
		pFree = ptmp;
	}
	pFree = NULL;
	ptmp = NULL;
}
