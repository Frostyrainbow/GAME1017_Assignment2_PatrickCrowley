#include "Engine.h"
#include "StateManager.h"
#include "States.h"


int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				 //Initialize subsystems
				if (IMG_Init(IMG_INIT_PNG || IMG_INIT_JPG) != 0)
				{
					m_pTexture = IMG_LoadTexture(m_pRenderer, "Img/Zato.png");
					m_pMaingroundTexture = IMG_LoadTexture(m_pRenderer, "Img/Mainground.png");
					m_pForegroundTexture = IMG_LoadTexture(m_pRenderer, "Img/foreground.png");
					m_pBackgroundTexture = IMG_LoadTexture(m_pRenderer, "Img/background.png");
					m_pPlatformTexture = IMG_LoadTexture(m_pRenderer, "Img/Platform.png");
					m_pNinjastarTexture= IMG_LoadTexture(m_pRenderer, "Img/ninjastar.png");
					m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "Img/eRun1.png");
					m_pResumeButtonTexture = IMG_LoadTexture(m_pRenderer, "Img/ResumeButton.png");
					m_pMenuButtonTexture = IMG_LoadTexture(m_pRenderer, "Img/MenuButton.png");
					m_pStartButtonTexture = IMG_LoadTexture(m_pRenderer, "Img/StartButton.png");
					m_pMenuBackgroundTexture = IMG_LoadTexture(m_pRenderer, "Img/Title.jpg");
					m_pGameOverBackgroundTexture = IMG_LoadTexture(m_pRenderer, "Img/Dead.jpg");

					
				}
				else
				{
					return false; //Image init failed
				}
				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					//Configure mixer
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
					Mix_AllocateChannels(32);
					//Load sounds
					m_jump = Mix_LoadWAV("Aud/jump.wav");
					m_death = Mix_LoadWAV("Aud/playerdeath.wav");
					m_theme = Mix_LoadMUS("Aud/MaiTheme.mp3");
				}
				else
				{
					return false; //Mixer init failed
				}
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // initalization failed.
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000); // Converts FPS into milliseconds, e.g. 16.67
	m_keystates = SDL_GetKeyboardState(nullptr);
	//mouse init
	m_mouseCurr = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
	m_mouseLast = m_mouseCurr;
	
	
	m_pResumeButton.SetRekts({ 0,0,189,58 }, { 420, 300, 189, 58 });
	m_pMenuButton.SetRekts({ 0,0,189,58 }, { 420, 200, 189, 58 });
	m_pStartButton.SetRekts({ 0,0,189,58 }, { 420, 400, 189, 58 });
	m_pMenuBackground.SetRekts({ 0, 0, WIDTH, HEIGHT }, { 0, 0, WIDTH, HEIGHT });
	m_pGameOverBackground.SetRekts({ 0, 0, WIDTH, HEIGHT }, { 0, 0, WIDTH, HEIGHT });
	m_mainGround1.SetRekts({ 0, 0, WIDTH, HEIGHT }, { 0, 0, WIDTH, HEIGHT });
	m_pPlatform1.SetRekts({ 0, 0, 1024, 145 }, { 0, 665, 1024, 145 });
	m_foreGround1.SetRekts({ 0, 0, WIDTH, HEIGHT }, { 0, 0, WIDTH, HEIGHT });
	m_backGround1.SetRekts({ 0, 0, WIDTH, HEIGHT }, { 0, 0, WIDTH, HEIGHT });
	m_mainGround2.SetRekts({ 0, 0, WIDTH, HEIGHT }, { WIDTH, 0, WIDTH, HEIGHT });
	m_foreGround2.SetRekts({ 0, 0, WIDTH, HEIGHT }, { WIDTH, 0, WIDTH, HEIGHT });
	m_backGround2.SetRekts({ 0, 0, WIDTH, HEIGHT }, { WIDTH, 0, WIDTH, HEIGHT });
	m_pPlatform2.SetRekts({ 0, 0, 1024, 145 }, { WIDTH, 665, 1024, 145 });
	for (int i = 0; i < m_ninjastars.size(); i++)
	{
		m_ninjastars[i]->SetRekts( {0, 0, 128, 128}, { WIDTH + 50, 100, 128, 128} );
	}
	for (int i = 0; i < m_enemys.size(); i++)
	{
		m_enemys[i]->SetRekts({ 0, 0, 512, 256 }, { rand() % WIDTH + 0, 400, 128, 128 });
	}
	STMA::PushState(new TitleState());
	cout << "Initialization successful!" << endl;
	m_running = true;

	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
	
}

void Engine::HandleEvents()
{
	SDL_Event event;

	m_mouseLast = m_mouseCurr;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		
		}
	}

	m_mouseCurr = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1)
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}




void Engine::Update()
{
	STMA::Update();
}

void Engine::Render()
{
	STMA::Render();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);

}

int Engine::Run()
{
	if (m_running == true)
	{
		return 1;
	}
	// Start and run the "engine"
	if (Init("GAME1007 M1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{
		return 2;
	}
	// We passed our initial checks, start the loop!
	while (m_running == true)
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}


//Engine* Engine::Instance()
//{
//	static Engine* instance = new Engine(); //Magic statics creating the object
//	return instance;
//} pointer way

Engine& Engine::Instance()
{
	static Engine instance; //Magic statics creating the object
	return instance;
}



bool Engine::MousePressed(const int b)
{
	return ((m_mouseCurr & SDL_BUTTON(b)) > (m_mouseLast & SDL_BUTTON(b)));
}


SDL_Point& Engine::GetMousePos()
{
	return m_mousePos;
}

bool Button::GetPressed(Button button)
{
	const auto mousePos = Engine::GetMousePos();
	if (SDL_PointInRect(&mousePos, button.GetDst()) && Engine::MousePressed(1))
	{
		m_Pressed = true;
	}
	return m_Pressed;
}



void Engine::Clean()
{
	cout << "Cleaning engine..." << endl;
	for (int i = 0; i < m_ninjastars.size(); i++)
	{
		delete m_ninjastars[i];
		m_ninjastars[i] = nullptr;
	}
	m_ninjastars.clear();
	m_ninjastars.shrink_to_fit();
	for (int i = 0; i < m_enemys.size(); i++)
	{
		delete m_enemys[i];
		m_enemys[i] = nullptr;
	}
	m_enemys.clear();
	m_enemys.shrink_to_fit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pTexture);
	SDL_DestroyTexture(m_pMaingroundTexture);
	SDL_DestroyTexture(m_pForegroundTexture);
	SDL_DestroyTexture(m_pBackgroundTexture);
	SDL_DestroyTexture(m_pNinjastarTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	Mix_FreeChunk(m_jump);
	Mix_FreeChunk(m_death);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


Uint32 Engine::m_mouseCurr;
Uint32 Engine::m_mouseLast;
SDL_Point Engine::m_mousePos;



