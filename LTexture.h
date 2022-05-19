#ifndef LTexture__h
#define LTexture__h
#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include "init_board.h"
#include "SDL_Utils.h"
#include "Ltexture.h"
using namespace std;
SDL_Texture* loadTexture( string path );
void ApplyTexture(string path,int x,int y,int w,int h);
void loadMedia();
#endif
