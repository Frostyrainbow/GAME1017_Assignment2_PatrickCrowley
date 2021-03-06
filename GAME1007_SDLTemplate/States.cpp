#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include "Player.h"
#include "CollisionManager.h"
#include <iostream>
using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume() {}

// Begin TitleState
TitleState::TitleState() {}

void TitleState::Enter()
{
	cout << "Entering TitleState..." << endl;
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
		STMA::ChangeState(new GameState());// Change to new GameState
	if (Engine::Instance().m_pStartButton.GetPressed(Engine::Instance().m_pStartButton))
	{
		Engine::Instance().m_pStartButton.SetPressed(false);
		StateManager::ChangeState(new GameState);
	}
}

void TitleState::Render()
{
	
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pMenuBackgroundTexture, Engine::Instance().m_pMenuBackground.GetSrc(), Engine::Instance().m_pMenuBackground.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pStartButtonTexture, Engine::Instance().m_pStartButton.GetSrc(), Engine::Instance().m_pStartButton.GetDst());
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting TitleState..." << endl;
}
// End TitleState

//Begin gamestate

GameState::GameState()
{
}

void GameState::Enter()
{
	Engine::Instance().m_player->SetDying(false);
	Mix_PlayMusic(Engine::Instance().m_theme, -1); // 0-n for #number of loops, or -1 for infinite looping
	Mix_VolumeMusic(16); //0-128
	Mix_VolumeChunk(Engine::Instance().m_jump, 8);
	Mix_VolumeChunk(Engine::Instance().m_death, 13);
}

