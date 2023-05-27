#include "Game.h"
#include "GameMode3.h"
#include <vector>

// 게임 창 크기
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int CH_X = 360;
const int CH_Y = 450;
const int CH_WIDTH = 120;
const int CH_HEIGHT = 144;

// 돌 크기
const int STONE_WIDTH = 64;
const int STONE_HEIGHT = 64;

// 돌 이미지 출력 개수
const int NUM_STONES = 5;

int currentCharacterIndex; // 현재 캐릭터 인덱스

// 돌 이미지 위치
const int STONE_X = 370; // 돌의 X 좌표
const int STONE_Y = 350; // 돌의 Y 좌표
const int STONE_SPACING = 110; // 돌 사이의 간격
Uint32 startTime = 0;

// 랜덤 숫자 생성기 초기화
std::default_random_engine randomEngine(static_cast<unsigned int>(time(0)));
std::uniform_int_distribution<int> randomDistribution(1, 4);

// 현재 돌의 숫자
int currentNumber = randomDistribution(randomEngine); // 랜덤 값으로 초기화

// 이미지 텍스처 로드
SDL_Texture* stoneTextures[4];

std::vector<int> stoneNumbers; // 숫자 저장용 배열
int stonesOnScreen; // 화면에 출력되는 돌의 개수

bool correct_button;

// 원숭이 위치
int MONKEY_START_Y;
const int monkeySpeed = 5; // 원숭이의 속도, 클라이밍 속도에 따라 조절 가능
int monkeyY = MONKEY_START_Y;
int velocityY = 0;


// 플레이어가 돌을 누른 시간 기록
Uint32 lastStonePressTime = 0;


Mode3::Mode3() {

    MONKEY_START_Y = 500;
    

    currentCharacterIndex = 0;
    correct_button = false;
    tutorial = true;
    ready = false;
    game_start = false;
    game_over = false;

    

    // 돌 숫자 초기화
    stoneNumbers.resize(NUM_STONES);
    for (int i = 0; i < NUM_STONES; ++i) {
        stoneNumbers[i] = randomDistribution(randomEngine);
    }
    {
        for (int i = 0; i < 4; ++i) {
            std::string imagePath = "../../Resources/stone_" + std::to_string(i + 1) + ".png";
            SDL_Surface* surface = IMG_Load(imagePath.c_str());
            stoneTextures[i] = SDL_CreateTextureFromSurface(g_renderer, surface);
            SDL_FreeSurface(surface);
        }
    }

    //캐릭터
    {
        for (int i = 0; i < 2; ++i) {
            std::string imagePath = "../../Resources/User" + std::to_string(i + 1) + ".png";
            SDL_Surface* surface = IMG_Load(imagePath.c_str());
            chTextures[i] = SDL_CreateTextureFromSurface(g_renderer, surface);
            SDL_FreeSurface(surface);
        }
    }


    //Tutorial
    {
        SDL_Surface* bg_surface = IMG_Load("../../Resources/tt_3.png");
        g_tt_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
        SDL_FreeSurface(bg_surface);

        g_tt_source_rect.x = 0;
        g_tt_source_rect.y = 0;
        g_tt_source_rect.w = 800;
        g_tt_source_rect.h = 600;

        g_tt_destination_rect.x = 100;
        g_tt_destination_rect.y = 30;
        g_tt_destination_rect.w = 650;
        g_tt_destination_rect.h = 450;
    }

    // monkey
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/monkey.png");
        texture_monkey = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_monkey, NULL, NULL, &source_rectangle_monkey.w, &source_rectangle_monkey.h);
        source_rectangle_monkey.x = 0;
        source_rectangle_monkey.y = 0;

        destination_rectangle_monkey.w = 120;
        destination_rectangle_monkey.h = 144;
        destination_rectangle_monkey.x = 430;
    }

    //BackGround
    {
        SDL_Surface* bg_surface = IMG_Load("../../Resources/Mode3_bg.png");
        g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
        SDL_FreeSurface(bg_surface);

        g_bg_source_rect.x = 0;
        g_bg_source_rect.y = 0;
        g_bg_source_rect.w = 800;
        g_bg_source_rect.h = 600;

        g_bg_destination_rect.x = 0;
        g_bg_destination_rect.y = 0;
        g_bg_destination_rect.w = 800;
        g_bg_destination_rect.h = 600;
    }
    //wall
    {
        SDL_Surface* bg_surface = IMG_Load("../../Resources/wall.png");
        g_w_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
        SDL_FreeSurface(bg_surface);

        g_w_source_rect.x = 0;
        g_w_source_rect.y = 0;
        g_w_source_rect.w = 400;
        g_w_source_rect.h = 600;

        g_w_destination_rect.x = 250;
        g_w_destination_rect.y = 0;
        g_w_destination_rect.w = 300;
        g_w_destination_rect.h = 600;
    }

    //caught 돌
    {
        SDL_Surface* bg_surface = IMG_Load("../../Resources/caught.png");
        g_caught_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
        SDL_FreeSurface(bg_surface);

        g_caught_source_rect.x = 0;
        g_caught_source_rect.y = 0;
        g_caught_source_rect.w = 64;
        g_caught_source_rect.h = 64;

        g_caught_destination_rect.x = 370;
        g_caught_destination_rect.y = 480;
        g_caught_destination_rect.w = 64;
        g_caught_destination_rect.h = 64;
    }


    // Start Button
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/Play_Btn_Up.png");
        texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
        destination_rectangle_.x = 320;
        destination_rectangle_.y = 480;
        source_rectangle_.x = 0;
        source_rectangle_.y = 0;

        destination_rectangle_.w = 160;
        destination_rectangle_.h = 80;
    }


    // 뒤로가기 Button
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/Back_Btn_Up.png");
        texture_bb = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_bb, NULL, NULL, &source_rectangle_bb.w, &source_rectangle_bb.h);

        source_rectangle_bb.x = 0;
        source_rectangle_bb.y = 0;
        source_rectangle_bb.w = 80;
        source_rectangle_bb.h = 73;

        destination_rectangle_bb.x = 20;
        destination_rectangle_bb.y = 30;
        destination_rectangle_bb.w = 73;
        destination_rectangle_bb.h = 66;
    }

    // Ready
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/ready.png");
        texture_ready = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_ready, NULL, NULL, &source_rectangle_ready.w, &source_rectangle_ready.h);

        source_rectangle_ready.x = 0;
        source_rectangle_ready.y = 0;
        source_rectangle_ready.w = 800;
        source_rectangle_ready.h = 600;

        destination_rectangle_ready.x = 30;
        destination_rectangle_ready.y = 30;
        destination_rectangle_ready.w = 700;
        destination_rectangle_ready.h = 500;
    }

}

