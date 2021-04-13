#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "MathManager.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;
class Sprite
{
protected:
	SDL_Rect m_src; //Source rect
	SDL_Rect m_dst; //Destination Rect
public:
	void SetRekts(SDL_Rect s, SDL_Rect d)
	{
		m_src = s;
		m_dst = d;
	}
	SDL_Rect* GetSrc()
	{
		return &m_src;
	}
	SDL_Rect* GetDst()
	{
		return &m_dst;
	}
};

class AnimatedSprite : public Sprite
{
public:
	unsigned short m_frame, m_frameMax, m_sprite, m_spriteMin, m_spriteMax;

	AnimatedSprite(const SDL_Rect s, SDL_Rect d)
	{
		m_frame = 0;
		m_frameMax = 0;
		m_sprite = 0;
		m_spriteMin = 0;
		m_spriteMax = 0;
	}
	void SetAnimation(const unsigned short fMax, const unsigned short sMin, const unsigned short sMax, const int srcY = 0)
	{
		m_frame = 0;
		m_frameMax = fMax;
		m_sprite = m_spriteMin = sMin;
		m_spriteMax = sMax;
		m_src.x = m_src.w * m_sprite;
		m_src.y = srcY;
	}
	void Animate()
	{
		if (m_frame++ == m_frameMax)
		{
			m_frame = 0;
			if (++m_sprite == m_spriteMax)
				m_sprite = m_spriteMin; // I don't use % in case m_sprite doesn't start at 0.
			m_src.x = m_src.w * m_sprite; // Moved to outside if.
		}
	}
	void Render(SDL_Renderer* rend, SDL_Texture* tex)
	{
		SDL_RenderCopyEx(rend, tex, &m_src, &m_dst, 0, NULL, SDL_FLIP_NONE);
	}
};

class Button : public Sprite
{
public:
	Button() {}
	Button(SDL_Rect spawnLoc)
	{
		this->m_dst.x = spawnLoc.x;
		this->m_dst.y = spawnLoc.y;
		this->m_dst.w = 189;
		this->m_dst.h = 58;
		m_src = { 0, 0, 189, 58 };
	}

	~Button()
	{

	}

	void Update()
	{

	}

	void SetLoc(SDL_Point loc)
	{
		m_dst.x = loc.x;
		m_dst.y = loc.y;
	}

	void Render(SDL_Renderer* rend, SDL_Texture* tex)
	{
		SDL_RenderCopyEx(rend, tex, &m_src, &m_dst, 0, NULL, SDL_FLIP_NONE);
	}

	bool GetPressed(Button button);

	void SetPressed(bool pressed)
	{
		m_Pressed = pressed;
	}

private:
	bool m_Pressed;
};

class Ninjastar : public Sprite
{
private:
	//SDL_Rect m_rect; //Single rect for destination
public:
	Ninjastar(SDL_Rect spawnLoc) //Non-default constructor
	{
		this->m_dst.x = spawnLoc.x; // this-> is optional
		this->m_dst.y = spawnLoc.y; // this-> is optional
		this->m_dst.w = 64;
		this->m_dst.h = 64;
		m_src = { 0, 0, 64, 64 };

		//m_rect = { spawnLoc.x, spawnLoc.y, 4, 4 }; short way
	}

	~Ninjastar() //Destructor
	{

	}

	void SetLoc(SDL_Point loc)
	{
		m_dst.x = loc.x;
		m_dst.y = loc.y;
	}

	void Update()
	{
		this->m_dst.x -= 5;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* tex)
	{
		/*SDL_SetRenderDrawColor(rend, 0, 128, 255, 255);
		SDL_RenderFillRect(rend, &m_dst);*/
		SDL_RenderCopy(rend, tex, &m_src, &m_dst);
	}



};

class Enemy : public Sprite
{
public:
	int m_jumpTimer = rand() % 10;
	double m_accelY = 0.0;
	double m_velY = 0.0;
	double m_maxVelY = 0.0;
	double m_grav = 1.0;
	Enemy(SDL_Rect spawnLoc)
	{
		this->m_dst.x = spawnLoc.x; // this-> is optional
		this->m_dst.y = spawnLoc.y; // this-> is optional
		this->m_dst.w = 100;
		this->m_dst.h = 100;
		this->m_src = { 0, 0, 835, 482 };

	}

	~Enemy()
	{

	}

	void SetLoc(SDL_Point loc)
	{
		m_dst.x = loc.x;
		m_dst.y = loc.y;
	}

	void Update()
	{
		this->m_dst.x -= 7;
		m_jumpTimer--;
		if (m_jumpTimer <= 0)
		{
			//implement a jump the same way a player jumps
			m_accelY = -10.0;
			m_velY += m_accelY + m_grav;
			m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
			m_dst.y += m_velY;
			m_jumpTimer = m_jumpTimer;
		}
		m_accelY = 0.0f;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* tex)
	{
		SDL_RenderCopy(rend, tex, &m_src, &m_dst);
	}

};




class Platform : public Sprite
{
public:
	Platform(SDL_Rect spawnLoc)
	{
		this->m_dst.x = spawnLoc.x; // this-> is optional
		this->m_dst.y = spawnLoc.y; // this-> is optional
		this->m_dst.w = 100;
		this->m_dst.h = 100;
		this->m_src = { 0, 0, 300, 300 };

	}

	~Platform()
	{

	}


	void SetLoc(SDL_Point loc)
	{
		m_dst.x = loc.x;
		m_dst.y = loc.y;
	}

	void Update()
	{
		this->m_dst.x -= 3;
	}
	void Render(SDL_Renderer* rend, SDL_Texture* tex)
	{
		SDL_RenderCopy(rend, tex, &m_src, &m_dst);
	}
};