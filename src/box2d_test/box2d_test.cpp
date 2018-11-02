
#include "box2d_test.h"

#include <queue>

#include <GL/gl.h>

#include <spine/spine.h>

#include "util.h"
#include "draw.h"
#include "window.h"

spine::SpineExtension* spine::getDefaultExtension()
{
	return new DefaultSpineExtension();
}

class MyTextureLoader : public spine::TextureLoader
{
	virtual void load(spine::AtlasPage& page, const spine::String& path)
	{
		void* texture = nullptr;
		page.setRendererObject(texture);
	}

	virtual void unload(void* texture)
	{}
};

box2d_test_t::box2d_test_t()
	: mouse_pressed_(false)
	, world_(b2Vec2(0.0f, -10.0f))
	, current_selected_(-1)
	, min_remove_num_(3), max_obj_num_(40)
	, max_gap_(20.0f), extra_dist_(15.0f)
	, bomb_1_radius_(120.0f), bomb_2_size_(60.0f)
{}

void box2d_test_t::init()
{
	bomb_level_[1] = 4;
	bomb_level_[2] = 5;

	int win_width = window_t::width();
	int win_height = window_t::height();

	b2BodyDef bottomBodyDef;
	bottomBodyDef.position.Set(win_width * 0.5, 0.0f);
	b2Body* bottomBody = world_.CreateBody(&bottomBodyDef);
	b2PolygonShape bottomBox;
	bottomBox.SetAsBox(win_width * 0.5f, 10.0f);
	bottomBody->CreateFixture(&bottomBox, 0.0f);

	b2BodyDef topBodyDef;
	topBodyDef.position.Set(win_width * 0.5,  win_height * 2.0);
	b2Body* topBody = world_.CreateBody(&topBodyDef);
	b2PolygonShape topBox;
	topBox.SetAsBox(win_width * 0.5f, 10.0f);
	topBody->CreateFixture(&topBox, 0.0f);

	b2BodyDef leftBodyDef;
	leftBodyDef.position.Set(0.0f, win_height);
	b2Body* leftBody = world_.CreateBody(&leftBodyDef);
	b2PolygonShape leftBox;
	leftBox.SetAsBox(10.0f, win_height);
	leftBody->CreateFixture(&leftBox, 0.0f);

	b2BodyDef rightBodyDef;
	rightBodyDef.position.Set(win_width, win_height);
	b2Body* rightBody = world_.CreateBody(&rightBodyDef);
	b2PolygonShape rightBox;
	rightBox.SetAsBox(10.0f, win_height);
	rightBody->CreateFixture(&rightBox, 0.0f);

	b2BodyDef centerBodyDef;
	centerBodyDef.position.Set(win_width * 0.5f, win_height * 0.25f);
	b2Body* centerBody = world_.CreateBody(&centerBodyDef);
	b2PolygonShape centerBox;
	centerBox.SetAsBox(win_width * 0.25f, 10.0f);
	centerBody->CreateFixture(&centerBox, 0.0f);

	generate(max_obj_num_, false);
}

void box2d_test_t::reset()
{
	remove(bodies_, false);

	current_selected_ = -1;
	selected_stack_.clear();
	all_selected_.clear();
	connected_list_.clear();
	explode_preview_list_.clear();
	explode_list_.clear();
	bomb_list_.clear();
	auto_remove_list_.clear();
	auto_connect_list_.clear();

	generate(max_obj_num_, false);
}

