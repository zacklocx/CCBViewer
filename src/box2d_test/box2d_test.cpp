
#include "box2d_test.h"

#include "util.h"
#include "draw.h"
#include "window.h"

box2d_test_t::box2d_test_t()
	: ground_hw_(50.0f), ground_hh_(10.0f)
	, box_hw_(16.0f), box_hh_(16.0f)
	, world_(b2Vec2(0.0f, -10.0f))
{}

void box2d_test_t::init()
{
	int win_width = window_t::width();
	int win_height = window_t::height();

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(win_width * 0.5, 0.0f);

	b2Body* groundBody = world_.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;

	groundBox.SetAsBox(ground_hw_, ground_hh_);

	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;

	for(int i = 0; i < 10; ++i)
	{
		bodyDef.position.Set(rand_real(win_width * 0.1, win_width * 0.9),
			rand_real(win_height * 0.8, win_width));

		b2Body* body = world_.CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(box_hw_, box_hh_);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		body->CreateFixture(&fixtureDef);

		bodys_.push_back(body);
	}

	texture_.load("/Users/zacklocx/tmp/ball.png");
}

void box2d_test_t::render()
{
	int win_width = window_t::width();
	int win_height = window_t::height();

	use_color("red");
	draw_rect(win_width * 0.5, 0.0f, ground_hw_ * 2.0f, ground_hh_ * 2.0f, true);
	use_color("white");

	for(const auto& body : bodys_)
	{
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		use_color("blue");
		draw_circle(position.x, position.y, 0.0f, box_hw_);
		use_color("white");
	}

	// blend_mode(4, 5);
	// texture_.draw(position.x, position.y, box_hw_ * 2.0f, box_hh_ * 2.0f, angle);
	// blend_mode(-1, -1);
}

void box2d_test_t::update()
{
	float32 timeStep = 1.0f / 1.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world_.Step(timeStep, velocityIterations, positionIterations);
}
