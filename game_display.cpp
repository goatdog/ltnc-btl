#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<cstring>
#include "init_board.h"
using namespace std;
string number[]={" ","1","2","3","4","5","6","7","8","9"};
int checker[9][9];
bool canFill[9][9];
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Sudoku - SDL";

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);
    if (TTF_Init()!= 0)
        logSDLError(std::cout, "TTF_Init", true);
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
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
void DrawNumber(SDL_Renderer* renderer,int X,int Y,string num,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg){
    font=TTF_OpenFont("font-times-new-roman/times new roman.ttf",20);
    //SDL_Color fg={0,0,0};
    surface=TTF_RenderText_Solid(font,num.c_str(),fg);
    texture=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
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
	//SDL_RenderPresent(renderer);
}
struct Grid{
    int x;
    int y;
    int Size=40;
    int StepX=5;
    int StepY=5;
    void render(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,int checker[][9]){
        /*SDL_Rect filled_rect;
        filled_rect.x = x;
        filled_rect.y = y;
        filled_rect.w = Size;
        filled_rect.h = Size;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
        SDL_RenderFillRect(renderer, &filled_rect);*/
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
                DrawNumber(renderer,x+j*Size,y+i*Size,number[tmp],font,surface,texture,fg);
            }
        }
    }
};
struct Box{
    int x;
    int y;
    int Size=40;
    int StepX=1;
    int StepY=1;
    void render(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,int checker[][9]){
        SDL_Rect filled_rect;
        filled_rect.x = x;
        filled_rect.y = y;
        filled_rect.w = Size;
        filled_rect.h = Size;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0); // blue
        SDL_RenderDrawLine(renderer,x,y,x+Size,y);
        SDL_RenderDrawLine(renderer,x+Size,y,x+Size,y+Size);
        SDL_RenderDrawLine(renderer,x+Size,y+Size,x,y+Size);
        SDL_RenderDrawLine(renderer,x,y+Size,x,y);
    }
    void moveLeft(int x1){
        if(x>x1) x-=StepX*Size;
    }
    void moveRight(int x2){
        if(x<x2) x+=StepX*Size;
    }
    void moveDown(int y2){
        if(y<y2) y+=StepY*Size;
    }
    void moveUp(int y1){
        if(y>y1) y-=StepY*Size;
    }
    bool inside(int x1,int y1,int x2,int y2){
        return (x1<=x && x<=x2 && y1<=y && y<=y2);
    }
};
int main(int argc, char* argv[])
{
    int **numboard=new int*[9];
    for(int i=0;i<9;i++){
        numboard[i]=new int[9];
    }
    numboard=initBoard();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            checker[i][j]=numboard[i][j];
            if(checker[i][j]==0) canFill[i][j]=true;
        }
    }
    TTF_Font *font=NULL;
    SDL_Surface *surface=NULL;
    SDL_Texture *texture=NULL;
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    Grid grid;
    grid.x=40;
    grid.y=100;
    /*SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    grid.render(renderer,font,surface,texture,numboard);
    SDL_RenderPresent(renderer);*/
    Box box;
    box.x=40;
    box.y=100;
    SDL_Event e;
    while(box.inside(0,0,SCREEN_WIDTH,SCREEN_HEIGHT)){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        grid.render(renderer,font,surface,texture,checker);
    //SDL_RenderPresent(renderer);
        //SDL_RenderClear(renderer);
        box.render(renderer,font,surface,texture,checker);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        if(SDL_WaitEvent(&e)==0) continue;
        if(e.type==SDL_QUIT) break;
        if(e.type==SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_ESCAPE: break;
                case SDLK_LEFT: box.moveLeft(grid.x);
                    break;
                case SDLK_RIGHT: box.moveRight(grid.x+8*40);
                    break;
                case SDLK_DOWN: box.moveDown(grid.y+8*40);
                    break;
                case SDLK_UP: box.moveUp(grid.y);
                    break;
                case SDLK_1:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=1;}
                    break;
                case SDLK_2:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=2;}
                    break;
                case SDLK_3:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=3;}
                    break;
                case SDLK_4:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=4;}
                    break;
                case SDLK_5:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=5;}
                    break;
                case SDLK_6:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=6;}
                    break;
                case SDLK_7:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=7;}
                    break;
                case SDLK_8:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=8;}
                    break;
                case SDLK_9:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=9;}
                    break;
                case SDLK_BACKSPACE:
                    {int r=(box.y-grid.y)/box.Size;
                    int c=(box.x-grid.x)/box.Size;
                    if(canFill[r][c]) checker[r][c]=0;}
                    break;
                default: break;
            }
        }
        /*if(e.type==SDL_MOUSEBUTTONDOWN){
            switch(e.button.buttonsym.sym){
                case SDL_MOUSEBUTTONDOWN:

            }
        }*/
    }
    waitUntilKeyPressed();
    quitSDL(window, renderer);
    TTF_CloseFont(font);
    return 0;
}
