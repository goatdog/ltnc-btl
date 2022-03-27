#include<bits/stdc++.h>
using namespace std;
int a[9][9],row[9],board[9][9],realBoard[9][9],ck;
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
    if(ck>5) return;
    else{
        if(y==9){
            if(x==8){
                ck=rand()%10+1;
                if(ck>5){
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
int main(){
    srand(time(0));
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            a[i][j]=0;
        }
    }
    a[0][0]=rand()%9+1;
    if(a[0][0]>5) a[0][1]=rand()%5+1;
    else a[0][1]=rand()%4+6;
    solveSudoku(a,0,0);
    cout<<"Puzzle:\n";
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            realBoard[i][j]=board[i][j];
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Display:\n";
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            int tmp=rand()%10+1;
            if(tmp>5) realBoard[i][j]=0;
            cout<<realBoard[i][j]<<" ";
        }
        cout<<endl;
    }
}
