#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string> 

using namespace sf;

struct pair {
	int num1;
	int num2;
};

struct figure {
	char type;
	int id;
	int x;
	int y;
	bool active;
	Texture figure_texture;
	Sprite	figure_sprite;
};

struct game {
	int cell[6][6];//map
	figure items[36];//fiugures
	int active_item;
	int count;
};

void print_array(game game_data) {
	int i, j;
	for (i = 0; i < 36; i++) {
		for (j = 0; j < 36; j++) {
			std::cout << game_data.cell << " ";
		}
		std::cout << "\n";
	}
}

void search_cells(Vector2f & pos) {
	int cell_number;
	int i, j, cell_x, cell_y;
	cell_x = 0;
	cell_y = 0;
	int num1 = 100;
	int num2 = 100;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 6; j++) {
			if ((pos.x >= 100 * cell_x) && (pos.x < 100 * cell_x + 100) && (pos.y >= 100 * cell_y) && (pos.y < 100 * cell_y + 100)) {
				num1 = j;
				num2 = i;
			}
			cell_x++;
			if (cell_x >= 6) {
				cell_y++;
				cell_x = 0;
			}
		}
	}
	pos.x = num1;
	pos.y = num2;
	//std::cout << "x:" << pos.x << "\n y:" << pos.y;
}

figure creat_figure() {
	srand(time(NULL));
	char figure_num = std::rand() % 8;
	//figure_num = 0;
	Texture new_texture;
	new_texture.loadFromFile("images/figure" + std::to_string(figure_num) + ".png");
	new_texture.setSmooth(true);
	new_texture.setRepeated(true);
	Sprite new_sprite;
	new_sprite.setTexture(new_texture);
	new_sprite.setPosition(20, 600);
	new_sprite.setScale(0.3f, 0.3f);
	figure fig;
	fig.figure_texture = new_texture;
	fig.figure_sprite = new_sprite;
	fig.x = 20;
	fig.y = 600;
	fig.type = figure_num;
	return fig;
}

void drag(figure & hero, Vector2f & pos) {
	hero.x = pos.x - 25;
	hero.y = pos.y - 50;
}

void drop(figure & hero, game & game_data, Vector2f & pos) {
	search_cells(pos);
	const int cell_size = 100;
	std::cout << "drop:" << pos.x << pos.y << "\n";
	if (pos.x > 6 || pos.y > 6) {
		hero.x = 20;
		hero.y = 600;
	}
	else {
		//std::cout << 'x' << pos.x << 'y' << pos.y;
		hero.x = pos.x * cell_size + 20;
		hero.y = pos.y * cell_size;
		if (game_data.count < 36) {
			figure hero = creat_figure();
			game_data.count++;
			game_data.items[game_data.count] = hero;
			std::cout << pos.x << "\n" << pos.y;
		}
		else {
			std::cout << "Error:count of items:" << game_data.count;
		}
		int x = pos.x;
		int y = pos.y;
		game_data.cell[x][y] = hero.type;
	}
}

void processEvents(game & game_data, RenderWindow & window, bool & isMove)
{
	Sprite herosprite;
	herosprite = game_data.items[game_data.count].figure_sprite;
	Event event;
	while (window.pollEvent(event)) {
		Vector2i localPosition = Mouse::getPosition(window);
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

		if ((event.type == Event::Closed) || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
			window.close();//Close window

		//drag and drop
		if (!isMove && event.type == Event::MouseButtonPressed) { //start to drag sprite
			if (event.key.code == Mouse::Left) {
				if (game_data.items[game_data.count].figure_sprite.getGlobalBounds().contains(pos.x, pos.y))
					isMove = true;
			}
		}
		else if (isMove && event.type == Event::MouseButtonReleased) {//drop sprite
			if (event.key.code == Mouse::Left) {
				drop(game_data.items[game_data.count], game_data, pos);
				isMove = false;
			}
		}

		if (isMove)
			drag(game_data.items[game_data.count], pos);//Drag sprite
	
	}	
}


int main()
{
	RenderWindow window(VideoMode(600, 700), "My first game");//,Style::Fullscreen);
	//Работа со временем
	Clock clock;
	float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
	clock.restart(); //перезагружает время
	time = time / 800; //скорость игры

	//Инициализация работы курсора
	Vector2i localPosition = Mouse::getPosition(window);

	game game_data;
	//Создание карты
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 6;j++) {
			game_data.cell[i][j] = 0;
		}
	}

	Texture bgtexture;
	bgtexture.loadFromFile("images/background.png");
	bgtexture.setSmooth(true);
	bgtexture.setRepeated(true);

	Sprite BGsprite;
	BGsprite.setTexture(bgtexture);
	BGsprite.setScale(0.48f, 0.48f);
	BGsprite.setPosition(-5, -10);

	//Создание персонажа
	game_data.count = 0;
	game_data.items[game_data.count] = creat_figure();
	//служебные переменные
	bool isMove = false;
	while (window.isOpen())
	{	
		processEvents(game_data, window, isMove);

		window.clear();
		window.draw(BGsprite);
		//Отрисовка спрайтов
		int i;
		for (i = 0;i <= game_data.count;i++) {
			figure hero = game_data.items[i];
			hero.figure_sprite.setPosition(hero.x, hero.y);
			hero.figure_sprite.setTexture(hero.figure_texture);
			window.draw(hero.figure_sprite);
		}
		
		window.display();
	}

	return 0;
}

/*
if (Keyboard::isKeyPressed(Keyboard::Left)) { herosprite.move(-0.5, 0); } //первая координата Х отрицательна =>идём влево
if (Keyboard::isKeyPressed(Keyboard::Right)) { herosprite.move(0.5, 0); } //первая координата Х положительна =>идём вправо
if (Keyboard::isKeyPressed(Keyboard::Up)) { herosprite.move(0, -0.5); } //вторая координата (У) отрицательна =>идём вверх (вспоминаем из предыдущих уроков почему именно вверх, а не вниз)
if (Keyboard::isKeyPressed(Keyboard::Down)) { herosprite.move(0, 0.5); } //вторая координата (У) положительна =>идём вниз (если не понятно почему именно вниз - смотрим предыдущие уроки)
*/
