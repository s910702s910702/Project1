#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int TILE_SIZE = 40;


SDL_Renderer * renderer = NULL;
SDL_Window * window = NULL;

SDL_Texture * LoadImage(string file) {
	SDL_Surface * image = IMG_Load(file.c_str());
	SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, image);;
	SDL_FreeSurface(image);
	return tex;
}


void ApplySurface(int x, int y, SDL_Texture * tex, SDL_Renderer * rend) {
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);

	SDL_RenderCopy(rend, tex, NULL, &pos);
}

void Halo(int x, int y, int radius, double theta, SDL_Texture * circle, SDL_Renderer * rend) {
	int placex = x + radius * cos(theta);
	int placey = y + radius * sin(theta);
	ApplySurface(placex, placey, circle, rend);
}

int main(int argc, char ** argv) {
	//for fps//
	Uint32 timeA = 0, timeB, diff, fps;

	//initial//
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cout << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow("L_3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << SDL_GetError() << endl;
		return 2;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cout << SDL_GetError() << endl;
		return 3;
	}

	SDL_Texture * background = NULL, *image = NULL, *earth = NULL, *moon = NULL;
	try {
		background = LoadImage("material/Games/background.png");
		image = LoadImage("material/Games/big_circle.png");
		earth = LoadImage("material/Games/small_circle.png");
		moon = LoadImage("material/Games/moon.png");
	}
	catch (const runtime_error &e) {
		cout << e.what() << endl;

		return 4;
	}

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				quit = false;
				break;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				quit = false;
				break;
			}
		}
		
		SDL_RenderClear(renderer);
		
		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		ApplySurface(0, 0, background, renderer);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = 100;
		//int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		ApplySurface(x, y, image, renderer);

		int eW, eH;
		SDL_QueryTexture(earth, NULL, NULL, &eW, &eH);
		double nowtime = SDL_GetTicks();
		int eCx = x + (iW >> 1) - (eW >> 1), eCy = y + (iH >> 1) - (eH >> 1);
		Halo(eCx, eCy, (iW >> 1), nowtime / 1500, earth, renderer);

		int mW, mH;
		SDL_QueryTexture(moon, NULL, NULL, &mW, &mH);
		nowtime = SDL_GetTicks();
		int mCx = eCx - (mW >> 1), mCy = eCy - (mH >> 1);
		Halo(mCx, mCy, (eW >> 1), nowtime / 800, moon, renderer);

		timeB = SDL_GetTicks();
		diff = timeB - timeA;
		fps = 1000 / diff;
		timeA = timeB;
		stringstream fps_ss;
		fps_ss << "FPS: " << fps;
		
		SDL_SetWindowTitle(window, fps_ss.str().c_str());
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	SDL_Quit();
	return 0;
}