#include <socklib.h>
#include <raylib.h>
#include "TicTacToe.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int PORT = 1234;
const std::string SERVER_ADDRESS = "127.0.0.1";

char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };

bool isMyTurn;

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

		if (!isMyTurn)
		{
			char buffer[sizeof(int) * 2];

			if (client.Recv(buffer, sizeof(buffer)) > 0)
			{
				int recvX = *(int*)(buffer);
				int recvY = *(int*)(buffer + sizeof(int));

				if (board[recvX][recvY] == ' ')
				{
					board[recvX][recvY] = 'o';
					isMyTurn = !isMyTurn;
				}
			}
		}
		else if (isMyTurn)
		{
			std::pair<int, int> input = { 2, 2 };

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMyTurn)
			{
				std::pair<int, int> input = ReadInput(GetMouseX(), GetMouseY());
				if (board[input.first][input.second] == ' ')
				{
					board[input.first][input.second] = 'x';
					isMyTurn = !isMyTurn;
				}
			}
			//if(input.first)
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

		

		if (isMyTurn)
		{
			std::pair<int, int> input = { 1, 1 };

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMyTurn)
			{
				input = ReadInput(GetMouseX(), GetMouseY());
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

				if (board[recvX][recvY] == ' ')
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
