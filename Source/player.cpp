#include "player.h"

//Analog joystic dead zone
const int JOYSTICK_DEAD_ZONE = 8000;


//Player creation method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	//set the player number 0 or 1;
	playerNum = pNum;

	//set float for player speed
	speed = 500.0f;

	//see if this is player 1, or player 2, and create the correct file path
	if(playerNum == 0)
	{//Create the player 1 texture
		playerPath = filePath + "player.png";
	}else
	{//Create the player 2 texture
		playerPath = filePath + "player2.png";
	}
	//load the surface
	surface = IMG_Load(playerPath.c_str());

	//load the surface into the texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface for later use
	SDL_FreeSurface(surface);

	//set the SDL_Rect X and Y for the player
	posRect.x = x;

	posRect.y = y;

	//Use SDL_QueryTexture to get the kW and H of the player's texture
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;

	posRect.h = h;

	//Set the movement floats to the players original X and Y
	pos_X = x;
	pos_Y = y;

	//set the xDir and yDir for the joysticks
	xDir = 0;
	yDir = 0;

}

//Player Update method
void Player::Update(float deltaTime)
{
	//Adjust position floats based on speed, direction of joystick axis and deltaTime
	pos_X += (speed *xDir) *deltaTime;
	pos_Y += (speed *yDir) *deltaTime;

	//update player position with code to account for precision loss
	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if(posRect.x < 0)
	{
		posRect.x = 0;
		pos_X = posRect.x;
	}
	if(posRect.x > 1024 - posRect.w)
	{
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}
	if(posRect.y < 0)
	{
		posRect.y = 0;
		pos_Y = posRect.y;
	}
	if(posRect.y > 768 - posRect.h)
	{
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

}

//player draw method
void Player::Draw(SDL_Renderer *renderer)
{
	//Draw the player texture using the vars texture and posRect
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	//if the player's number is 0 and the joystick button is from joystick 0
	if(event.which == 0 && playerNum == 0)
	{
		//if A Button
		if(event.button == 0)
		{
			cout << "Plyaer 1 - Button A" << endl;
		}
	}

	//if the player's number is 1 and the joystick button is from joystick 1
	if(event.which == 1 && playerNum == 1)
	{
		//if A Button
		if(event.button == 0)
		{
			cout << "Plyaer 2 - Button A" << endl;
		}
	}
}

//Player Joystick Axis Method
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	// Axis movements and button presses both sent here as SDL_ControllerAxisEvent structures

	//if the playser's number is 0 and the joystick axis is from joystick 0
	if(event.which == playerNum == 0)
	{
		//X axis
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}

		}

		//Y axis
		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}
			else
			{
				yDir = 0.0f;
			}

		}
	}


}



