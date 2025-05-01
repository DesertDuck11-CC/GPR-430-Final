#include <utility>
#include "TicTacToe.h"

void DrawBoard(char board[3][3])
{
	Image boardImg = LoadImage("../../../images/board.png");
	ImageResize(&boardImg, 600, 600);

	//ImageDraw(&boardImg, boardImg, { 0.0f, 0.0f, 600.0f, 600.0f }, { 0.0f, 0.0f, 600.0f, 600.0f }, WHITE);
	Texture2D boardTexture = LoadTextureFromImage(boardImg);
	UnloadImage(boardImg);

	Image xImg = LoadImage("../../../images/x_img.png");
	ImageResize(&xImg, 174, 174);
	Texture2D xTexture = LoadTextureFromImage(xImg);
	UnloadImage(xImg);

	Image oImg = LoadImage("../../../images/o_img.png");
	ImageResize(&oImg, 174, 174);
	Texture2D oTexture = LoadTextureFromImage(oImg);
	UnloadImage(oImg);


	DrawTexture(boardTexture, 0, 0, WHITE);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if(board[i][j] == 'x')
				DrawTexture(xTexture, 13 + 200 * i, 13 + 200 * j, WHITE);
			else if(board[i][j] == 'o')
				DrawTexture(oTexture, 13 + 200 * i, 13 + 200 * j, WHITE);
		}
	}
}

std::pair<int, int> ReadInput(int row, int column)
{
	return std::pair(row / 200, column / 200);
}

char CheckForWin(char board[3][3])
{
	char initial;
	for (int i = 0; i < 3; i++) {
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
			return board[i][0];
		}
	}
	for (int i = 0; i < 3; i++) {
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
			return board[0][i];
		}
	}

	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
		return board[0][0];
	}
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
		return board[0][2];
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == ' ') return ' ';
		}
	}

	return 'd';
}

void DrawWinMessage(const char* message)
{
	int fontSize = 40;
	Color textColor = GREEN;

	int textWidth = MeasureText(message, fontSize);
	int textHeight = fontSize;

	int x = (GetScreenWidth() - textWidth) / 2;
	int y = (GetScreenHeight() - textHeight) / 2;

	DrawText(message, x, y, fontSize, textColor);
}
