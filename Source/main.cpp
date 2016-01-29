// Example program:
// Using SDL2 to create an application window

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

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
void UpdateBackground()
{
	//Update Background 1
	BG1pos_Y += (bkgdSpeed * 1)* deltaTime;

	//set the new bkgd1 position
	bkgd1Pos.y = (int)(BG1pos_Y + .5f);

	//reset when off the bottom of the screen
	if(bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y = -768;
		BG1pos_Y = bkgd1Pos.y;


	}

	//Update Background 2
	BG2pos_Y += (bkgdSpeed * 1)* deltaTime;

	//set the new bkgd2 position
	bkgd2Pos.y = (int)(BG2pos_Y + .5f);

	//reset when off the bottom of the screen
	if(bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y = -768;
		BG2pos_Y = bkgd2Pos.y;


	}


}




int main(int argc, char* argv[]) {

//Output for Apple
#if defined(__APPLE__)
	cout << "Running on Apple" << endl;
	//get the current working directory
	string s_cwd(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images";

	//test
	cout << s_cwd_images << endl;
#endif

//Output for Linux
#if defined(__linux__)
cout << "Running on Linux" << endl;
cout << "Added on Linux" << endl;
#endif

//Output for Windows
#if defined(_WIN32) || (_WIN64)
cout << "Running on Windows" << endl;
cout << "Added on Windows" << endl;
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
    string BKGDpath = s_cwd_images + "/Bacground.png";

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

    //********Backgrounds - END***********//


    //***** Create CURSOR*****
    //create cursor
    string CURSORpath = s_cwd_images + "/CursorNew.png";

    // create a SDL surface to hold the background image
    surface = IMG_Load(CURSORpath.c_str());

    // create a SDL texture
    SDL_Texture *cursor;

    //place surface into the texture cursor
    cursor = SDL_CreateTextureFromSurface(renderer, surface);


    //free the SDL surface
    SDL_FreeSurface(surface);

    // create the SDL_Rectangle for the texture's position and size - x,y,w,h
    SDL_Rect cursorPos, activePos;

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


    //var for cursor speed
    int cursorSpeed = 400;

    //*************CURSOR - END**************

    //****** Create Menu - START ******

    // ********* TITLE - START******
    string Titlepath = s_cwd_images + "/Title.png";

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

    string Ply1path = s_cwd_images + "/OnePlayer.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Ply1path.c_str());

    // create a SDL texture
    SDL_Texture *players1N;

    //place surface into the texture title
    players1N = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    string Ply1Opath = s_cwd_images + "/OnePlayerO.png";

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

    string Ply2path = s_cwd_images + "/TwoPlayer.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Ply2path.c_str());

    // create a SDL texture
    SDL_Texture *players2N;

    //place surface into the texture title
    players2N = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    string Ply2Opath = s_cwd_images + "/TwoPlayerO.png";

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

    string Instructionspath = s_cwd_images + "/Instructions.png";

    // create a SDL surface to hold the title image
    surface = IMG_Load(Instructionspath.c_str());

    // create a SDL texture
    SDL_Texture *InstructionsN;

    //place surface into the texture title
    InstructionsN = SDL_CreateTextureFromSurface(renderer, surface);

    //free the SDL surface
    SDL_FreeSurface(surface);

    string InstructionsOpath = s_cwd_images + "/InstructionsO.png";

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

     string Quitpath = s_cwd_images + "/Quit.png";

     // create a SDL surface to hold the title image
     surface = IMG_Load(Quitpath.c_str());

     // create a SDL texture
     SDL_Texture *QuitN;

     //place surface into the texture title
     QuitN = SDL_CreateTextureFromSurface(renderer, surface);

     //free the SDL surface
     SDL_FreeSurface(surface);

     string QuitOpath = s_cwd_images + "/QuitO.png";

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
     string InstructionsTextpath = s_cwd_images + "/InstructionsText.png";

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



	//******* set up a Game Controller variable *****//
	SDL_GameController* gGameController = NULL;

	//****** Open Game Controller*****
	gGameController = SDL_GameControllerOpen(0);

	//***** Turn on Game Controller Events
	SDL_GameControllerEventState(SDL_ENABLE);

	//***** SDL Event to handle input****
	SDL_Event event;

	//***** set up variables for the game states*****
	enum GameState{MENU,INSTRUCTIONS,PLAYERS1,PLAYERS2,WIN,LOSE};

	// set up the initial state
	GameState gameState = MENU;

	//boolean values to control movement through the states****
	bool menu, instructions, players1, players2, win, lose, quit = false;



    // The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)
	{
		switch(gameState)
		{
			case MENU:
				menu = true;
				cout << "The Game State is Menu" << endl;
				cout << "Press the A Button for Instructions" << endl;
				cout << "Press the B Button for 1 Player Game" << endl;
				cout << "Press the X Button for 2 Player Game" << endl;
				cout << "Press the Y Button for Quit Game" << endl;
				cout << endl;

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
									menu = false;
									gameState = INSTRUCTIONS;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
								{
									menu = false;
									gameState = PLAYERS1;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
								{
									menu = false;
									gameState = PLAYERS2;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
								{
									menu = false;
									quit = true;
								}
							}
							break;
						}
					}


					//Update
					UpdateBackground();



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
					SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);

					//Draw Two Player image
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);

					//Draw Instructions image
					SDL_RenderCopy(renderer, InstructionsN, NULL, &InstructionsNPos);


					//Draw Quit image
					SDL_RenderCopy(renderer, QuitN, NULL, &QuitNPos);


					//Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


					// SDL Render present
					SDL_RenderPresent(renderer);


				}

				break;	// end menu case

			case INSTRUCTIONS:
				instructions = true;
				cout << "The Game State is Instructions" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << endl;

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

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									instructions = false;
									gameState = MENU;
								}
							}
							break;
						}
					}

					//Update
					UpdateBackground();


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

					//Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);


					// SDL Render present
					SDL_RenderPresent(renderer);


				}

				break;	// end instructions case


			case PLAYERS1:
				players1 = true;
				cout << "The Game State is 1 Player Game" << endl;
				cout << "Press the A Button for Win Screen" << endl;
				cout << "Press the B Button for Lose Screen" << endl;
				cout << endl;

				while(players1)
				{
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
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									players1 = false;
									gameState = WIN;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
								{
									players1 = false;
									gameState = LOSE;
								}
							}
							break;
						}
					}
				}

				break;	// end players1 case

			case PLAYERS2:
				players2 = true;
				cout << "The Game State is 2 Player Game" << endl;
				cout << "Press the A Button for Win Screen" << endl;
				cout << "Press the B Button for Lose Screen" << endl;
				cout << endl;

				while(players2)
				{
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

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									players2 = false;
									gameState = WIN;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
								{
									players2 = false;
									gameState = LOSE;
								}
							}
							break;
						}
					}
				}

				break;	// end players2 case

			case WIN:
				win = true;
				cout << "The Game State is Win" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << "Press the B Button to Quit" << endl;
				cout << endl;

				while(win)
				{
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

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									win = false;
									gameState = MENU;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
								{
									win = false;
									quit = true;
								}
							}
							break;
						}
					}
				}

				break;	// end Win case


			case LOSE:
				lose = true;
				cout << "The Game State is Lose" << endl;
				cout << "Press the A Button for Main Menu" << endl;
				cout << "Press the B Button to Quit" << endl;
				cout << endl;

				while(lose)
				{
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

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									lose = false;
									gameState = MENU;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
								{
									lose = false;
									quit = true;
								}
							}
							break;
						}
					}
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
