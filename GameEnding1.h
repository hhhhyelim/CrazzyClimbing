#pragma once

#include "Game.h"

class Ending1 : public PhaseInterface
{
public:
	Ending1(); //Init_Ending()
	~Ending1();  //Clear_Ending()

	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

	//void ResetGame();

private:
	//Background
	SDL_Rect g_bg_source_rect;
	SDL_Rect g_bg_destination_rect;
	SDL_Texture* g_bg_texture;

	// ���ӿ���
	SDL_Texture* texture_ending;
	SDL_Rect source_rectangle_ending;
	SDL_Rect destination_rectangle_ending;

	// Ȩ��ư
	SDL_Texture* texture_home;
	SDL_Rect source_rectangle_home;
	SDL_Rect destination_rectangle_home;

	// �ٽý��۹�ư
	SDL_Texture* texture_retry;
	SDL_Rect source_rectangle_retry;
	SDL_Rect destination_rectangle_retry;

};