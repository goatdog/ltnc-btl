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
extern SDL_Rect src,des;
extern SDL_Surface *surface;
extern TTF_Font *font;
extern SDL_Texture* texture;
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
    if(timeLeft<=10) fg={255,0,0};
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
Box initButton(Box button,int x,int y,int w,int h){
    button.x=x;
    button.y=y;
    button.SizeX=w;
    button.SizeY=h;
    return button;
}
void close(){
    cleanup(window,renderer,gImage);
}
/*int main(int argc, char* argv[])
{
    gettable(checker,false);
    gettable(verdict,true);
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(checker[i][j]==0) canFill[i][j]=true;
        }
    }
    initSDL(window, renderer, gImage);
    Grid grid;
    grid.x=40;
    grid.y=100;
    Box box,pause,hint,kiemtra;
    box=initButton(box,40,100,40,40);
    pause=initButton(pause,10,10,30,30);
    hint=initButton(hint,420,200,100,40);
    kiemtra=initButton(kiemtra,420,250,100,40);
    SDL_Event e;
    SDL_Color fg={0,0,0};
    Uint32 secondpassed=0,realTime=0; bool pflag=false,hflag=false, cflag=false, canWin=true;
    int availhints=3;
    while(!stop){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        //SDL_BlitSurface(image, NULL, gImage, NULL );
        //SDL_UpdateWindowSurface( window );
        if(!cflag&&!pflag){
            ApplyTexture("sudoku-background.png",0,0,556,600);
            ApplyTexture("pause-button.png",10,10,30,30);
            ApplyTexture(hintbutton[availhints],420,200,100,40);
            ApplyTexture("check-button.png",420,250,100,40);
            ApplyTexture("clock.png",460,20,15,15);
            int r=(box.y-grid.y)/box.SizeX;
            int c=(box.x-grid.x)/box.SizeY;
            isBox[r][c]=true;
            if(hflag==true){
                checker[r][c]=verdict[r][c];
                availhints--;
                Hinted[r][c]=true;
                hflag=false;
            }
            grid.render(renderer,font,surface,texture,checker);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
            box.render(renderer);
            Uint32 timer;
            if(SDL_GetTicks()/1000>realTime){
                realTime=SDL_GetTicks()/1000;
                secondpassed++;
            }
            timer=totaltime-secondpassed;
            Clock(renderer,font,surface,texture,fg,timer);
            if(timer==0){
                cflag=true;
                canWin=false;
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
                if(e.type==SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_ESCAPE: stop=true;
                        break;
                    case SDLK_LEFT:
                        {box.moveLeft(grid.x);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_RIGHT:
                        {box.moveRight(grid.x+8*40);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_DOWN:
                        {box.moveDown(grid.y+8*40);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_UP:
                        {box.moveUp(grid.y);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_1:
                        {if(canFill[r][c]) checker[r][c]=1;}
                        break;
                    case SDLK_2:
                        {if(canFill[r][c]) checker[r][c]=2;}
                        break;
                    case SDLK_3:
                        {if(canFill[r][c]) checker[r][c]=3;}
                        break;
                    case SDLK_4:
                        {if(canFill[r][c]) checker[r][c]=4;}
                        break;
                    case SDLK_5:
                        {if(canFill[r][c]) checker[r][c]=5;}
                        break;
                    case SDLK_6:
                        {if(canFill[r][c]) checker[r][c]=6;}
                        break;
                    case SDLK_7:
                        {if(canFill[r][c]) checker[r][c]=7;}
                        break;
                    case SDLK_8:
                        {if(canFill[r][c]) checker[r][c]=8;}
                        break;
                    case SDLK_9:
                        {if(canFill[r][c]) checker[r][c]=9;}
                        break;
                    case SDLK_BACKSPACE:
                        {if(canFill[r][c]) checker[r][c]=0;}
                        break;
                    default: break;
                }
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(e.button.button==SDL_BUTTON_LEFT&&grid.inside(e.motion.x,e.motion.y)){
                        isBox[r][c]=false;
                        box.x=grid.x+((e.motion.x-grid.x)/box.SizeX)*box.SizeX;
                        box.y=grid.y+((e.motion.y-grid.y)/box.SizeY)*box.SizeY;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&pause.inside(e.motion.x,e.motion.y)){
                        if(pflag==false) pflag=true;
                        else pflag=false;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&hint.inside(e.motion.x,e.motion.y)){
                        if(hflag==false&&availhints>0&&canFill[r][c]&&!Hinted[r][c]) hflag=true;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&kiemtra.inside(e.motion.x,e.motion.y)){
                        for(int i=0;i<9;i++){
                            for(int j=0;j<9;j++){
                                if(checker[i][j]!=verdict[i][j]){
                                    canWin=false;
                                    break;
                                }
                            }
                            if(canWin==false) break;
                        }
                        cflag=true;
                    }
                }
            }
        }
        else if(!cflag&&pflag){
            DrawText(renderer,200,200,"Press left mouse to continue!",font,surface,texture,fg,30);
            if(SDL_GetTicks()/1000>realTime){
                realTime=SDL_GetTicks()/1000;
            }SDL_RenderPresent(renderer);
            SDL_Delay(10);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(e.button.button==SDL_BUTTON_LEFT){
                        pflag=false;
                        break;
                    }
                }
            }
        }
        else if(cflag){
            if(canWin){
                ApplyTexture("You-win.png",0,0,556,600);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                while(SDL_PollEvent(&e)!=0){
                    if(e.type==SDL_QUIT){
                        stop=true;
                        break;
                    }
                }
            }else{
                ApplyTexture("game-over.png",0,22,556,556);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                while(SDL_PollEvent(&e)!=0){
                    if(e.type==SDL_QUIT){
                        stop=true;
                        break;
                    }
                }
            }
        }
    }
    atexit(close);
    exit(0);
}*/