void box2d_test_t::render()
{
	int win_width = window_t::width();
	int win_height = window_t::height();

	use_color("black");
	draw_rect(win_width * 0.5f, 0.0f, win_width, 20.0f, true);
	draw_rect(0.0f, win_height, 20.0f, win_height * 2.0f, true);
	draw_rect(win_width, win_height, 20.0f, win_height * 2.0f, true);
	draw_rect(win_width * 0.5f, win_height * 0.25f, win_width * 0.5f, 20.0f, true);
	use_color("white");

	int index = 0;

	for(const auto& body : bodies_)
	{
		b2Vec2 body_pos = body->GetPosition();
		float radius = body->GetFixtureList()[0].GetShape()->m_radius;

		use_color(colors_[body]);
		draw_circle(body_pos.x, body_pos.y, 0.0f, radius);
		use_color("white");

		if(connected_list_.count(body) > 0)
		{
			use_color("black");
			draw_circle(body_pos.x, body_pos.y, radius - 6.0f, radius - 4.0f);

			use_color("blue");
			draw_circle(body_pos.x, body_pos.y, 0.0f, radius - 8.0f);

			use_color("white");
		}

		if(current_selected_ == index || all_selected_.count(index) > 0)
		{
			use_color("black");
			draw_circle(body_pos.x, body_pos.y, radius - 6.0f, radius - 4.0f);

			use_color("blue");
			draw_circle(body_pos.x, body_pos.y, 0.0f, radius - 8.0f);

			use_color("white");
		}

		if(explode_preview_list_.count(body) > 0)
		{
			use_color("black");
			draw_circle(body_pos.x, body_pos.y, radius - 6.0f, radius - 4.0f);

			use_color("red");
			draw_circle(body_pos.x, body_pos.y, 0.0f, radius - 8.0f);

			use_color("white");
		}

		if(bomb_list_.count(body) > 0)
		{
			use_color("black");
			draw_circle(body_pos.x, body_pos.y, radius - 6.0f, radius - 4.0f);

			use_color("red");
			draw_circle(body_pos.x, body_pos.y, 0.0f, radius - 8.0f);

			int level = bomb_list_[body];

			if(1 == level)
			{
				use_color("black");
				draw_circle(body_pos.x, body_pos.y, 5.0f, 10.0f);
			}
			else if(2 == level)
			{
				use_color("black");
				draw_line(body_pos.x - 10.0f, body_pos.y, body_pos.x + 10.0f, body_pos.y, 5.0f);
				draw_line(body_pos.x, body_pos.y - 10.0f, body_pos.x, body_pos.y + 10.0f, 5.0f);
			}

			use_color("white");
		}

		++index;
	}

	index = 0;

	for(const auto& body : bodies_)
	{
		if(current_selected_ == index && bomb_list_.count(body) > 0)
		{
			b2Vec2 body_pos = body->GetPosition();

			int level = bomb_list_[body];

			if(1 == level)
			{
				use_color("red");
				draw_circle(body_pos.x, body_pos.y, bomb_1_radius_ - 2.0f, bomb_1_radius_);
			}
			else if(2 == level)
			{
				use_color("red");

				draw_line(0.0f, body_pos.y - bomb_2_size_, win_width, body_pos.y - bomb_2_size_, 2.0f);
				draw_line(0.0f, body_pos.y + bomb_2_size_, win_width, body_pos.y + bomb_2_size_, 2.0f);

				draw_line(body_pos.x - bomb_2_size_, 0.0f, body_pos.x - bomb_2_size_, win_height, 2.0f);
				draw_line(body_pos.x + bomb_2_size_, 0.0f, body_pos.x + bomb_2_size_, win_height, 2.0f);
			}

			use_color("white");
		}

		++index;
	}

	use_color("yellow");

	glLineWidth(2.0f);

	glBegin(GL_LINE_STRIP);

	for(const auto& index : selected_stack_)
	{
		auto body = bodies_[index];
		auto pos = body->GetPosition();

		glVertex2f(pos.x, pos.y);
	}

	if(current_selected_ >= 0)
	{
		auto body = bodies_[current_selected_];
		auto pos = body->GetPosition();

		glVertex2f(pos.x, pos.y);
	}

	glEnd();
}

