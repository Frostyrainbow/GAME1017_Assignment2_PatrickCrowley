#pragma once
#ifndef __PLAYER__
#define __PLAYER__
#include "SDL.h"
#include "Sprites.h"
#define GRAV 1.0
#define JUMPFORCE 20.0

enum PlayerState { STATE_CROUCHING, STATE_RUNNING, STATE_JUMPING, STATE_DEATH };

class Player : public AnimatedSprite
{
public: // Methods.
	Player(SDL_Rect s, SDL_Rect d);
	void Update();
	void Render();
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded();
	void SetGrounded(bool g);
	void SetDying(bool d);
	bool IsDying();
	double GetVelX();
	double GetVelY();
	void SetX(float x);
	void SetY(float y);
private: // Properties.
	bool m_grounded;
	bool m_crouched;
	bool m_dying;
	double m_accelX;
	double	m_accelY;
	double	m_velX;
	double	m_maxVelX;
	double	m_velY;
	double m_maxVelY;		
	double m_drag;
	double m_grav;
	PlayerState m_state;
};

#endif