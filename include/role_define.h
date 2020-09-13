#ifndef _ROLE_DEFINE_H
#define _ROLE_DEFINE_H
#include "property_define.h"
#include <vector>
class Plant_role{
	private:
		Plant_ppt* property;
		int health;
		int bullet_time;
		vector <PEffect> effect;
		vector <TSkill> skill;
		int row;
		int column;
		bool die;
	public:
		Plant_role (Plant_ppt* ppt, int row, int column);
		Plant_ppt* get_property(){return property;}
		void inc_health(int x);
		void modify_health (int x);
		int get_health(){return health;}
		int get_row(){return row;}
		int get_column(){return column;}
		bool do_bullet();
		int locate_effect(string name);
		int locate_skill(string name);
		bool gain_effect (string name);
		bool gain_skill (string name);
		bool disable_effect (string name);
		bool disable_skill (string name);
		bool is_killed(){return die;}
		void be_killed(){die = true;}
		vector <string> get_attacking_skill();
		vector <string> get_attacked_skill();
		vector <string> effect_count();
		~Plant_role();
};

class Zombie_role{
	private:
		Zombie_ppt* property;
		int speed;
		int health;
		int attack;
		int move; //next time to move to the next field
		Status status;
		vector <PEffect> effect;
		vector <TSkill> skill;
		int row;
		int column;
		bool die;
	public:
		Zombie_role(Zombie_ppt* ppt, int row, int column);
		Zombie_ppt* get_property(){return property;}
		void inc_health(int x);
		void modify_health(int x);
		void modify_attack(int x);
		void modify_speed(int x);
		int get_health(){return health;}
		int get_row(){return row;}
		int get_column(){return column;}
		int get_attack(){return attack;}
		Status get_status(){return status;}
		void reset_status_time(int x){status.time=x;}
		void set_status(Status s){status = s;}
		bool move_forward();
		void transfer(int row, int column);
		int locate_effect(string name);
		int locate_skill(string name);
		bool gain_effect (string name);
		bool gain_skill (string name);
		bool disable_effect (string name);
		bool disable_skill (string name);
		bool is_killed(){return die;}
		void be_killed(){die = true; attack = 0; move = 999;}
		vector <string> get_attacking_skill();
		vector <string> get_attacked_skill();
		vector <string> effect_count();
		~Zombie_role();
};

#endif
