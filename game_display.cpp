#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include "init_board.h"
#include "SDL_Utils.h"
#include "LTexture.h"
#include "game_display.h"
using namespace std;
string number[]={" ","1","2","3","4","5","6","7","8","9"};
string hintbutton[]={"hint-button-0.png","hint-button-1.png","hint-button-2.png","hint-button-3.png"};
int checker[9][9],verdict[9][9];
bool canFill[9][9],isBox[9][9],Hinted[9][9];
extern bool stop;
const int totaltime=600;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Surface* gImage;
extern Mix_Music* gMusic;
extern Mix_Chunk* gMove;
extern Mix_Chunk* gClick;
extern Mix_Chunk* gCount;
extern Mix_Chunk* gTick;
extern Mix_Chunk* gWin;
extern Mix_Chunk* gLose;
/*extern SDL_Rect src,des;
extern SDL_Surface *surface;
extern TTF_Font *font;
extern SDL_Texture* texture;*/
/*SDL_Surface* loadSurface( string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gImage->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return optimizedSurface;
}*/
void DrawText(SDL_Renderer* renderer,int X,int Y,string num,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg, int fontsize){
    font=TTF_OpenFont("yugothib.ttf",fontsize);
    surface=TTF_RenderText_Solid(font,num.c_str(),fg);
    texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    surface=NULL;
    SDL_Rect srcRect;
    SDL_Rect desRect;
    TTF_SizeText(font,num.c_str(),&srcRect.w,&srcRect.h);
    srcRect.x=0;
    srcRect.y=0;
    desRect.x=X+10;
    desRect.y=Y+10;
    desRect.w = srcRect.w;
	desRect.h = srcRect.h;
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	SDL_RenderCopy(renderer,texture,&srcRect,&desRect);
	SDL_DestroyTexture(texture);
	texture=NULL;
	TTF_CloseFont(font);
	font=NULL;
	//SDL_RenderPresent(renderer);
}
void Grid::render(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,int checker[][9]){
    SDL_Color fg;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            SDL_Rect fillable;
            fillable.x=x+j*Size;
            fillable.y=y+i*Size;
            fillable.w=fillable.h=Size;
            if(canFill[i][j]){
                SDL_SetRenderDrawColor(renderer,224,255,255,255);
                SDL_RenderFillRect(renderer,&fillable);
            }else{
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderFillRect(renderer,&fillable);
            }
            if(isBox[i][j]){
                SDL_SetRenderDrawColor(renderer, 0, 250, 154, 0); //medium spring green
                SDL_RenderFillRect(renderer,&fillable);
            }
            SDL_SetRenderDrawColor(renderer,0,0,0,0);
            SDL_RenderDrawLine(renderer,x+j*Size,y+i*Size,x+(j+1)*Size,y+i*Size);
            SDL_RenderDrawLine(renderer,x+(j+1)*Size,y+i*Size,x+(j+1)*Size,y+(i+1)*Size);
            SDL_RenderDrawLine(renderer,x+(j+1)*Size,y+(i+1)*Size,x+j*Size,y+(i+1)*Size);
            SDL_RenderDrawLine(renderer,x+j*Size,y+(i+1)*Size,x+j*Size,y+i*Size);
            int tmp=checker[i][j];
            checker[i][j]=0;
            if(check(checker,i,j,tmp)){
                //cout<<"false"<<endl;
                fg={0,0,0};
            }
            else fg={255,0,0};
            checker[i][j]=tmp;
            DrawText(renderer,x+j*Size,y+i*Size,number[checker[i][j]],font,surface,texture,fg,20);
        }
    }
}
bool Grid::inside(int xi,int yi){
    return (xi>=x && xi<=x+9*Size && yi>=y && yi<=y+9*Size);
}
Box::Box(int _x,int _y,int _SizeX,int _SizeY){
    x=_x;
    y=_y;
    SizeX=_SizeX;
    SizeY=_SizeY;
}
void Box::render(SDL_Renderer* renderer){
    SDL_RenderDrawLine(renderer,x,y,x+SizeX,y);
    SDL_RenderDrawLine(renderer,x+SizeX,y,x+SizeX,y+SizeY);
    SDL_RenderDrawLine(renderer,x+SizeX,y+SizeY,x,y+SizeY);
    SDL_RenderDrawLine(renderer,x,y+SizeY,x,y);
}
void Box::moveLeft(int x1){
    if(x>x1) x-=StepX*SizeX;
}
void Box::moveRight(int x2){
    if(x<x2) x+=StepX*SizeX;
}
void Box::moveDown(int y2){
    if(y<y2) y+=StepY*SizeY;
}
void Box::moveUp(int y1){
    if(y>y1) y-=StepY*SizeY;
}
bool Box::inside(int xi,int yi){
    return (xi>=x && xi<=x+SizeX && yi>=y && yi<=y+SizeY);
}
void Clock(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg,int timeLeft){
    int minute=timeLeft/60;
    int second=timeLeft%60;
    string minu,sec;
    fg={0,0,0};
    if(timeLeft<=15) fg={255,0,0};
    char d1=(char)(minute/10+48);
    char d2=(char)(minute%10+48);
    minu=minu+d1+d2;
    DrawText(renderer,470,10,minu,font,surface,texture,fg,15);
    d1=(char)(second/10+48);
    d2=(char)(second%10+48);
    sec=sec+d1+d2;
    DrawText(renderer,490,10,":",font,surface,texture,fg,15);
    DrawText(renderer,500,10,sec,font,surface,texture,fg,15);
}
void clearOldBox(int r,int c){
    if(c<8&&isBox[r][c+1]) isBox[r][c+1]=false;
    if(c>0&&isBox[r][c-1]) isBox[r][c-1]=false;
    if(r<8&&isBox[r+1][c]) isBox[r+1][c]=false;
    if(r>0&&isBox[r-1][c]) isBox[r-1][c]=false;
    if(r>0&&c>0&&isBox[r-1][c-1]) isBox[r-1][c-1]=false;
    if(r<8&&c>0&&isBox[r+1][c-1]) isBox[r+1][c-1]=false;
    if(r>0&&c<8&&isBox[r-1][c+1]) isBox[r-1][c+1]=false;
    if(r<8&&c<8&&isBox[r+1][c+1]) isBox[r+1][c+1]=false;
}
void close(){
    Mix_FreeChunk(gMove);
    Mix_FreeChunk(gClick);
    Mix_FreeChunk(gCount);
    Mix_FreeChunk(gTick);
    Mix_FreeChunk(gWin);
    Mix_FreeChunk(gLose);
    Mix_FreeMusic(gMusic);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(gImage);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}
