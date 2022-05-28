#ifndef game_display__h
#define game_display__h
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
extern string number[];
extern string hintbutton[];
extern int checker[9][9],verdict[9][9];
extern bool canFill[9][9],isBox[9][9],Hinted[9][9];
extern const int totaltime;
/*extern SDL_Rect src,des;
extern SDL_Surface *surface;
extern TTF_Font *font;
extern SDL_Texture* texture;*/
void DrawText(SDL_Renderer* renderer,int X,int Y,string num,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg, int fontsize);
class Grid{
    private:
    int Size=40;
    int StepX=5;
    int StepY=5;
    public:
    int x;
    int y;
    void render(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,int checker[][9]);
    bool inside(int xi,int yi);
};
class Box{
    private:
    int StepX=1;
    int StepY=1;
    public:
    int x;
    int y;
    int SizeX;
    int SizeY;
    Box(int _x,int _y,int _SizeX,int _SizeY);
    void render(SDL_Renderer* renderer);
    void moveLeft(int x1);
    void moveRight(int x2);
    void moveDown(int y2);
    void moveUp(int y1);
    bool inside(int xi,int yi);
};
void Clock(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg,int timeLeft);
void clearOldBox(int r,int c);
Box initButton(Box button,int x,int y,int w,int h);
void close();
#endif
