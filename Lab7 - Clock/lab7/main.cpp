#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <ctime>
#include <string>

static const std::string BG_IMG_FILE_NAME = "images/bgtexture.png";
static const sf::Color BG_COLOR = sf::Color(89, 84, 79);
static const sf::Color RAW_COLOR = sf::Color(255, 255, 255, 200);
static const sf::Color WINDOW_COLOR = sf::Color(255, 255, 255);
static const sf::Vector2f HOUR_SIZE = sf::Vector2f(80, 14);
static const sf::Vector2f MIN_SIZE = sf::Vector2f(90, 10);
static const sf::Vector2f SEC_SIZE = sf::Vector2f(100, 5);
static const unsigned int NUMBER_OF_DIGITS = 12;
static const float CLOCK_RADIUS = 100.f;

sf::CircleShape * createBackground() {
	sf::CircleShape * bg = new sf::CircleShape;
	bg->setRadius(CLOCK_RADIUS);
	bg->setOrigin(bg->getGlobalBounds().width * 0.5f, bg->getGlobalBounds().height * 0.5f);
	
	bg->setOutlineThickness(10);
	
	sf::Texture * bgTexture = new sf::Texture;
	if (bgTexture->loadFromFile(BG_IMG_FILE_NAME)) {
		bgTexture->setSmooth(true);
		bg->setTexture(bgTexture);
	}
	else {
		delete bgTexture;
		bg->setFillColor(BG_COLOR);
	}
	return bg;
}

sf::RectangleShape * creatRow(sf::Vector2f size) {
	sf::RectangleShape * sec = new sf::RectangleShape(size);
	sec->setOrigin(0, float(sec->getGlobalBounds().height*0.5));
	sec->setFillColor(RAW_COLOR);
	return sec;
}
struct vectorObjects {
	sf::CircleShape * bg;
	sf::RectangleShape * sec;
	sf::RectangleShape * min;
	sf::RectangleShape * hour;
};

struct vectorObjects initVectorObjects() {
	struct vectorObjects result;
	result.bg = createBackground();
	result.hour = creatRow(HOUR_SIZE);
	result.min = creatRow(MIN_SIZE);
	result.sec = creatRow(SEC_SIZE);
	return result;
}

void destroyVectorObjects(struct vectorObjects shapes) {
	delete shapes.bg;
	delete shapes.hour;
	delete shapes.min;
	delete shapes.sec;
}


sf::Text initText(std::string fileName) {
	sf::Font *font = new sf::Font;
	font->loadFromFile(fileName.c_str());
	sf::Text text("", *font, 20);
	text.setColor(sf::Color::White);
	return text;
}

sf::Vector2f polarToCartesianSys(float radius, float alpha) {
	sf::Vector2f result;
	alpha = float(alpha * M_PI / 180);
	result.x = radius * cos(alpha);
	result.y = radius * sin(alpha);
	return result;
}

struct tm getTime() {
	time_t now = time(nullptr);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	return timeinfo;
}

sf::Vector2f getCenter(sf::RenderWindow & window) {
	sf::Vector2f center;
	center.x = window.getSize().x * 0.5f;
	center.y = window.getSize().y * 0.5f;
	return center;
}

void drawNumbers(sf::RenderWindow & window,sf::Text & text, float bgHeight) {
	std::string textNum;
	sf::Vector2f position = getCenter(window);

	for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
		position = polarToCartesianSys(bgHeight * 0.5f - text.getGlobalBounds().height * 2, 30.f * ( i + 1 ) - 90);
		text.setString(std::to_string( i + 1 ));
		text.setOrigin(text.getGlobalBounds().width * 0.5f, text.getGlobalBounds().height * 0.5f);
		text.setPosition(position.x + window.getSize().x * 0.5f, position.y + window.getSize().y * 0.5f);
		window.draw(text);
	}
}

void animationUpdate(sf::Time elapsed, vectorObjects shapes) {
	shapes.sec->rotate(elapsed.asSeconds() * 6);
	shapes.min->rotate(elapsed.asMilliseconds() * 0.0001f);
	shapes.hour->rotate(elapsed.asMilliseconds() * 0.00001f / 6);
}

void resize(sf::RenderWindow & window, struct vectorObjects shapes){

	sf::Vector2f center = getCenter(window);

	shapes.bg->setPosition(center.x, center.y);
	shapes.hour->setPosition(center.x, center.y);
	shapes.min->setPosition(center.x, center.y);
	shapes.sec->setPosition(center.x, center.y);

	sf::View view = sf::View(sf::FloatRect(0, 0, float(window.getSize().x), float(window.getSize().y)));
	window.setView(view);
}

void events(sf::RenderWindow & window, vectorObjects shapes) {
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window.close();
			destroyVectorObjects(shapes);
		}
		if (event.type == sf::Event::Resized) {
			resize(window,shapes);
		}
	}
}

void drawWindows(sf::RenderWindow & window, vectorObjects & shapes, sf::Text & text) {
	window.clear(WINDOW_COLOR);
	window.draw(*shapes.bg);
	drawNumbers(window, text, shapes.bg->getLocalBounds().height);
	window.draw(*shapes.hour);
	window.draw(*shapes.min);
	window.draw(*shapes.sec);
}

void setFirstPosition(vectorObjects shapes) {
	struct tm time = getTime();
	shapes.sec->setRotation(time.tm_sec * 6 - 90);
	shapes.min->setRotation(time.tm_min * 6 - 90);
	shapes.hour->setRotation(time.tm_hour % 12 * 30 - 90);
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Lab7", sf::Style::Default, settings);
	sf::View view = sf::View(sf::FloatRect(0, 0, float(window.getSize().x), float(window.getSize().y)));
	window.setView(view);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;
	
	//init some objects
	struct tm time = getTime();
	struct vectorObjects shapes = initVectorObjects();
	sf::Vector2f center = getCenter(window);
	sf::Text text = initText("fonts/rubik.ttf");
	resize(window, shapes);
	setFirstPosition(shapes);

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		animationUpdate(elapsed, shapes);

		drawWindows(window, shapes, text);

		window.display();
		events(window, shapes);
	}

	return 0;
}