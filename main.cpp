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
Mix_Music* gMusic=NULL;
Mix_Chunk* gMove=NULL;
Mix_Chunk* gClick=NULL;
Mix_Chunk* gCount=NULL;
Mix_Chunk* gTick=NULL;
Mix_Chunk* gWin=NULL;
Mix_Chunk* gLose=NULL;
bool stop=false;
int main(int argc, char* argv[]){
    initSDL(window, renderer, gImage);
    loadMedia();
    Grid grid;
    grid.x=40;
    grid.y=100;
    Box box(40,100,40,40);
    Box pause(10,10,30,30);
    Box hint(420,200,100,40);
    Box kiemtra(420,250,100,40);
    Box play(160,258,237,53);
    Box instruction(160,349,237,53);
    Box exit(160,440,237,53);
    Box backbutton(0,0,40,41);
    Box backtomenu(128,225,300,150);
    Box cont(160,188,237,53);
    Box music(160,279,237,53);
    Box exit2(160,370,237,53);
    SDL_Event e;
    SDL_Color fg={0,0,0};
    Uint32 secondpassed=0,realTime=0;
    bool pflag=false,hflag=false,cflag=false,canWin=true,mflag=true,sfx=true;
    int availhints=3,screenlabel=0,start,cooldown;
    while(!stop){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        if(screenlabel==0){
            if(mflag==true) Mix_PlayMusic(gMusic,-1);
            memset(checker,0,sizeof(checker));
            memset(verdict,0,sizeof(verdict));
            memset(canFill,false,sizeof(canFill));
            memset(isBox,false,sizeof(isBox));
            memset(Hinted,false,sizeof(Hinted));
            gettable(checker,false);
            gettable(verdict,true);
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(checker[i][j]==0) canFill[i][j]=true;
                }
            }
            box.x=40; box.y=100;
            cooldown=200;
            start=3;
            hflag=false;
            cflag=false;
            canWin=true;
            sfx=true;
            realTime=secondpassed=0;
            availhints=3;
            screenlabel=1;
        }
        else if(screenlabel==1){
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
                        Mix_PlayChannel(-1,gClick,0);
                        screenlabel=2;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&instruction.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        screenlabel=4;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&exit.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        stop=true;
                        break;
                    }
                }
            }
        }
        else if(screenlabel==2){
            if(start==3){
                ApplyTexture("3.png",0,0,556,600);
                Mix_PlayChannel(-1,gCount,0);
            }
            else if(start==2) ApplyTexture("2.png",0,0,556,600);
            else if(start==1){
                ApplyTexture("1.png",0,0,556,600);
                screenlabel=3;
            }
            SDL_RenderPresent(renderer);
            start--;
            SDL_Delay(1000);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
            }
        }
        else if(screenlabel==3){
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
                if(checker[r][c]!=verdict[r][c]){
                    checker[r][c]=verdict[r][c];
                    availhints--;
                    Hinted[r][c]=true;
                }
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
            if(timer==16) Mix_PlayChannel(-1,gTick,0);
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
                        {Mix_PlayChannel(-1,gMove,0);
                        box.moveLeft(grid.x);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_RIGHT:
                        {Mix_PlayChannel(-1,gMove,0);
                        box.moveRight(grid.x+8*40);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_DOWN:
                        {Mix_PlayChannel(-1,gMove,0);
                        box.moveDown(grid.y+8*40);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_UP:
                        {Mix_PlayChannel(-1,gMove,0);
                        box.moveUp(grid.y);
                        r=(box.y-grid.y)/box.SizeX;
                        c=(box.x-grid.x)/box.SizeY;
                        clearOldBox(r,c);}
                        break;
                    case SDLK_1:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=1;}
                        break;
                    case SDLK_2:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=2;}
                        break;
                    case SDLK_3:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=3;}
                        break;
                    case SDLK_4:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=4;}
                        break;
                    case SDLK_5:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=5;}
                        break;
                    case SDLK_6:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=6;}
                        break;
                    case SDLK_7:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=7;}
                        break;
                    case SDLK_8:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=8;}
                        break;
                    case SDLK_9:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=9;}
                        break;
                    case SDLK_BACKSPACE:
                        {Mix_PlayChannel(-1,gMove,0);
                        if(canFill[r][c]) checker[r][c]=0;}
                        break;
                    default: break;
                }
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(e.button.button==SDL_BUTTON_LEFT&&grid.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        isBox[r][c]=false;
                        box.x=grid.x+((e.motion.x-grid.x)/box.SizeX)*box.SizeX;
                        box.y=grid.y+((e.motion.y-grid.y)/box.SizeY)*box.SizeY;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&pause.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        if(pflag==false) pflag=true;
                        else pflag=false;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&hint.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        if(hflag==false&&availhints>0&&canFill[r][c]&&!Hinted[r][c]) hflag=true;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&kiemtra.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
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
            if(SDL_GetTicks()/1000>realTime){
                realTime=SDL_GetTicks()/1000;
            }
            ApplyTexture("menu-background.png",0,0,556,600);
            ApplyTexture("back-button.png",0,0,40,41);
            if(mflag==true){
                ApplyTexture("pause-menu-ver1.png",128,150,300,300);
                if(Mix_PlayingMusic()!=0) Mix_ResumeMusic();
                else Mix_PlayMusic(gMusic,-1);
            }
            else{
                ApplyTexture("pause-menu-ver2.png",128,150,300,300);
                Mix_PauseMusic();
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(e.button.button==SDL_BUTTON_LEFT&&backbutton.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        screenlabel=0;
                        pflag=false;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&cont.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        pflag=false;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&music.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        mflag^=1;
                        break;
                    }
                    if(e.button.button==SDL_BUTTON_LEFT&&exit2.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        stop=true;
                        break;
                    }
                }
            }
        }
        else if(cflag){
            Mix_HaltMusic();
            if(canWin){
                if(sfx==true){
                    Mix_PlayChannel(-1,gWin,0);
                    sfx=false;
                }
                ApplyTexture("You-win.png",0,0,556,600);
                if(cooldown>0) cooldown--;
                else ApplyTexture("back-to-menu.png",128,225,300,150);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                while(SDL_PollEvent(&e)!=0){
                    if(e.type==SDL_QUIT){
                        stop=true;
                        break;
                    }
                    if(e.type==SDL_MOUSEBUTTONDOWN){
                        if(e.button.button==SDL_BUTTON_LEFT&&backtomenu.inside(e.motion.x,e.motion.y)&&cooldown==0){
                            Mix_PlayChannel(-1,gClick,0);
                            screenlabel=0;
                            break;
                        }
                    }
                }
            }else{
                if(sfx==true){
                    Mix_PlayChannel(-1,gLose,0);
                    sfx=false;
                }
                ApplyTexture("game-over.png",0,22,556,556);
                if(cooldown>0) cooldown--;
                else ApplyTexture("back-to-menu.png",128,225,300,150);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
                while(SDL_PollEvent(&e)!=0){
                    if(e.type==SDL_QUIT){
                        stop=true;
                        break;
                    }
                    if(e.type==SDL_MOUSEBUTTONDOWN){
                        if(e.button.button==SDL_BUTTON_LEFT&&backtomenu.inside(e.motion.x,e.motion.y)&&cooldown==0){
                            Mix_PlayChannel(-1,gClick,0);
                            screenlabel=0;
                            break;
                        }
                    }
                }
            }
        }
        }
        else if(screenlabel==4){
            ApplyTexture("instructions.png",0,0,556,600);
            ApplyTexture("back-button.png",0,0,40,41);
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    if(e.button.button==SDL_BUTTON_LEFT&&backbutton.inside(e.motion.x,e.motion.y)){
                        Mix_PlayChannel(-1,gClick,0);
                        screenlabel=1;
                        break;
                    }
                }
            }
        }
    }
    atexit(close);
    return 0;
}
