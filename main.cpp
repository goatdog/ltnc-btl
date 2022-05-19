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
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* gImage=NULL;
SDL_Rect src,des;
SDL_Surface *surface=NULL;
TTF_Font *font=NULL;
SDL_Texture* texture=NULL;
bool stop=false;
int main(int argc, char* argv[]){
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
    int availhints=3,screenlabel=1,start=4;
    Box play,instruction,exit;
    play=initButton(play,160,258,237,53);
    instruction=initButton(instruction,160,349,237,53);
    exit=initButton(exit,160,440,237,53);
    while(!stop){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        if(screenlabel==1){
            ApplyTexture("menu-background.png",0,0,556,600);
            ApplyTexture("sudoku-title.png",156,100,243,103);
            ApplyTexture("menu-button-ver2.png",128,220,300,300);
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(e.button.button==SDL_BUTTON_LEFT&&play.inside(e.motion.x,e.motion.y)){
                        screenlabel=2;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&instruction.inside(e.motion.x,e.motion.y)){
                        screenlabel=4;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&exit.inside(e.motion.x,e.motion.y)){
                        stop=true;
                        break;
                    }
                }
            }
        }
        if(screenlabel==2){
            if(start==4) ApplyTexture("hidden-layer.png",0,0,556,600);
            else if(start==3) ApplyTexture("3.png",0,0,556,600);
            else if(start==2) ApplyTexture("2.png",0,0,556,600);
            else if(start==1) ApplyTexture("1.png",0,0,556,600);
            else screenlabel=3;
            SDL_RenderPresent(renderer);
            start--;
            SDL_Delay(1000);
        }
        if(screenlabel==3){
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
        if(screenlabel==4){
            break;
        }
    }
    atexit(close);
    //exit(0);
}
