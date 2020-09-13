#include "..\include\game_system.h"
#include <conio.h>
#include "windows.h"
//in property_define.h
PEffect _make_effect(string name, int role){
	PEffect ef;
	ef.effect_name = name;
	ef.role = role;
	if(name.compare("sun_generating")==0){
		int rand_num = rand()%29+12;  //12-40
		ef.remaining_time = rand_num;
		ef.time = 96;
	}
	if(name.compare("potato_activating")==0){
		ef.time = ef.remaining_time = 60;
	}
	if(name.compare("cherry_exploding")==0){
		ef.time = ef.remaining_time = 2;
	}
	if(name.compare("chilly_exploding")==0){
		ef.time = ef.remaining_time = 2;
	}
	//TODO: add more effects here.
	return ef;
}

TSkill _make_skill(string name, int role){
	TSkill sk;
	sk.role = role;
	sk.skill_name = name;
	if(name.compare("pump_exploding")==0){
		sk.trigger = TS_ATTACKED;
	}
	if(name.compare("water_split")==0){
		sk.trigger = TS_ATTACKING;
	}
	if(name.compare("slow_you_down")==0){
		sk.trigger = TS_ATTACKING;
	}
	if(name.compare("smell_attack")==0){
		sk.trigger = TS_ATTACKED;
	}
	if(name.compare("newspaper_crazy")==0){
		sk.trigger = TS_ATTACKED;
	}
	if(name.compare("bar_jumping")==0){
		sk.trigger = TS_ATTACKING;
	}
	//TODO: add more skills here.
	return sk;
}

//in role_gallery
string RoleGallery::get_skill_desc(string name){
	if(name == "sun_generating"){
		return "ÿ��һ��ʱ�䣬�����һ�����������⡣";
	}
	if(name == "slow_you_down"){
		return "�����㶹���Խ��ͽ�ʬ���ж��ٶȡ�";
	}
	if(name == "pump_exploding"){
		return "�ѹϿ���˲���ұⵥ���ڵ����н�ʬ��";
	}
	if(name == "potato_activating"){
		return "��Ҫһ����װ��ʱ����ܼ��";
	}
	if(name == "water_split"){
		return "��������ʱ����Χ�Ľ�ʬ���ܵ�һ�����Ľ����˺���";
	}
	if(name == "cherry_exploding"){
		return "��ֲ�����ʱ���ڱ�ը����ɱ�����Ź�������н�ʬ";
	}
	if(name == "chilly_exploding"){
		return "��ֲ�����ʱ���ڱ�ը����ɱ�����е����н�ʬ";
	}
	if(name == "smell_attack"){
		return "��ʬҧ��һ�ں󣬻ᱻ�ϵ����ڵ�����ȥ��";
	}
	if(name == "newspaper_crazy"){
		return "������ʬ�ı�ֽ���ƻ��󽫻ᱩ�ߡ�";
	}
	if(name == "bar_jumping"){
		return "�Ÿ˽�ʬ����Խ�������ĵ�һ��ֲ����˸߼����";
	}
	//TODO: add more descriptions of skills here.
	else{
		string s;
		s.clear();
		return s;
	}
}


//in game_system.h
void Game::do_effect(string effect_name, Plant_role* p){
	if(effect_name=="sun_generating"){
		sun_generating(p);
	}
	if(effect_name=="potato_activating"){
		potato_activating(p);
	}
	if(effect_name=="cherry_exploding"){
		cherry_exploding(p);
	}
	if(effect_name=="chilly_exploding"){
		chilly_exploding(p);
	}
	//TODO: add more effects here.
} 

void Game::do_effect(string effect_name, Zombie_role* z){
	//TODO: add more effects here.
}

void Game::do_skill(string skill_name, Plant_role* p, Zombie_role* z){
	if(skill_name == "pump_exploding"){
		pump_exploding(p,z);
	}
	if(skill_name == "water_split"){
		water_split(p,z);
	}
	if(skill_name == "slow_you_down"){
		slow_you_down(p,z);
	}
	if(skill_name == "smell_attack"){
		smell_attack(p,z);
	}
	//TODO: add more skills here.
}

void Game::do_skill(string skill_name, Zombie_role* z, Plant_role* p){
	if(skill_name == "newspaper_crazy"){
		newspaper_crazy(z,p);
	}
	if(skill_name == "bar_jumping"){
		bar_jumping(z,p);
	}
	//TODO: add more skills here.
}

//definition of skills and effects

void Game::sun_generating(Plant_role* p){
	this->sun += 25;
	sun_log = "���տ�(";
	sun_log += to_string(p->get_row()+1);
	sun_log += ",";
	sun_log += to_string(p->get_column()+1);
	sun_log += ")��������+25"; 
} 

