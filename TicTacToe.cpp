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
