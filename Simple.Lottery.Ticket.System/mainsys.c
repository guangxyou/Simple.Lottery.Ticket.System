#include "mainsys.h"

pUser puHead = NULL;		//彩民链表头指针
pLott plHead = NULL;		//彩票链表头指针

GLO glo;				//全局变量,包含彩票期号和奖池金额
GLO *pglo = &glo;		//pglo point to global struct

static int xxx = 0;		//输入选项验证,定义为全局变量否则每个函数都需定义一个

int main(int argc, char *argv[])
{	
	puHead = creat_void_user_list();
	plHead = creat_void_lott_list();
	read_glo_file(pglo);				//加载全局变量
	plHead->issue = pglo->g_issue;		//每次加载期号读入到彩票链表头指针中
	//sys_reset();	//重置所有信息	
	load_user_info(puHead);	
	load_user_trade(plHead);
	
	main_menu();
	
	save_user_info(puHead);
	free_user_list(puHead);
	puHead = NULL;
	save_user_trade(plHead);
	free_lott_list(plHead);
	plHead = NULL;

	return 0;
}

void main_menu()
{
	char login_flag = 0;		//注册完成后直接登录标志位
	int	num = 0;
	INPUT user;
	INPUT *pu = &user;
	INPUT *ptmp = &user;		//输入quit后再注册时pu=NULL,pu必须指向原来分配的地址,否则会段错误
	pUser pCurr = NULL;

	do
	{
		if (!login_flag)		//注册完成后直接登录跳过此流程
		{
			usleep(500000);		//延时0.5秒
			sys_main_menu();
			while ((xxx=operate_choice(&num)) == false);	//强制输入数字
		}
		switch(num)
		{
			case 1:
				admin_func(pu);
				break;
			case 2:
				user_func(pu, pCurr, login_flag);
				login_flag = 0;
				break;
			case 3:
				usleep(500000);
				user_register_menu();
				pu = user_register(puHead, pu);
				if (pu == NULL)
				{
					pu = ptmp;
					break;
				}
				else
				{
					login_flag = 1;
					num = 2;
					break;
				}
			case 0:
				break;
			case -1:		//输入无效字符,将num置为-1,重新输入,否则num会根据上次输入的正确选项进入相应菜单
				break;
			default:
				printf("\n\t\t\t\t\tWrong choice.");
				press_any_key();
				break;
		}
	}while (num);
}

void sys_reset(void)
{
	pglo->g_issue = START_ISSUE;
	pglo->g_jackpot = 10000;

	write_glo_file(pglo);		//重置全局变量,issue=1301,jackpot=10000
	write_admfile("admin");		//重置管理员账户,name=admin,p=admin
	//四个初始化账户
	clean_file(PATH_USER);		//清除以前账户信息
	insert_user_tail(puHead, "zhao", "123456", 100);
	insert_user_tail(puHead, "qian", "123456", 100);
	insert_user_tail(puHead, "sun", "123456", 100);
	insert_user_tail(puHead, "li", "123456", 100);
	clean_file(PATH_LOTT);		//清除以前彩票信息
	clean_file(PATH_WIN);		//清除以前开奖信息
}

