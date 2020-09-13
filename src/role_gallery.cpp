#include "..\include\game_system.h"
#include <conio.h>
#include "windows.h"
RoleGallery::RoleGallery(string pfile, string zfile){
	this->filename_plant = pfile;
	this->filename_zombie = zfile;
	plant_num = 0;
	zombie_num = 0;
	//input plants property:
	ifstream ifile1;
	ifile1.open(pfile.c_str());
	string name;
	int type;
	string bullet_activated;
	int speed;
	int attack;
	int gap;
	int btype;
	int pcd;
	int spend;
	int maxhp;
	string effect;
	string skill;
	string describe;
	while(ifile1>>name){
		plant_num++;
		ifile1>>type;
		ifile1>>bullet_activated;
		ifile1>>speed;
		ifile1>>attack;
		ifile1>>gap;
		ifile1>>btype;
		ifile1>>pcd;
		ifile1>>spend;
		ifile1>>maxhp;
		ifile1>>effect;
		ifile1>>skill;
		ifile1>>describe;
		Bullet b;
		b.activated = ((bullet_activated == "true")? true : false);
		b.speed = speed;
		b.attack = attack;
		b.gap = gap;
		b.bullet_type = btype;
		if(effect.compare("null")==0) effect.clear();
		if(skill.compare("null")==0) skill.clear();
		Plant_ppt _p(name,(PType)type,b,pcd,spend,maxhp,describe,effect,skill);
		plant_list.push_back(_p);
	}
	ifile1.close();
	
	//inputs zombie property
	ifstream ifile2;
	ifile2.open(zfile.c_str());
	while(ifile2>>name){
	//	cout<<"name: "<<name<<endl;
		zombie_num++;
		ifile2>>maxhp;
	//	cout<<"maxhp: "<<maxhp<<endl;
		ifile2>>attack;
	//	cout<<"attack: "<<attack<<endl;
		ifile2>>speed;
	//	cout<<"speed: "<<speed<<endl;
		ifile2>>effect;
	//	cout<<"effect: "<<effect<<endl; 
		ifile2>>skill;
	//	cout<<"skill: "<<skill<<endl;
		ifile2>>describe;
	//	cout<<"describe: "<<describe<<endl;
		if(effect.compare("null")==0) effect.clear();
		if(skill.compare("null")==0) skill.clear();
		Zombie_ppt _z(name,maxhp,attack,speed,describe,effect,skill);
		zombie_list.push_back(_z);
	}
	ifile2.close();
}

void RoleGallery::enter_display(){
	int index = 1;
	char ch;
	do{
		system("cls");
		cout<<endl<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout<<"                        疯 狂 戴 夫 的 图 鉴"<<endl;
		if(index == 1){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
			cout<<"                         *******************"<<endl;
			cout<<"                         *  ★★★★★★★ *"<<endl;
			cout<<"                         *     植    物    *"<<endl;
			cout<<"                         *      plants     *"<<endl;
			cout<<"                         *  ★★★★★★★ *"<<endl;
			cout<<"                         *******************"<<endl<<endl;
		}
		else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			cout<<"                         ==================="<<endl;
			cout<<"                         |                 |"<<endl;
			cout<<"                         |     植    物    |"<<endl;
			cout<<"                         |      plants     |"<<endl;
			cout<<"                         |                 |"<<endl;
			cout<<"                         ==================="<<endl<<endl;
		}
		
		 
		if(index == 2){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |  FOREGROUND_BLUE);
			cout<<"                         *******************"<<endl;
			cout<<"                         *  ★★★★★★★ *"<<endl;
			cout<<"                         *     僵    尸    *"<<endl;
			cout<<"                         *      zombies    *"<<endl;
			cout<<"                         *  ★★★★★★★ *"<<endl;
			cout<<"                         *******************"<<endl<<endl;
		} 
		else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			cout<<"                         ==================="<<endl;
			cout<<"                         |                 |"<<endl;
			cout<<"                         |     僵    尸    |"<<endl;
			cout<<"                         |      zombies    |"<<endl;
			cout<<"                         |                 |"<<endl;
			cout<<"                         ==================="<<endl<<endl;
		} 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout<<"           操作提示：按'W'和'S'移动光标，按下enter进入图鉴，按下ESC返回"<<endl; 
		ch = getch();
		if(ch == 's'){
			if(index == 1) index ++;
		} 
		if(ch == 'w'){
			if(index == 2) index --;
		}
		if(ch == 27){
			return;
		}
		if(ch == 13){
			if(index==1) display_plant();
			if(index==2) display_zombie();
		}
	}while(true);
	
	 
}

