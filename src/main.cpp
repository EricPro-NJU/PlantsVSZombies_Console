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
		cout<<"  ��ǰ�汾�����԰�"<<endl;
		cout<<endl<<endl<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout<<"                    ֲ     ��     ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout<<"��     ս     ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout<<"��     ʬ     "<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout<<"                ==============================================="<<endl;
		cout<<"                * ";
		if(choose==1) cout<<"-------->";
		else cout<<"         ";
		cout<<"1.   ��    ʼ    ��    Ϸ";
		if(choose==1) cout<<"<-------- *";
		else cout<<"          *";
		cout<<endl<<"                * ";
		if(choose==2) cout<<"-------->";
		else cout<<"         ";
		cout<<"2.   ͼ    ��    һ    ��";
		if(choose==2) cout<<"<-------- *";
		else cout<<"          *";
		cout<<endl<<"                * ";
		if(choose==3) cout<<"-------->";
		else cout<<"         ";
		cout<<"3.   ��    ��    ��    Ϸ";
		if(choose==3) cout<<"<-------- *";
		else cout<<"          *";
		cout<<endl<<"                ==============================================="<<endl;
		cout<<"                ����W,S����ѡ�񣬰���enter���롣"<<endl;
		ch = getch();
		if(ch=='w') choose = ((choose-1<0)? 0: choose-1);
		if(ch=='s') choose = ((choose+1>3)? 3: choose+1);
		if(ch==13){
			switch(choose){
				case 1:
				{
					system("cls");
					cout<<endl<<endl;
					cout<<"                                  ��Ϸ������ʾ��"<<endl;
					cout<<"            =========================================================="<<endl;
					cout<<"            *��Ϸ��ʤ��Ŀ���ǣ�����ֲ�������ƺ����־����ܶ��ʱ�䣡"<<endl;
					cout<<"            *����W,A,S,Dѡ����ƺ������I,J,K,Lѡ���̵�ֲ�"<<endl;
					cout<<"            *���¡��ո񡿰���ֲ�����C����ֲ����⽫���Զ���ȡ��"<<endl;
					cout<<"            *���ٴ�ȷ���������뷨�Ѿ��رգ�׼���þͿ��Կ�ʼ�ˣ�"<<endl;
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

