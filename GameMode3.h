#pragma once

#include "SDL.h"
#include <random>



class Mode3 : public PhaseInterface
{
public:
	Mode3();
	~Mode3();
	//bool CheckCollision(int a);
	//int get_monkeyY_value();
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();


private:
	//Tutorial
	SDL_Rect g_tt_source_rect;
	SDL_Rect g_tt_destination_rect;
	SDL_Texture* g_tt_texture;

	//stone
	SDL_Rect g_stone_source_rect;
	SDL_Rect g_stone_destination_rect;
	SDL_Texture* stoneTextures[4];

	//캐릭터
	SDL_Rect g_ch_source_rect;
	SDL_Rect g_ch_destination_rect;
	

	//BackGround

// 배경 이미지 크기
	const int BACKGROUND_WIDTH = 800;
	const int BACKGROUND_HEIGHT = 3000;
	// bg
	SDL_Surface* bg_surface;
	SDL_Texture* bg_texture;
	SDL_Rect bg_rect;
	SDL_Rect bg_dest_rect;
	int backgroundY; // 배경 이미지의 Y 좌표

// 게임 창 크기
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	//wall
	SDL_Rect g_w_source_rect;
	SDL_Rect g_w_destination_rect;
	SDL_Texture* g_w_texture;

	//잡은 돌
	SDL_Rect g_caught_source_rect;
	SDL_Rect g_caught_destination_rect;
	SDL_Texture* g_caught_texture;

	// Start Button
	SDL_Texture* texture_;
	SDL_Rect source_rectangle_;
	SDL_Rect destination_rectangle_;

	//monkey
	SDL_Texture* texture_monkey;
	SDL_Rect source_rectangle_monkey;
	SDL_Rect destination_rectangle_monkey;

	// Bakc Button
	SDL_Texture* texture_bb;
	SDL_Rect source_rectangle_bb;
	SDL_Rect destination_rectangle_bb;

	// Ready
	SDL_Texture* texture_ready;
	SDL_Rect source_rectangle_ready;
	SDL_Rect destination_rectangle_ready;

	// Start
	SDL_Texture* texture_start;
	SDL_Rect source_rectangle_start;
	SDL_Rect destination_rectangle_start;
	

	// game_ending
	
	// finish
	SDL_Texture* texture_finish;
	SDL_Rect source_rectangle_finish;
	SDL_Rect destination_rectangle_finish;

	// gameover
	SDL_Texture* texture_gameover;
	SDL_Rect source_rectangle_gameover;
	SDL_Rect destination_rectangle_gameover;

	// Home Button
	SDL_Texture* texture_hb;
	SDL_Rect source_rectangle_hb;
	SDL_Rect destination_rectangle_hb;

	// Retry Button
	SDL_Texture* texture_rb;
	SDL_Rect source_rectangle_rb;
	SDL_Rect destination_rectangle_rb;


	bool tutorial;
	bool ready;
	bool start;
	bool game_start;
	bool game_over;
	bool game_ending;

	int result;

	int currentCharacterIndex;
	bool correct_button;

	// 원숭이 위치
	int MONKEY_START_Y = 150;
	const int monkeySpeed = 3; // 원숭이의 속도, 클라이밍 속도에 따라 조절 가능
	int monkeyY = MONKEY_START_Y;
	int velocityY = 0;

	//캐릭터
	//int CH_X = 360;
	//int CH_WIDTH = 120;
	//int CH_HEIGHT = 144;
	int CH_START_Y = 420;
	int chY = CH_START_Y;
};

