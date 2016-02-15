//needed includes for cross platform development
#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
//#include "SDL2_mixer/SDL_mixer.h"
//#include "SDL2_ttf/SDL_ttf.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
//#include "SDL2/SDL_mixer.h"
//#include "SDL2/SDL_ttf.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"
#endif

//needed includes
#include <string>
//#include <stdio.h>
#include <iostream>
//#include <sstream>
using namespace std;

class Explode
{
public:

	//Boolean for state of explosion
	bool active;

	//Explosion texture
	SDL_Texture *texture;

	//Explosion rectangle for position (X,Y) and size (W, H)
	SDL_Rect posRect;

	//Explosion rectangle for position (X,Y) and size (W, H)
	SDL_Rect drawRect;

	//width and height of each frame plus width and height of entire image
	int frameWidth, frameHeight, textureWidth, textureHeight;

	//float values to track time until next fram of animation
	float frameCounter;

	//Explode creation method. Requires the render, filepath, and x and y position
	Explode(SDL_Renderer *renderer, string filePath, float x, float y);

	//Explode Update - requires deltaTime be passed
	void Update(float deltaTime);

	//Explode Draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	//Explode reset
	void Reset();

	//Explode destruction method
	//~Explode();
};
