#include<bits/stdc++.h>
#include "init_board.h"
using namespace std;
int a[9][9],row[9],board[9][9],realBoard[9][9],pick,cnt;
int hidden[]={3,4,5,6};
int random(int i){
    return rand()%i;
}
void printBoard(int a[][9]){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
}
void fillEmptyDiagonalBox(int ind){
    int start=ind*3;
    for(int i=0;i<9;i++) row[i]=i+1;
    random_shuffle(row,row+9,random);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            a[start+i][start+j]=row[i*3+j];
        }
    }
}
bool check(int a[][9],int x,int y,int k){
    for(int i=0;i<9;i++){
        if(a[x][i]==k) return false;
        if(a[i][y]==k) return false;
    }
    int gg=x/3,ff=y/3;
    for(int i=3*gg;i<gg*3+3;i++){
        for(int j=3*ff;j<3*ff+3;j++){
            if(a[i][j]==k) return false;
        }
    }
}
void solveSudoku(int a[][9],int x,int y){
    if(pick>5) return;
    else{
        if(y==9){
            if(x==8){
                pick=rand()%10+1;
                if(pick>5){
                    for(int i=0;i<9;i++){
                        for(int j=0;j<9;j++){
                            board[i][j]=a[i][j];
                        }
                    }
                }
            }else{
                solveSudoku(a,x+1,0);
            }
        }
        else if(a[x][y]==0){
            for(int i=1;i<=9;i++){
                if(check(a,x,y,i)){
                    a[x][y]=i;
                    solveSudoku(a,x,y+1);
                    a[x][y]=0;
                }
            }
        }else{
            solveSudoku(a,x,y+1);
        }
    }
}
void initSeed(){
    fillEmptyDiagonalBox(0);
    fillEmptyDiagonalBox(1);
    fillEmptyDiagonalBox(2);
    solveSudoku(a,0,0);
}
void puzzleValidation(int a[][9],int x,int y){
    //if(cnt>=2) return;
    if(y==9){
        if(x==8){
            cnt++;
            return;
        }
        else puzzleValidation(a,x+1,0);
    }
    else if(a[x][y]==0){
        for(int i=1;i<=9;i++){
            if(check(a,x,y,i)){
                a[x][y]=i;
                puzzleValidation(a,x,y+1);
                a[x][y]=0;
            }
        }
    }
    else puzzleValidation(a,x,y+1);
}
bool isValid(int a[][9]){
    cnt=0;
    puzzleValidation(a,0,0);
    if(cnt==1) return true;
    else return false;
}
void createPuzzle(){
    for(int i=0;i<9;i++){
        row[i]=i;
    }
    int tmp=0,zeroamount=0;
    while(zeroamount<25){
        zeroamount=0;
        for(int i=0;i<9;i++){
            random_shuffle(row,row+9,random);
            if(i%3==0) random_shuffle(hidden,hidden+4,random);
            for(int j=0;j<9;j++){
                realBoard[i][j]=board[i][j];
            }
            cout<<hidden[tmp]<<endl;
            for(int j=0;j<hidden[tmp];j++){
                realBoard[i][row[j]]=0;
            }
            tmp=(tmp+1)%3;
        }
    //printBoard(realBoard);
        if(!isValid(realBoard)){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(realBoard[i][j]==0){
                        realBoard[i][j]=board[i][j];
                        if(isValid(realBoard)) break;
                    }
                }
                if(isValid(realBoard)) break;
            }
        }
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(realBoard[i][j]==0) zeroamount+=1;
            }
        }
        cout<<zeroamount<<endl;
    }
}
int **initBoard(){
    srand(time(0));
    initSeed();
    createPuzzle();
    //cout<<endl;
    int **p=new int* [9];
    for(int i=0;i<9;i++){
        p[i]=new int[9];
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            p[i][j]=realBoard[i][j];
        }
    }
    return p;
}
