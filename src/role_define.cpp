#include "..\include\role_define.h"
Plant_role::Plant_role(Plant_ppt* ppt, int row, int column){
	this->property = ppt;
	health = ppt->get_hp();
	bullet_time = 1;
	this->row = row;
	this->column = column;
	effect.clear();
	skill.clear();
	if(ppt->has_effect()){
		effect.push_back(ppt->make_effect());
	}
	if(ppt->has_skill()){
		skill.push_back(ppt->make_skill());
	}
	die = false;
}

bool Plant_role::do_bullet(){
	bullet_time --;
	if(bullet_time == 0){
		bullet_time = property->get_bullet().gap;
		return true;
	}
	return false;
}

void Plant_role::inc_health(int x){
	this->health += x;
}

void Plant_role::modify_health(int x){
	this->health = x;
}

int Plant_role::locate_effect(string name){
	if(effect.empty()) return -1;
	int len = effect.size();
	for(int i=0;i<len;i++){
		if(effect[i].effect_name == name) return i;
	}
	return -1;
}

int Plant_role::locate_skill(string name){
	if(skill.empty()) return -1;
	int len = skill.size();
	for(int i=0;i<len;i++){
		if(skill[i].skill_name == name) return i;
	}
	return -1;
}

bool Plant_role::gain_effect(string name){
	if(locate_effect(name)!=-1) return false;
	effect.push_back(_make_effect(name,1));
	return true;
}

bool Plant_role::gain_skill(string name){
	if(locate_skill(name)!=-1) return false;
	skill.push_back(_make_skill(name,1));
	return true;
} 

bool Plant_role::disable_effect(string name){
	int index = locate_effect(name);
	if(index==-1) return false;
	effect.erase(effect.begin()+index);
	return true;
}

bool Plant_role::disable_skill(string name){
	int index = locate_skill(name);
	if(index==-1) return false;
	skill.erase(skill.begin()+index);
	return true;
}

vector <string> Plant_role::get_attacked_skill(){
	vector <string> s;
	s.clear();
	int size = skill.size();
	for(int i=0;i<size;i++){
		if(skill[i].trigger == TS_ATTACKED){
			s.push_back(skill[i].skill_name);
		}
	}
	return s;
}

vector <string> Plant_role::get_attacking_skill(){
	vector <string> s;
	s.clear();
	int size = skill.size();
	for(int i=0;i<size;i++){
		if(skill[i].trigger == TS_ATTACKING){
			s.push_back(skill[i].skill_name);
		}
	}
	return s;
}

vector <string> Plant_role::effect_count(){
	vector <string> res;
	res.clear();
	int sz = effect.size();
	for(int i=0;i<sz;i++){
		effect[i].remaining_time --;
		if(effect[i].remaining_time == 0){
			res.push_back(effect[i].effect_name);
			effect[i].remaining_time = effect[i].time;
		}
	}
	return res;
}

Plant_role::~Plant_role(){
	effect.clear();
	skill.clear();
	property = NULL;
}

Zombie_role::Zombie_role(Zombie_ppt* ppt, int row, int column){
	this->property = ppt;
	health = ppt->get_hp();
	move = ppt->get_speed();
	attack = ppt->get_attack();
	Status st;
	st.time = 0;
	st.type = S_NONE;
	status = st;
	speed = ppt->get_speed();
	this->row = row;
	this->column = column;
	effect.clear();
	skill.clear();
	if(ppt->has_effect()){
		effect.push_back(ppt->make_effect());
	}
	if(ppt->has_skill()){
		skill.push_back(ppt->make_skill());
	}
	die = false;
}

void Zombie_role::inc_health(int x){
	this->health += x;
}

void Zombie_role::modify_health(int x){
	this->health = x;
}

void Zombie_role::modify_attack(int x){
	this->attack = x;
}

void Zombie_role::modify_speed(int x){
	this->speed = x;
	if(move>speed) move = speed;
}

bool Zombie_role::move_forward(){
	bool suc = true;
	if(status.type == S_FROZEN){
		status.time --;
		return true;
	}
	else if(status.type == S_SLOW){
		if(status.flag == true){
			status.flag = false;
			status.time--;
			if(status.time == 0){
				Status s;
				s.type = S_NONE;
				set_status(s);
			} 
			return true;
		}
		else{
			status.flag = true;
			status.time --;
			if(status.time == 0){
				Status s;
				s.type = S_NONE;
				set_status(s);
			} 
			move -- ;
		}
	}
	else{
		move -- ;
	}
	
	if(move==0) {
		column --;
		if(column < 0) return false;
		move = speed;
	}
	return suc;
}

void Zombie_role::transfer(int row, int column){
	this->row = row;
	this->column = column;
}


int Zombie_role::locate_effect(string name){
	if(effect.empty()) return -1;
	int len = effect.size();
	for(int i=0;i<len;i++){
		if(effect[i].effect_name == name) return i;
	}
	return -1;
}

int Zombie_role::locate_skill(string name){
	if(skill.empty()) return -1;
	int len = skill.size();
	for(int i=0;i<len;i++){
		if(skill[i].skill_name == name) return i;
	}
	return -1;
}

bool Zombie_role::gain_effect(string name){
	if(locate_effect(name)!=-1) return false;
	effect.push_back(_make_effect(name,1));
	return true;
}

bool Zombie_role::gain_skill(string name){
	if(locate_skill(name)!=-1) return false;
	skill.push_back(_make_skill(name,1));
	return true;
} 

bool Zombie_role::disable_effect(string name){
	int index = locate_effect(name);
	if(index==-1) return false;
	effect.erase(effect.begin()+index);
	return true;
}

bool Zombie_role::disable_skill(string name){
	int index = locate_skill(name);
	if(index==-1) return false;
	skill.erase(skill.begin()+index);
	return true;
}

vector <string> Zombie_role::get_attacked_skill(){
	vector <string> s;
	s.clear();
	int size = skill.size();
	for(int i=0;i<size;i++){
		if(skill[i].trigger == TS_ATTACKED){
			s.push_back(skill[i].skill_name);
		}
	}
	return s;
}

vector <string> Zombie_role::get_attacking_skill(){
	vector <string> s;
	s.clear();
	int size = skill.size();
	for(int i=0;i<size;i++){
		if(skill[i].trigger == TS_ATTACKING){
			s.push_back(skill[i].skill_name);
		}
	}
	return s;
}

vector <string> Zombie_role::effect_count(){
	vector <string> res;
	res.clear();
	int sz = effect.size();
	for(int i=0;i<sz;i++){
		effect[i].remaining_time --;
		if(effect[i].remaining_time == 0){
			res.push_back(effect[i].effect_name);
			effect[i].remaining_time = effect[i].time;
		}
	}
	return res;
}

Zombie_role::~Zombie_role(){
	effect.clear();
	skill.clear();
	property = NULL;
}
