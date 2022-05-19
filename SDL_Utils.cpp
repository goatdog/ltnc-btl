#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include "init_board.h"
#include "SDL_Utils.h"
#include "LTexture.h"
using namespace std;
const int SCREEN_WIDTH=556;
const int SCREEN_HEIGHT=600;
const string WINDOW_TITLE="Sudoku - SDL";
void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void logTTFError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << TTF_GetError() << std::endl;
    if (fatal) {
        TTF_Quit();
        exit(1);
    }
}
void logIMGError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << IMG_GetError() << std::endl;
    if (fatal) {
        IMG_Quit();
        exit(1);
    }
}
void logMIXError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << Mix_GetError() << std::endl;
    if (fatal) {
        Mix_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Surface* &gImage)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        logSDLError(std::cout, "SDL_Init", true);
    if (TTF_Init()< 0)
        logTTFError(std::cout, "TTF_Init", true);
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        logIMGError(std::cout,"IMG_Init",true);
    }
    else
    {
        //Get window surface
        gImage = SDL_GetWindowSurface( window );
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        logMIXError(std::cout,"MIX_Init",true);
    }
}
