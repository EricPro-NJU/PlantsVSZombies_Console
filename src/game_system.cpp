#include "..\include\game_system.h"
#include <conio.h>
#include "windows.h"
string getSysTime(){
	SYSTEMTIME m_time;	
	GetLocalTime(&m_time);	
	char szDateTime[100] = { 0 };	
	sprintf(szDateTime, "%02d-%02d-%02d %02d:%02d:%02d", m_time.wYear, m_time.wMonth,		
		m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond);	
	string time(szDateTime);	
	return time;
	//credit: thank CSDN blogger WFLin0103 for help!
	//https://blog.csdn.net/weixin_39490421/article/details/80662311
}
Damage::Damage(Plant_role* p, Zombie_role*z, int i){
	this->p = p;
	this->z = z;
	attack_type = 1;
	index = i;
}

Damage::Damage(Zombie_role* z, Plant_role* p, int i){
	this->p = p;
	this->z = z;
	attack_type = 2;
	index = i;
}

Damage::~Damage(){
	p = NULL;
	z = NULL;
}

void Damage::dealt(){
	if(attack_type == 1){
		if(index==-1) z->modify_health(0);
		else z->inc_health(-index);
		if(z->get_health()<0) z->modify_health(0);
	}
	else{
		if(index==-1) p->modify_health(0);
		else p->inc_health(-index);
		if(p->get_health()<0) p->modify_health(0);
	}
}


Game::Game(string s):roles("sysfile\\plants.txt","sysfile\\zombies.txt"){
	for(int i=0;i<5;i++){
		for(int j=0;j<8;j++){
			land[i][j].row = i;
			land[i][j].column = j;
			land[i][j].strike = false;
			land[i][j].plants = NULL;
			land[i][j].zombies.clear();
		}
	}
	point = 0;
	wave = 0;
	sun = 0;
	chosen_row = 0;
	chosen_column = 0;
	chosen_plant = 0;
	if(s=="auto") game_running = true;
	else game_running = false;
	extra_point = false;
	log.clear();
	shop_log.clear();
	sun_log.clear();
	wave_log.clear();
}

bool Game::zombies_alive(){
	bool res = false;
	for(int i=0;i<5;i++){
		for(int j=0;j<8;j++){
			if(!land[i][j].zombies.empty()){
				res = true;
				break;
			}
		}
	}
	return res;
}

void Game::reset(){
	for(int i=0;i<5;i++){
		for(int j=0;j<8;j++){
			land[i][j].row = i;
			land[i][j].column = j;
			land[i][j].strike = false;
			land[i][j].plants = NULL;
			land[i][j].zombies.clear();
		}
	}
	point = 0;
	wave = 0;
	sun = 0;
	chosen_row = 0;
	chosen_column = 0;
	chosen_plant = 0;
	game_running = false;
	extra_point = false;
	log.clear();
	shop_log.clear();
	sun_log.clear();
	wave_log.clear();
}

void Game::keyboard_operation(){
	char ch;
	while(game_running){
		ch = getch();
		int plantnum = roles.get_plantnum();
		switch(ch){
			case 'w': chosen_row = ((chosen_row-1<0)? 0: chosen_row-1); break;
			case 's': chosen_row = ((chosen_row+1>4)? 4: chosen_row+1); break;
			case 'a': chosen_column = ((chosen_column-1<0)? 0: chosen_column-1); break;
			case 'd': chosen_column = ((chosen_column+1>7)? 7: chosen_column+1); break;
			case 'i': chosen_plant = ((chosen_plant-4<0)? chosen_plant: chosen_plant-4); break;
			case 'k': chosen_plant = ((chosen_plant+4>plantnum-1)? chosen_plant: chosen_plant+4); break;
			case 'j': chosen_plant = ((chosen_plant-1<0)? 0: chosen_plant-1); break;
			case 'l': chosen_plant = ((chosen_plant+1>plantnum-1)? plantnum-1: chosen_plant+1); break;
			case 32: //plant
				{if(land[chosen_row][chosen_column].plants!=NULL || land[chosen_row][chosen_column].strike){
					shop_log = "此地块无法种植！";
					break;
				}
				Plant_ppt* pt = roles.get_plant(chosen_plant);
				if(pt->get_charging()!=0){
					shop_log = "冷却时间未到！无法种植！";
					break;
				}
				if(pt->get_spend()>sun){
					shop_log = "阳光不足！无法种植！";
					break;
				}
				if(pt->get_name()=="地刺"){
					land[chosen_row][chosen_column].strike = true;
					pt->recharge();
					sun -= pt->get_spend();
					break;
				} 
				else{
					Plant_role* pr = new Plant_role(pt,chosen_row,chosen_column);
					set_plant(pr);
					pt->recharge();
					sun -= pt->get_spend();
					break;
				}
				} 
				
			case 'c': 
				{if(land[chosen_row][chosen_column].plants==NULL && !land[chosen_row][chosen_column].strike){
					shop_log = "按下'C'键可以铲除对应格中植物"; 
					break;
				}
				else if(land[chosen_row][chosen_column].strike){
					land[chosen_row][chosen_column].strike = false;
					log = "地刺 被铲除了";
					extra_point = false; 
					break;
				}
				else{
					log = land[chosen_row][chosen_column].plants->get_property()->get_name();
					log += " 被铲除了";
					remove_plant(land[chosen_row][chosen_column].plants);
					extra_point = false;
					break;
				}} 
		}
	}
}

