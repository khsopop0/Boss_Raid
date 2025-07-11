#pragma once
#pragma warning(disable:4996)
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define BOSS bossAttack[i].x, bossAttack[i].y,


typedef enum { false, true } bool;

extern int COLS;
extern int LOWS;

// â 
void setWindowSize(int cols, int lows);

//폰트 설정
void setFont(int size);

//Ŀ ̵ Լ
void gotoxy(int x, int y);

//ȭ 
void clear();

//ؽƮ   Լ
void textColor(int color);

// 0 or 1
int getColorBit(int background, int bright,
	int red, int green, int blue);

void ScreenInit(void);

void ScreenFlipping(void);

void ScreenClear(void);

void ScreenRelease(void);

void ScreenPrint(int x, int y, char* string);
void Prison(int x1, int x2, int y1, int y2);