#include "..\include\game_system.h"
#include "windows.h"
#include <conio.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void enter_main(Game& g){
	char ch;
	int choose = 1;
	do{
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout<<"  当前版本：测试版"<<endl;
		cout<<endl<<endl<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout<<"                    植     物     ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout<<"大     战     ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout<<"僵     尸     "<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout<<"                ==============================================="<<endl;
		cout<<"                * ";
		if(choose==1) cout<<"-------->";
		else cout<<"         ";
		cout<<"1.   开    始    游    戏";
		if(choose==1) cout<<"<-------- *";
		else cout<<"          *";
		cout<<endl<<"                * ";
		if(choose==2) cout<<"-------->";
		else cout<<"         ";
		cout<<"2.   图    鉴    一    览";
		if(choose==2) cout<<"<-------- *";
		else cout<<"          *";
		cout<<endl<<"                * ";
		if(choose==3) cout<<"-------->";
		else cout<<"         ";
		cout<<"3.   退    出    游    戏";
		if(choose==3) cout<<"<-------- *";
		else cout<<"          *";
		cout<<endl<<"                ==============================================="<<endl;
		cout<<"                按下W,S上下选择，按下enter进入。"<<endl;
		ch = getch();
		if(ch=='w') choose = ((choose-1<0)? 0: choose-1);
		if(ch=='s') choose = ((choose+1>3)? 3: choose+1);
		if(ch==13){
			switch(choose){
				case 1:
				{
					system("cls");
					cout<<endl<<endl;
					cout<<"                                  游戏操作提示："<<endl;
					cout<<"            =========================================================="<<endl;
					cout<<"            *游戏的胜利目标是：安放植物，保护草坪，坚持尽可能多的时间！"<<endl;
					cout<<"            *按下W,A,S,D选定草坪，按下I,J,K,L选择商店植物。"<<endl;
					cout<<"            *按下【空格】安放植物，按下C铲除植物，阳光将被自动获取。"<<endl;
					cout<<"            *请再次确认您的输入法已经关闭！准备好就可以开始了！"<<endl;
					cout<<"            *";
					system("pause");
					g.run(); 
					break;
				}
				case 2: g.enter_display(); break;
				case 3: return;
			}
		}
	}while(true);
	
}
int main(int argc, char** argv) {
	srand((unsigned)time(NULL));  
	Game g("auto");
	enter_main(g);
	return 0;
}

