#include "Game.h"
//#include "GameHome.h"
#include "GameMode2.h"
//#include "GameEnding2.h"


/////////////////////////////////////////////////
// Declaration
SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

// Game Phases
int g_current_game_phase;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    g_window = SDL_CreateWindow("Crazzy Climbing", 400, 200, 800, 600, 0);
    g_renderer = SDL_CreateRenderer(g_window, -1, 0);

    InitGame();

    PhaseInterface* game_phases[8];
    //game_phases[PHASE_HOME] = new Home;
    //game_phases[PHASE_MODE1] = new Mode1;
    game_phases[PHASE_MODE2] = new Mode2;
    //game_phases[PHASE_MODE3] = new Mode3;
    //game_phases[PHASE_ENDING2] = new Ending2;
    //game_phases[PHASE_ENDING3] = new Ending3;

    g_current_game_phase = PHASE_MODE2;

    while (g_flag_running)
    {
        Uint32 cur_time_ms = SDL_GetTicks();

        if (cur_time_ms - g_last_time_ms < 33)
            continue;
            
        game_phases[g_current_game_phase]->HandleEvents();
        game_phases[g_current_game_phase]->Update();
        game_phases[g_current_game_phase]->Render();

        g_last_time_ms = cur_time_ms;
    }

    for (int i = 0; i < 8; i++) {
        delete game_phases[i];
    }

    ClearGame();

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}