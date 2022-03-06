#include<bits/stdc++.h>
using namespace std;
int a[9][9];
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
    if(y==9){
        if(x==8){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    cout<<a[i][j]<<" ";
                }
                cout<<endl;
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
int main(){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin>>a[i][j];
        }
    }
    solveSudoku(a,0,0);
}
