#include "Game.h"
#include "GameMode3.h"
#include <vector>




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
Uint32 gameoverTime = 0;


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




// 플레이어가 돌을 누른 시간 기록
Uint32 lastStonePressTime = 0;


SDL_Texture* chTextures[2];
SDL_Rect chRect;


SDL_Texture* monkeyTextures[3];
SDL_Rect monkeyRect;
int monkeyFrame = 0;
int monkeyTimer = 0;



Mode3::Mode3() {

  
    //STATE
    tutorial = true;
    ready = false;
    start = false;
    game_start = false;
    game_over = false;
    game_ending = false;

    //result 1 = 게임오버/ result 1 = finish
    result = 0;

    correct_button = false;


    // 원숭이 이미지 로드
    SDL_Surface* monkeySurface;
    for (int i = 0; i < 3; ++i)
    {
        std::string monkeyFilename = "../../Resources/monkey" + std::to_string(i + 1) + ".png";
        monkeySurface = IMG_Load(monkeyFilename.c_str());
        if (!monkeySurface)
        {
            std::cout << "Failed to load monkey image " << i << ": " << IMG_GetError() << std::endl;
        }
        monkeyTextures[i] = SDL_CreateTextureFromSurface(g_renderer, monkeySurface);
        SDL_FreeSurface(monkeySurface);
    }

    // 원숭이 초기 위치 설정
    monkeyRect.x = 430;
    monkeyRect.y = MONKEY_START_Y;
    monkeyRect.w = monkeySurface->w;
    monkeyRect.h = monkeySurface->h;

    MONKEY_START_Y = 150;

    currentCharacterIndex = 0;
    //캐릭터
    SDL_Surface* surface;
    {
        for (int i = 0; i < 2; ++i) {
            std::string imagePath = "../../Resources/User" + std::to_string(i + 1) + ".png";
            surface = IMG_Load(imagePath.c_str());
            chTextures[i] = SDL_CreateTextureFromSurface(g_renderer, surface);
            SDL_FreeSurface(surface);
        }
    }

    // 원숭이 초기 위치 설정
    chRect.x = 360;
    chRect.y = CH_START_Y;
    chRect.w = surface->w;
    chRect.h = surface->h;

    CH_START_Y = 420;



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


    // 배경 이미지 로드
    bg_surface = IMG_Load("../../Resources/Mode3_bg_all.png");
    bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface); // GPU로 옮기기 
    SDL_FreeSurface(bg_surface);
    backgroundY = -2400; 	// 배경 이미지 초기 위치 설정 (화면 위로)

    //wall
    {
        SDL_Surface* bg_surface = IMG_Load("../../Resources/Mode3_wall.png");
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
        source_rectangle_ready = { 0, 0, temp_surface->w, temp_surface->h };
        destination_rectangle_ready = { 20, 20, temp_surface->w, temp_surface->h };
       
    }

    // Start
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/start.png");
        texture_start = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);
        SDL_QueryTexture(texture_start, NULL, NULL, &source_rectangle_start.w, &source_rectangle_start.h);
        source_rectangle_start = {0, 0, temp_surface->w, temp_surface->h };
        destination_rectangle_start = {20, 20, temp_surface->w, temp_surface->h };

    }

    // game_ending
    // 
    // Finish
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

    // game over
    {
        SDL_Surface* temp_surface = IMG_Load("../../Resources/m3_gameover.png");
        texture_gameover = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

        SDL_QueryTexture(texture_gameover, NULL, NULL, &source_rectangle_gameover.w, &source_rectangle_gameover.h);

        source_rectangle_gameover.x = 0;
        source_rectangle_gameover.y = 0;
        source_rectangle_gameover.w = 800;
        source_rectangle_gameover.h = 600;

        destination_rectangle_gameover.x = 0;
        destination_rectangle_gameover.y = 0;
        destination_rectangle_gameover.w = 800;
        destination_rectangle_gameover.h = 600;
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

Mode3::~Mode3() {
    SDL_DestroyTexture(g_tt_texture);
    SDL_DestroyTexture(bg_texture);
    SDL_DestroyTexture(g_w_texture);
    SDL_DestroyTexture(texture_);
    SDL_DestroyTexture(texture_bb);
    SDL_DestroyTexture(texture_ready);
    SDL_DestroyTexture(texture_start);
    //SDL_DestroyTexture(texture_monkey);
    for (int i = 0; i < 3; ++i)
    {
        SDL_DestroyTexture(monkeyTextures[i]);
    }
    SDL_DestroyTexture(texture_finish);
    SDL_DestroyTexture(texture_gameover);
    SDL_DestroyTexture(texture_hb);
    SDL_DestroyTexture(texture_rb);
}

void Mode3::Update()
{
    Uint32 currentTime = SDL_GetTicks();

    // Ready, Start
    if (ready) {
        if (currentTime - startTime >= 2000) {
            startTime = currentTime;
            tutorial = false;
            ready = false;
            start = true;
            game_start = false;
            game_over = false;
            game_ending = false;

            
        }
    }
    else if (start) {
        if (currentTime - startTime >= 1000) {
       
            startTime = currentTime;

            tutorial = false;
            ready = false;
            start = false;
            game_start = true;
            game_over = false;
            game_ending = false;
        }
    }
   

    // game_start
    if (game_start && !game_over) {

        // 원숭이 계속 위로 올라감 (중력 위로 적용)
        monkeyY -= 1;

        // 원숭이 애니메이션 및 이동 처리
        monkeyTimer++;
        if (monkeyTimer % 15 == 0) // 1초에 한 번씩 애니메이션 변경
        {
            monkeyFrame = (monkeyFrame + 1) % 2; // monkey1.png과 monkey2.png 사이에서 반복
        }

        // 원숭이 잡힐 경우
        SDL_Rect intersection;
        if (SDL_IntersectRect(&chRect, &monkeyRect, &intersection) == SDL_TRUE && intersection.h >= 60)
        {
            gameoverTime = SDL_GetTicks();
            monkeyFrame = 2; // 원숭이 잡힌 경우 monkey3.png로 변경
           
            result = 2;
            tutorial = false;
            ready = false;
            start = false;
            game_start = false;
            game_over = true;
            game_ending = false;
        }

    }

    // game_over
    if (game_over) {
        if (result == 2) {
            monkeyY += 5;
        }
        else if (result == 1) {
            chY += 5;
        }
       
        // 현재 시간과 충돌 감지 시간 간의 차이 계산
        Uint32 elapsedTime = currentTime - gameoverTime;

        // 2초가 경과한 경우 게임 엔딩으로 전환
        if (game_over && elapsedTime >= 1000) {
            tutorial = false;
            ready = false;
            start = false;
            game_start = false;
            game_over = false;
            game_ending = true;
        }
    }

    if (game_start) {
        if (stonesOnScreen == 0) {
            // 화면에 돌이 없는 경우 새로운 돌을 추가
            stoneNumbers.push_back(randomDistribution(randomEngine));
            stonesOnScreen = NUM_STONES;
        }
    }

    if (correct_button == true) {
        correct_button = false;
        backgroundY += 30;
       
        monkeyY -= velocityY;
    }

    if (game_ending) {
        tutorial = false;
        ready = false;
        start = false;
        game_start = false;
        game_over = false;
    }

}

void Mode3::Render() {


    SDL_RenderClear(g_renderer);

    //배경이어지기
    SDL_Rect backgroundRect = { 0, backgroundY, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
    SDL_RenderCopy(g_renderer, bg_texture, NULL, &backgroundRect);

    // Wall
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
        // Back Button
        SDL_RenderCopy(g_renderer, texture_bb, &source_rectangle_bb, &destination_rectangle_bb);
    }
    else if (ready) {
        SDL_RenderCopy(g_renderer, texture_ready, &source_rectangle_ready, &destination_rectangle_ready);
    }
    else if (start) {
        SDL_RenderCopy(g_renderer, texture_start, &source_rectangle_start, &destination_rectangle_start);
    }
    else if (game_start || game_over) {

        //잡은 돌
        SDL_RenderCopy(g_renderer, g_caught_texture, &g_caught_source_rect, &g_caught_destination_rect);

        //캐릭터
        {
            chRect.y = chY; // 캐릭터 Y 좌표 업데이트
            SDL_Rect tmp_r;
            tmp_r.x = chRect.x;
            tmp_r.y = chRect.y;
            tmp_r.w = chRect.w;
            tmp_r.h = chRect.h;
            SDL_RenderCopy(g_renderer, chTextures[currentCharacterIndex], nullptr, &chRect);
        }
       
        //원숭이
        {
            monkeyRect.y = monkeyY; // 원숭이의 Y 좌표 업데이트
            SDL_Rect tmp_r;
            tmp_r.x = monkeyRect.x;
            tmp_r.y = monkeyRect.y;
            tmp_r.w = monkeyRect.w;
            tmp_r.h = monkeyRect.h;
            SDL_RenderCopy(g_renderer, monkeyTextures[monkeyFrame], nullptr, &tmp_r);

        }
        // 돌
        for (int i = 0; i < NUM_STONES; ++i) {
            int stoneNumber = stoneNumbers[i];
            SDL_Rect stoneRect = { STONE_X, STONE_Y - (STONE_SPACING * i), STONE_WIDTH, STONE_HEIGHT };
            SDL_RenderCopy(g_renderer, stoneTextures[stoneNumber - 1], nullptr, &stoneRect);
        }
        
    }
    else if (game_over) {

    }
    else if (game_ending) {
        
        if (result == 1) {
            SDL_RenderCopy(g_renderer, texture_gameover, &source_rectangle_gameover, &destination_rectangle_gameover);
        }
        else if (result == 2) {
            SDL_RenderCopy(g_renderer, texture_finish, &source_rectangle_finish, &destination_rectangle_finish);
        }

        // Home Button
        SDL_RenderCopy(g_renderer, texture_hb, &source_rectangle_hb, &destination_rectangle_hb);
        // Retry Button
        SDL_RenderCopy(g_renderer, texture_rb, &source_rectangle_rb, &destination_rectangle_rb);

    }


    SDL_RenderPresent(g_renderer);

}


void Mode3::HandleEvents() {
    //monkeyY = get_monkeyY_value();
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
                    tutorial = true;
                    ready = false;
                    start = false;
                    game_start = false;
                    game_over = false;
                    game_ending = false;
                    ResetGame();
                }
                //뒤로가기(home으로)
                if (mouseX >= destination_rectangle_hb.x && mouseX < destination_rectangle_hb.x + destination_rectangle_hb.w
                    && mouseY >= destination_rectangle_hb.y && mouseY < destination_rectangle_hb.y + destination_rectangle_hb.h
                    && g_current_game_phase == PHASE_MODE3) {
                    g_current_game_phase = PHASE_HOME;
                    tutorial = true;
                    ready = false;
                    start = false;
                    game_start = false;
                    game_over = false;
                    game_ending = false;
                    ResetGame();
                }
                //game restart
                if (mouseX >= destination_rectangle_rb.x && mouseX < destination_rectangle_rb.x + destination_rectangle_rb.w
                    && mouseY >= destination_rectangle_rb.y && mouseY < destination_rectangle_rb.y + destination_rectangle_rb.h
                    && g_current_game_phase == PHASE_MODE3) {
                    tutorial = true;
                    ready = false;
                    start = false;
                    game_start = false;
                    game_over = false;
                    game_ending = false;
                    ResetGame();
                }
                //game start
                if (mouseX >= destination_rectangle_.x && mouseX < destination_rectangle_.x + destination_rectangle_.w
                    && mouseY >= destination_rectangle_.y && mouseY < destination_rectangle_.y + destination_rectangle_.h
                    && g_current_game_phase == PHASE_MODE3 && tutorial == true) {
                    tutorial = false;
                    ready = true;
                    start = false;
                    game_start = false;
                    game_over = false;
                    game_ending = false;
                    startTime = SDL_GetTicks();
           
                }

            }
            break;

        case SDL_KEYDOWN:
            int key = event.key.keysym.sym - SDLK_0;
            if (key >= 1 && key <= 4) {
                if (!ready && !start && game_start) {
                    // 맞는 돌을 눌렀을 때 캐릭터 이미지 변경
                    currentCharacterIndex = (currentCharacterIndex + 1) % 2;
                    if (key == stoneNumbers[0]) {

                        // 올바른 숫자를 입력한 경우 돌들을 아래로 이동
                        stoneNumbers.erase(stoneNumbers.begin());
                        stoneNumbers.push_back(randomDistribution(randomEngine));
                        correct_button = true;

                        // 플레이어가 돌을 누른 시간 기록
                        lastStonePressTime = SDL_GetTicks();

                        velocityY -= monkeySpeed;
                       
                    }
                    else {
                        // 잘못된 숫자를 입력한 경우 게임 오버
                        result = 1;
                        game_over = true;
                        gameoverTime = SDL_GetTicks();
                    }

                }

            }
            break;



        }
    }
}

void Mode3::ResetGame() {

    monkeyY = 100;
    //result 1 = 게임오버/ result 1 = finish
    result = 0;
    velocityY = 0;
    correct_button = false;

    // 원숭이 초기 위치 설정
    MONKEY_START_Y = 150;
    monkeyRect.x = 430;
    monkeyRect.y = MONKEY_START_Y;

    

    currentCharacterIndex = 0;

    // 캐릭터 초기 위치 설정
    CH_START_Y = 420;
    chRect.x = 360;
    chRect.y = CH_START_Y;
    chY = 420;

  

    backgroundY = -2400; 	// 배경 이미지 초기 위치 설정 (화면 위로)



}