bool Game::game_over(){
	if(!game_running) return false;
	log = "僵尸吃掉了你的脑子！游戏结束！";
	game_running = false;
	return true;
}

bool Game::set_plant(Plant_role *p){
	int row = p->get_row();
	int column = p->get_column();
	if(land[row][column].plants!=NULL) return false;
	land[row][column].plants = p;
	return true;
}

void Game::set_zombie(Zombie_role* z){
	int row = z->get_row();
	land[row][7].zombies.push_back(z);
}

void Game::damage(Plant_role* p, Zombie_role* z, int index){
	Damage d(p,z,index);
	d.dealt();
	bool skill_activated = true;
	if(z->get_health()<=0 && !z->is_killed()){
		skill_activated = false;
		
		log = p->get_property()->get_name();
		log += " 击败了 ";
		log +=z->get_property()->get_name(); 
		int d_point = roles.point_rules(z->get_property()->get_name());
		log += " +";
		log += to_string(d_point);
		log += "p";
		point += d_point;
		z->be_killed();
	}
	vector <string> ps = p->get_attacking_skill();
	int size1 = ps.size();
	for(int i=0;i<size1;i++){
		do_skill(ps[i],p,z);
	}
	if(skill_activated){
		vector <string> zs = z->get_attacked_skill();
		int size2 = zs.size();
		for(int j=0;j<size2;j++){
			do_skill(zs[j],z,p);
		}
	}
	
	
}

void Game::damage(Zombie_role* z, Plant_role* p, int index){
	Damage d(z,p,index);
	d.dealt();
	bool skill_activated = true;
	if(p->get_health()<=0 && !z->is_killed()){
		skill_activated = false;
		
		log = z->get_property()->get_name();
		log += " 击败了 ";
		log +=p->get_property()->get_name(); 
		p->be_killed();
		extra_point = false;
	}
	vector <string> zs = z->get_attacking_skill();
	int size1 = zs.size();
	for(int i=0;i<size1;i++){
		do_skill(zs[i],z,p);
	}
	if(skill_activated){
		vector <string> ps = p->get_attacked_skill();
		int size2 = ps.size();
		for(int j=0;j<size2;j++){
			do_skill(ps[j],p,z);
		}
	}
	
	
}

bool Game::remove_zombie(Zombie_role* z){
	int row = z->get_row();
	int column = z->get_column();
	Field* f = &land[row][column];
	int size = f->zombies.size();
	for(int i=0;i<size;i++){
		if(z == f->zombies[i]){
			f->zombies.erase(f->zombies.begin()+i);
			delete z;
			return true;
		}
	}
	return false;
}

bool Game::remove_plant(Plant_role* p){
	int row = p->get_row();
	int column = p->get_column();
	Field* f = &land[row][column];
	if(f->plants!=p) return false;
	else {
		f->plants = NULL;
		delete p;
		return true;
	} 
}