void box2d_test_t::update()
{
	float32 timeStep = 1.0f / 10.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_.Step(timeStep, velocityIterations, positionIterations);

	if(mouse_pressed_)
	{
		if(current_selected_ >= 0)
		{
			auto body = bodies_[current_selected_];
			b2Vec2 mouse(window_t::mouse_x(), window_t::mouse_y());

			if(!is_rough_touched(body, mouse))
			{
				current_selected_ = -1;
				selected_stack_.clear();
				all_selected_.clear();
				connected_list_.clear();
				explode_preview_list_.clear();
			}
		}
	}

	if(explode_list_.size() > 0)
	{
		remove(explode_list_, false);
		explode_list_.clear();

		
	}

	auto_connect();

	int need_gen_num = max_obj_num_ - bodies_.size();

	if(need_gen_num > 0)
	{
		generate(need_gen_num, false);
	}
}

void save(const std::string& sav)
{

}

void load(const std::string& sav)
{

}

void box2d_test_t::key_up(int key)
{

}

void box2d_test_t::key_down(int key)
{
	if('f' == key)
	{
		rand_force();
	}
	else if('r' == key)
	{
		reset();
	}
}

void box2d_test_t::mouse_move(int x, int y)
{

}

void box2d_test_t::mouse_up(int x, int y, int btn)
{
	mouse_pressed_ = false;

	if(current_selected_ >= 0)
	{
		auto body = bodies_[current_selected_];
		b2Vec2 mouse(x, y);

		if(is_touched(body, mouse))
		{
			int min_connected = bomb_level_.begin()->second;
			int max_connected = bomb_level_.rbegin()->second;

			int all_connected = get_connected_num();

			if(all_connected >= min_remove_num_)
			{
				if(all_connected >= min_connected)
				{
					if(all_connected > max_connected)
					{
						all_connected = max_connected;
					}

					int bomb_level = bomb_level_.begin()->first;

					for(const auto& [level, connected] : bomb_level_)
					{
						if(all_connected <= connected)
						{
							bomb_level = level;
							break;
						}
					}

					bomb_list_.insert({ bodies_[current_selected_], bomb_level });
				}
				else
				{
					selected_stack_.push_back(current_selected_);
					all_selected_.insert(current_selected_);
				}

				remove(selected_stack_, true);
			}
			else if(bomb_list_.count(body) > 0)
			{
				explode(body);
			}
		}		
	}

	current_selected_ = -1;
	selected_stack_.clear();
	all_selected_.clear();
	connected_list_.clear();
	explode_preview_list_.clear();

	int need_gen_num = max_obj_num_ - bodies_.size();

	if(need_gen_num > 0)
	{
		generate(need_gen_num, true);
	}
}

void box2d_test_t::mouse_down(int x, int y, int btn)
{
	mouse_pressed_ = true;

	current_selected_ = -1;

	b2Vec2 mouse(x, y);

	int index = 0;

	for(const auto& body : bodies_)
	{
		b2Vec2 position = body->GetPosition();
		float radius = body->GetFixtureList()[0].GetShape()->m_radius;

		if(b2DistanceSquared(mouse, position) <= radius * radius)
		{
			current_selected_ = index;

			connect(body);

			if(bomb_list_.count(body) > 0)
			{
				explode_preview(body);
			}

			break;
		}

		++index;
	}
}

void box2d_test_t::mouse_drag(int x, int y, int btn)
{
	if(current_selected_ < 0)
	{
		return;
	}

	b2Vec2 mouse(x, y);

	b2Body* target_body = bodies_[current_selected_];

	int index = 0;

	for(const auto& body : bodies_)
	{
		if(index == current_selected_)
		{
			++index;
			continue;
		}

		if(is_touched(body, mouse))
		{
			explode_preview_list_.clear();

			if(selected_stack_.size() > 0 && index == selected_stack_.back())
			{
				current_selected_ = index;
				all_selected_.erase(index);
				selected_stack_.pop_back();

				break;
			}

			if(all_selected_.count(index) > 0)
			{
				break;
			}

			if(is_connected(target_body, body))
			{
				bool need_refresh = find_base_color() < 0;

				selected_stack_.push_back(current_selected_);
				all_selected_.insert(current_selected_);
				current_selected_ = index;

				if(need_refresh)
				{
					connected_list_.clear();
					connect(body);
				}

				break;
			}
		}

		++index;
	}
}

