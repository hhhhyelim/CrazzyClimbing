#pragma once

#include "Game.h"

class Intro1 : public PhaseInterface {
public:
	Intro1();
	~Intro1();

	virtual void HandleEvents() override; //오버라이드는 위에서 작성한 내용이 있던없던 다시 작성하겠다
	virtual void Update() override;
	virtual void Render() override;

private:
	SDL_Texture* texture_intro_;
	SDL_Rect source_rectangle_intro_;
	SDL_Rect destination_rectangle_intro_;
};