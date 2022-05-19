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
SDL_Texture* loadTexture( string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
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