void box2d_test_t::mouse_wheel(int x, int y, int dir)
{

}

bool box2d_test_t::is_rough_touched(b2Body* body, const b2Vec2& pos)
{
	b2Vec2 body_pos = body->GetPosition();
	float radius = body->GetFixtureList()[0].GetShape()->m_radius + extra_dist_;

	return b2DistanceSquared(pos, body_pos) <= radius * radius;
}

bool box2d_test_t::is_nearby(b2Body* this_body, b2Body* that_body)
{
	b2Vec2 this_position = this_body->GetPosition();
	b2Vec2 that_position = that_body->GetPosition();

	float this_radius = this_body->GetFixtureList()[0].GetShape()->m_radius;
	float that_radius = that_body->GetFixtureList()[0].GetShape()->m_radius;

	float dist = this_radius + that_radius + max_gap_;

	return b2DistanceSquared(this_position, that_position) <= dist * dist;
}

bool box2d_test_t::is_connected(b2Body* this_body, b2Body* that_body)
{
	bool ret = false;

	int base_color = find_base_color();

	bool is_this_bomb = bomb_list_.count(this_body) > 0;
	bool is_that_bomb = bomb_list_.count(that_body) > 0;

	if(is_nearby(this_body, that_body))
	{
		bool same_color = colors_[this_body] == colors_[that_body];

		if(base_color < 0)
		{
			if(is_this_bomb || is_that_bomb)
			{
				ret = true;
			}
			else
			{
				ret = same_color;
			}
		}
		else
		{
			if(is_this_bomb && is_this_bomb)
			{
				ret = true;
			}
			else if(is_this_bomb)
			{
				ret = base_color == colors_[that_body];
			}
			else if(is_that_bomb)
			{
				ret = base_color == colors_[this_body];
			}
			else
			{
				ret = same_color;
			}
		}
	}

	return ret;
}

bool box2d_test_t::is_touched(b2Body* body, const b2Vec2& pos)
{
	b2Vec2 body_pos = body->GetPosition();
	float radius = body->GetFixtureList()[0].GetShape()->m_radius;

	return b2DistanceSquared(pos, body_pos) <= radius * radius;
}

void box2d_test_t::generate(int num, bool auto_remove)
{
	int win_width = window_t::width();
	int win_height = window_t::height();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	for(int i = 0; i < num; ++i)
	{
		bodyDef.position.Set(rand_real(win_width * 0.1, win_width * 0.9),
			rand_real(win_height * 1.0, win_height * 1.8));

		b2Body* body = world_.CreateBody(&bodyDef);

		b2CircleShape dynamicCircle;
		dynamicCircle.m_radius = rand_real(30.0, 30.0);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicCircle;

		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		body->CreateFixture(&fixtureDef);

		b2Vec2 force(0.0f, rand_real(-100.0f, -200.0f));
		body->ApplyForce(force, body->GetWorldCenter(), true);

		b2Vec2 velocity(0.0f, rand_real(-100.0f, -200.0f));
		body->SetLinearVelocity(velocity);

		b2Vec2 impulse(0.0f, rand_real(-100.0f, -200.0f));
		body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);

		bodies_.push_back(body);

		colors_[body] = rand_int(10, 13);

		if(auto_remove)
		{
			auto_remove_list_.insert(body);
		}
	}
}

void box2d_test_t::connect(b2Body* body)
{
	connected_list_.clear();

	std::queue<b2Body*> open_list;

	open_list.push(body);

	while(!open_list.empty())
	{
		auto target_body = open_list.front();
		open_list.pop();

		for(const auto& body : bodies_)
		{
			if(body != target_body &&
				0 == connected_list_.count(body) &&
				is_connected(body, target_body))
			{
				open_list.push(body);
				connected_list_.insert(body);
			}
		}
	}
}