void Game::draw(){
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  当前模式：无尽（白天）"<<endl;
	draw_shop();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  ########################################################################################"<<endl;
	for(int i=0;i<5;i++){   //output line1: if there are more than one zombie, print the number of the zombies
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<"  #";
		for(int j=0;j<8;j++){
			if(i==chosen_row&&j==chosen_column){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY  | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"@";
			}
			else cout<<" ";
			int znum = land[i][j].zombies.size();
			string out1;
			out1.clear();
			if(znum>=2){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				out1 += "僵尸(x";
				string znum_s = to_string(znum);
				out1 += znum_s;
				out1 +=")";
				
			}
			int al = (8-out1.size())/2+out1.size();
			cout<<setw(al)<<out1;
			for(int i=0;i<8-al;i++){
				cout<<" ";
			}
			if(i==chosen_row&&j==chosen_column){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"@";
			}
			
			else cout<<" ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"#";	 
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<endl;
		cout<<"  #";
		for(int j=0;j<8;j++){  //output line2 : print the name of the first zombie & bullet 
			string out2;
			out2.clear();
			if(!land[i][j].blts.empty()) {
				int drawn = 0;
				int bsz = land[i][j].blts.size();
				//cout<<"size: "<<bsz;
				for(int bi=0;bi<bsz;bi++){
					if(drawn < 5){
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN );
						drawn ++;
						switch(land[i][j].blts[bi].type){
							
							case 1: out2 += "○"; break;
							case 2: out2 += "○○" ; break;
							case 3: out2 += "※" ; break;
							case 4: out2 += "□"; break;
						}
					}
					
				}
			}
			if(!land[i][j].zombies.empty()){
				Zombie_role* z = land[i][j].zombies[0];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				out2 += z->get_property()->get_name();
				
			}
			int al = (10-out2.size())/2+out2.size();
			cout<<setw(al)<<out2;
			for(int i=0;i<10-al;i++){
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"#";
		}
		cout<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<"  #";
		for(int j=0;j<8;j++){   //output line3: if there is zombie, print the property of the zombie, else print the plant name
			string out3;
			out3.clear();
			if(!land[i][j].zombies.empty()){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED );
				Zombie_role* z = land[i][j].zombies[0];
				int hp = z->get_health();
				int maxhp = z->get_property()->get_hp();
				int per = hp*100/maxhp;
				if(z->get_property()->get_name()=="读报僵尸" ){
					if(z->get_health()>200){
						out3 += "(";
						out3 += "报";
						int np = (z->get_health()-200)*100/150;
						out3 += to_string(np);
						out3 += "%)";
					}
					else{
						out3 += "(";
						out3 += "怒";
						int np = hp*100/200;
						out3 += to_string(np);
						out3 += "%)";
					}
				} 
				else if(z->get_property()->get_name()=="撑杆僵尸"){
					if(z->locate_skill("bar_jumping")!=-1){
						out3 += "(杆"; 
					}
					else{
						out3 += "(";
					}
					out3 += to_string(per);
					out3 += "%)";
				}
				else{
					out3 += "(";
					out3 += to_string(per);
					out3 += "%)";
				}
				
				if(z->get_status().type == S_SLOW){
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE );
					out3 += "慢"; 
				}
				if(z->get_status().type == S_FROZEN){
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN );
					out3 += "定";
				}
			}			
			else if(land[i][j].plants!=NULL){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				Plant_role* p = land[i][j].plants;
				out3 += p->get_property()->get_name();
			}
			int al = (10-out3.size())/2+out3.size();
			cout<<setw(al)<<out3;
			for(int i=0;i<10-al;i++){
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"#";
		}
		cout<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<"  #";
		for(int j=0;j<8;j++){   //output line4: if zombie, print plant name, else print plant status.
			string out4;
			out4.clear();
			if(land[i][j].plants!=NULL){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				Plant_role* p = land[i][j].plants;
				if(!land[i][j].zombies.empty()){
					
					out4 += p->get_property()->get_name();
				}
				else{
					
					int hp = p->get_health();
					int maxhp = p->get_property()->get_hp();
					int per = hp*100/maxhp;
					if(p->get_property()->get_name()=="窝瓜" && hp<maxhp){
						out4 += "  压扁！"; 
					} 
					
					else if(p->get_property()->get_name()=="土豆地雷" ){
						if(p->is_killed() && p->locate_skill("pump_exploding")!=-1){
							out4 += "  渣渣！";
						}
						else{
							if(p->locate_skill("pump_exploding")==-1){
								out4 += "(装填中)";
							}
							else{
								out4 += "(装填完毕)";
							}
						}
						
					}
					
					else if(p->get_property()->get_name()=="火爆辣椒"||p->get_property()->get_name()=="樱桃炸弹"){
						out4 += "BOOM!!"; 
					} 
					else{
						out4 += "(";
						out4 += to_string(per);
						out4 += "%)";
					}
					
				}
			}
			int al = (10-out4.size())/2+out4.size();
			cout<<setw(al)<<out4;
			for(int i=0;i<10-al;i++){
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"#";	
		}
		cout<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<"  #";
		for(int j=0;j<8;j++){  //output line 5:if strike then print. if zombie, print plant status.
			if(i==chosen_row&&j==chosen_column){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"@";
			}
			else cout<<" ";
			string out5;
			out5.clear();
			if(land[i][j].strike){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
				out5 += "<地刺>";
			}
			if(!land[i][j].zombies.empty() && land[i][j].plants!=NULL){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				Plant_role* p = land[i][j].plants;
				int hp = p->get_health();
				int maxhp = p->get_property()->get_hp();
				int per = hp*100/maxhp;		
				out5 += "(";
				out5 += to_string(per);
				out5 += "%)";
				
			}
			int al = (8-out5.size())/2+out5.size();
			cout<<setw(al)<<out5;
			for(int i=0;i<8-al;i++){
				cout<<" ";
			}
			
			if(i==chosen_row&&j==chosen_column){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"@";
			}
			else cout<<" ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"#";	
		}
		cout<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout<<"  ########################################################################################"<<endl;
	} 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  当前得分：";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout<<setw(25)<<point;
	if(wave_log == "僵尸即将开始进攻，请做好准备..." || wave_log == "一大波僵尸即将来袭..."){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |  FOREGROUND_RED );
	}
	else{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED );
	} 
	cout<<wave_log<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout<<"  "<<log<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  ########################################################################################"<<endl;
	
}

