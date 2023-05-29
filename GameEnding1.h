#pragma once

#include "SDL.h"

class Ending1 : public PhaseInterface
{
public:
	Ending1();
	~Ending1();
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

private:

};

