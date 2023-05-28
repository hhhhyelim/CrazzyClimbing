#pragma once

#include "Game.h"

class Mode2 : public PhaseInterface {
public:
	Mode2();
	~Mode2();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
	void userMove();
	void holdMove();
	bool checkHold();
	void ResetGame();

private:
	int cur_i;
	int prevHold;
	/*
	// bg
	SDL_Surface* bg_surface;
	SDL_Texture* bg_texture;
	SDL_Rect bg_rect;
	SDL_Rect bg_dest_rect;

	// wall
	SDL_Surface* wall_surface;
	SDL_Texture* wall_texture;
	SDL_Rect wall_rect;
	SDL_Rect wall_dest_rect;
	*/
};