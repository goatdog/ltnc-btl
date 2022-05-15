#include<bits/stdc++.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<cstring>
#include "init_board.h"
using namespace std;
string number[]={" ","1","2","3","4","5","6","7","8","9"};
int checker[9][9],verdict[9][9];
bool canFill[9][9],stop=false,isBox[9][9],Hinted[9][9],arrowkey;
const int totaltime=600;
const int SCREEN_WIDTH=800;
const int SCREEN_HEIGHT=600;
const string WINDOW_TITLE="Sudoku - SDL";
void gettable(int a[][9], bool b){
    int **numboard=new int*[9];
    for(int i=0;i<9;i++){
        numboard[i]=new int[9];
    }
    if(b==false) numboard=initBoard();
    else numboard=answer();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            a[i][j]=numboard[i][j];
            if(a[i][j]==0) canFill[i][j]=true;
        }
    }
    for(int i=0;i<9;i++){
        delete []numboard[i];
    }
    delete []numboard;
}
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

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

void cleanup()
{
	TTF_Quit();
	SDL_Quit();
}
void DrawText(SDL_Renderer* renderer,int X,int Y,string num,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg, int fontsize){
    font=TTF_OpenFont("font-times-new-roman/times new roman.ttf",fontsize);
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
    bool inside(int xi,int yi){
        return (xi>=x && xi<=x+9*Size && yi>=y && yi<=y+9*Size);
    }
};
struct Box{
    int x;
    int y;
    int SizeX;
    int SizeY;
    int StepX=1;
    int StepY=1;
    void render(SDL_Renderer* renderer){
        SDL_RenderDrawLine(renderer,x,y,x+SizeX,y);
        SDL_RenderDrawLine(renderer,x+SizeX,y,x+SizeX,y+SizeY);
        SDL_RenderDrawLine(renderer,x+SizeX,y+SizeY,x,y+SizeY);
        SDL_RenderDrawLine(renderer,x,y+SizeY,x,y);
    }
    void moveLeft(int x1){
        if(x>x1) x-=StepX*SizeX;
    }
    void moveRight(int x2){
        if(x<x2) x+=StepX*SizeX;
    }
    void moveDown(int y2){
        if(y<y2) y+=StepY*SizeY;
    }
    void moveUp(int y1){
        if(y>y1) y-=StepY*SizeY;
    }
    bool inside(int xi,int yi){
        return (xi>=x && xi<=x+SizeX && yi>=y && yi<=y+SizeY);
    }
};
void Clock(SDL_Renderer* renderer,TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg,int timeLeft){
    int minute=timeLeft/60;
    int second=timeLeft%60;
    string minu,sec;
    fg={0,0,0};
    char d1=(char)(minute/10+48);
    char d2=(char)(minute%10+48);
    minu=minu+d1+d2;
    DrawText(renderer,480,10,minu,font,surface,texture,fg,20);
    d1=(char)(second/10+48);
    d2=(char)(second%10+48);
    sec=sec+d1+d2;
    DrawText(renderer,500,10,":",font,surface,texture,fg,20);
    DrawText(renderer,520,10,sec,font,surface,texture,fg,20);
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
void Button(Box button, SDL_Renderer* renderer, string name, int fontsize, TTF_Font *font,SDL_Surface *surface,SDL_Texture *texture,SDL_Color fg, bool flag){
    fg={0,0,0};
    DrawText(renderer,button.x,button.y,name,font,surface,texture,fg,fontsize);
    if(flag==false) SDL_SetRenderDrawColor(renderer,0,0,0,0);
    else SDL_SetRenderDrawColor(renderer,0,0,255,0);
    button.render(renderer);
}
int main(int argc, char* argv[])
{
    gettable(checker,false);
    gettable(verdict,true);
    TTF_Font *font=NULL;
    SDL_Surface *surface=NULL;
    SDL_Texture *texture=NULL;
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    Grid grid;
    grid.x=40;
    grid.y=100;
    Box box,pause,hint,kiemtra;
    box.x=40;
    box.y=100;
    box.SizeX=box.SizeY=40;
    pause.x=10;
    pause.y=10;
    pause.SizeX=30;
    pause.SizeY=30;
    hint.x=480;
    hint.y=200;
    hint.SizeX=80;
    hint.SizeY=40;
    kiemtra.x=480;
    kiemtra.y=250;
    kiemtra.SizeX=80;
    kiemtra.SizeY=40;
    SDL_Event e;
    SDL_Color fg={0,0,0};
    Uint32 secondpassed=0,realTime=0; bool pflag=false,hflag=false, cflag=false, canWin=true;
    int availhints=3;
    while(!stop){
        if(!cflag&&!pflag){
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderClear(renderer);
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
            Button(pause,renderer,"||",10,font,surface,texture,fg,pflag);
            Button(hint,renderer,"Hint",20,font,surface,texture,fg,hflag);
            Button(kiemtra,renderer,"Check",20,font,surface,texture,fg,cflag);
            if(timer==0){
                stop=true;
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
            while(SDL_PollEvent(&e)!=0){
                if(e.type==SDL_QUIT){
                    stop=true;
                    break;
                }
                //}
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
                        if(hflag==false&&canFill[r][c]&&!Hinted[r][c]) hflag=true;
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
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderClear(renderer);
            DrawText(renderer,200,200,"Press left mouse to continue!",font,surface,texture,fg,30);
            if(SDL_GetTicks()/1000>realTime){
                realTime=SDL_GetTicks()/1000;
            }SDL_RenderPresent(renderer);
            SDL_Delay(500);
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
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderClear(renderer);
                DrawText(renderer,200,200,"Congratulations! You win!",font,surface,texture,fg,30);
                SDL_RenderPresent(renderer);
                SDL_Delay(500);
                while(SDL_PollEvent(&e)!=0){
                    if(e.type==SDL_QUIT){
                        stop=true;
                        break;
                    }
                }
            }else{
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderClear(renderer);
                DrawText(renderer,250,200,"Haha loser!",font,surface,texture,fg,30);
                SDL_RenderPresent(renderer);
                SDL_Delay(500);
                while(SDL_PollEvent(&e)!=0){
                    if(e.type==SDL_QUIT){
                        stop=true;
                        break;
                    }
                }
            }
        }
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    atexit(cleanup);
    return 0;
}