void box2d_test_t::auto_connect()
{
	for(std::unordered_set<b2Body*>::const_iterator it = auto_remove_list_.begin();
		it != auto_remove_list_.end();)
	{
		auto auto_body = *it;

		b2Vec2 velocity = auto_body->GetLinearVelocity();

		if(velocity.Length() > 1.0f)
		{
			++it;
			continue;
		}

		std::queue<b2Body*> open_list;

		open_list.push(auto_body);

		while(!open_list.empty())
		{
			auto target_body = open_list.front();
			open_list.pop();

			for(const auto& body : bodies_)
			{
				if(0 == bomb_list_.count(body))
				{
					if(body != target_body &&
						0 == auto_connect_list_[auto_body].count(body) &&
						is_connected(body, target_body))
					{
						open_list.push(body);
						auto_connect_list_[auto_body].insert(body);
					}
				}
			}
		}

		auto_remove();

		it = auto_remove_list_.erase(it);

		if(0 == auto_remove_list_.size())
		{
			auto_explode();
		}
	}
}

int box2d_test_t::find_base_color()
{
	int ret = -1;

	for(const auto& index : selected_stack_)
	{
		auto body = bodies_[index];

		if(0 == bomb_list_.count(body))
		{
			ret = colors_[body];
			break;
		}
	}

	if(ret < 0)
	{
		if(current_selected_ > 0)
		{
			auto body = bodies_[current_selected_];

			if(0 == bomb_list_.count(body))
			{
				ret = colors_[body];
			}
		}
	}

	return ret;
}

int box2d_test_t::get_connected_num()
{
	int ret = 0;

	if(current_selected_ >= 0)
	{
		auto body = bodies_[current_selected_];

		if(0 == bomb_list_.count(body))
		{
			++ret;
		}

		for(const auto& index : selected_stack_)
		{
			auto body = bodies_[index];

			if(0 == bomb_list_.count(body))
			{
				++ret;
			}
		}
	}

	return ret;
}

void box2d_test_t::rand_force()
{
	for(const auto& body : bodies_)
	{
		b2Vec2 force(rand_real(-80.0f, 80.0f), rand_real(20.0f, 50.0f));
		body->ApplyForce(force, body->GetWorldCenter(), true);

		b2Vec2 velocity(rand_real(-80.0f, 80.0f), rand_real(20.0f, 50.0f));
		body->SetLinearVelocity(velocity);

		b2Vec2 impulse(rand_real(-80.0f, 80.0f), rand_real(20.0f, 50.0f));
		body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
	}
}

void box2d_test_t::auto_remove()
{
	int min_connected = bomb_level_.begin()->second;
	int max_connected = bomb_level_.rbegin()->second;

	for(std::unordered_map<b2Body*, std::unordered_set<b2Body*>>::const_iterator it = auto_connect_list_.begin();
		it != auto_connect_list_.end();)
	{
		auto auto_body = it->first;
		auto auto_list = it->second;

		b2Vec2 velocity = auto_body->GetLinearVelocity();

		if(velocity.Length() > 1.0f)
		{
			++it;
			continue;
		}

		int all_connected = auto_list.size();

		if(all_connected >= min_remove_num_)
		{
			if(all_connected >= min_connected)
			{
				if(all_connected > max_connected)
				{
					all_connected = max_connected;
				}

				int bomb_level = bomb_level_.begin()->first;

				for(const auto& [level, connected] : bomb_level_)
				{
					if(all_connected <= connected)
					{
						bomb_level = level;
						break;
					}
				}

				auto_list.erase(auto_body);
				bomb_list_.insert({ auto_body, bomb_level });
			}

			remove(auto_list, false);
		}

		it = auto_connect_list_.erase(it);
	}
}