void GameState::Update()
{
	m_delta++;

	

	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		StateManager::ChangeState(new TitleState());// Change to new TitleState
	}

	else if(Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState());
	}

	//Scroll the background
	//Engine::Instance().m_bg1.GetDst()->y += Engine::Instance().m_speed/2;
	Engine::Instance().m_mainGround1.GetDst()->x -= Engine::Instance().m_mainGroundSpeed;
	Engine::Instance().m_foreGround1.GetDst()->x -= Engine::Instance().m_foreGroundSpeed;
	Engine::Instance().m_backGround1.GetDst()->x -= Engine::Instance().m_backGroundSpeed;
	Engine::Instance().m_mainGround2.GetDst()->x -= Engine::Instance().m_mainGroundSpeed;
	Engine::Instance().m_foreGround2.GetDst()->x -= Engine::Instance().m_foreGroundSpeed;
	Engine::Instance().m_backGround2.GetDst()->x -= Engine::Instance().m_backGroundSpeed;
	Engine::Instance().m_pPlatform1.GetDst()->x -= Engine::Instance().m_foreGroundSpeed;
	Engine::Instance().m_pPlatform2.GetDst()->x -= Engine::Instance().m_foreGroundSpeed;
	//wrap backgrounds
	if (Engine::Instance().m_mainGround1.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_mainGround1.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_mainGround2.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_mainGround2.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_foreGround1.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_foreGround1.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_foreGround2.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_foreGround2.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_backGround1.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_backGround1.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_backGround2.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_backGround2.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_pPlatform1.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_pPlatform1.GetDst()->x = WIDTH - 5;
	}
	if (Engine::Instance().m_pPlatform2.GetDst()->x <= -WIDTH)
	{
		//Bounce back to org pos
		Engine::Instance().m_pPlatform2.GetDst()->x = WIDTH - 5;
	}

	Engine::Instance().m_n_spawnTimer--;

	if (Engine::Instance().m_n_spawnTimer <= 0)
	{
		Engine::Instance().m_ninjastars.push_back(new Ninjastar({ rand() % 1140 + WIDTH, rand() % 300 + 200 }));
		cout << "ninjastar spawned" << endl;
		Engine::Instance().m_n_spawnTimer = 120;
	}

	for (int i = 0; i < Engine::Instance().m_ninjastars.size(); i++)
	{
		Engine::Instance().m_ninjastars[i]->Update();
	}

	for (int i = 0; i < Engine::Instance().m_ninjastars.size(); i++)
	{
		if (Engine::Instance().m_ninjastars[i]->GetDst()->x <= -50)
		{
			delete Engine::Instance().m_ninjastars[i];
			Engine::Instance().m_ninjastars[i] = nullptr;
			Engine::Instance().m_ninjastars.erase(Engine::Instance().m_ninjastars.begin() + i);
			Engine::Instance().m_ninjastars.shrink_to_fit();
			cout << "Ninjastar deleted" << endl;
			break;
		}
	}

	for (int i = 0; i < Engine::Instance().m_ninjastars.size(); i++)
	{
		if (SDL_HasIntersection(Engine::Instance().m_ninjastars[i]->GetDst(), Engine::Instance().m_player->GetDst()))
		{
			//delete Engine::Instance().m_player;
			Engine::Instance().m_player->SetDying(true);
			cout << "Collision!" << endl;
			break;
		}
	}

	Engine::Instance().m_e_spawnTimer--;

	if (Engine::Instance().m_e_spawnTimer <= 0)
	{
		Engine::Instance().m_enemys.push_back(new Enemy({ rand() % 1140 + WIDTH, 600 })); 
		cout << "enemy spawned" << endl;
		Engine::Instance().m_e_spawnTimer = 120;
	}

	for (int i = 0; i < Engine::Instance().m_enemys.size(); i++)
	{
		Engine::Instance().m_enemys[i]->Update();
	}

	for (int i = 0; i < Engine::Instance().m_enemys.size(); i++)
	{
		if (Engine::Instance().m_enemys[i]->GetDst()->x <= -50)
		{
			delete Engine::Instance().m_enemys[i];
			Engine::Instance().m_enemys[i] = nullptr;
			Engine::Instance().m_enemys.erase(Engine::Instance().m_enemys.begin() + i);
			Engine::Instance().m_enemys.shrink_to_fit();
			cout << "Enemy deleted" << endl;
			break;
		}
	}

	for (int i = 0; i < Engine::Instance().m_enemys.size(); i++)
	{
		if (SDL_HasIntersection(Engine::Instance().m_enemys[i]->GetDst(), Engine::Instance().m_player->GetDst()))
		{
			//delete Engine::Instance().m_player;
			
			
			cout << "Collision!" << endl;
			break;
		}
	}
		
	Engine::Instance().m_player->Update();
	Player* player = Engine::Instance().m_player;
	SDL_Rect* playerPos = Engine::Instance().m_player->GetDst();
	if (CollisionManager::AABBCheck(playerPos, Engine::Instance().m_pPlatform1.GetDst()))
	{
		if ((playerPos->y + playerPos->h) - (float)player->GetVelY() <= Engine::Instance().m_pPlatform1.GetDst()->y)
		{
			player->StopY();
			player->SetY(Engine::Instance().m_pPlatform1.GetDst()->y - playerPos->h);
			player->SetGrounded(true);
		}
	}
	if (CollisionManager::AABBCheck(playerPos, Engine::Instance().m_pPlatform2.GetDst()))
	{
		if ((playerPos->y + playerPos->h) - (float)player->GetVelY() <= Engine::Instance().m_pPlatform2.GetDst()->y)
		{
			player->StopY();
			player->SetY(Engine::Instance().m_pPlatform2.GetDst()->y - playerPos->h);
			player->SetGrounded(true);
		}
	}

	if (Engine::Instance().m_player->IsDying())
	{
		deathClock--;
		if (deathClock <= 0)
		{
			StateManager::ChangeState(new EndState);
		}
	}
	
}

