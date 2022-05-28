#ifndef SDL_Utils__h
#define SDL_Utils__h
#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include "init_board.h"
#include "SDL_Utils.h"
#include "LTexture.h"
using namespace std;
void logSDLError(std::ostream& os, const std::string &msg, bool fatal);
void logTTFError(std::ostream& os, const std::string &msg, bool fatal);
void logIMGError(std::ostream& os, const std::string &msg, bool fatal);
void logMIXError(std::ostream& os, const std::string &msg, bool fatal);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Surface* &gImage);
#endif
