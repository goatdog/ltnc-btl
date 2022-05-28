#ifndef init_board__h
#define init_board__h
#include<bits/stdc++.h>
using namespace std;

int random(int i);
void fillEmptyDiagonalBox();
bool check(int a[][9],int x,int y,int k);
void solveSudoku(int a[][9],int x,int y);
void initSeed();
void puzzleValidation(int a[][9],int x,int y);
bool isValid(int a[][9]);
void createPuzzle();
int **initBoard();
int **answer();
void gettable(int a[][9], bool b);
#endif