void GameState::Render()
{		
	
	

	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().m_pRenderer);
	//Any drawing here...
	
	
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pBackgroundTexture, Engine::Instance().m_backGround1.GetSrc(), Engine::Instance().m_backGround1.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pBackgroundTexture, Engine::Instance().m_backGround2.GetSrc(), Engine::Instance().m_backGround2.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pForegroundTexture, Engine::Instance().m_foreGround1.GetSrc(), Engine::Instance().m_foreGround1.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pForegroundTexture, Engine::Instance().m_foreGround2.GetSrc(), Engine::Instance().m_foreGround2.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pMaingroundTexture, Engine::Instance().m_mainGround1.GetSrc(), Engine::Instance().m_mainGround1.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pMaingroundTexture, Engine::Instance().m_mainGround2.GetSrc(), Engine::Instance().m_mainGround2.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pPlatformTexture, Engine::Instance().m_pPlatform1.GetSrc(), Engine::Instance().m_pPlatform1.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pPlatformTexture, Engine::Instance().m_pPlatform2.GetSrc(), Engine::Instance().m_pPlatform2.GetDst());
	for (int i = 0; i < Engine::Instance().m_ninjastars.size(); i++)
	{
		Engine::Instance().m_ninjastars[i]->Render(Engine::Instance().m_pRenderer, Engine::Instance().m_pNinjastarTexture);

	}

	for (int i = 0; i < Engine::Instance().m_enemys.size(); i++)
	{
		Engine::Instance().m_enemys[i]->Render(Engine::Instance().m_pRenderer, Engine::Instance().m_pEnemyTexture);
	}

	Engine::Instance().m_player->Render();
	SDL_RenderPresent(Engine::Instance().m_pRenderer); // Flip buffers - send data to window.
	if (dynamic_cast<GameState*>(StateManager::GetStates().back()))
	{
		State::Render();
	}
		
}

void GameState::Exit()
{
	//Clean the screen
	for (int i = 0; i < Engine::Instance().m_ninjastars.size(); i++)
	{
		delete Engine::Instance().m_ninjastars[i];
		Engine::Instance().m_ninjastars[i] = nullptr;
	}
	Engine::Instance().m_ninjastars.clear();
	Engine::Instance().m_ninjastars.shrink_to_fit();
	for (int i = 0; i < Engine::Instance().m_enemys.size(); i++)
	{
		delete Engine::Instance().m_enemys[i];
		Engine::Instance().m_enemys[i] = nullptr;
	}
	Engine::Instance().m_enemys.clear();
	Engine::Instance().m_enemys.shrink_to_fit();
	cout << "Exiting GameState..." << endl;
}

void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
}

PauseState::PauseState(){}

void PauseState::Enter()
{
	cout << "Pausing Game...\n";
}

void PauseState::Update()
{
	if(Engine::Instance().m_pResumeButton.GetPressed(Engine::Instance().m_pResumeButton))
	{
		Engine::Instance().m_pResumeButton.SetPressed(false);
		StateManager::PopState();
	}
	if(Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		StateManager::PopState();
	}
	
}

void PauseState::Render()
{
	SDL_RenderClear(Engine::Instance().m_pRenderer);
	//Rendering GameState
	StateManager::GetStates().front()->Render();
	//Rendering of Pause Menu
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 128);
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	Engine::Instance().m_pResumeButton.Render(Engine::Instance().m_pRenderer, Engine::Instance().m_pResumeButtonTexture);
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting Pause State...\n";
}


EndState::EndState(){}

void EndState::Enter()
{
	cout << "Entering End State...\n";
}

void EndState::Update()
{
	if (Engine::Instance().m_pMenuButton.GetPressed(Engine::Instance().m_pMenuButton))
	{
		Engine::Instance().m_pMenuButton.SetPressed(false);		
		StateManager::ChangeState(new TitleState);
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pGameOverBackgroundTexture, Engine::Instance().m_pGameOverBackground.GetSrc(), Engine::Instance().m_pGameOverBackground.GetDst());
	SDL_RenderCopy(Engine::Instance().m_pRenderer, Engine::Instance().m_pMenuButtonTexture, Engine::Instance().m_pMenuButton.GetSrc(), Engine::Instance().m_pMenuButton.GetDst());
	State::Render();
	
}

void EndState::Exit()
{
	cout << "Exiting End State...\n";
}
