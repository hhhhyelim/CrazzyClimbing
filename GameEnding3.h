#pragma once

#include "SDL.h"

class Ending3 : public PhaseInterface
{
public:
	Ending3();
	~Ending3();
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

private:
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

};

