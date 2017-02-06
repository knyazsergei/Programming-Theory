#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <iostream>
#include <sstream>
#include "main.h"


float get_file_size(string path) {
	LPCTSTR file = path.c_str();
	// Размер файла
	DWORD file_size = 0;
	// Открываем файл
	HANDLE h_File = CreateFile(file,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (h_File == INVALID_HANDLE_VALUE)
		cout << "Не удалось открыть файл";
	// Получаем размер файла
	file_size = GetFileSize(h_File, NULL);
	if (file_size == INVALID_FILE_SIZE)
		cout << "Не удалось определить размер файла";
	// Закрываем файл
	CloseHandle(h_File);
	// Выводим результат
	return file_size / 1024.0 / 1024.0;
}

bool is_image(string file_name) {

	if (!strrchr(file_name.c_str(), '.')) {
		return false;
	}
	string ext_array[] = { "jpg","jpeg","png","gif","bmp" };
	int i;
	for (i = 0;i < 5;i++)
		if (file_name.substr(file_name.find_last_of(".") + 1) == ext_array[i])
			return true;
	return false;
}
Files get_file_list(string & old_path) {
	string path = old_path + string("*");
	Files files;
	files.path = old_path;
	unsigned long i = 0;

	WIN32_FIND_DATA file_data;
	HANDLE h_file = FindFirstFile(path.c_str(), &file_data);//поиск первого файла
	if (h_file != INVALID_HANDLE_VALUE) {//Если файл существует
		do {
			if (!(file_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {//если файл не папка
				files.arr_size++;
				if (strlen(file_data.cFileName) > files.name_size)
					files.name_size = strlen(file_data.cFileName);
			}

		} while (FindNextFile(h_file, &file_data));

		files.files = new string[files.arr_size];
		i = 0;
		h_file = FindFirstFile(path.c_str(), &file_data);
		do {
			if (!(file_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				if (is_image(file_data.cFileName)) {
					files.files[i] = file_data.cFileName;
					i++;
				}
			}

		} while (FindNextFile(h_file, &file_data));
		FindClose(h_file);
	}
	files.arr_size = i;
	return files;
	//delete[] files;
}

void init_picture(Vector2u window_size, Files files, Picture *pic, char diraction) {
	if (!(pic->num < 0)) {
		if (!pic->error) {
			delete(pic->sprite);
		}
		Image *image = new Image;
		string path = files.path + files.files[pic->num];
		if (get_file_size(path) > 10) {
			std::cout << "error (too big size) with: \n" << files.path + files.files[pic->num] << '\n';
			pic->error = true;
		}
		if (!(image->loadFromFile(files.path + files.files[pic->num]))) {
			std::cout << "error with: \n" << files.path + files.files[pic->num] << '\n';
			pic->error = true;
		}else if (image->getSize().x > 16000 || image->getSize().y > 16000) {
			pic->error = true;
		}
		else {
			pic->error = false;
		}
		diraction = 0;
		if (!pic->error) {
			if (diraction == 1) {
				pic->n_texture = pic->texture;
				pic->ne = true;
			}
			else if (diraction == 2) {
				pic->p_texture = pic->texture;
				pic->pr = true;
			}
		}
		if (diraction == 1 && pic->pr) {
			pic->texture = pic->p_texture;
			pic->pr = false;
		}
		else if (diraction == 2 && pic->ne) {
			pic->texture = pic->n_texture;
			pic->ne = false;
		}
		else {
			delete(pic->texture);
			pic->texture = new Texture;
			pic->texture->loadFromImage(*image);
			delete(image);
		}
		//Vector2u size = pic->texture->getSize();
		pic->sprite = new Sprite; 
		pic->sprite->setPosition(0, 0);
		pic->sprite->setTexture(*(pic->texture));
		pic->sprite->setOrigin(pic->texture->getSize().x / 2.0, pic->texture->getSize().y / 2.0);
		//cout << pic->texture->getSize().x << "/ 2 = " << pic->texture->getSize().x / 2.0 << '\n';
		pic->title = string(files.files[pic->num]);
		resize_picture(window_size, *pic);
	}
}

void n_init_picture(Load_pic* lpd) {
	Picture *pic = lpd->pic;
	Files files = lpd->files;
	Vector2u window_size = lpd->window_size;
	char diraction = lpd->diraction;
	if (!(pic->num < 0)) {
		if (!pic->error) {
			delete(pic->sprite);
		}
		Image *image = new Image;
		string path = files.path + files.files[pic->num];
		if (get_file_size(path) > 10) {
			std::cout << "error (too big size) with: \n" << files.path + files.files[pic->num] << '\n';
			pic->error = true;
			//return false;
		}
		if (!(image->loadFromFile(files.path + files.files[pic->num]))) {
			std::cout << "error with: \n" << files.path + files.files[pic->num] << '\n';
			pic->error = true;
			//return false;
		}
		if (image->getSize().x > 16000 || image->getSize().y > 16000) {
			pic->error = true;
			//return false;
		}
		diraction = 0;
		if (!pic->error) {
			if (diraction == 1) {
				pic->n_texture = pic->texture;
				pic->ne = true;
			}
			else if (diraction == 2) {
				pic->p_texture = pic->texture;
				pic->pr = true;
			}
		}
		if (diraction == 1 && pic->pr) {
			pic->texture = pic->p_texture;
			pic->pr = false;
		}
		else if (diraction == 2 && pic->ne) {
			pic->texture = pic->n_texture;
			pic->ne = false;
		}
		else {
			delete(pic->texture);
			pic->texture = new Texture;
			pic->texture->loadFromImage(*image);
			delete(image);
		}
		//Vector2u size = pic->texture->getSize();
		pic->sprite = new Sprite;
		pic->sprite->setPosition(0, 0);
		pic->sprite->setTexture(*(pic->texture));
		pic->sprite->setOrigin(pic->texture->getSize().x / 2.0, pic->texture->getSize().y / 2.0);
		//cout << pic->texture->getSize().x << "/ 2 = " << pic->texture->getSize().x / 2.0 << '\n';
		pic->title = string(files.files[pic->num]);
		resize_picture(window_size, *pic);
	}
}

void resize_picture(Vector2u window_size, Picture & picture) {
	float scale;
	float old_scale = picture.sprite->getScale().x;
	if (picture.texture->getSize().x * old_scale <= window_size.x || picture.texture->getSize().y * old_scale) {
		if (picture.texture->getSize().x * old_scale - window_size.x < picture.texture->getSize().y * old_scale - window_size.y) {
			scale = float(window_size.y) / picture.texture->getSize().y;
		}
		else {
			scale = float(window_size.x) / picture.texture->getSize().x;
		}
		if (scale > 1)
			scale = 1;
		picture.sprite->setScale(Vector2f(scale, scale));
	}
	else {
		scale = 1;
		picture.sprite->setScale(Vector2f(scale, scale));
	}

	//центрирование изображения
	picture.left = float(window_size.x) / 2.0;
	picture.top = float(window_size.y) / 2.0;
	picture.sprite->setPosition(Vector2f(picture.left, picture.top));
	//сглаживание
	picture.texture->setSmooth(true);
	picture.texture->setRepeated(false);
}

void tr_test(int *kran) {
	*kran =10;
}

int main()
{	
	RenderWindow window(VideoMode(800,600), "Loading...");
	window.setVerticalSyncEnabled(true);
	Vector2u window_size = window.getSize();
	Clock clock;
	bool loading = false;
	string path = "C:\\lab4\\";//Адрес папки
	cout << "enter address like C:\\\\lab4\\\\: \n";
	cin >> path;

	//Открытие и создание изображения
	Files files = get_file_list(path);
	Picture picture;
	picture.texture->loadFromFile("./img/transparent.png");
	init_picture(window_size, files, &picture, 0);

	//Кнопки управления
	Texture left_t, right_t;
	left_t.loadFromFile("./img/left.png");
	right_t.loadFromFile("./img/right.png");

	Sprite left, right;
	left.setTexture(left_t);
	right.setTexture(right_t);

	//Иконка приложения
	Image icon;
	icon.loadFromFile("./img/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());

	//Размер файла
	RectangleShape file_size_bg(Vector2f(100, 20));
	file_size_bg.setFillColor(Color(100, 250, 50));
	file_size_bg.setPosition(Vector2f(10, 10));

	//Сообщени об ошибке
	Font font;
	font.loadFromFile("./img/monoid.ttf");
	Text error("ERROR", font, 60);
	error.setColor(Color(255, 0, 0));
	error.setOrigin(strlen("ERROR") / 2 * 60, 10);

	Load_pic lpd;
	Vector2u size;
	bool fullsize = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			Vector2i localPosition = Mouse::getPosition(window);
			Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
			Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

			if (event.type == sf::Event::Closed) {
				delete[] files.files;
				window.close();
			}

			if (event.type == Event::MouseButtonReleased) {
				if (event.key.code == Mouse::Left) {
					if (left.getGlobalBounds().contains(pos.x, pos.y)) {
						if (picture.num == 0) {
							picture.num = files.arr_size;
						}
						picture.num--;
						init_picture(window_size, files, &picture, 1);
					}
					else if (right.getGlobalBounds().contains(pos.x, pos.y)) {
						if (picture.num + 1== files.arr_size) {
							picture.num = 0;
						}else {
							picture.num++;
						}
						init_picture(window_size, files, &picture, 2);
					}
				}
			}

			if (Keyboard::isKeyPressed(sf::Keyboard::F11)) {
				if (!fullsize) {
					window.create(sf::VideoMode(800, 600, 32), "Loading...", sf::Style::Fullscreen);
					fullsize = true;
				}
				else {
					window.create(sf::VideoMode(800, 600, 32), "Loading...", sf::Style::Default);
					fullsize = false;
				}
			}

			if (Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				if (fullsize) {
					window.create(sf::VideoMode(800, 600, 32), "Loading...", sf::Style::Default);
					fullsize = false;
				}
				else {
					delete[] files.files;
					window.close();
				}
			}

			if (Keyboard::isKeyPressed(sf::Keyboard::Left)){
				if (picture.num == 0) {
					picture.num = files.arr_size;
				}
				picture.num--;
				init_picture(window_size, files, &picture,1);
			}
			if (Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (picture.num + 1 == files.arr_size) {
					picture.num = 0;
				}
				else {
					picture.num++;
				}
				/*
				lpd.diraction = 2;
				lpd.pic = &picture;
				lpd.files = files;
				//Thread load_pic(&init_picture, &lpd);
				int kran = 6;
				Thread tr_testa(&tr_test, &kran);
				tr_testa.launch();
				tr_testa.terminate();
				cout << kran;
				*/
				init_picture(window_size, files, &picture,2);
			}

			if (event.type == Event::MouseMoved) {
				if (left.getGlobalBounds().contains(pos.x, pos.y)) {
					left.setColor(Color(255, 255, 255, 100));
				}
				else {
					left.setColor(Color(255, 255, 255, 255));
				}
				if (right.getGlobalBounds().contains(pos.x, pos.y)) {
					right.setColor(Color(255, 255, 255, 100));
				}
				else {
					right.setColor(Color(255, 255, 255, 255));
				}
			}
			if (event.type == Event::Resized){
				window_size.x = event.size.width;
				window_size.y = event.size.height;
				resize_picture(window_size, picture);
				//установка камеры
				window.setView(sf::View(sf::FloatRect(0, 0, window_size.x, window_size.y)));
			}
			if (event.type == Event::MouseWheelMoved) {
				if (event.mouseWheel.delta == 1) {
					if (picture.num + 1 == files.arr_size) {
						picture.num = 0;
					}
					else {
						picture.num++;
					}
					init_picture(window_size, files, &picture,1);
				}
				else if (event.mouseWheel.delta == -1) {
					if (picture.num == 0) {
						picture.num = files.arr_size;
					}
					picture.num--;
					init_picture(window_size, files, &picture,2);
				}
			}
		}

		//установка размера файла
		file_size_bg.setSize(Vector2f(window_size.x, 10));
		file_size_bg.setPosition(Vector2f(0, window_size.y - 10));

		//Установка заголовка
		window.setTitle(picture.title);

		//отрисовка изображения
		picture.sprite->setTexture((*picture.texture));

		//Установка кнопок
		size = left_t.getSize();
		left.setPosition(Vector2f(0, (window_size.y - size.y) / 2));
		size = right_t.getSize();
		right.setPosition(Vector2f(window_size.x - size.x,(window_size.y - size.y) / 2));

		window.clear(Color(255, 255, 255));
		if (picture.error) {
			error.setPosition(window_size.x / 2.0, window_size.y / 2.0);
			window.draw(error);
		}
		else {
			window.draw((*picture.sprite));
		}
		window.draw(left);
		window.draw(right);
		if (loading == true) {
			float time = clock.getElapsedTime().asMicroseconds();
		}
		window.draw(file_size_bg);
		window.display();
		window_size = window.getSize();
	}

	return 0;
}