#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;
using namespace std;

Vector2f WINDOW_SIZE = { 980, 240 };
Vector2f LEFT_WHEEL = { 162, 62 };
Vector2f RIGHT_WHEEL = { 43, 62 };
const int RADIUS_WHEEL = 16;
const float CIRCUMFERENCE = RADIUS_WHEEL * 2 * M_PI;

struct CarSprites {
	Sprite body_car, lef_wheel, right_wheel;
	float rotation = 0, speed = 0, acceleration = 0.0005;
	Vector2f coord_car = { 0, 110 };

	void set_sprites_origin() {
		lef_wheel.setOrigin(RADIUS_WHEEL, RADIUS_WHEEL);
		right_wheel.setOrigin(RADIUS_WHEEL, RADIUS_WHEEL);
	}

	void set_position_car() {
		lef_wheel.setRotation(rotation);
		right_wheel.setRotation(rotation);
		body_car.setPosition(coord_car);
		lef_wheel.setPosition(coord_car.x + LEFT_WHEEL.x, coord_car.y + LEFT_WHEEL.y);
		right_wheel.setPosition(coord_car.x + RIGHT_WHEEL.x, coord_car.y + RIGHT_WHEEL.y);
	}

	void calculation_speed_and_rotation(Event event, float time) {
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) {
			speed += acceleration * time;
		}
		else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) {
			speed -= acceleration * time;
		}
		else {
			if (speed > 0)
				speed -= (acceleration * time) / 2;
			if (speed < 0)
				speed += (acceleration * time) / 2;
		}
		coord_car.x += speed;
		rotation = coord_car.x * 360 / CIRCUMFERENCE;
		set_position_car();
	}
};

void init_asphalt(RectangleShape &asphalt) {
	asphalt.setSize(Vector2f(WINDOW_SIZE.x, WINDOW_SIZE.y - LEFT_WHEEL.y - 240));
	asphalt.setPosition(0, 250);
	asphalt.setFillColor(Color::White);
}

void start_move_time(RenderWindow &window, CarSprites &car) {
	Clock clock;
	RectangleShape asphalt;
	init_asphalt(asphalt);
	while (window.isOpen()) {
		Event event;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 700;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		car.calculation_speed_and_rotation(event, time);
		window.clear();
		window.draw(asphalt);
		window.draw(car.lef_wheel);
		window.draw(car.right_wheel);
		window.draw(car.body_car);
		window.display();
	}
}

int main() {
	Texture body_car_image, wheel_image;
	body_car_image.loadFromFile("images/car.png");
	wheel_image.loadFromFile("images/wheel.png");
	CarSprites car;
	car.body_car.setTexture(body_car_image);
	car.lef_wheel.setTexture(wheel_image);
	car.right_wheel.setTexture(wheel_image);
	car.set_sprites_origin();
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "lab_8.1", Style::Default, settings);
	start_move_time(window, car);
	return 0;
}