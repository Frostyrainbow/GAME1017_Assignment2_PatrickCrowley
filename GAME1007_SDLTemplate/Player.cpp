#include "Player.h"
#include "MathManager.h"
#include "Engine.h"
#include <cmath>

Player::Player(SDL_Rect s, SDL_Rect d) : AnimatedSprite(s, d),
m_state(STATE_RUNNING), m_grounded(true), m_crouched(false), m_dying(false), m_maxVelX(10.0),
m_maxVelY(JUMPFORCE), m_grav(GRAV), m_drag(0.8)
{
	SetRekts(s, d);
	m_accelX = 0.0;
	m_accelY = 0.0;
	m_velX = 0.0;
	m_velY = 0.0;
	SetAnimation(8, 0, 6, 0); //set walking animation
}

void Player::Update()
{
	//Check states
	switch (m_state)
	{
	case STATE_RUNNING:
		m_src.w = 187;
		m_src.h = 263;
		m_dst.h = 126;
		//Transition to Jump
		if (Engine::Instance().KeyDown(SDL_SCANCODE_W) && m_grounded)
		{
			Mix_PlayChannel(-1, Engine::Instance().m_jump, 0);
			m_grounded = false;
			m_accelY = -JUMPFORCE;
			m_state = STATE_JUMPING;
			SetAnimation(6, 0, 6, 750);
		}
		//Transition to Crouch
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && !m_crouched)
		{
			m_crouched = true;
			m_state = STATE_CROUCHING;
			SetAnimation(8, 0, 8, 276);
		}

		//Movement
		if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -5;
		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
		{
			m_accelX = 5;
		}
		if (m_dying)
		{
			Mix_PlayChannel(-1, Engine::Instance().m_death, 0);
			m_state = STATE_DEATH;
			SetAnimation(4, 0, 4, 469);
		}
		break;

	case STATE_JUMPING:
		m_src.w = 230;
		m_src.h = 318;
		m_dst.h = 126;
		if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -5;
		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
		{
			m_accelX = 5;
		}

		//Hit the ground transition to run
		if (m_grounded)
		{
			m_state = STATE_RUNNING;
			SetAnimation(6, 0, 6, 0);
		}
		if (m_dying)
		{
			Mix_PlayChannel(-1, Engine::Instance().m_death, 0);
			m_state = STATE_DEATH;
			SetAnimation(4, 0, 4, 469);
		}
		break;

	case STATE_CROUCHING:
		m_src.w = 200;
		m_src.h = 166;
		if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -5;
		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_dst.x < WIDTH - m_dst.w)
		{
			m_accelX = 5;
		}

		if (!Engine::Instance().KeyDown(SDL_SCANCODE_S))
		{
			m_crouched = false;
			m_state = STATE_RUNNING;
			SetAnimation(8, 0, 8);
		}
		if (m_dying)
		{
			Mix_PlayChannel(-1, Engine::Instance().m_death, 0);
			m_state = STATE_DEATH;
			SetAnimation(4, 0, 4, 469);
		}

		break;

	case STATE_DEATH:
		
		m_src.w = 126;
		m_src.h = 266;
		if (!m_dying)
		{
			m_state = STATE_RUNNING;
			SetAnimation(8, 0, 6, 0);
		}
		break;

	}
	//Player movement
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += m_velX;
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += m_velY;

	m_accelX = 0.0f;
	m_accelY = 0.0f;
	//Invoke animation
	Animate();
}

void Player::Render()
{
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().m_pTexture, GetSrc(), GetDst(), 0.0, NULL, SDL_FLIP_NONE);
}

void Player::Stop()
{
	StopX();
	StopY();
}

void Player::StopX() 
{ 
	m_velX = 0.0; 
}

void Player::StopY() 
{ 
	m_velY = 0.0; 
}

void Player::SetAccelX(double a) 
{ 
	m_accelX = a; 
}

void Player::SetAccelY(double a) 
{ 
	m_accelY = a; 
}

bool Player::IsGrounded() 
{ 
	return m_grounded; 
}

void Player::SetGrounded(bool g) 
{ 
	m_grounded = g;
}

void Player::SetDying(bool d)
{
	m_dying = d;
}

bool Player::IsDying()
{
	return m_dying;
}

double Player::GetVelX() 
{ 
	return m_velX; 
}

double Player::GetVelY() 
{
	return m_velY; 
}

void Player::SetX(float x) 
{ 
	m_dst.x = x; 
}

void Player::SetY(float y) 
{ 
	m_dst.y = y; 
}
