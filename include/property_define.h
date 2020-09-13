#ifndef _PROPERTY_DEFINE_H
#define _PROPERTY_DEFINE_H
#include <bits/stdc++.h>
using namespace std;
enum PType{P_NONE,P_ATTACK,P_DEFENCE,P_BOOM,P_EFFECT};
enum TSType{  //trigger skill type
	TS_NONE,  //inv
	TS_ALL,  //can be triggered every frame
	TS_ATTACKING, //can be triggered when dealt damage
	TS_ATTACKED, //can be triggered when under attack
};
enum STType{  //zombie's status type
	S_NONE,  //no status
	S_SLOW,  //the speed of movement and attacking slows to 1/2
	S_FROZEN,  //the speed come to 0.
};
struct PEffect{  //passive effects
	string effect_name;
	int role;  //1:plant 2:zombie
	int remaining_time;
	int time;
	PEffect():remaining_time(0),time(0){}
};
struct TSkill{  //trigger skill
	string skill_name;
	int role;  //1:p->z 2:z->p
	TSType trigger;
	TSkill():trigger(TS_NONE){}
};
struct Status{  //zombie status
	STType type;
	bool flag;
	int time;
	Status():type(S_NONE),flag(true),time(0){}
};
struct Bullet{
	bool activated; //has bullet or not
//	bool target_mode; 
	/*if is true, then bullet will reach the nearest zombie in a specific time period. 
	Otherwise, the bullet go a specific distance for the same time  (discarded)*/ 
	int speed;
	/*speed is the distance of which bullets fly for 0.25s (a frame)
	in order to simplify the code frame, we discard this as well, making bullets move 1 field per frame
	*/
	int attack; //attack index of bullet
	int gap; //time period between which the plant shoots (frame)
	int bullet_type; //specify the bullet type
	/*
	type_1: ○  type_2: ○○ type_3: ※ type_4:□ 
	*/
};

class Plant_ppt{
	private:
		string name;
		PType type;
		Bullet bullet;
		int pcd;
		int charging_time; // the cd time remaining.
		int spend;
		int maxhp;
		string init_effect; //we acknowledge that one plant has mostly one effect and one skill initially.
		string init_skill;
		string describe; //the description of the plant
	public:
		Plant_ppt(string n, PType t, Bullet b, int pcd, int spend, int maxhp, string describe,
					string effect_name, string skill_name);
		string get_name(){return this->name;}
		string get_describe(){return this->describe;}
		string get_type(){
			switch(type){
				case P_ATTACK: return "攻击型";break;
				case P_DEFENCE: return "防御型";break;
				case P_BOOM: return "爆炸型";break;
				case P_EFFECT: return "效果型";break;
				default: return "undefined";break; 
			}	
		}
		Bullet get_bullet(){return this->bullet;}
		int get_pcd(){return this->pcd;}
		int get_spend(){return this->spend;}
		int get_hp(){return this->maxhp;}
		int get_charging(){return this->charging_time;}
		void recharge(){charging_time = pcd;}
		void discharge(){charging_time -= 1;}
		bool has_effect(){return !init_effect.empty();}
		bool has_skill(){return !init_skill.empty();}
		string get_effect_name(){return init_effect;} 
		string get_skill_name(){return init_skill;}
		PEffect make_effect();
		TSkill make_skill();
};

class Zombie_ppt{
	private:
		string name;
		int maxhp;
		int attack; //the attacking index per frame
		int speed; //time to move to the next field
		string init_effect;
		string init_skill;
		string describe;
	public:
		Zombie_ppt(string n, int maxhp, int attack, int speed, string describe,
					string effect_name, string skill_name);
		string get_name(){return this->name;}
		string get_describe(){return this->describe;}
		int get_hp(){return this->maxhp;}
		int get_attack(){return this->attack;}
		int get_speed(){return this->speed;}
		bool has_effect(){return !init_effect.empty();}
		bool has_skill(){return !init_skill.empty();}
		string get_effect_name(){return init_effect;} 
		string get_skill_name(){return init_skill;}
		PEffect make_effect();
		TSkill make_skill();
};

PEffect _make_effect(string name, int role);
TSkill _make_skill(string name, int role);

#endif
