#pragma once

#include "SDL.h"
#include <random>

// 게임 상태
enum class GameState {
	Start, // 시작 상태
	Playing, // 게임 플레이 중 상태
	GameOver // 게임 오버 상태
};

class Mode3 : public PhaseInterface
{
public:
	Mode3();
	~Mode3();
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
	SDL_Texture* chTextures[2];

	//BackGround
	SDL_Rect g_bg_source_rect;
	SDL_Rect g_bg_destination_rect;
	SDL_Texture* g_bg_texture;

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

	bool tutorial;
	bool ready;
	bool game_start;
	bool game_over;
	
	int currentCharacterIndex;
	bool correct_button;



};