void box2d_test_t::explode(b2Body* bomb)
{
	int level = bomb_list_[bomb];

	b2Vec2 target_pos = bomb->GetPosition();

	explode_list_.insert(bomb);

	for(const auto& body : bodies_)
	{
		if(explode_list_.count(body) > 0)
		{
			continue;
		}

		b2Vec2 pos = body->GetPosition();
		float radius = body->GetFixtureList()[0].GetShape()->m_radius;

		if(1 == level)
		{
			float dist = bomb_1_radius_ + radius - 2;

			if(b2DistanceSquared(target_pos, pos) <= dist * dist)
			{
				if(bomb_list_.count(body) > 0)
				{
					explode(body);
				}
				else
				{
					explode_list_.insert(body);
				}
			}
		}
		else if(2 == level)
		{
			float min_x, max_x, min_y, max_y;

			min_x = target_pos.x - bomb_2_size_ + 2;
			max_x = target_pos.x + bomb_2_size_ - 2;
			min_y = target_pos.y - bomb_2_size_ + 2;
			max_y = target_pos.y + bomb_2_size_ - 2;

			if((pos.x + radius >= min_x && pos.x - radius <= max_x) ||
				(pos.y + radius >= min_y && pos.y - radius <= max_y))
			{
				if(bomb_list_.count(body) > 0)
				{
					explode(body);
				}

				explode_list_.insert(body);
			}
		}
	}
}

void box2d_test_t::explode_preview(b2Body* bomb)
{
	int level = bomb_list_[bomb];

	b2Vec2 target_pos = bomb->GetPosition();

	explode_preview_list_.insert(bomb);

	for(const auto& body : bodies_)
	{
		if(explode_preview_list_.count(body) > 0)
		{
			continue;
		}

		b2Vec2 pos = body->GetPosition();
		float radius = body->GetFixtureList()[0].GetShape()->m_radius;

		if(1 == level)
		{
			float dist = bomb_1_radius_ + radius - 2;

			if(b2DistanceSquared(target_pos, pos) <= dist * dist)
			{
				if(bomb_list_.count(body) > 0)
				{
					explode_preview(body);
				}

				explode_preview_list_.insert(body);
			}
		}
		else if(2 == level)
		{
			float min_x, max_x, min_y, max_y;

			min_x = target_pos.x - bomb_2_size_ + 2;
			max_x = target_pos.x + bomb_2_size_ - 2;
			min_y = target_pos.y - bomb_2_size_ + 2;
			max_y = target_pos.y + bomb_2_size_ - 2;

			if((pos.x + radius >= min_x && pos.x - radius <= max_x) ||
				(pos.y + radius >= min_y && pos.y - radius <= max_y))
			{
				if(bomb_list_.count(body) > 0)
				{
					explode_preview(body);
				}
				else
				{
					explode_preview_list_.insert(body);
				}
			}
		}
	}
}

void box2d_test_t::auto_explode()
{
	for(const auto& [body, level] : bomb_list_)
	{
		explode(body);
	}

	bomb_list_.clear();
}

void box2d_test_t::remove(const std::vector<int>& remove_list, bool enable_explode)
{
	int index = 0;

	for(std::vector<b2Body*>::const_iterator it = bodies_.begin(); it != bodies_.end();)
	{
		if(std::find(remove_list.begin(), remove_list.end(), index) != remove_list.end())
		{
			auto body = *it;

			if(enable_explode && bomb_list_.count(body) > 0)
			{
				explode(body);
				++it;
			}
			else
			{
				colors_.erase(body);
				bomb_list_.erase(body);
				world_.DestroyBody(body);

				it = bodies_.erase(it);
			}
		}
		else
		{
			++it;
		}

		++index;
	}
}

void box2d_test_t::remove(const std::vector<b2Body*> remove_list, bool enable_explode)
{
	std::vector<int> index_list;

	for(size_t i = 0; i < bodies_.size(); ++i)
	{
		auto body = bodies_[i];

		if(std::find(remove_list.begin(), remove_list.end(), body) != remove_list.end())
		{
			index_list.push_back(i);
		}
	}

	remove(index_list, enable_explode);
}

void box2d_test_t::remove(const std::unordered_set<b2Body*> remove_list, bool enable_explode)
{
	std::vector<int> index_list;

	for(size_t i = 0; i < bodies_.size(); ++i)
	{
		auto body = bodies_[i];

		if(std::find(remove_list.begin(), remove_list.end(), body) != remove_list.end())
		{
			index_list.push_back(i);
		}
	}

	remove(index_list, enable_explode);
}
