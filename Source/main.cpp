// Example program:
// Using SDL2 to create an application window

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#inlcude "SDL2_mixer/SDL_mixer.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#inlcude "SDL2/SDL_mixer.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#endif


#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#if defined(__linux__)
#include <unistd.h>
#endif


#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

// code for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//set speed for background
int bkgdSpeed = 100;
//set temp variables to hold movement - background 1
float BG1pos_X=0, BG1pos_Y=0;
//set temp variables to hold movement - background 2
float BG2pos_X=0, BG2pos_Y= -768;

SDL_Rect bkgd1Pos;
SDL_Rect bkgd2Pos;

//move the background
void UpdateBackground(float deltaTime)
{
	//Update Background 1
	BG1pos_Y += (bkgdSpeed * 1)*deltaTime;

	//set the new bkgd1 position
	bkgd1Pos.y = (int)(BG1pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y = -768;
		BG1pos_Y = bkgd1Pos.y;
	}

	//Update Background 2
	BG2pos_Y += (bkgdSpeed * 1)*deltaTime;

	//set the new bkgd2 position
	bkgd2Pos.y = (int)(BG2pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y = -768;
		BG2pos_Y = bkgd2Pos.y;
	}
}
//*************END OF UPDATEBACKGROUND**********


//new joystick vars
//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE =8000;

// joystick direction vars
float xDir, yDir;

// cursor float vars for movement
float pos_X, pos_Y;

//create SDL Rectangle for the 2 player graphic
SDL_Rect cursorPos, activePos;

//var from cursor speed
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event)
{
	// check joystick 0 - first player
	if(event.which == 0)
	{
		// check X axis
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}else
			{
				xDir = 0.0f;
			}
		}

		// check Y axis
		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;
			}else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}else
			{
				yDir = 0.0f;
			}
		}
	}
}
//**********END OF MOVECURSOR

