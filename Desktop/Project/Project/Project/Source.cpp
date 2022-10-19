#include <SFML/Graphics.hpp>
#include <math.h>
#include <ctime>


#define M_PI 3.14159265358979323846
using namespace sf;

int width = 800, height = 800;
RenderWindow window(VideoMode(width, height), "SFMLworks");

struct myVector {
	float x;
	float y;

	float distance() {
		return sqrt(x * x + y * y);
	}

	Vector2f normalized() {
		if (distance() > 0 ) return Vector2f(x/ distance(), y/ distance());
		else return Vector2f(0, 0);
	}
};
struct myClock {
	Clock clock;
	float time = 0;

	void Update() {
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 3000;
	}
};
myVector Controller() {

	myVector input;

	if      (Keyboard::isKeyPressed(Keyboard::Up))		input.y = -1.0;
	else if (Keyboard::isKeyPressed(Keyboard::Down))	input.y = 1.0;
	else												input.y = 0.0;

	if      (Keyboard::isKeyPressed(Keyboard::Left))	input.x = -1.0;
	else if (Keyboard::isKeyPressed(Keyboard::Right))	input.x = 1.0;
	else												input.x = 0;

	//if (input.distance() > 0) {
		//printf("%f %f\n", input.x, input.y);
		//printf("%f %f\n", input.normalized().x, input.normalized().y);
	//}
	return input;
}

class Character{
private:
	bool lookRight = true;
	float angle = 0;
	Vector2f position;
	Vector2f size;

	Image image;
	Texture texture;
	Sprite sprite;
	myClock clock;
	
public:
	void init() {
		position.x = (float)width / 2;
		position.y = (float)height / 2;
		image.loadFromFile("../Image/draw.png");
		
		size.x = image.getSize().x;
		size.y = image.getSize().y;

		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(position.x - size.x/2, position.y - size.y/2);
	}
	void Draw() {
		window.draw(sprite);
	}

	void Update() {
		angle += (M_PI / 360) * clock.time;
		clock.Update();

		float R = 30;
		Vector2f newPosition;

		if (Controller().distance() > 0) {
			position.x += Controller().normalized().x * clock.time;
			position.y += Controller().normalized().y * clock.time;

		}

		if ( (Controller().x > 0 && !lookRight) || (Controller().x < 0 && lookRight)){
			sprite.scale(-1, 1); 
			lookRight = !lookRight;
		}


		/*	newPosition.x = position.x - size.x / 2 + R * sin(angle);
			newPosition.y = position.y - size.y / 2 + R * cos(angle);*/
			newPosition.x = position.x - size.x / 2 + R * sin(angle);
			newPosition.y = position.y - size.y / 2 + R * cos(angle);
		
		sprite.setPosition(newPosition.x, newPosition.y);
		Draw();
	}

protected:
	
};

class Star {
private:
	Vector2f position;
	Vector2f direction;
	Vector2f size;
	float scale;
	float rotate;

	Image image;
	Texture texture;
	Sprite sprite;

public: 
	void init() {
		scale = (rand() % 10 + 1);
		scale /= 10;
		rotate = rand() % 180 + 1;
		position.x = rand() % width - 0;
		position.y = rand() % height - 0;

		image.loadFromFile("../Image/star.png");
	
		size.x = image.getSize().x;
		size.y = image.getSize().y;

		texture.loadFromImage(image);
	
		sprite.setTexture(texture);
		sprite.setPosition(position.x - size.x / 2, position.y - size.y / 2);
		sprite.scale(scale, scale);

		sprite.rotate(rotate);
	}
	void Draw() {
		window.draw(sprite);
	}
	void Update() {
		sprite.setPosition(position.x - size.x / 2, position.y - size.y / 2);
		Draw();
	}
};

class Meteorit{
private:
	myClock clock;
	Vector2f position;
	Vector2f direction;
	Vector2f size;
	float scale;
	float rotate;
	float speed;
	Image image;
	Texture texture;
	Sprite sprite;

public:
	void init() {

		position.x = rand() % width + 20;
		position.y = rand() % height + 20;

		Random();

		image.loadFromFile("../Image/meteorit.png");

		size.x = image.getSize().x;
		size.y = image.getSize().y;

		texture.loadFromImage(image);

		sprite.setTexture(texture);
		sprite.setPosition(position.x - size.x / 2, position.y - size.y / 2);
		sprite.scale(scale, scale);

		sprite.rotate(rotate);
	}
	void Draw() {
		window.draw(sprite);
	}
	void Random() {
		position.x = width + width / 2;
		speed = rand() % 10 + 1;
		speed /= 10;

		scale = (rand() % 8 + 3);
		scale /= 10;
		rotate = rand() % 180 + 1;

		direction.x = -1;
		direction.y = rand() % 50 + (-50);
		//direction.x /= 100;
		direction.y /= 100;
	}
	void Update() {
		clock.Update();


		position.x += direction.x * speed * clock.time;
		position.y += direction.y * speed * clock.time;

		if (position.x < 0) {
			
			Random();
			position.y = rand() % height + 20;
		}

		sprite.setPosition(position.x - size.x / 2, position.y - size.y / 2);
		
		Draw();
	}
};

int main(){

	Character character;
	character.init();

	srand(time(0));

	const int starsAmount = 15;
	Star star[starsAmount];
	for(int i = 0; i < starsAmount; i++)
		star[i].init();

	const int meteoritAmount = 5;
	Meteorit meteorit[meteoritAmount];
	for (int i = 0; i < meteoritAmount; i++)
		meteorit[i].init();

	while (window.isOpen()){


		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed)
				window.close();
		}


		//window.clear();
		sf::RectangleShape rectangle(sf::Vector2f(0, 0));
		rectangle.setSize(sf::Vector2f(width, height));
		rectangle.setFillColor(sf::Color(1, 12, 39));
		window.draw(rectangle);

		for (int i = 0; i < starsAmount; i++)
			star[i].Update();

		for (int i = 0; i < meteoritAmount; i++)
			meteorit[i].Update();

		character.Update();
		window.display();

	}

	return 0;

}