void Game::draw_shop(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  ****************************************************************************************"<<endl;
	cout<<"    阳光总数："<<setw(6)<<sun;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout<<setw(25)<<sun_log;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"-----植物商店-----"<<endl;
	cout<<"  ****************************************************************************************"<<endl;
	int n = roles.get_plantnum();
	for(int i=0;i<n;i++){
		Plant_ppt* p = roles.get_plant(i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		if(i%4==0) cout<<"  *";
		if(chosen_plant==i){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY  | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<">[";
		}
		else cout<<"  ";
		string outplant;
		outplant.clear();
		outplant += p->get_name();
		if(p->get_charging()==0){
			outplant+=" $ ";
			outplant+=to_string(p->get_spend());
			if(p->get_spend()>sun){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED );
			}
			else{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			}
		}
		else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED );
			int time = p->get_charging();
			int pcd = p->get_pcd();
			int res = (pcd-time)*100/pcd;
			outplant+="(";
			outplant+=to_string(res);
			outplant+="%)"; 
		}
		cout<<setw(16)<<left<<outplant;
		if(chosen_plant==i){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"]<";
		}
		else cout<<"  ";
		if((i+1)%4==0||i==n-1) cout<<endl;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  ****************************************************************************************"<<endl;
	cout<<"  当前选择地块：第"<<(chosen_row+1)<<"行，第"<<(chosen_column+1)<<"列。"<<"           ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED );
	cout<<shop_log<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout<<"  当前选择植物："<<roles.get_plant(chosen_plant)->get_name()<<"           "; 
	if(wave_log == "僵尸即将开始进攻，请做好准备..." || wave_log == "一大波僵尸即将来袭..."){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |  FOREGROUND_RED );
	}
	else{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED );
	} 
	cout<<wave_log<<endl;
}

void Game::reset_bullet(){
	for(int i=0;i<5;i++){
		for(int j=0;j<8;j++){
			if(!land[i][j].blts.empty()){
				int bsz = land[i][j].blts.size();
				for(int bi = 0;bi<bsz;bi++){
					land[i][j].blts[bi].moved = false;
				}
			}		
		}
	}
}

