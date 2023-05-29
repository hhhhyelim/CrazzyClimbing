#include "Game.h"
#include "GameEnding3.h"
#include "GameMode3.h"

//int result = Mode3::m3_result; // 비정적 멤버 변수 값 가져오기





Ending3::Ending3() {

    //Mode3::m3_result = 10; // 비정적 멤버 변수에 값 할당

    
    // Ready
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/finish.png");
        texture_finish = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_finish, NULL, NULL, &source_rectangle_finish.w, &source_rectangle_finish.h);

        source_rectangle_finish.x = 0;
        source_rectangle_finish.y = 0;
        source_rectangle_finish.w = 800;
        source_rectangle_finish.h = 600;

        destination_rectangle_finish.x = 30;
        destination_rectangle_finish.y = 30;
        destination_rectangle_finish.w = 700;
        destination_rectangle_finish.h = 500;
    }

    // Ready
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/m3_gameover.png");
        texture_gameover = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_gameover, NULL, NULL, &source_rectangle_gameover.w, &source_rectangle_gameover.h);

        source_rectangle_gameover.x = 0;
        source_rectangle_gameover.y = 0;
        source_rectangle_gameover.w = 800;
        source_rectangle_gameover.h = 600;

        destination_rectangle_gameover.x = 30;
        destination_rectangle_gameover.y = 30;
        destination_rectangle_gameover.w = 700;
        destination_rectangle_gameover.h = 500;
    }

    // Home Button
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/home_button.png");
        texture_hb = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_hb, NULL, NULL, &source_rectangle_hb.w, &source_rectangle_hb.h);
        destination_rectangle_hb.x = 210;
        destination_rectangle_hb.y = 480;
        source_rectangle_hb.x = 0;
        source_rectangle_hb.y = 0;

        destination_rectangle_hb.w = 160;
        destination_rectangle_hb.h = 80;
    }

    // Retry Button
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/retry_button.png");
        texture_rb = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_rb, NULL, NULL, &source_rectangle_rb.w, &source_rectangle_rb.h);
        destination_rectangle_rb.x = 400;
        destination_rectangle_rb.y = 480;
        source_rectangle_rb.x = 0;
        source_rectangle_rb.y = 0;

        destination_rectangle_rb.w = 160;
        destination_rectangle_rb.h = 80;
    }
}

Ending3::~Ending3() {
    SDL_DestroyTexture(texture_finish);
    SDL_DestroyTexture(texture_gameover);
    SDL_DestroyTexture(texture_hb);
    SDL_DestroyTexture(texture_rb);
}

void Ending3::Update()
{

}

void Ending3::Render() {

  /*  if (result == 0) {
        SDL_RenderCopy(g_renderer, texture_finish, &source_rectangle_finish, &destination_rectangle_finish);
        
    }
    else {
        SDL_RenderCopy(g_renderer, texture_gameover, &source_rectangle_gameover, &destination_rectangle_gameover);
    }*/
    // Home Button
    SDL_RenderCopy(g_renderer, texture_hb, &source_rectangle_hb, &destination_rectangle_hb);
    // Retry Button
    SDL_RenderCopy(g_renderer, texture_rb, &source_rectangle_rb, &destination_rectangle_rb);
    SDL_RenderCopy(g_renderer, texture_finish, &source_rectangle_finish, &destination_rectangle_finish);
    SDL_RenderPresent(g_renderer);
}

void Ending3::HandleEvents() {

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            g_flag_running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:

            // If the mouse left button is pressed. 
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                //뒤로가기(home으로)
                if (mouseX >= destination_rectangle_hb.x && mouseX < destination_rectangle_hb.x + destination_rectangle_hb.w
                    && mouseY >= destination_rectangle_hb.y && mouseY < destination_rectangle_hb.y + destination_rectangle_hb.h
                    && g_current_game_phase == PHASE_ENDING3) {
                    g_current_game_phase = PHASE_HOME;

                }
                //game start
                if (mouseX >= destination_rectangle_rb.x && mouseX < destination_rectangle_rb.x + destination_rectangle_rb.w
                    && mouseY >= destination_rectangle_rb.y && mouseY < destination_rectangle_rb.y + destination_rectangle_rb.h
                    && g_current_game_phase == PHASE_ENDING3) {
                    g_current_game_phase = PHASE_MODE3;
                }

            }
            break;

        }
    }
}