Mode3::~Mode3() {
    SDL_DestroyTexture(g_tt_texture);
    SDL_DestroyTexture(g_bg_texture);
    SDL_DestroyTexture(g_w_texture);
    SDL_DestroyTexture(texture_);
    SDL_DestroyTexture(texture_bb);
    SDL_DestroyTexture(texture_ready);
}

void Mode3::Update()
{
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - startTime >= 3000) {
        ready = false;
        game_start = true;
    }

    if (game_start) {
        // 중력 적용
        
    }

    if (game_start && !ready) {
        if (stonesOnScreen == 0) {
            // 화면에 돌이 없는 경우 새로운 돌을 추가
            stoneNumbers.push_back(randomDistribution(randomEngine));
            stonesOnScreen = NUM_STONES;
        }
        //velocityY += 1;
        monkeyY -= 1;
        
        // 원숭이 속도 조절
    /*    monkeyY -=0.5;*/
    }
    if (correct_button == true) {
        correct_button = false;
        // Background Update
        g_bg_destination_rect.y += 10;
        if (g_bg_destination_rect.y >= 600) {
            g_bg_destination_rect.y = 0;
        }
        g_w_destination_rect.y += 100;
        if (g_w_destination_rect.y >= 600) {
            g_w_destination_rect.y = 0;
        }
        monkeyY -= velocityY;
        //velocityY -= monkeySpeed;
    }
    //// 원숭이 위치 업데이트
    //monkeyY += monkeySpeed;
    //if (monkeyY <= 0) {
    //    // 원숭이가 화면 위로 올라가면 게임 클리어
    //    game_over = true;
    //}

}

