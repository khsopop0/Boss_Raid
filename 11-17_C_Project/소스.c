#include"tool.h"

int COLS;
int LOWS;

// 윈도우 11에서 안됨1 유지보수 포기
void setWindowSize(int cols, int lows)
{
	COLS = cols;
	LOWS = lows;

	char buffer[128];
	sprintf(buffer,
		"mode con:cols=%d lines=%d",
		cols, lows);
	system(buffer);
}

// 윈도우 11에서 안됨2
void setFont(int size)
{
	CONSOLE_FONT_INFOEX cfi = {0};
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = size;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, TEXT("DotumChe"), &cfi);

	if (SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi)) {
		//MessageBox(NULL, TEXT("성공"), NULL, NULL);
	}
}

void gotoxy(int x, int y)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };

	//
	SetConsoleCursorPosition(hOutput, pos);
}

void clear()
{
	unsigned long dw;
	COORD pos = { 0, 0 };

	//
	FillConsoleOutputCharacter(
		GetStdHandle(STD_OUTPUT_HANDLE),
		' ',
		COLS * LOWS,
		pos,
		&dw
	);
	FillConsoleOutputAttribute(
		GetStdHandle(STD_OUTPUT_HANDLE),
		getColorBit(0, 1, 1, 1, 0),
		COLS * LOWS,
		pos,
		&dw
	);
	textColor(getColorBit(0, 1, 1, 1, 0));

	gotoxy(0, 0);
}

void textColor(int color)
{
	//
	SetConsoleTextAttribute(
		GetStdHandle(STD_OUTPUT_HANDLE),
		color
	);
}

int getColorBit(int background, int bright, int red, int green, int blue)
{
	return (bright << 3 | red << 2 | green << 1 | blue)
		<< background * 4;
}

static int g_nScreenIndex;
static HANDLE g_hScreen[2];

void ScreenInit(void)
{
	CONSOLE_CURSOR_INFO cci;

	//  2 .
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// .
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void ScreenFlipping(void)
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}

void ScreenClear(void)
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 320 * 84, Coor, &dw);
}

void ScreenRelease(void)
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

void ScreenPrint(int x, int y, char* string)
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}
