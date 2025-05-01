#include "TicTacToe.h"

void DrawBoard(char board[3][3])
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Color color = (board[i][j] == 'X') ? RED : (board[i][j] == 'O') ? BLUE : GRAY;
			DrawRectangle(j * 200, i * 200, 200, 200, color);
			DrawText(&board[i][j], j * 200 + 70, i * 200 + 70, 100, BLACK);
		}
	}
}
