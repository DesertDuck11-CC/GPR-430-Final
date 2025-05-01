#include <socklib.h>
#include <raylib.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int PORT = 1234;
const std::string SERVER_ADDRESS = "127.0.0.1";

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
			InitServer();
		}
		else if (input == "c")
		{
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

	InitWindow(800, 600, "Player 1");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

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

	InitWindow(800, 600, "Player 2");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawFPS(20, 20);
		EndDrawing();
	}

	CloseWindow();
}
