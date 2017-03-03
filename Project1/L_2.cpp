#include <SDL.h>
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;

using namespace std;


SDL_Texture * LoadImage(string file) {
	SDL_Surface * loadedImage = NULL;
	SDL_Texture * texture = NULL;

	loadedImage = SDL_LoadBMP(file.c_str());

	if (loadedImage != NULL) {
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	else cout << SDL_GetError() << endl;

	return texture;
}


void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend) {
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);

	SDL_RenderCopy(rend, tex, NULL, &pos);
}


int main(int argc, char ** argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow("L_2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << SDL_GetError() << endl;
		return 2;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << SDL_GetError() << endl;
		return 3;
	}

	SDL_Texture * background = NULL, *image = NULL;
	background = LoadImage("material/background.bmp");
	image = LoadImage("material/image.bmp");
	if (background == NULL || image == NULL) {
		return 4;
	}

	SDL_RenderClear(renderer);

	int bW, bH;
	SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
	for (int i = 0; i < SCREEN_WIDTH / bW; i++) {
		for (int j = 0; j < SCREEN_HEIGHT / bH; j++) {
			ApplySurface(i * bW, j * bH, background, renderer);
		}
	}

	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	ApplySurface(x, y, image, renderer);


	SDL_RenderPresent(renderer);
	SDL_Delay(2000);


	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}