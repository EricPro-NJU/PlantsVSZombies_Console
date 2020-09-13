#ifndef _GAME_SYSTEM_H
#define _GAME_SYSTEM_H
#include "role_define.h"
#define PREPARE_TIME 28500
#define INIT_SUN 100
string getSysTime();
class Damage{
	private:
		Plant_role* p;
		Zombie_role* z;
		int attack_type; //1: p->z;  2:z->p
		int index;
	public:
		Damage(Plant_role* p, Zombie_role* z, int i); //1: p->z
		Damage(Zombie_role* z, Plant_role* p, int i); //2: z->p
		void dealt();
		~Damage();
};

class RoleGallery{
	private:
		string filename_plant;
		string filename_zombie;
		vector <Plant_ppt> plant_list;
		vector <Zombie_ppt> zombie_list;
		int plant_num;
		int zombie_num;
	public:
		RoleGallery(string pfile, string zfile);
		Plant_ppt* get_plant(string name);
		Plant_ppt* get_plant(int id);
		Zombie_ppt* get_zombie(string name);
		Zombie_ppt* get_zombie(int id);
		void display();
		void enter_display();
		void display_plant();
		void display_zombie();
		int get_plantnum(){return plant_num;}
		int get_zombienum(){return zombie_num;}
		string get_skill_desc(string name); 
		int point_rules(string name); 
		~RoleGallery(){
			plant_list.clear();
			zombie_list.clear();	
		}
};
struct BAttack{
	int type;
	int index;
	Plant_ppt* attacker;
	bool moved;
};
//5rows 8columns
struct Field{
	int row;
	int column;
	bool strike; //dici effect
	vector <BAttack> blts;
	Plant_role* plants; //only one plant can be set in one field.
	vector <Zombie_role*> zombies; 
};
class Game{
	private:
		RoleGallery roles;
		Field land[5][8];
		int point;
		int wave;
		int sun;
		bool game_running;
		bool extra_point; 
		string log;
		string shop_log;
		string sun_log;
		string wave_log;
		int chosen_row;
		int chosen_column;
		int chosen_plant;
		bool zombies_alive();
	public:
		Game(string s);
		void reset();
		//running funcs
		void loops();
		void run();
		void wave_attack();
		void keyboard_operation();
		void suninc();
		void enter_display(){roles.enter_display();}
		void frame_forward();
		void reset_bullet();
		bool game_over();
		Plant_ppt* get_plant(string name){return roles.get_plant(name);}
		Zombie_ppt* get_zombie(string name){return roles.get_zombie(name);}
		bool set_plant(Plant_role* p);
		void set_zombie(Zombie_role* z);
		void damage(Plant_role* attacker, Zombie_role* victim, int index);
		void damage(Zombie_role* attacker, Plant_role* victim, int index);
		bool remove_zombie(Zombie_role* z);
		bool remove_plant(Plant_role* p);
		void draw();
		void draw_shop();
		void logging(){cout<<log<<endl;}
		bool is_running(){return game_running;}
		void do_effect(string effect_name, Plant_role* p);
		void do_effect(string effect_name, Zombie_role* z);
		void do_skill(string effect_name, Plant_role* p, Zombie_role* z);
		void do_skill(string effect_name, Zombie_role* z, Plant_role* p);
		//TODO: skills and effects defined here
		void sun_generating(Plant_role* p);
		void pump_exploding(Plant_role* p, Zombie_role* z);
		void potato_activating(Plant_role* p);
		void water_split(Plant_role* p, Zombie_role* z);
		void cherry_exploding(Plant_role *p);
		void chilly_exploding(Plant_role *p); 
		void slow_you_down(Plant_role* p, Zombie_role* z);
		void smell_attack(Plant_role* p, Zombie_role* z);
		void newspaper_crazy(Zombie_role* z,Plant_role* p);
		void bar_jumping(Zombie_role* z, Plant_role* p);
};


#endif
