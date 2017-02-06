#pragma once
using namespace sf;
using namespace std;

struct Files {
	string *files = new string[0];
	unsigned int name_size = 0;
	unsigned int arr_size = 0;
	string path;
}typedef Files;

struct Picture {
	string title;
	float left;
	float top;
	Texture *texture = new Texture;
	Texture *p_texture = new Texture;
	Texture *n_texture = new Texture;
	bool pr = false;
	bool ne = false;
	bool error = true;
	Sprite *sprite = new Sprite;
	unsigned int num = 0;
}typedef Picture;

struct Load_pic {
	Vector2u window_size;
	Files files; 
	Picture *pic; 
	char diraction;
}typedef Load_pic;

void resize_picture(Vector2u window_size, Picture & picture);