void RoleGallery::display_plant(){
	int index = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	char ch;
	do{
		system("cls");
		cout<<endl<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout<<"                    植    物   图   鉴"<<endl; 
		cout<<"    ============================================="<<endl;
		for(int i=0;i<plant_num;i++){
			if(i%4==0) cout<<"    *";
			if(index == i){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"[";
			}
			else{
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout<<setw(8)<<plant_list[i].get_name();
			if(index == i){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"]";
			}
			else{
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			if((i+1)%4==0 || i==plant_num-1) cout<<endl;
		}
		cout<<"    ============================================="<<endl;
		cout<<"   操作提示：'W''A''S''D'键上下移动，ESC退出"<<endl; 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		Plant_ppt* plant = (&plant_list[index]);
		cout<<"    * "<<plant->get_name()<<"    "<<plant->get_type()<<endl;;
		cout<<"    * "<<plant->get_describe()<<endl;
		if(!plant->get_effect_name().empty()){
			cout<<"    * 效果：";
			cout<<get_skill_desc(plant->get_effect_name())<<endl;
		}
		if(!plant->get_skill_name().empty()){
			cout<<"    * 技能：";
			cout<<get_skill_desc(plant->get_skill_name())<<endl;
		}
		cout<<"    * 阳光消费："<<plant->get_spend()<<"    冷却时间：";
		int pcd = plant->get_pcd();
		if(pcd<=30) cout<<"短";
		else if(pcd<=120) cout<<"长";
		else cout<<"非常长";
		cout<<endl<<endl;
		ch = getch();
		switch(ch){
			case 'w': if(index>=4) index-=4; break;
			case 's': if(index<plant_num-4) index+=4; break;
			case 'a': if(index>0) index--; break;
			case 'd': if(index<plant_num-1) index++; break;
			case 27: return;
		} 
	}while(true);
} 

void RoleGallery::display_zombie(){
	int index = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	char ch;
	do{
		system("cls");
		cout<<endl<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout<<"                    僵   尸   图   鉴"<<endl; 
		cout<<"    ============================================="<<endl;
		for(int i=0;i<zombie_num;i++){
			if(i%4==0) cout<<"    *";
			if(index == i){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"[";
			}
			else{
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout<<setw(8)<<zombie_list[i].get_name();
			if(index == i){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
				cout<<"]";
			}
			else{
				cout<<" ";
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			if((i+1)%4==0 || i==zombie_num-1) cout<<endl;
		}
		cout<<"    ============================================="<<endl;
		cout<<"   操作提示：'W''A''S''D'键上下移动，ESC退出"<<endl; 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		Zombie_ppt* zombie = (&zombie_list[index]);
		cout<<"    $ "<<zombie->get_name()<<endl;
		cout<<"    $ 生命值：";
		int hp = zombie->get_hp();
		if(hp<500) cout<<"低";
		else if(hp<1000) cout<<"中";
		else if(hp<1500) cout<<"高";
		else cout<<"很高";
		cout<<endl;
		if(!zombie->get_effect_name().empty()){
			cout<<"    $ 效果：";
			cout<<get_skill_desc(zombie->get_effect_name())<<endl;
		}
		if(!zombie->get_skill_name().empty()){
			cout<<"    $ 技能：";
			cout<<get_skill_desc(zombie->get_skill_name())<<endl;
		}
		cout<<"    $ "<<zombie->get_describe()<<endl<<endl;
		cout<<endl<<endl;
		ch = getch();
		switch(ch){
			case 'w': if(index>=4) index-=4; break;
			case 's': if(index<zombie_num-4) index+=4; break;
			case 'a': if(index>0) index--; break;
			case 'd': if(index<zombie_num-1) index++; break;
			case 27: return;
		} 
	}while(true);
}

void RoleGallery::display(){
	cout<<"zombie_nums: "<<zombie_num<<", plant_nums: "<<plant_num<<endl;
	for(int i=0;i<plant_num;i++){
		Plant_ppt* plant = (&plant_list[i]);
		cout<<"* "<<plant->get_name()<<"    "<<plant->get_type()<<endl;;
		cout<<"* "<<plant->get_describe()<<endl;
		if(!plant->get_effect_name().empty()){
			cout<<"* 效果：";
			cout<<get_skill_desc(plant->get_effect_name())<<endl;
		}
		if(!plant->get_skill_name().empty()){
			cout<<"* 技能：";
			cout<<get_skill_desc(plant->get_skill_name())<<endl;
		}
		cout<<"* 阳光消费："<<plant->get_spend()<<"    冷却时间：";
		int pcd = plant->get_pcd();
		if(pcd<=30) cout<<"短";
		else if(pcd<=120) cout<<"长";
		else cout<<"非常长";
		cout<<endl;
		cout<<endl; 	
	}
	cout<<endl;
	for(int i=0;i<zombie_num;i++){
		Zombie_ppt* zombie = (&zombie_list[i]);
		cout<<"$ "<<zombie->get_name()<<endl;
		cout<<"$ 生命值：";
		int hp = zombie->get_hp();
		if(hp<500) cout<<"低";
		else if(hp<1000) cout<<"中";
		else if(hp<1500) cout<<"高";
		else cout<<"很高";
		cout<<endl;
		if(!zombie->get_effect_name().empty()){
			cout<<"$ 效果：";
			cout<<get_skill_desc(zombie->get_effect_name())<<endl;
		}
		if(!zombie->get_skill_name().empty()){
			cout<<"$ 技能：";
			cout<<get_skill_desc(zombie->get_skill_name())<<endl;
		}
		cout<<"$ "<<zombie->get_describe()<<endl<<endl;
	}
}



Plant_ppt* RoleGallery::get_plant(string name){
	for(int i=0;i<plant_num;i++){
		if(plant_list[i].get_name()==name) return(&plant_list[i]);
	}
	return NULL;
}

Plant_ppt* RoleGallery::get_plant(int id){
	if(id>=plant_num) return NULL;
	return (&plant_list[id]);
}

Zombie_ppt* RoleGallery::get_zombie(string name){
	for(int i=0;i<zombie_num;i++){
		if(zombie_list[i].get_name()==name) return(&zombie_list[i]);
	}
	return NULL;
}

Zombie_ppt* RoleGallery::get_zombie(int id){
	if(id>=zombie_num) return NULL;
	return (&zombie_list[id]);
}

int RoleGallery::point_rules(string name){
	if(name == "僵尸") return 10;
	if(name == "路障僵尸") return 12;
	if(name == "铁桶僵尸") return 15;
	if(name == "橄榄僵尸") return 18;
	if(name == "摇旗僵尸") return 10; 
	if(name == "读报僵尸") return 12;
	if(name == "撑杆僵尸") return 15; 
}