void Mode3::Render() {


    // Background Render
    {
        SDL_Rect tmp_r;
        tmp_r.x = 0;
        tmp_r.y = g_bg_destination_rect.y - 600;
        tmp_r.w = g_bg_destination_rect.w;
        tmp_r.h = g_bg_destination_rect.h;
        SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &tmp_r);
        tmp_r.x = 0;
        tmp_r.y = g_bg_destination_rect.y;
        tmp_r.w = g_bg_destination_rect.w;
        tmp_r.h = g_bg_destination_rect.h;
        SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &tmp_r);
    }
    {
        SDL_Rect tmp_r;
        tmp_r.x = 250;
        tmp_r.y = g_w_destination_rect.y - 600;
        tmp_r.w = g_w_destination_rect.w;
        tmp_r.h = g_w_destination_rect.h;
        SDL_RenderCopy(g_renderer, g_w_texture, &g_w_source_rect, &tmp_r);
        tmp_r.x = 250;
        tmp_r.y = g_w_destination_rect.y;
        tmp_r.w = g_w_destination_rect.w;
        tmp_r.h = g_w_destination_rect.h;
        SDL_RenderCopy(g_renderer, g_w_texture, &g_w_source_rect, &tmp_r);
    }


    if (tutorial) {
        //Tutorial
        SDL_RenderCopy(g_renderer, g_tt_texture, &g_tt_source_rect, &g_tt_destination_rect);
        // Start Button
        SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);
    }

    //돌
    if (game_start && !tutorial) {
        for (int i = 0; i < NUM_STONES; ++i) {
            int stoneNumber = stoneNumbers[i];
            SDL_Rect stoneRect = { STONE_X, STONE_Y - (STONE_SPACING * i), STONE_WIDTH, STONE_HEIGHT };
            SDL_RenderCopy(g_renderer, stoneTextures[stoneNumber - 1], nullptr, &stoneRect);
        }

        //잡은 돌
        SDL_RenderCopy(g_renderer, g_caught_texture, &g_caught_source_rect, &g_caught_destination_rect);

        //캐릭터
         // 캐릭터 출력
        SDL_Rect chRect = { CH_X, CH_Y, CH_WIDTH, CH_HEIGHT };
        SDL_RenderCopy(g_renderer, chTextures[currentCharacterIndex], nullptr, &chRect);

        // monkey
        {
            SDL_Rect tmp_r;
            tmp_r.x = destination_rectangle_monkey.x;
            tmp_r.y = monkeyY;
            tmp_r.w = destination_rectangle_monkey.w;
            tmp_r.h = destination_rectangle_monkey.h;
            SDL_RenderCopy(g_renderer, texture_monkey, &source_rectangle_monkey, &tmp_r);
        }
        
    }

    // Back Button
    SDL_RenderCopy(g_renderer, texture_bb, &source_rectangle_bb, &destination_rectangle_bb);

    if (ready) {
        SDL_RenderCopy(g_renderer, texture_ready, &source_rectangle_ready, &destination_rectangle_ready);
    }

    SDL_RenderPresent(g_renderer);

}

void Mode3::HandleEvents() {


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
                if (mouseX >= destination_rectangle_bb.x && mouseX < destination_rectangle_bb.x + destination_rectangle_bb.w
                    && mouseY >= destination_rectangle_bb.y && mouseY < destination_rectangle_bb.y + destination_rectangle_bb.h
                    && g_current_game_phase == PHASE_MODE3) {
                    g_current_game_phase = PHASE_HOME;
                    ready = false;
                    tutorial = true;
                }
                //game start
                if (mouseX >= destination_rectangle_.x && mouseX < destination_rectangle_.x + destination_rectangle_.w
                    && mouseY >= destination_rectangle_.y && mouseY < destination_rectangle_.y + destination_rectangle_.h
                    && g_current_game_phase == PHASE_MODE3) {
                    ready = true;
                    tutorial = false;
                    startTime = SDL_GetTicks();
                }

            }
            break;

        case SDL_KEYDOWN:
            int key = event.key.keysym.sym - SDLK_0;
            if (key >= 1 && key <= 4) {
                if (!ready && game_start) {
                    if (key == stoneNumbers[0]) {
                        // 올바른 숫자를 입력한 경우 돌들을 아래로 이동
                        stoneNumbers.erase(stoneNumbers.begin());
                        stoneNumbers.push_back(randomDistribution(randomEngine));
                        correct_button = true;
                        // 맞는 돌을 눌렀을 때 캐릭터 이미지 변경
                        currentCharacterIndex = (currentCharacterIndex + 1) % 2;
                        // 플레이어가 돌을 누른 시간 기록
                        lastStonePressTime = SDL_GetTicks();

                        velocityY -= monkeySpeed;
                        //// 원숭이 속도 갱신
                        //int timeInterval = lastStonePressTime - startTime;
                        //if (timeInterval > 0) {
                        //    monkeySpeed += 1+(int)(1000 / timeInterval); // 돌을 누르는 간격이 좁아질수록 속도가 빨라짐
                        //}
                    }
                    else {
                        // 잘못된 숫자를 입력한 경우 게임 오버
                        game_over = true;
                        g_current_game_phase = PHASE_ENDING1;
                    }
                }

            }
            break;



        }
    }
}
