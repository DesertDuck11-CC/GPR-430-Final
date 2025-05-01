#pragma once
#include "raylib.h"

void DrawBoard(char board[3][3]);
std::pair<int, int> ReadInput(int row, int column);