#ifndef BOX2D_TEST_INCLUDED
#define BOX2D_TEST_INCLUDED

#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "Box2D/Box2D.h"

#include "texture.h"

class box2d_test_t
{
public:
	box2d_test_t();

	void init();
	void reset();
	void render();
	void update();

	void save(const std::string& sav);
	void load(const std::string& sav);

	void key_up(int key);
	void key_down(int key);

	void mouse_move(int x, int y);
	void mouse_up(int x, int y, int btn);
	void mouse_down(int x, int y, int btn);
	void mouse_drag(int x, int y, int btn);
	void mouse_wheel(int x, int y, int dir);

private:
	bool mouse_pressed_;

	b2World world_;
	std::vector<b2Body*> bodies_;
	std::unordered_map<b2Body*, int> colors_;

	int current_selected_;
	std::vector<int> selected_stack_;
	std::unordered_set<int> all_selected_;

	std::unordered_set<b2Body*> auto_remove_list_;
	std::unordered_map<b2Body*, std::unordered_set<b2Body*>> auto_connect_list_;

	std::unordered_set<b2Body*> connected_list_;
	std::unordered_set<b2Body*> explode_preview_list_;
	std::unordered_set<b2Body*> explode_list_;

	std::map<int, int> bomb_level_;
	float bomb_1_radius_, bomb_2_size_;

	std::unordered_map<b2Body*, int> bomb_list_;

	float max_gap_, extra_dist_;
	int min_remove_num_, max_obj_num_;

	bool is_nearby(b2Body* this_body, b2Body* that_body);
	bool is_connected(b2Body* this_body, b2Body* that_body);
	bool is_touched(b2Body* body, const b2Vec2& pos);
	bool is_rough_touched(b2Body* body, const b2Vec2& pos);

	void generate(int num, bool auto_remove);
	void connect(b2Body* body);
	void auto_connect();
	int find_base_color();
	int get_connected_num();

	void rand_force();
	void auto_remove();

	void explode(b2Body* bomb);
	void explode_preview(b2Body* bomb);
	void auto_explode();

	void remove(const std::vector<int>& remove_list, bool enable_explode);
	void remove(const std::vector<b2Body*> remove_list, bool enable_explode);
	void remove(const std::unordered_set<b2Body*> remove_list, bool enable_explode);
};

#endif