void Game::frame_forward(){
	reset_bullet();
	//pcd
	int n = roles.get_plantnum();
	for(int i=0;i<n;i++){
		Plant_ppt* p = roles.get_plant(i);
		if(p->get_charging()!=0) p->discharge();
	}
	//for every field 
	for(int i=0;i<5;i++){
		for(int j=0;j<8;j++){
			Plant_role* pp = land[i][j].plants;
			//kills 
			if(pp!=NULL && pp->is_killed()){
				remove_plant(pp);
			}
			int zsz = land[i][j].zombies.size();
			for(int k=0;k<zsz;k++){
				Zombie_role* zz= land[i][j].zombies[k];
				if(zz==NULL) {
					zsz = land[i][j].zombies.size();
					continue;
				}
				else{
					if(zz->is_killed()){
						remove_zombie(zz);
						zsz = land[i][j].zombies.size();   //bug fixed
						k--;
					}
				}
			}
			//skills and effects
			pp = land[i][j].plants;
			if(pp!=NULL){
				vector <string> effect = land[i][j].plants->effect_count();
				int esz = effect.size();
				//cout<<"the size is "<<esz<<endl;
				for(int k=0;k<esz;k++){
					do_effect(effect[k], land[i][j].plants);
				}
			}
			
			zsz = land[i][j].zombies.size();
			for(int k=0;k<zsz;k++){
				Zombie_role* zz= land[i][j].zombies[k];
				if(zz==NULL) {
					zsz = land[i][j].zombies.size();
					continue;
				}
				vector<string> zeffect = zz->effect_count();
				int zesz = zeffect.size();
				for(int kk=0;kk<zesz;kk++){
					do_effect(zeffect[k], zz);
				}
			}
			pp = land[i][j].plants;
			zsz = land[i][j].zombies.size();
			//dici effects
			if(land[i][j].strike){
				Plant_role* ppp = new Plant_role(roles.get_plant("地刺"),0,0);
				for(int k=0;k<zsz;k++){
					Zombie_role* zz= land[i][j].zombies[k];
					damage(ppp,zz,10);
				}
				delete ppp;
			}
			
			
			pp = land[i][j].plants;
			zsz = land[i][j].zombies.size();
			//bite plants
			if(pp!=NULL){
				for(int k=0;k<zsz;k++){
					if(land[i][j].zombies.size()==0) break; 
					Zombie_role* zz= land[i][j].zombies[k];
					if(zz==NULL) {
						zsz = land[i][j].zombies.size();
						continue;
					}
					if(zz->get_status().type == S_FROZEN){
						//do nothing
					}
					else if(zz->get_status().type == S_SLOW){
						damage(zz,pp,zz->get_attack()/2);
					}
					else{
						damage(zz,pp,zz->get_attack());
					}
					
				}
			}
			//move forward
			else{
				for(int k=0;k<zsz;k++){
					Zombie_role* zz= land[i][j].zombies[k];
					if(zz==NULL) {
						zsz = land[i][j].zombies.size();
						continue;
					}
					bool suc = zz->move_forward();
					if(zz->get_column()<0){
						game_over();
						return;
					}
					if(zz->get_row()!=i||zz->get_column()!=j){
						land[zz->get_row()][zz->get_column()].zombies.push_back(zz);
						land[i][j].zombies.erase(land[i][j].zombies.begin()+k);
					//	cout<<"zombies has been transferred from"<<i<<", "<<j<<" to"<<zz->get_row()<<", "<<zz->get_column()<<endl;
						k--;
						zsz--;
					}
				}
			}
			pp = land[i][j].plants;
			zsz = land[i][j].zombies.size();
			//bullets activated
			if(pp!=NULL && pp->get_property()->get_bullet().activated){
				bool yes_zombie = false;
				for(int k = j;k<8;k++){
					if(!land[i][k].zombies.empty()){
						yes_zombie = true;
						break;
					}
				}
				if(yes_zombie){
					bool brelease = pp->do_bullet();
					if(brelease){
						BAttack ba;
						ba.type = pp->get_property()->get_bullet().bullet_type;
						ba.index = pp->get_property()->get_bullet().attack;
						ba.attacker = pp->get_property();
						ba.moved = true;
						land[i][j].blts.push_back(ba);
					//	cout<<"bullet release at ("<<i<<","<<j<<")"<<endl; 
					}
					
				}
			}
			pp = land[i][j].plants;
			zsz = land[i][j].zombies.size();
			//bullets attack
			if(!land[i][j].blts.empty()){
				int bsize = land[i][j].blts.size();
				if(zsz!=0){
					Zombie_role* zz0 = land[i][j].zombies[0];
					for(int bi = 0;bi<bsize;bi++){
						Plant_ppt* att_ppt = land[i][j].blts[bi].attacker;
						Plant_role* attacker = new Plant_role(att_ppt,0,0);
						damage(attacker,zz0,land[i][j].blts[bi].index);
						delete attacker;
						if(land[i][j].zombies.size()==0) break;
						//cout<<"bullet attack at ("<<i<<","<<j<<")"<<endl; 
					}
					land[i][j].blts.clear();
				}
				//bullets move forward
				else{
					if(j==7) land[i][j].blts.clear();
					else{
						int bbsize = bsize;
						for(int bi = 0;bi<bbsize;bi++){
							if(!land[i][j].blts[bi].moved){
								land[i][j].blts[bi].moved = true;
								land[i][j+1].blts.push_back(land[i][j].blts[bi]);
								land[i][j].blts.erase(land[i][j].blts.begin()+bi);
								bbsize --;
								bi --;
							}	
						}
					}
					
					
				}
			}
		}
	}
}

