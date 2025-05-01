#include <socklib.h>
#include <raylib.h>
#include "TicTacToe.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int PORT = 1234;
const std::string SERVER_ADDRESS = "127.0.0.1";

char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };

bool isMyTurn;
int first = 10, second = 10;

void InitServer();
void InitClient();

int main(int argc, char* argv[])
{
	SockLibInit();

	std::string input;

	while (input != "s" && input != "c")
	{
		std::cout << "Would you like to run a server or a client? (Input [s] or [c] respectively):\n";

		std::cin >> input;

		if (input == "s")
		{
			isMyTurn = true;
			InitServer();
		}
		else if (input == "c")
		{
			isMyTurn = false;
			InitClient();
		}
		else
		{
			std::cout << "Not a valid input!\n";
		}
	}


	SockLibShutdown();
	return 0;
}

void InitServer()
{
	std::cout << "Server Initted\n";

	Socket server(Socket::INET, Socket::STREAM);
	Address address("0.0.0.0", PORT);

	server.Bind(address);
	server.Listen();

	Socket client = server.Accept();
	client.SetNonBlockingMode(true);

	InitWindow(600, 600, "Player 1");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);
		
		DrawBoard(board);		

		char win = CheckForWin(board);
		if (win == 'o')
		{
			isMyTurn = true;
			DrawWinMessage("Player 2 Wins!\n'Space to Restart'");
		}
		else if (win == 'x')
		{
			isMyTurn = true;
			DrawWinMessage("Player 1 Wins!\n'Space to Restart'");
		}
		else if (win == 'd')
		{
			isMyTurn = true;
			DrawWinMessage("Draw!\n'Space to Restart'");
		}

		if (win != ' ')
		{
			std::pair<int, int> reset = { 3, 3 };

			if (IsKeyDown(KEY_SPACE))
			{
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						board[i][j] = ' ';
						win = ' ';
					}
				}

				reset = { 5, 5 };
			}

			client.SendAll((char*)&reset.first, sizeof(int));
			client.SendAll((char*)&reset.second, sizeof(int));
		}

		if (!isMyTurn)
		{
			char buffer[sizeof(int) * 2];

			if (client.Recv(buffer, sizeof(buffer)) > 0)
			{
				int recvX = *(int*)(buffer);
				int recvY = *(int*)(buffer + sizeof(int));

				if (recvX == 5 && recvY == 5)
				{
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							board[i][j] = ' ';
						}
					}
				}
				else if (recvX != 3 && recvY != 3 && board[recvX][recvY] == ' ')
				{
					board[recvX][recvY] = 'o';
					isMyTurn = !isMyTurn;
				}
			}
		}
		else if (isMyTurn)
		{
			std::pair<int, int> input = { 3, 3 };

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMyTurn)
			{
				input = ReadInput(GetMouseX(), GetMouseY());
				if (board[input.first][input.second] == ' ')
				{
					board[input.first][input.second] = 'x';
					isMyTurn = !isMyTurn;
				}
			}

			client.SendAll((char*)&input.first, sizeof(int));
			client.SendAll((char*)&input.second, sizeof(int));
		}

		DrawFPS(20, 20);
		EndDrawing();
	}

	CloseWindow();
}

void InitClient()
{
	std::cout << "Client Initted\n";

	Socket client(Socket::INET, Socket::STREAM);
	Address address(SERVER_ADDRESS, PORT);

	if (client.Connect(address) < 0)
	{
		std::cout << "Failed to connect!";
		return;
	}

	InitWindow(600, 600, "Player 2");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		DrawBoard(board);

		char win = CheckForWin(board);
		if (win == 'o')
		{
			isMyTurn = false;
			DrawWinMessage("Player 2 Wins!\n'Space to Restart'");
		}
		else if (win == 'x')
		{
			isMyTurn = false;
			DrawWinMessage("Player 1 Wins!\n'Space to Restart'");
		}
		else if (win == 'd')
		{
			isMyTurn = false;
			DrawWinMessage("Draw!\n'Space to Restart'");
		}

		/*if (win != ' ')
		{
			if (IsKeyDown(KEY_SPACE))
			{
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						board[i][j] = ' ';
						win = ' ';
					}
				}

				std::pair<int, int> reset = { 5, 5 };

				client.Send((char*)&reset.first, sizeof(int));
				client.Send((char*)&reset.second, sizeof(int));
			}
		}*/

		if (isMyTurn)
		{
			std::pair<int, int> input = { 3, 3 };

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMyTurn)
			{
				input = ReadInput(GetMouseX(), GetMouseY());
				std::cout << input.first;
				std::cout << input.second;
				if (board[input.first][input.second] == ' ')
				{
					board[input.first][input.second] = 'o';
					isMyTurn = !isMyTurn;
				}
			}


			client.Send((char*)&input.first, sizeof(input.first));
			client.Send((char*)&input.second, sizeof(input.second));
		}
		else if (!isMyTurn)
		{
			char buffer[sizeof(int) * 2];
			int recvBytes = client.Recv(buffer, sizeof(buffer));

			if (recvBytes > 0)
			{
				int recvX = *(int*)(buffer);
				int recvY = *(int*)(buffer + sizeof(int));

				if (recvX == 5 && recvY == 5)
				{
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							board[i][j] = ' ';
						}
					}
				}
				else if (recvX != 3 && recvY != 3 && board[recvX][recvY] == ' ')
				{
					board[recvX][recvY] = 'x';
					isMyTurn = !isMyTurn;
				}
			}
		}

		DrawFPS(20, 20);
		EndDrawing();
	}

	CloseWindow();
	
}
