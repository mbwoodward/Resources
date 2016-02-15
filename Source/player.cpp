#include "player.h"

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//Player creation method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{

	//activate the player
	active = true;

	//set the player number 0 or 1
	playerNum = pNum;

	//set float for player speed
	speed = 500.0f;

	laser = Mix_LoadWAV((audioPath + "bullet.wav").c_str());

	// init score and lives vars
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	//init the font system
	TTF_Init();

	//load the font
	font = TTF_OpenFont((audioPath + "Amperzand.ttf").c_str(), 40);

	//see if this is player 1 or player 2, and create the correct X and Y locations
	if (playerNum == 0)
	{
		//Create the score texture X and Y position
		scorePos.x = scorePos.y = 10;
		livesPos.x = 10;
		livesPos.y = 50;
	}
	else
	{
		scorePos.x = 650;
		scorePos.y = 10;
		livesPos.x = 650;
		livesPos.y = 50;
	}

	// update score method
	UpdateScore(renderer);

	//Update lives method
	UpdateLives(renderer);

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

	//String to create the path to the player's bullet image
	string bulletPath;

	//see if this is player 1, or player 2, and create the correct file path
	if(playerNum == 0){
		//Create the bullet 1 texture
		bulletPath = filePath + "bullet.png";
	}else{
		//Create the bullet 2 texture
		bulletPath = filePath + "bullet2.png";
	}

	//Create the player's bullet pool
	for(int i = 0; i < 10; i++)
	{
		//create the bullet and move offscreen, out of the game play area
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000);

		//add to bulletlist
		bulletList.push_back(tmpBullet);

	}


}

//Update Lives
void Player::UpdateLives(SDL_Renderer *renderer)
{
	//fix for to_string problems on linux
	string Result;			//string which will contain the result
	ostringstream convert;	//stream used for the conversion
	convert << playerLives;	//insert the textual representation of 'Number' in the characters in the stream
	Result = convert.str();	//set 'Result' to the contents of the stream

							//create the text for the font texture
	tempLives = "Player Lives: " + Result;

	//check to see what playser this is and color the font as needed
	if (playerNum == 0)
	{
		//Place the player 1 score info into a surface
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);
	}
	else
	{
		//Place the player 2 score info into a surface
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP2);
	}

	//create the playewr score texture
	livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);

	//get the Width and Height of the texture
	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);

	//free the surface
	SDL_FreeSurface(livesSurface);

	//set old score
	oldLives = playerLives;

	//if player has no more lives
	if (playerLives <= 0)
	{
		//deactivate the player
		active = false;

		//move the player off screen
		posRect.x = posRect.y = -2000;

		//set float values to location values
		pos_X = pos_Y = -2000;

	}
}

//Update Score
void Player::UpdateScore(SDL_Renderer *renderer)
{
	//fix for to_string problems on linux
	string Result;			//string which will contain the result
	ostringstream convert;	//stream used for the conversion
	convert << playerScore;	//insert the textual representation of 'Number' in the characters in the stream
	Result = convert.str();	//set 'Result' to the contents of the stream

	//create the text for the font texture
	tempScore = "Player Score: " + Result;

	//check to see what playser this is and color the font as needed
	if (playerNum == 0)
	{
		//Place the player 1 score info into a surface
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);
	}
	else
	{
		//Place the player 2 score info into a surface
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP2);
	}

	//create the playewr score texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	//get the Width and Height of the texture
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	//free the surface
	SDL_FreeSurface(scoreSurface);

	//set old score
	oldScore = playerScore;
}

void Player::Reset()
{
	//place the player based on player number
	if (playerNum == 0)
	{
		//set X and Y for Player 1
		posRect.x = 250.0;
		posRect.y = 500.0;
	}
	else
	{
		//set X and Y for Player 2
		posRect.x = 550.0;
		posRect.y = 500.0;
	}

	pos_X = posRect.x;
	pos_Y = posRect.y;
	playerLives = 3;
	playerScore = 0;
	xDir = 0;
	yDir = 0;
	active = true;

}

//Player Update method
void Player::Update(float deltaTime, SDL_Renderer *renderer)
{
	//Adjust position floats based on speed, direction of joystick axis and deltaTime
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

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

	//Update the player's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active){

			//Update bullet
			bulletList[i].Update(deltaTime);
		}
	}

	//should score be updated?
	if (playerScore != oldScore)
	{
		UpdateScore(renderer);
	}

	//should lives be updated?
	if (playerLives != oldLives)
	{
		UpdateLives(renderer);
	}
}

//player draw method
void Player::Draw(SDL_Renderer *renderer)
{
	//Draw the player texture using the vars texture and posRect
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	//draw the player's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		//check to see if the bullet is active
		if(bulletList[i].active){

			//Draw Bullet
			bulletList[i].Draw(renderer);
		}
	}

	//draw the player score
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

	//draw the player lives
	SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);
}

//Create a bullet
void Player::CreateBullet(){
	//see if there is a bullet active to fire
	for(int i = 0; i < bulletList.size(); i++)
	{
		//see if the bullet is not active
		if(bulletList[i].active == false){

			//Play sound for laser
			Mix_PlayChannel(-1, laser, 0);

			//set bullet to active
			bulletList[i].active = true;

			//use some math in the x position to get the bullet close to
			//the center of the player using player width
			bulletList[i].posRect.x = (pos_X + (posRect.w/2));

			//finishing aligning to the player center using the texture width
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w/2));
			bulletList[i].posRect.y = posRect.y;

			//Set the x and y positions of the bullet's float positions
			bulletList[i].pos_X = pos_X;
			bulletList[i].pos_Y = pos_Y;

			//once bullet is found, break out of loop
			break;
		}
	}
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	//if the player's number is 0 and the joystick button is from joystick 0
	if(event.which == 0 && playerNum == 0)
	{
		//if A Button
		if(event.button == 0)
		{
			

			//Create a bullet
			CreateBullet();
		}
	}

	//if the player's number is 1 and the joystick button is from joystick 1
	if(event.which == 1 && playerNum == 1)
	{
		//if A Button
		if(event.button == 0)
		{


			//Create a bullet
			CreateBullet();
		}
	}
}

//Player Joystick Axis Method
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	// Axis movements and button presses both sent here as SDL_ControllerAxisEvent structures

	//if the player's number is 0 and the joystick axis is from joystick 0
	if(event.which == 0 && playerNum == 0)
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

	//if the player's number is 1 and the joystick axis is from joystick 1
	if(event.which == 1 && playerNum == 1)
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


Player::~Player()
{
	SDL_DestroyTexture(texture);
}