void admin_func(INPUT *pu)
{
	getinput(pu);
	if ( admin_login_chk(pu) )
	{
		printf("\t\t\t\t\tLogin Success.\n");
	}
	else
	{
		printf("\t\t\t\t\tLogin failed, check your passwd.\n");
		return ;
	}
	
	pUser pMod = NULL;
	int a_num = 0;		//管理员主界面选择项
	int v_num = 0;		//管理员查看信息子界面选择项
	int uid = 0;
	int modify = 0;
	int win_num = 0;
	int charge_money = 0;
	int cnt = 0;		//检测输入有效数字个数
	char ch  = '\0';	//接受模糊搜索输入的字符
	bool search_flag;
	int magic_num = 0;		//要搜索的期号或者UID
	char magic_str[NAME_LEN];
	char c;		//确认删除，接收一个字符
	
	do
	{
		usleep(500000);
		admin_menu();
		while ((xxx=operate_choice(&a_num)) == false);
		switch(a_num)
		{
			case 1:
				system("clear");
				divise_line();
				before_print_info();
				admin_view_info(puHead);
				divise_line();
				printf("\n\t\t\tEnter the UID to charge：");
				scanf("%d",&uid);
				getchar();
				printf("\t\t\tEnter recharge amount：");
				scanf("%d",&charge_money);
				getchar();
				pMod = getuP_byuid(puHead, uid, pMod);
				if (pMod == NULL)
				{
					printf("\t\t\tNot find uid %d.\n",uid);
					press_any_key();
					break;
				}
				charge_user(pMod, charge_money);
				printf("\t\t\tCharge user %d  %dyuan.\n",uid, charge_money);
				divise_line();
				before_print_info();
				user_view_info(pMod);
				divise_line();
				press_any_key();
				break;
			case 2:
				printf("\n\t\t\tGenerate Lottery winning number.......\n");
				sleep(1);
				win_num = gene_ram_num();
				printf("\t\t\tThe %d lottery issue winning number is %d.\n"
						,pglo->g_issue, win_num);
				cash_award(win_num, pglo, puHead, plHead);	//兑奖
				press_any_key();
				break;
			case 3:
				do
				{
					usleep(500000);
					admin_view_menu();
					while ((xxx=operate_choice(&v_num)) == false);
					switch(v_num)
					{
						case 1:
							system("clear");
							divise_line();
							before_print_info();
							admin_view_info(puHead);
							divise_line();
							press_any_key();
							break;
						case 2:
							system("clear");
							divise_line();
							printf("\n\t\t\tNext issue is %d.\n",pglo->g_issue);
							printf("\t\t\tThe jackpot amount is %.2f yuan.\n", pglo->g_jackpot);
							divise_line();
							before_print_trade(0);		//管理员查看交易信息模式
							admin_view_trade(plHead);
							divise_line();
							press_any_key();
							break;
						case 3:
							system("clear");
							divise_line();
							read_winner_file();		//读取中奖者文件并输出
							divise_line();
							press_any_key();
							break;
						case 4:
							system("clear");
							divise_line();
							printf("\t\t\tEnter Something,");
							printf("System will print all related infomation.");
							divise_line();
							printf("\n");
							printf("\t\t\tEnter something：");
							cnt = scanf("%d",&magic_num);
							search_flag = true;		//要搜索的是数字
							if (cnt == 0)
							{
								while (cnt<NAME_LEN && (ch=getchar())!='\n' && ch!=EOF)
								{
									magic_str[cnt] = ch;
									cnt++;
								}
								magic_str[cnt] = '\0';
								if (cnt >= NAME_LEN)		//清空缓存
								{
									while ((ch=getchar())!='\n' && ch!=EOF);
								}
								search_flag = false;		//要搜索的是字符串
							}
							else
							{
								getchar();	//scanf();
							}
							searching();
							fuzzy_lookup(magic_num, magic_str, search_flag, puHead, plHead);//模糊查找
							press_any_key();
							break;
						case 5:
							system("clear");
							divise_line();
							printf("\t\t\tSort by user's balance");
							divise_line();
							before_print_info();
							sort_balance(puHead);
							divise_line();
							press_any_key();
							break;
						case 0:
							break;
						case -1:
							break;
						default:
							printf("\n\t\t\t\t\tWrong choice.\n");
							break;
					}
				}while (v_num);
				break;
			case 4:
				system("clear");
				divise_line();
				before_print_info();
				admin_view_info(puHead);
				divise_line();
				printf("\n\t\t\tEnter the UID to modify: ");
				scanf("%d",&uid);
				getchar();
				if (uid == START_UID)
				{
					mod_admin_pw();
				}
				else if (uid == 8888)	//删除用户,隐藏功能
				{
					printf("\t\t\t\033[;31m\033[1mNOTICE: Delete user operation may affect ");
					printf("causes uncontrolled for subsequent operations.\n\033[0m");
					
					printf("\n\t\t\tAre you sure to delete(Y/n): ");
					c = getchar();
					if (c != 'Y')
						break;
					printf("\n\t\t\tEnter UID to delete: ");
					scanf("%d",&uid);
					getchar();
					admin_del_user(puHead, uid);
					printf("\n\t\t\tDelete succeed.\n");
				}
				else
				{
					mod_choice();
					do
					{
						scanf("%d",&modify);
						getchar();
						switch (modify)
						{
							case 1:
								pMod = admin_mod_uname(puHead, uid, pMod);
								break;
							case 2:
								pMod = admin_mod_upass(puHead, uid, pMod);
								break;
							case 0:
								break;
							default:
								printf("\n\t\t\t\t\tWrong choice, reinput: ");
								break;
						}
						if (pMod == NULL)
						{
							printf("\t\t\tNot find uid %d.\n",uid);
							press_any_key();
							break;
						}
						else if ( pMod!=NULL && modify )
						{
							divise_line();
							before_print_info();
							user_view_info(pMod);
							divise_line();
							modify = 0;
							press_any_key();
							break;
						}
					}while (modify);
				}
				break;
			case 0:
				break;
			case -1:
				break;
			default:
				printf("\\n\t\t\t\t\tWrong choice, reinput: ");
				break;
		}			
	}while (a_num);
}

