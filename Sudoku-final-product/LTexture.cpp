#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include "init_board.h"
#include "SDL_Utils.h"
#include "Ltexture.h"
using namespace std;
extern SDL_Renderer* renderer;
extern SDL_Rect src,des;
extern Mix_Chunk* gMove;
extern Mix_Chunk* gClick;
extern Mix_Chunk* gCount;
extern Mix_Chunk* gWin;
extern Mix_Chunk* gLose;
extern Mix_Chunk* gTick;
extern Mix_Music* gMusic;
SDL_Texture* loadTexture( string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    string newpath=path;
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", newpath.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", newpath.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
void ApplyTexture(string path,int x,int y,int w,int h){
    SDL_Texture* texture=loadTexture(path.c_str());
    (src.x)=0; (src.y)=0;
    src.w=w;  src.h=h;
    (des.x)=x; (des.y)=y;
    des.w=w; des.h=h;
    SDL_RenderCopy(renderer,texture,&src,&des);
    SDL_DestroyTexture(texture);
}
void loadMedia(){
    gMusic=Mix_LoadMUS("Brain-out-theme-song.wav");
    if(gMusic==NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n",Mix_GetError());
    }
    gMove=Mix_LoadWAV("movement.wav");
    if(gMove==NULL)
    {
        printf("Failed to load movement sound effect! SDL_mixer Error: %s\n",Mix_GetError());
    }
    gClick=Mix_LoadWAV("click.wav");
    if(gClick==NULL)
    {
        printf("Failed to load click sound effect! SDL_mixer Error: %s\n",Mix_GetError());
    }
    gCount=Mix_LoadWAV("countdown.wav");
    if(gCount==NULL)
    {
        printf("Failed to load countdown sound effect! SDL_mixer Error: %s\n",Mix_GetError());
    }
    gTick=Mix_LoadWAV("tick-tock.wav");
    if(gTick==NULL){
        printf("Failed to load ticktock sound effect! SDL_mixer Error: %s\n",Mix_GetError());
    }
    gWin=Mix_LoadWAV("Victory.wav");
    if(gWin==NULL){
        printf("Failed to load winning sound effect! SDL_mixer Error: %s\n",Mix_GetError());
    }
    gLose=Mix_LoadWAV("Failure.wav");
    if(gLose==NULL){
        printf("Failed to load losing sound effect! SDL_mixer Error: %s\n",Mix_GetError());
    }
}
