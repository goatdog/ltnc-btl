#ifndef SDL_Utils__h
#define SDL_Utils__h
#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include "init_board.h"
#include "SDL_Utils.h"
using namespace std;
void logSDLError(std::ostream& os, const std::string &msg, bool fatal);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Surface* &gImage);
void cleanup(SDL_Window* &window, SDL_Renderer* &renderer, SDL_Surface* &gImage);
#endif
