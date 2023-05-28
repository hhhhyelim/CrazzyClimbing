#pragma once

#include "Game.h"

class Intro2 : public PhaseInterface {
public:
	Intro2();
	~Intro2();

	virtual void HandleEvents() override; //�������̵�� ������ �ۼ��� ������ �ִ����� �ٽ� �ۼ��ϰڴ�
	virtual void Update() override;
	virtual void Render() override;

private:
	SDL_Texture* texture_intro_;
	SDL_Rect source_rectangle_intro_;
	SDL_Rect destination_rectangle_intro_;
};