void Game::pump_exploding(Plant_role* p,Zombie_role* z){
	int row = p->get_row();
	int column = p->get_column();
	int size = land[row][column].zombies.size();
	for(int i=0;i<size;i++){
		Zombie_role* zz = land[row][column].zombies[i];
		damage(p,zz,-1);
	}
	p->disable_skill("pump_exlopding");
	p->be_killed();

}

void Game::potato_activating(Plant_role* p){
	p->disable_effect("potato_activating");
	p->gain_skill("pump_exploding");
}

void Game::water_split(Plant_role* p, Zombie_role* z){
	int row = z->get_row();
	int column = z->get_column();
	int rmin = ((row==0)? 0: row-1);
	int rmax = ((row==4)? 4: row+1);
	int cmin = ((column==0)? 0: column-1);
	int cmax = ((column==7)? 7: column+1);
	for(int i=rmin;i<=rmax;i++){
		for(int j=cmin;j<=cmax;j++){
			if(i!=row&&j!=column) continue;
			int size = land[i][j].zombies.size();
			//cout<<"size == "<<size<<endl;
			for(int k=0;k<size;k++){
				Zombie_role* zz = land[i][j].zombies[k];
				//cout<<"����"<<i<<","<<j<<","<<k<<zz->get_property()->get_name()<<endl; 
				if(z==zz) continue;
				Plant_role* pp = new Plant_role(roles.get_plant("����Ͷ��"),0,0);
				pp->disable_skill("water_split");
				damage(pp,zz,26); 
				//cout<<"����"<<endl; 
				delete pp;
			}
		}
	}
}

void Game::cherry_exploding(Plant_role* p){
	int row = p->get_row();
	int column = p->get_column();
	int rmin = ((row==0)? 0: row-1);
	int rmax = ((row==4)? 4: row+1);
	int cmin = ((column==0)? 0: column-1);
	int cmax = ((column==7)? 7: column+1);
	for(int i=rmin;i<=rmax;i++){
		for(int j=cmin;j<=cmax;j++){
			int size = land[i][j].zombies.size();
			for(int k=0;k<size;k++){
				Zombie_role* zz = land[i][j].zombies[k];
				damage(p,zz,-1);
				
			}
		}
	}
	p->be_killed();
}

void Game::chilly_exploding(Plant_role* p){
	int row = p->get_row();
	for(int i=0;i<8;i++){
		int size = land[row][i].zombies.size();
		for(int k=0;k<size;k++){
			Zombie_role* zz = land[row][i].zombies[k];
			damage(p,zz,-1);
		}
	}
	p->be_killed();
}

void Game::slow_you_down(Plant_role* p,Zombie_role* z){
	//slow peashots are uneffective to zombies with newspapers.
	if(z->get_property()->get_name()=="������ʬ" && z->get_health()>200){
		return;
	} 
	if(z->get_status().type == S_FROZEN) return;
	if(z->get_status().type == S_SLOW){
		z->reset_status_time(40);
	}
	else{
		Status s;
		s.type = S_SLOW;
		s.time = 40;
		s.flag = true;
		z->set_status(s);
	}
}

void Game::smell_attack(Plant_role* p, Zombie_role* z){
	int row = z->get_row();
	int column = z->get_column();
	int randnum = rand()%2;
	if(row == 0){
		z->transfer(1,column);	
	}
	else if(row == 4){
		z->transfer(3,column);
	}
	else{
		if(randnum==1){
			z->transfer(row+1,column);
		}
		else{
			z->transfer(row-1,column);
		}
	}
	int size = land[row][column].zombies.size();
	for(int k=0;k<size;k++){
		Zombie_role* zz = land[row][column].zombies[k];
		if(z==zz){
			land[row][column].zombies.erase(land[row][column].zombies.begin()+k);
			set_zombie(z);
			break;
		}
	}
}

void Game::newspaper_crazy(Zombie_role* z, Plant_role* p){
	if(z->get_health()<=200){
		z->modify_speed(8);
		z->disable_skill("newspaper_crazy");
	}
}

void Game::bar_jumping(Zombie_role* z, Plant_role* p){
	int row = z->get_row();
	int column = z->get_column();
	z->modify_attack(25);
	z->modify_speed(20);
	if(p->get_property()->get_name()!="�߼��"){
		z->transfer(row,column-1);
		if(column-1<0){
			game_over();
			return;
		}
		int size = land[row][column].zombies.size();
		for(int k=0;k<size;k++){
			Zombie_role* zz = land[row][column].zombies[k];
			if(z==zz){
				land[row][column].zombies.erase(land[row][column].zombies.begin()+k);
				set_zombie(z);
				break;
			}
		}
	}
	z->disable_skill("bar_jumping");
}