//update cursor on screen
void UpdateCursor(float deltaTime)
{

	//Update cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	//assign to SDL_Rect ints X and Y
	cursorPos.x = (int) (pos_X + 0.5f);
	cursorPos.y = (int) (pos_Y + 0.5f);

	//update active position of cursor - collision box
	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	//off the screen in X
	if(cursorPos.x < 0)
	{
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}
	if(cursorPos.x > 1024 - cursorPos.w)
	{
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	//off the screen in Y
	if(cursorPos.y < 0)
	{
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}
	if(cursorPos.y > 768 - cursorPos.h)
	{
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}
//*************END OF UPDATECURSOR************



bool players1Over = false, players2Over = false, instructionsOver = false,
	quitOver = false, menuOver = false, playOver = false;

//class header includes
#include "player.h"
#include "enemy.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "explode.h"


//variable list to hold the list of enemies; for 1 player game - 6 total, for 2 player game - 12 total
vector<Enemy> enemyList; 

vector<Explode> explodeList;

void MakeExplosion(int x, int y)
{
	//see if there is a explosion not active to use
	for (int i = 0; i < explodeList.size(); i++)
	{
		//see if the bullet is not active
		if (explodeList[i].active == false)
		{
			//set explosion to active
			explodeList[i].active = true;

			//use some math in the x position to get the explosion close to
			//the center of the player using player width
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			//once explosion is found, break out of loop
			break;
		}
	}
}





//**********************************MAIN - START***************************************
int main(int argc, char* argv[]) {

	/* initialize random seed; */
		srand(time(NULL));

//Output for Apple
#if defined(__APPLE__)
	//get the current working directory
	string s_cwd(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

	//create a string to link to the audio folder on Mac
	string audio_dir = s_cwd + "/Resources/Audio/";
#endif

//Output for Linux
#if defined(__linux__)
//get the current working directory
	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the linux's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

	//create a string to link to the audio folder on Linux
	string audio_dir = s_cwd + "/Resources/Audio/";
#endif

//Output for Windows
#if defined(_WIN32) || (_WIN64)
//get the current working directory
string s_cwd(getcwd(NULL, 0));

//create a string linking to the Windows images folder
string s_cwd_images = s_cwd + "\\Resources\\Images\\";

//create a string to link to the audio folder on Windows
string audio_dir = s_cwd + "\\Resources\\Audio\\";

#endif

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    //declare the renderer
    SDL_Renderer *renderer = NULL;

    // create the renderer
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    //****** Create Background ******
    string BKGDpath = s_cwd_images + "Bacground.png";

    // create a SDL surface to hold the background image
    SDL_Surface *surface = IMG_Load(BKGDpath.c_str());

    // create a SDL texture
    SDL_Texture *bkgd1;

    //place surface into the texture bkgd1
    bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);


    //create a SDL texture - background 2
    SDL_Texture *bkgd2;

    //place surface into the texture bkgd1
    bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);


    // set the X,Y,W, and H for the Rectangle
    bkgd1Pos.x=0;
    bkgd1Pos.y=0;
    bkgd1Pos.w=1024;
    bkgd1Pos.h=768;


    // set the X,Y,W, and H for the Rectangle
    bkgd2Pos.x=0;
    bkgd2Pos.y=-768;
    bkgd2Pos.w=1024;
    bkgd2Pos.h=768;

    //*****************Backgrounds - END********************//


    //***************** Create CURSOR*****************
    //create cursor
    string CURSORpath = s_cwd_images + "CursorNew.png";

    // create a SDL surface to hold the background image
    surface = IMG_Load(CURSORpath.c_str());

    // create a SDL texture
    SDL_Texture *cursor;

    //place surface into the texture cursor
    cursor = SDL_CreateTextureFromSurface(renderer, surface);


    //free the SDL surface
    SDL_FreeSurface(surface);

    // create the SDL_Rectangle for the texture's position and size - x,y,w,h
    //SDL_Rect cursorPos, activePos;

    // set the X,Y,W, and H for the Rectangle - cursor graphic
    cursorPos.x=0;
    cursorPos.y=0;
    cursorPos.w=30;
    cursorPos.h=28;

    // set the X,Y,W, and H for the Rectangle - cursor graphic
    activePos.x=10;
    activePos.y=10;
    activePos.w=10;
    activePos.h=10;

    //*************CURSOR - END**************


    //***************** Create Menu - START *****************

    // ****************** TITLE - START****************
    string Titlepath = s_cwd_images + "Title.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Titlepath.c_str());

    // create a SDL texture
    SDL_Texture *title;

    //place surface into the texture title
    title = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    // create SDL Rectangle for the title graphic
    SDL_Rect titlePos;


    // set the X,Y,W, and H for the Rectangle
    titlePos.x=331;
    titlePos.y=61;
    titlePos.w=335;
    titlePos.h=76;

    //********TITLE - END***********


    // ********* ONE PLAYER - START******

    string Ply1path = s_cwd_images + "OnePlayer.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Ply1path.c_str());

    // create a SDL texture
    SDL_Texture *players1N;

    //place surface into the texture title
    players1N = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    string Ply1Opath = s_cwd_images + "OnePlayerO.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Ply1Opath.c_str());

    // create a SDL texture
    SDL_Texture *players1O;

    //place surface into the texture title
    players1O = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    // create SDL Rectangle for the title graphic
    SDL_Rect players1NPos;


    // set the X,Y,W, and H for the Rectangle
    players1NPos.x=394;
    players1NPos.y=266;
    players1NPos.w=203;
    players1NPos.h=49;

    //********ONE PLAYER - END***********


    // ********* TWO PLAYER - START******

    string Ply2path = s_cwd_images + "TwoPlayer.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Ply2path.c_str());

    // create a SDL texture
    SDL_Texture *players2N;

    //place surface into the texture title
    players2N = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    string Ply2Opath = s_cwd_images + "TwoPlayerO.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Ply2Opath.c_str());

    // create a SDL texture
    SDL_Texture *players2O;

    //place surface into the texture title
    players2O = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    // create SDL Rectangle for the title graphic
    SDL_Rect players2NPos;


    // set the X,Y,W, and H for the Rectangle
    players2NPos.x=394;
    players2NPos.y=355;
    players2NPos.w=206;
    players2NPos.h=49;

    //********TWO PLAYER - END***********

    // ********* INSTRUCTIONS - START******

    string Instructionspath = s_cwd_images + "Instructions.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Instructionspath.c_str());

    // create a SDL texture
    SDL_Texture *InstructionsN;

    //place surface into the texture title
    InstructionsN = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    string InstructionsOpath = s_cwd_images + "InstructionsO.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(InstructionsOpath.c_str());

    // create a SDL texture
    SDL_Texture *InstructionsO;

    //place surface into the texture title
    InstructionsO = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    // create SDL Rectangle for the title graphic
    SDL_Rect InstructionsNPos;


    // set the X,Y,W, and H for the Rectangle
    InstructionsNPos.x=377;
    InstructionsNPos.y=439;
    InstructionsNPos.w=238;
    InstructionsNPos.h=41;

    //********INSTRUCTIONS - END***********

    // ********* QUIT - START******

     string Quitpath = s_cwd_images + "Quit.png";

     // create a SDL surface to hold the title image
     surface = IMG_Load(Quitpath.c_str());

     // create a SDL texture
     SDL_Texture *QuitN;

     //place surface into the texture title
     QuitN = SDL_CreateTextureFromSurface(renderer, surface);

     //free the SDL surface
     SDL_FreeSurface(surface);

     string QuitOpath = s_cwd_images + "QuitO.png";

     // create a SDL surface to hold the title image
     surface = IMG_Load(QuitOpath.c_str());

     // create a SDL texture
     SDL_Texture *QuitO;

     //place surface into the texture title
     QuitO = SDL_CreateTextureFromSurface(renderer, surface);

     //free the SDL surface
     SDL_FreeSurface(surface);

     // create SDL Rectangle for the title graphic
     SDL_Rect QuitNPos;


     // set the X,Y,W, and H for the Rectangle
     QuitNPos.x=455;
     QuitNPos.y=548;
     QuitNPos.w=82;
     QuitNPos.h=43;

     //********Quit - END***********


    //*****************CREATE MENU - END********************


     //****** Create INSTRUCTIONS GRAPHICS - START **********

     // ********* TEXT - START******
     string InstructionsTextpath = s_cwd_images + "InstructionsText.png";

     // create a SDL surface to hold the title image
     surface = IMG_Load(InstructionsTextpath.c_str());

     // create a SDL texture
     SDL_Texture *InstText;

     //place surface into the texture title
     InstText = SDL_CreateTextureFromSurface(renderer, surface);

     //free the SDL surface
     SDL_FreeSurface(surface);

     // create SDL Rectangle for the title graphic
     SDL_Rect InstTextPos;


     // set the X,Y,W, and H for the Rectangle
     InstTextPos.x=233;
     InstTextPos.y=253;
     InstTextPos.w=539;
     InstTextPos.h=215;

     //********TEXT - END***********


	 //****** Create INSTRUCTIONS GRAPHICS - START **********

	 // ********* TEXT - START******
	 string Menupath = s_cwd_images + "Menu.png";

	 // create a SDL surface to hold the title image
	 surface = IMG_Load(Menupath.c_str());

	 // create a SDL texture
	 SDL_Texture *Menu;

	 //place surface into the texture title
	 Menu = SDL_CreateTextureFromSurface(renderer, surface);

	 //free the SDL surface
	 SDL_FreeSurface(surface);


	 string MenuOpath = s_cwd_images + "MenuO.png";

	 // create a SDL surface to hold the title image
	 surface = IMG_Load(MenuOpath.c_str());

	 // create a SDL texture
	 SDL_Texture *MenuO;

	 //place surface into the texture title
	 MenuO = SDL_CreateTextureFromSurface(renderer, surface);

	 //free the SDL surface
	 SDL_FreeSurface(surface);

	 // create SDL Rectangle for the title graphic
	 SDL_Rect MenuPos;


	 // set the X,Y,W, and H for the Rectangle
	 MenuPos.x = 128;
	 MenuPos.y = 659;
	 MenuPos.w = 227;
	 MenuPos.h = 41;

	 //********TEXT - END***********

	 // ********* PLAY AGAIN - START******
	 string Playpath = s_cwd_images + "PlayAgain.png";

	 // create a SDL surface to hold the title image
	 surface = IMG_Load(Playpath.c_str());

	 // create a SDL texture
	 SDL_Texture *Play;

	 //place surface into the texture title
	 Play = SDL_CreateTextureFromSurface(renderer, surface);

	 //free the SDL surface
	 SDL_FreeSurface(surface);


	 string PlayOpath = s_cwd_images + "PlayAgainO.png";

	 // create a SDL surface to hold the title image
	 surface = IMG_Load(PlayOpath.c_str());

	 // create a SDL texture
	 SDL_Texture *PlayO;

	 //place surface into the texture title
	 PlayO = SDL_CreateTextureFromSurface(renderer, surface);

	 //free the SDL surface
	 SDL_FreeSurface(surface);

	 // create SDL Rectangle for the title graphic
	 SDL_Rect PlayPos;


	 // set the X,Y,W, and H for the Rectangle
	 PlayPos.x = 644;
	 PlayPos.y = 659;
	 PlayPos.w = 205;
	 PlayPos.h = 49;

	 //********PLAY AGAIN - END***********

     //****** Create INSTRUCTIONS GRAPHICS - END **********


	 //****** Create WIN GRAPHICS - START **********

	 // ********* TEXT - START******
	 string WinTextpath = s_cwd_images + "WinText.png";

	 // create a SDL surface to hold the title image
	 surface = IMG_Load(WinTextpath.c_str());

	 // create a SDL texture
	 SDL_Texture *WinText;

	 //place surface into the texture title
	 WinText = SDL_CreateTextureFromSurface(renderer, surface);

	 //free the SDL surface
	 SDL_FreeSurface(surface);

	 // create SDL Rectangle for the title graphic
	 SDL_Rect WinTextPos;


	 // set the X,Y,W, and H for the Rectangle
	 WinTextPos.x = 317;
	 WinTextPos.y = 268;
	 WinTextPos.w = 379;
	 WinTextPos.h = 106;

	 //********TEXT - END***********

	 //****** Create WIN GRAPHICS - END **********


	 //****** Create LOSE GRAPHICS - START **********

	 // ********* TEXT - START******
	 string LoseTextpath = s_cwd_images + "LoseText.png";

	 // create a SDL surface to hold the title image
	 surface = IMG_Load(LoseTextpath.c_str());

	 // create a SDL texture
	 SDL_Texture *LoseText;

	 //place surface into the texture title
	 LoseText = SDL_CreateTextureFromSurface(renderer, surface);

	 //free the SDL surface
	 SDL_FreeSurface(surface);

	 // create SDL Rectangle for the title graphic
	 SDL_Rect LoseTextPos;


	 // set the X,Y,W, and H for the Rectangle
	 LoseTextPos.x = 264;
	 LoseTextPos.y = 272;
	 LoseTextPos.w = 507;
	 LoseTextPos.h = 99;

	 //********TEXT - END***********

	 //****** Create LOSE GRAPHICS - END **********

		//***** Turn on Game Controller Events
		SDL_GameControllerEventState(SDL_ENABLE);

	//******* set up a Game Controller variable - 2 *****//
	SDL_GameController* gGameController0 = NULL;

	//****** Open Game Controller - 1*****
	gGameController0 = SDL_GameControllerOpen(0);

	//******* set up a Game Controller variable - 2 *****//
	SDL_GameController* gGameController1 = NULL;

	//****** Open Game Controller - 2*****
	gGameController1 = SDL_GameControllerOpen(1);



	//***** SDL Event to handle input****
	SDL_Event event;

	//***** set up variables for the game states*****
	enum GameState{MENU,INSTRUCTIONS,PLAYERS1,PLAYERS2,WIN,LOSE};

	// set up the initial state
	GameState gameState = MENU;

	//boolean values to control movement through the states****
	bool menu, instructions, players1, players2, win, lose, quit = false;
	


	////////////////**********************AUDIO SET UP*******************************

	//Open Audio Channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load a MUSIC file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

	if (bgm == NULL) {
		// In the case that the window could not be made...
		printf("Could not create music: %s\n", SDL_GetError());
		return 1;
	}

	//if the MUSIC file is not playing - play it
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	//Set up a Sound Effect CHUNK for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	//Set up a Sound Effect CHUNK for the button pressed state
	Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	//Set up a Sound Effect CHUNK for collision
	Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "enemy.wav").c_str());

	//bool value to control the over sound effect and the buttons
	bool alreadyOver = false;

	//create players
	Player player1 = Player(renderer, 0, s_cwd_images.c_str(), audio_dir.c_str(), 250.0, 500.0);
	Player player2 = Player(renderer, 1, s_cwd_images.c_str(), audio_dir.c_str(), 750.0, 500.0);

	//Create a pool of explosions - 20
	for (int i = 0; i < 20; i++)
	{
		//create the explosion
		Explode tmpExplode(renderer, s_cwd_images, -1000, -1000);

		//add to enemyList
		explodeList.push_back(tmpExplode); 
	}
    // The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)
	{
		switch(gameState)
		{
			case MENU:

				alreadyOver = false;
				menu = true;

				while(menu)
				{
					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;






					// check for input events
					if(SDL_PollEvent(&event))
					{
						//check to see if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							menu = false;
							break;

						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									//if player chooses 1 player game
									if(players1Over)
									{
										//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
										Mix_PlayChannel(-1, pressedSound, 0);
										menu = false;
										gameState = PLAYERS1;
										players1Over = false;
									}
									//if player chooses 2 player game
									if(players2Over)
									{
										//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
										Mix_PlayChannel(-1, pressedSound, 0);
										menu = false;
										gameState = PLAYERS2;
										players2Over = false;
									}
									//if player chooses instructions
									if(instructionsOver)
									{
										//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
										Mix_PlayChannel(-1, pressedSound, 0);
										menu = false;
										gameState = INSTRUCTIONS;
										instructionsOver = false;
									}
									//if player chooses quit
									if(quitOver)
									{
										//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
										Mix_PlayChannel(-1, pressedSound, 0);
										//Add a slight delay
										SDL_Delay(200); 
										menu = false;
										quit = true;
										quitOver = false;
									}
								}
							}
							break;
						case SDL_CONTROLLERAXISMOTION:
							moveCursor(event.caxis);
							break;
						}
					}


					//Update
					UpdateBackground(deltaTime);

					//Update Cursor
					UpdateCursor(deltaTime);


					//check for collision between cursor active state and buttons
					players1Over = SDL_HasIntersection(&activePos, &players1NPos);
					players2Over = SDL_HasIntersection(&activePos, &players2NPos);
					instructionsOver = SDL_HasIntersection(&activePos, &InstructionsNPos);
					quitOver = SDL_HasIntersection(&activePos, &QuitNPos);

					//If the cursor is over a button, play the over sound
					if (players1Over || players2Over || instructionsOver || quitOver) 
					{
						if (alreadyOver == false) 
						{
							Mix_PlayChannel(-1, overSound, 0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over ANY button, reset the already Over var
					if (!players1Over && !players2Over && !instructionsOver && !quitOver)
					{
						alreadyOver = false;
					}

					// Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);




					//Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title image
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw One Player image
					if(players1Over)
					{
						SDL_RenderCopy(renderer, players1O, NULL, &players1NPos);
					}else
					{
					SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);
					}
					//Draw Two Player image
					if(players2Over)
					{
						SDL_RenderCopy(renderer, players2O, NULL, &players2NPos);
					}else
					{
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);
					}

					//Draw Instructions image
					if(instructionsOver)
					{
						SDL_RenderCopy(renderer, InstructionsO, NULL, &InstructionsNPos);
					}else
					{
					SDL_RenderCopy(renderer, InstructionsN, NULL, &InstructionsNPos);
					}


					//Draw Quit image
					if(quitOver)
					{
						SDL_RenderCopy(renderer, QuitO, NULL, &QuitNPos);
					}else
					{
					SDL_RenderCopy(renderer, QuitN, NULL, &QuitNPos);
					}


					//Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


					// SDL Render present
					SDL_RenderPresent(renderer);


				}

				break;	// end menu case

			case INSTRUCTIONS:

				alreadyOver = false;

				instructions = true;

				while(instructions)
				{


					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;



					// check for input events
					if(SDL_PollEvent(&event))
					{
						//check to se if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							instructions = false;
							break;

						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								//if player chooses menu
								if(menuOver)
								{
								//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
								Mix_PlayChannel(-1, pressedSound, 0);
								instructions = false;
								gameState = MENU;
								menuOver = false;
								}
							}
							break;
						case SDL_CONTROLLERAXISMOTION:
							moveCursor(event.caxis);
							break;
						}
					}

					//Update
					UpdateBackground(deltaTime);

					//Update Cursor
					UpdateCursor(deltaTime);

					//check for cursor intersection with menu button
					menuOver = SDL_HasIntersection(&activePos, &MenuPos);


					//If the cursor is over a button, play the over sound
					if (menuOver)
					{
						if (alreadyOver == false)
						{
							Mix_PlayChannel(-1, overSound, 0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over ANY button, reset the already Over var
					if (!menuOver)
					{
						alreadyOver = false;
					}


					// Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);




					//Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title image
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw the Instructions Text image
					SDL_RenderCopy(renderer, InstText, NULL, &InstTextPos);

					//Draw Menu image
					if(menuOver)
					{
						SDL_RenderCopy(renderer, MenuO, NULL, &MenuPos);
					}else
					{
					SDL_RenderCopy(renderer, Menu, NULL, &MenuPos);
					}

					//Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


					// SDL Render present
					SDL_RenderPresent(renderer);


				}

				break;	// end instructions case


			case PLAYERS1:

				enemyList.clear();

				//reset the player
				player1.Reset();

				players1 = true;

				//Create the enemy
				for (int i = 0; i < 6; i++)
				{
					//create the enemy
					Enemy tmpEnemy(renderer, s_cwd_images);

					//add to enemyList
					enemyList.push_back(tmpEnemy);

				}

				while(players1)
				{


					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event))
					{
						//check to se if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							players1 = false;
							break;
						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
								{
									players1 = false;
									gameState = WIN;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
								{
									players1 = false;
									gameState = LOSE;
								}

								//send button press info to player1
								if(player1.active)
								player1.OnControllerButton(event.cbutton);
							}
							break;
						case SDL_CONTROLLERAXISMOTION:
							if(player1.active)
							player1.OnControllerAxis(event.caxis);
							break;
						}
					}


					//Update
					UpdateBackground(deltaTime);

					//Update player
					if(player1.active)
					player1.Update(deltaTime, renderer);

					//Update Enemies
					for (int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Update(deltaTime);
					}

					//only check if the player is active
					if (player1.active)
					{
						//use the player 1 bullet list to see if the active bullets
						//hit any of the enemies
						for (int i = 0; i < player1.bulletList.size(); i++)
						{
							//check to see if this bullet is active(onscreen)
							if (player1.bulletList[i].active == true)
							{
								//check all enemies against the active bullet
								for (int j = 0; j < enemyList.size(); j++)
								{
									//if there is a collision between the two objects
									if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
									{
										//play explosion sound
										Mix_PlayChannel(-1, explosionSound, 0);

										MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

										//reset the enemy
										enemyList[j].Reset();

										//reset the bullet 
										player1.bulletList[i].Reset();

										//give the player some points
										player1.playerScore += 50;

										//check to see if there is a winning condition
										if (player1.playerScore >= 1000)
										{
											//go to win
											players1 = false;
											gameState = WIN;
										}
									}
								}
							}
						}

						//check to see if the enemies hit the player
						for (int i = 0; i < enemyList.size(); i++)
						{
							//if there is a collision between the two objects
							if (SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect))
							{
								//play explosion sound
								Mix_PlayChannel(-1, explosionSound, 0);

								MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

								//reset the enemy
								enemyList[i].Reset();

								//take life away
								player1.playerLives -= 1;

								//if game over - player lives = 0
								if (player1.playerLives <= 0)
								{
									players1 = false;
									gameState = LOSE;
									break;
								}
							}
						}
					}

					////////////////******************player 1 active check ends***********

					//Update a pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Update(deltaTime);
						}
					}


					// Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);




					//Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw Enemies
					for (int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Draw(renderer);
					}

					//Draw the title image
					//SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw player image
					player1.Draw(renderer);

					//Update a pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Draw(renderer);
						}
					}


					// SDL Render present
					SDL_RenderPresent(renderer);
				}

				break;	// end players1 case

			case PLAYERS2:

				enemyList.clear();

				//Reset players 
				player1.Reset();
				player2.Reset();

				players2 = true;

				//Create the enemy
				for (int i = 0; i < 12; i++)
				{
					//create the enemy
					Enemy tmpEnemy(renderer, s_cwd_images);

					//add to enemyList
					enemyList.push_back(tmpEnemy);

				}

				while(players2)
				{


					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event))
					{
						//check to se if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							players2 = false;
							break;

						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cdevice.which == 0 || event.cdevice.which == 1)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
								{
									players2 = false;
									gameState = WIN;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
								{
									players2 = false;
									gameState = LOSE;
								}
							}
							// send button press info to player 1
							if(player1.active)
							player1.OnControllerButton(event.cbutton);

							// send button press info to player 2
							if(player2.active)
							player2.OnControllerButton(event.cbutton);
							break;
						case SDL_CONTROLLERAXISMOTION:
							//send axis info to player 1
							if(player1.active)
							player1.OnControllerAxis(event.caxis);

							//send axis info to player 2
							if(player2.active)
							player2.OnControllerAxis(event.caxis);
							break;

						}
					}


					//Update
					UpdateBackground(deltaTime);

					//Update players
					if(player1.active)
					player1.Update(deltaTime, renderer);

					if(player2.active)
					player2.Update(deltaTime, renderer);

					//Update Enemies
					for (int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Update(deltaTime);
					}


					//only check if the player is active
					if (player1.active)
					{
						//use the player 1 bullet list to see if the active bullets
						//hit any of the enemies
						for (int i = 0; i < player1.bulletList.size(); i++)
						{
							//check to see if this bullet is active(onscreen)
							if (player1.bulletList[i].active == true)
							{
								//check all enemies against the active bullet
								for (int j = 0; j < enemyList.size(); j++)
								{
									//if there is a collision between the two objects
									if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
									{
										//play explosion sound
										Mix_PlayChannel(-1, explosionSound, 0);

										MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

										//reset the enemy
										enemyList[j].Reset();

										//reset the bullet 
										player1.bulletList[i].Reset();

										//give the player some points
										player1.playerScore += 50;

										//check to see if there is a winning condition
										if (player1.playerScore >= 1000)
										{
											//go to win
											players2 = false;
											gameState = WIN;
										}
									}
								}
							}
						}

						//check to see if the enemies hit the player
						for (int i = 0; i < enemyList.size(); i++)
						{
							//if there is a collision between the two objects
							if (SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect))
							{
								//play explosion sound
								Mix_PlayChannel(-1, explosionSound, 0);

								MakeExplosion(player1.posRect.x - 32, player1.posRect.y - 32);

								//reset the enemy
								enemyList[i].Reset();

								//take life away
								player1.playerLives -= 1;

								//if game over - player lives = 0
								if (player1.playerLives <= 0 && player2.playerLives <= 0)
								{
									players2 = false;
									gameState = LOSE;
									break;
								}
							}
						}
					}

					////////////////******************player 1 active check ends***********

					//only check if the player is active
					if (player2.active)
					{
						//use the player 2 bullet list to see if the active bullets
						//hit any of the enemies
						for (int i = 0; i < player1.bulletList.size(); i++)
						{
							//check to see if this bullet is active(onscreen)
							if (player2.bulletList[i].active == true)
							{
								//check all enemies against the active bullet
								for (int j = 0; j < enemyList.size(); j++)
								{
									//if there is a collision between the two objects
									if (SDL_HasIntersection(&player2.bulletList[i].posRect, &enemyList[j].posRect))
									{
										//play explosion sound
										Mix_PlayChannel(-1, explosionSound, 0);

										MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

										//reset the enemy
										enemyList[j].Reset();

										//reset the bullet 
										player2.bulletList[i].Reset();

										//give the player some points
										player2.playerScore += 50;

										//check to see if there is a winning condition
										if (player1.playerScore >= 1000)
										{
											//go to win
											players2 = false;
											gameState = WIN;
										}
									}
								}
							}
						}

						//check to see if the enemies hit the player
						for (int i = 0; i < enemyList.size(); i++)
						{
							//if there is a collision between the two objects
							if (SDL_HasIntersection(&player2.posRect, &enemyList[i].posRect))
							{
								//play explosion sound
								Mix_PlayChannel(-1, explosionSound, 0);

								MakeExplosion(player2.posRect.x - 32, player2.posRect.y - 32);

								//reset the enemy
								enemyList[i].Reset();

								//take life away 
								player2.playerLives -= 1;

								//if game over - player lives = 0
								if (player1.playerLives <= 0 && player2.playerLives <= 0)
								{
									players2  = false;
									gameState = LOSE;
									break;
								}
							}
						}
					}

					////////////////******************player 2 active check ends***********

					//Update a pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Update(deltaTime);
						}
					}
					 
					// Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title image
					//SDL_RenderCopy(renderer, title, NULL, &titlePos);
				
					//Draw Enemies
					for (int i = 0; i < enemyList.size(); i++)
					{
						//update enemy
						enemyList[i].Draw(renderer);
					}

					//Draw the players
					player1.Draw(renderer);

					player2.Draw(renderer);

					//Update a pool of explosions - 20
					for (int i = 0; i < explodeList.size(); i++)
					{
						//check to see if active
						if (explodeList[i].active == true)
						{
							//draw explode
							explodeList[i].Draw(renderer);
						}
					}


					// SDL Render present
					SDL_RenderPresent(renderer);
				}

				break;	// end players2 case

			case WIN:

				alreadyOver = false;

				win = true;

				while(win)
				{


					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event))
					{
						//check to se if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							win = false;
							break;

						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								//if player chooses menu
								if(menuOver)
								{
									//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
									Mix_PlayChannel(-1, pressedSound, 0);
									win = false;
									gameState = MENU;
									menuOver = false;
								}
								if (playOver)
								{
									//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
									Mix_PlayChannel(-1, pressedSound, 0);
									win = false;
									gameState = PLAYERS1;
									playOver = false;
								}
							}
							break;
						case SDL_CONTROLLERAXISMOTION:
							moveCursor(event.caxis);
							break;
						}
					}



					//Update
					UpdateBackground(deltaTime);

					//UpdateCursor
					UpdateCursor(deltaTime);

					//check for cursor intersection with menu button
					menuOver = SDL_HasIntersection(&activePos, &MenuPos);

					//check for cursor intersection with Play button
					playOver = SDL_HasIntersection(&activePos, &PlayPos);


					// Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);

					//If the cursor is over a button, play the over sound
					if (menuOver || playOver)
					{
						if (alreadyOver == false)
						{
							Mix_PlayChannel(-1, overSound, 0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over ANY button, reset the already Over var
					if (!menuOver && !playOver)
					{
						alreadyOver = false;
					}


					//Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title image
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw the Win Text image
					SDL_RenderCopy(renderer, WinText, NULL, &WinTextPos);

					//Draw the menu button
					if(menuOver)
					{
						SDL_RenderCopy(renderer, MenuO, NULL, &MenuPos);
					}else
					{
					SDL_RenderCopy(renderer, Menu, NULL, &MenuPos);
					}


					//Draw the Play Button image
					if(playOver)
					{
						SDL_RenderCopy(renderer, PlayO, NULL, &PlayPos);
					}else
					{
					SDL_RenderCopy(renderer, Play, NULL, &PlayPos);
					}

					//Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


					// SDL Render present
					SDL_RenderPresent(renderer);
				}

				break;	// end Win case


			case LOSE:

				alreadyOver = false;

				lose = true;

				while(lose)
				{


					//set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime = (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event))
					{
						//check to se if the SDL Window is closed - player clicks X in window
						if(event.type == SDL_QUIT)
						{
							quit = true;
							lose = false;
							break;

						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								//if player chooses menu
								if(menuOver)
								{
									//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
									Mix_PlayChannel(-1, pressedSound, 0);
									lose = false;
									gameState = MENU;
									menuOver = false;
								}
								if(playOver)
								{
									//Play the Over sound - plays fine through levels, must pause/delay for QUIT 
									Mix_PlayChannel(-1, pressedSound, 0);
									lose = false;
									gameState = PLAYERS1;
									playOver = false;
								}
							}
							break;
						case SDL_CONTROLLERAXISMOTION:
							moveCursor(event.caxis);
							break;
						}
					}


					//Update
					UpdateBackground(deltaTime);

					//Update Cursor
					UpdateCursor(deltaTime);

					//check for cursor intersection with menu button
					menuOver = SDL_HasIntersection(&activePos, &MenuPos);

					//check for cursor intersection with Play button
					playOver = SDL_HasIntersection(&activePos, &PlayPos);

					//If the cursor is over a button, play the over sound
					if (menuOver || playOver)
					{
						if (alreadyOver == false)
						{
							Mix_PlayChannel(-1, overSound, 0);
							alreadyOver = true;
						}
					}

					//if the cursor is not over ANY button, reset the already Over var
					if (!menuOver && !playOver)
					{
						alreadyOver = false;
					}


					// Start Drawing
					//Clear SDL renderer
					SDL_RenderClear(renderer);




					//Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					//Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					//Draw the title image
					SDL_RenderCopy(renderer, title, NULL, &titlePos);

					//Draw the Instructions Text image
					SDL_RenderCopy(renderer, LoseText, NULL, &LoseTextPos);

					//Draw the menu button
					if(menuOver)
					{
						SDL_RenderCopy(renderer, MenuO, NULL, &MenuPos);
					}else
					{
					SDL_RenderCopy(renderer, Menu, NULL, &MenuPos);
					}


					//Draw the Play Button image
					if(playOver)
					{
						SDL_RenderCopy(renderer, PlayO, NULL, &PlayPos);
					}else
					{
					SDL_RenderCopy(renderer, Play, NULL, &PlayPos);
					}

					//Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


					// SDL Render present
					SDL_RenderPresent(renderer);
				}

				break;	// end Lose case
		}

	}


    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
