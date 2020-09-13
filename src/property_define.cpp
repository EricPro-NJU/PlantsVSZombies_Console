#include "..\include\property_define.h"
Plant_ppt::Plant_ppt(string n, PType t, Bullet b, int pcd, int spend, int maxhp, string describe,
			string effect_name, string skill_name){
	name = n;
	type = t;
	bullet = b;
	this->pcd = pcd;
	this->spend = spend;
	this->maxhp = maxhp;
	this->describe = describe;
	this->charging_time = 0;
	if(effect_name=="\0"){
		init_effect.clear();
	}
	else{
		this->init_effect = effect_name;
	}
	if(skill_name == "\0"){
		init_skill.clear();
	}
	else{
		this->init_skill = skill_name;			
	}
	
}

Zombie_ppt::Zombie_ppt(string n, int maxhp, int attack, int speed, string describe,
			  string effect_name, string skill_name){
	this->name = n;
	this->maxhp = maxhp;
	this->attack = attack;
	this->speed = speed;
	this->describe = describe;
	if(effect_name=="\0"){
		init_effect.clear();
	}
	else{
		this->init_effect = effect_name;
	}
	if(skill_name == "\0"){
		init_skill.clear();
	}
	else{
		this->init_skill = skill_name;			
	}		  	
}



PEffect Plant_ppt::make_effect(){
	return _make_effect(init_effect,1);
}

TSkill Plant_ppt::make_skill(){
	return _make_skill(init_skill,1);
}

PEffect Zombie_ppt::make_effect(){
	return _make_effect(init_effect,2);
}

TSkill Zombie_ppt::make_skill(){
	return _make_skill(init_skill,2);
}