void Game::loops(){
	int i=0;
	while(is_running()){
		if(i%2==0){
			draw();
			
		}
		Sleep(200) ;
		frame_forward();
		i++;
	}
}

void Game::suninc(){
	while(is_running()){
		for(int i=1;i<=25;i++){
			if(is_running()) Sleep(250);
			else break;
		}
		sun += 25;
		sun_log = "收集天上阳光+25";
	}
}

void Game::wave_attack(){
	Zombie_ppt* zp1 = roles.get_zombie("僵尸");
	Zombie_ppt* zp2 = roles.get_zombie("路障僵尸");
	Zombie_ppt* zp3 = roles.get_zombie("铁桶僵尸");
	Zombie_ppt* zp4 = roles.get_zombie("橄榄僵尸");
	Zombie_ppt* zp5 = roles.get_zombie("摇旗僵尸");
	Zombie_ppt* zp6 = roles.get_zombie("读报僵尸");
	Zombie_ppt* zp7 = roles.get_zombie("撑杆僵尸");
	while(is_running()){
		if(wave == 0 ) {
			wave_log = "僵尸即将开始进攻，请做好准备...";
			Sleep(PREPARE_TIME);
			extra_point = true;
		}
		else{
			wave_log = "一大波僵尸即将来袭...";
			Sleep(6000); 
			extra_point = true;
		}
		
		wave ++;
		wave_log = "当前进攻波次：第";
		wave_log += to_string(wave); 
		wave_log += "波"; 
		//set zombies  round 1
		if(wave == 1){  //wave 1: only one zombie 
			int rand_num = rand()%5;
			Zombie_role* zr1 = new Zombie_role(zp1,rand_num,7);
			set_zombie(zr1);
		}
		else{
			int rand_num_head = rand()%5;
			Zombie_role* zh = new Zombie_role(zp5,rand_num_head,7);
			set_zombie(zh);
			for(int i=0;i<((4+wave > 8)? 8: (4+wave));i++){
				int rand_num1 = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num1,7);
				set_zombie(zr1);
			} 
			for(int i=0;i<1+wave;i++){
				int rand_num2 = rand()%5;				
				Zombie_role* zr2 = new Zombie_role(zp2,rand_num2,7);
				set_zombie(zr2);
			}
			if(wave>=3){
				for(int i=0;i<wave;i++){
					int rand_num3 = rand()%5;				
					Zombie_role* zr3 = new Zombie_role(zp3,rand_num3,7);
					set_zombie(zr3);
				}
				for(int i=0;i<wave-1;i++){
					int rand_num6 = rand()%5;
					Zombie_role* zr6 = new Zombie_role(zp6,rand_num6,7);
					set_zombie(zr6);
				}
			}
			if(wave>=4){
				for(int i=0;i<wave-2;i++){
					int rand_num4 = rand()%5;				
					Zombie_role* zr4 = new Zombie_role(zp4,rand_num4,7);
					set_zombie(zr4);
				}
				for(int i=0;i<wave-1;i++){
					int rand_num7 = rand()%5;
					Zombie_role* zr7 = new Zombie_role(zp7,rand_num7,7);
					set_zombie(zr7);
				}
			}
		}
		Sleep(5000);
		//wait to the next round
		for(int i=0;i<40;i++){
			if(is_running()&&zombies_alive()){
				Sleep(1000); 
			}
			else{
				break; //ready for next round
			}
		}
		if(!is_running()) break;
		//set zombies for round 2
		if(wave == 1){
			for(int i=0;i<2;i++){
				int rand_num = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num,7);
				set_zombie(zr1);
			}
		}
		else{
			for(int i=0;i<((4+wave > 8)? 8: (4+wave));i++){
				int rand_num1 = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num1,7);
				set_zombie(zr1);
			} 
			for(int i=0;i<wave;i++){
				int rand_num2 = rand()%5;				
				Zombie_role* zr2 = new Zombie_role(zp2,rand_num2,7);
				set_zombie(zr2);
			}
			if(wave>=3){
				for(int i=0;i<wave-1;i++){
					int rand_num3 = rand()%5;				
					Zombie_role* zr3 = new Zombie_role(zp3,rand_num3,7);
					set_zombie(zr3);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num6 = rand()%5;
					Zombie_role* zr6 = new Zombie_role(zp6,rand_num6,7);
					set_zombie(zr6);
				}
			}
			if(wave>=4){
				for(int i=0;i<wave-2;i++){
					int rand_num7 = rand()%5;
					Zombie_role* zr7 = new Zombie_role(zp7,rand_num7,7);
					set_zombie(zr7);
				}
			}
			
		}
		Sleep(5000);
		//wait to the next round
		for(int i=0;i<45;i++){
			if(is_running()&&zombies_alive()){
				Sleep(1000); 
			}
			else{
				break; //ready for next round
			}
		}
		if(!is_running()) break;
		//set zombies for round 3
		if(wave == 1){
			for(int i=0;i<2;i++){
				int rand_num = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num,7);
				set_zombie(zr1);
			}
			int rand_num2 = rand()%5;
			Zombie_role* zr2 = new Zombie_role(zp2,rand_num2,7);
			set_zombie(zr2);
		}
		else{
			for(int i=0;i<((4+wave > 8)? 8: (4+wave));i++){
				int rand_num1 = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num1,7);
				set_zombie(zr1);
			} 
			for(int i=0;i<wave;i++){
				int rand_num2 = rand()%5;				
				Zombie_role* zr2 = new Zombie_role(zp2,rand_num2,7);
				set_zombie(zr2);
			}
			if(wave>=3){
				for(int i=0;i<wave-1;i++){
					int rand_num3 = rand()%5;				
					Zombie_role* zr3 = new Zombie_role(zp3,rand_num3,7);
					set_zombie(zr3);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num6 = rand()%5;
					Zombie_role* zr6 = new Zombie_role(zp6,rand_num6,7);
					set_zombie(zr6);
				}
			}
			if(wave>=4){
				for(int i=0;i<((wave-2)/2);i++){
					int rand_num4 = rand()%5;				
					Zombie_role* zr4 = new Zombie_role(zp4,rand_num4,7);
					set_zombie(zr4);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num7 = rand()%5;
					Zombie_role* zr7 = new Zombie_role(zp7,rand_num7,7);
					set_zombie(zr7);
				}
			}
		}
		Sleep(5000);
		 //wait to the next round
		for(int i=0;i<45;i++){
			if(is_running()&&zombies_alive()){
				Sleep(1000); 
			}
			else{
				break; //ready for next round
			}
		}
		if(!is_running()) break;
		//set zombies for round 4
		if(wave == 1){
			for(int i=0;i<3;i++){
				int rand_num = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num,7);
				set_zombie(zr1);
			}
			for(int i=0;i<2;i++){
				int rand_num2 = rand()%5;
				Zombie_role* zr2 = new Zombie_role(zp2,rand_num2,7);
				set_zombie(zr2);
			}
			
		}
		else{
			for(int i=0;i<((4+wave > 8)? 8: (4+wave));i++){
				int rand_num1 = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num1,7);
				set_zombie(zr1);
			} 
			if(wave>=2){
				for(int i=0;i<wave;i++){
					int rand_num3 = rand()%5;				
					Zombie_role* zr3 = new Zombie_role(zp3,rand_num3,7);
					set_zombie(zr3);
				}
			}
			if(wave>=3){
				for(int i=0;i<((wave-1)/2);i++){
					int rand_num4 = rand()%5;				
					Zombie_role* zr4 = new Zombie_role(zp4,rand_num4,7);
					set_zombie(zr4);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num6 = rand()%5;
					Zombie_role* zr6 = new Zombie_role(zp6,rand_num6,7);
					set_zombie(zr6);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num7 = rand()%5;
					Zombie_role* zr7 = new Zombie_role(zp7,rand_num7,7);
					set_zombie(zr7);
				}
			}
		}
		Sleep(5000);
		  //wait to the next round
		for(int i=0;i<45;i++){
			if(is_running()&&zombies_alive()){
				Sleep(1000); 
			}
			else{
				break; //ready for next round
			}
		}
		if(!is_running()) break;
		//set zombies for round 5
		if(wave == 1){
			for(int i=0;i<3;i++){
				int rand_num = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num,7);
				set_zombie(zr1);
			}
			for(int i=0;i<2;i++){
				int rand_num2 = rand()%5;
				Zombie_role* zr2 = new Zombie_role(zp2,rand_num2,7);
				set_zombie(zr2);
			}
			
		}
		else{
			for(int i=0;i<((4+wave > 8)? 8: (4+wave));i++){
				int rand_num1 = rand()%5;
				Zombie_role* zr1 = new Zombie_role(zp1,rand_num1,7);
				set_zombie(zr1);
			} 
			if(wave>=2){
				for(int i=0;i<wave;i++){
					int rand_num3 = rand()%5;				
					Zombie_role* zr3 = new Zombie_role(zp3,rand_num3,7);
					set_zombie(zr3);
				}
			}
			if(wave>=3){
				for(int i=0;i<((wave-1)/2);i++){
					int rand_num4 = rand()%5;				
					Zombie_role* zr4 = new Zombie_role(zp4,rand_num4,7);
					set_zombie(zr4);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num6 = rand()%5;
					Zombie_role* zr6 = new Zombie_role(zp6,rand_num6,7);
					set_zombie(zr6);
				}
				for(int i=0;i<wave-2;i++){
					int rand_num7 = rand()%5;
					Zombie_role* zr7 = new Zombie_role(zp7,rand_num7,7);
					set_zombie(zr7);
				}
			}
		}
		Sleep(5000);
		//wait to the next wave
		while(true){
			if(!is_running()) break;
			if(!zombies_alive()){
				if(!extra_point){
					log = "第";
					log += to_string(wave);
					log += "波通过，得分+";
					log += to_string(400+((wave/2)*50));
					point += 400+((wave/2)*50);
					break;
				}
				else{
					log = "在没有植物损耗的情况下通过第";
					log += to_string(wave);
					log += "波，得分+";
					log += to_string(400+((wave/2)*50)+wave*50);
					point += 400+((wave/2)*50)+wave*50;
					break;
				}
				
			}
		}
	}
}