void user_func(INPUT *pu, pUser pCurr, char flag)
{
	if (!flag)
	{
		getinput(pu);
	}

	pCurr = user_login_chk(puHead, pu);
	if (pCurr != NULL)
	{
		printf("\t\t\t\t\tLogin success.\n");
	}	
	else
	{
		printf("\t\t\t\t\tLogin failed, check your name and password.\n");
		usleep(500000);
		return ;
	}	
	int u_num = 0;
	int modify = 0;
	char c;
	do
	{
		usleep(500000);
		user_menu();
		while ((xxx=operate_choice(&u_num)) == false);
		switch(u_num)
		{
			case 1:
				usleep(300000);
				buy_lott_menu();
				printf("\n\t\t\t\t\033[;31m\033[1m");
				printf("Welcom to buy %dth lottery isssue.\033[0m\n",plHead->issue);
				do
				{
					if ( book_lott(plHead, pCurr) )	//购买彩票
					{
						printf("\t\t\t\033[1mBook lottery succeed.\033[0m\n");
						getchar();
						printf("\t\t\tBook again?(y/n)：");
						c = getchar();						
					}
					else
					{
						printf("\t\t\tYour balance is not enough, please recharge.\n");
						press_any_key();
					}
				}while (c=='y' || c=='Y' || c=='\n');
				break;
			case 2:
				system("clear");
				printf("\n\n\n\n\n");
				divise_line();
				before_print_info();
				user_view_info(pCurr);
				divise_line();
				before_print_trade(1);	//用户打印模式
				user_view_trade(plHead, pCurr->uid);
				divise_line();
				press_any_key();
				break;
			case 3:	
				system("clear");
				divise_line();
				before_print_info();
				user_view_info(pCurr);
				divise_line();
				mod_choice();			
				do
				{
					scanf("%d",&modify);
					getchar();
					if (modify == 1)
					{
						modify_user_name(pCurr);
						divise_line();
						before_print_info();
						user_view_info(pCurr);
						divise_line();
						modify = 0;
						press_any_key();
					}
					else if (modify == 2)
					{
						modify_user_passwd(pCurr);
						divise_line();
						before_print_info();
						user_view_info(pCurr);
						divise_line();
						modify = 0;
						press_any_key();
					}
					else if (modify != 0)
					{
						printf("\n\t\t\t\t\tWrong choice, reinput: ");
					}
				}while (modify);
				break;
				
			case 0:
				break;
			case -1:
				break;
			default:
				printf("\n\t\t\t\t\tWrong choice, reinput: ");
				break;
		}
	}while (u_num);
}

void divise_line(void)
{
	printf("\n\t\t\t********************************************************\n");
}

void before_print_info(void)
{
	printf("\n\t\t\tUID\t\tName\t\tPasswd\t\tBalance\n\n");
}

void before_print_trade(int print_flag)	//0 管理员打印模式 1用户打印模式
{
	if ( print_flag )
		printf("\n\t\t\tIssue\tNumber\tMulti\tAmount\t   Drawing\tWinning\n\n");
	else
		printf("\n\t\t\tIssue  UID    Name  Number  Multi  Amount  Draw  Winning\n\n");
}

void press_any_key(void)
{
	printf("\n\t\t\t\t\tPress any key to return.");
	getchar();
}

bool operate_choice(int *num)
{
	int cnt = 0;
	char c;
	printf("\n\t\t\t\t\tEnter your select：");
	cnt = scanf("%d",num);
	getchar();
	if ( !cnt )
	{
		while ((c=getchar())!='\n' && c!=EOF);
		*num = -1;
		return false;
	}
	return true;
}

void mod_choice(void)
{
	printf("\n\t\t\t1.Name\t\t2.Passwd\t0.return\n");
	printf("\n\t\t\tYour choice: ");
}

void hide_cursor(void)		//隐藏光标
{
	printf("\033[?25l");
}

void show_cursor(void)		//显示光标
{
	printf("\033[?25h");
}

void searching(void)		//动态显示searching字符
{
	int cnt = 0;
	printf("\n");
	printf("\t\t\t");
	printf("\033[0m");		//消除所有属性
	printf("\033[?25l");	//隐藏光标
	while (cnt < 5)
	{	
		printf("\033[;31m\033[1mSearching\n\033[0m");
		usleep(500000);
		printf("\033[1A\033[24C\033[K");	//光标返回上行,向右移动三个tab键的宽度,并清空到行尾的文字
		printf("\033[;31m\033[1mSearching···\n\033[0m");
		usleep(500000);
		printf("\033[1A\033[24C\033[K");
		printf("\033[;31m\033[1mSearching······\n\033[0m");
		usleep(500000);
		printf("\033[1A\033[24C\033[K");
		printf("\033[;31m\033[1mSearching·········\n\033[0m");
		usleep(500000);
		printf("\033[1A\033[24C\033[K");
		
		cnt++;
	}
	printf("\033[0m");
	printf("\033[?25h");	//显示光标
	printf("\n");
}
