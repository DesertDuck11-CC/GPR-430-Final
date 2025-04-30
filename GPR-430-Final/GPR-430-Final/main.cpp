#include <SDL.h>
#include <stdio.h>
#include <winsock.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not init SDL!");
	}
	else
	{
		window = SDL_CreateWindow("WINDOW TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL)
		{
			printf("Window not created");
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
			SDL_UpdateWindowSurface(window);

			SDL_Event e;
			bool quit = false;
			while (!quit)
			{
				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
			}
		}

		SDL_DestroyWindow(window);

		SDL_Quit();

		return 0;
	}
}