void Game::run(){
	system("cls");
	reset();
	game_running = true;
	sun = INIT_SUN;
	cout<<endl<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout<<"                 =============================="<<endl;
	cout<<"                 ||                          ||"<<endl;
	cout<<"                 ||       准        备       ||"<<endl;
	cout<<"                 ||           ready          ||"<<endl;
	cout<<"                 ||                          ||"<<endl;
	cout<<"                 =============================="<<endl;
	Sleep(1000);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout<<"                 =============================="<<endl;
	cout<<"                 ||                          ||"<<endl;
	cout<<"                 ||       安        放       ||"<<endl;
	cout<<"                 ||            set           ||"<<endl;
	cout<<"                 ||                          ||"<<endl;
	cout<<"                 =============================="<<endl;
	Sleep(1000);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout<<"                 =============================="<<endl;
	cout<<"                 ||                          ||"<<endl;
	cout<<"                 ||       植         物      ||"<<endl;
	cout<<"                 ||           plants         ||"<<endl;
	cout<<"                 ||                          ||"<<endl;
	cout<<"                 =============================="<<endl;
	Sleep(2500);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	thread t1(&Game::loops, this);
	thread t2(&Game::keyboard_operation, this);
	thread t3(&Game::suninc, this);
	thread t4(&Game::wave_attack, this);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	if(!game_running){
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout<<endl<<endl;
		cout<<"                 =============================="<<endl;
		cout<<"                 ||                          ||"<<endl;
		cout<<"                 ||       僵 尸 吃 掉 了     ||"<<endl;
		cout<<"                 ||        你 的 脑 子!      ||"<<endl;
		cout<<"                 ||                          ||"<<endl;
		cout<<"                 =============================="<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout<<"         您的最终得分："<<point<<endl;
		cout<<"         最大坚持波数："<<wave<<endl; 
		system("pause");
	}
}


