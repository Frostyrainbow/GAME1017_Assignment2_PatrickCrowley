#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Sprites.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;


class Engine
{
public: // private properties.
	bool m_running = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture; //player texture
	SDL_Texture* m_pMaingroundTexture; //background texture
	SDL_Texture* m_pForegroundTexture; //background texture
	SDL_Texture* m_pBackgroundTexture; //background texture
	SDL_Texture* m_pPlatformTexture; //background texture
	SDL_Texture* m_pNinjastarTexture; //Ninja star texture
	SDL_Texture* m_pEnemyTexture; //Enemy texture
	SDL_Texture* m_pResumeButtonTexture; // Resume button
	SDL_Texture* m_pMenuButtonTexture; //Main Menu button in End State
	SDL_Texture* m_pStartButtonTexture; //Start Button in Title
	SDL_Texture* m_pMenuBackgroundTexture; //Background for the title
	SDL_Texture* m_pGameOverBackgroundTexture; //Background for the title

	Player* m_player = new Player({ 0, 263, 187, 263 }, { 500, 0, 128, 128 });
	Sprite m_mainGround1, m_foreGround1, m_backGround1, m_mainGround2, m_foreGround2, m_backGround2, m_pMenuBackground, m_pGameOverBackground, m_pPlatform1, m_pPlatform2;
	int m_speed = 5;
	int m_e_spawnTimer = 180;
	int m_n_spawnTimer = 120;
	int m_mainGroundSpeed = 7;
	int m_foreGroundSpeed = 5;
	int m_backGroundSpeed = 3;
	int m_spawnLimit;
	vector<Ninjastar*> m_ninjastars;
	vector<Enemy*> m_enemys;
	Button m_pResumeButton;
	Button m_pMenuButton;
	Button m_pStartButton;
	//Sound effects
	Mix_Chunk* m_jump;
	Mix_Chunk* m_death;

	//Music tracks
	Mix_Music* m_theme;

	
	static SDL_Point m_mousePos;
	static Uint32 m_mouseCurr;
	static Uint32 m_mouseLast;
	SDL_Cursor* m_cursor;

private: // private method prototypes.
	Engine() {} //Prevents instantiantion outside class
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	void Update();
	void Render();
	void Sleep();
	

public: // public method prototypes.
	int Run();
	bool KeyDown(SDL_Scancode c);
	static Engine& Instance(); //Static methoid for object access
	SDL_Renderer* GetRenderer() { return m_pRenderer; }

	static bool MousePressed(const int b);
	bool& Running();
	static SDL_Point& GetMousePos();
	
};

#endif

// Reminder: you can ONLY have declarations in headers, not logical code
