#include "States.h"
#include "StateManager.h"
#include "Engine.h"
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
	
	Mix_PlayMusic(Engine::Instance().m_theme, -1); // 0-n for #number of loops, or -1 for infinite looping
	Mix_VolumeMusic(16); //0-128
	Mix_VolumeChunk(Engine::Instance().m_e_plane_fire, 8);
	Mix_VolumeChunk(Engine::Instance().m_p_tank_fire, 8);
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

	if (Engine::Instance().KeyDown(SDL_SCANCODE_W))
	{
		//Engine::Instance().m_player->GetDst()->y -= Engine::Instance().m_speed;
	}
	/*else if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && Engine::Instance().m_player->GetDst()->y < HEIGHT - Engine::Instance().m_player->GetDst()->h)
	{
		Engine::Instance().m_player->GetDst()->y += Engine::Instance().m_speed;
	}*/
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && Engine::Instance().m_player->GetDst()->x > 0)
	{
		Engine::Instance().m_player->GetDst()->x -= Engine::Instance().m_speed;
	}
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && Engine::Instance().m_player->GetDst()->x < WIDTH - Engine::Instance().m_player->GetDst()->w)
	{
		Engine::Instance().m_player->GetDst()->x += Engine::Instance().m_speed;
	}

	Engine::Instance().m_n_spawnTimer--;

	if (Engine::Instance().m_n_spawnTimer <= 0)
	{
		Engine::Instance().m_ninjastars.push_back(new Ninjastar({ WIDTH, rand() % HEIGHT/2 + 200 })); //min 0 max half of height
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

	Engine::Instance().m_e_spawnTimer--;

	if (Engine::Instance().m_e_spawnTimer <= 0)
	{
		Engine::Instance().m_enemys.push_back(new Enemy({ WIDTH+50, 600 })); //min 10 max 920
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

	Engine::Instance().m_c_spawnTimer--;

	if (Engine::Instance().m_c_spawnTimer <= 0)
	{
		Engine::Instance().m_caltrops.push_back(new Caltrop({ rand() % (WIDTH + WIDTH/2) + WIDTH , 600 })); //min 10 max 920
		cout << "caltrop spawned" << endl;
		Engine::Instance().m_c_spawnTimer = 120;
	}

	for (int i = 0; i < Engine::Instance().m_caltrops.size(); i++)
	{
		Engine::Instance().m_caltrops[i]->Update();
	}

	for (int i = 0; i < Engine::Instance().m_caltrops.size(); i++)
	{
		if (Engine::Instance().m_caltrops[i]->GetDst()->x <= -50)
		{
			delete Engine::Instance().m_caltrops[i];
			Engine::Instance().m_caltrops[i] = nullptr;
			Engine::Instance().m_caltrops.erase(Engine::Instance().m_caltrops.begin() + i);
			Engine::Instance().m_caltrops.shrink_to_fit();
			cout << "Caltrop deleted" << endl;
			break;
		}
	}

	//for (int i = 0; i < Engine::Instance().m_caltrops.size(); i++)
	//{
	//	if (SDL_HasIntersection(Engine::Instance().m_caltrops[i]->GetDst(), Engine::Instance().m_player->GetDst()))
	//	{
	//		//delete Engine::Instance().m_player;
	//		Mix_PlayChannel(-1, Engine::Instance().m_death, 0);
	//		StateManager::ChangeState(new EndState);
	//		cout << "Collision!" << endl;
	//		break;
	//	}
	//}
		

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
	SDL_RenderCopyEx(Engine::Instance().m_pRenderer, Engine::Instance().m_pTexture, Engine::Instance().m_player->GetSrc(), Engine::Instance().m_player->GetDst(), 0, NULL, SDL_FLIP_NONE);
	for (int i = 0; i < Engine::Instance().m_ninjastars.size(); i++)
	{
		Engine::Instance().m_ninjastars[i]->Render(Engine::Instance().m_pRenderer, Engine::Instance().m_pNinjastarTexture);

	}
	for (int i = 0; i < Engine::Instance().m_caltrops.size(); i++)
	{
		Engine::Instance().m_caltrops[i]->Render(Engine::Instance().m_pRenderer, Engine::Instance().m_pCaltropTexture);
	}
	for (int i = 0; i < Engine::Instance().m_enemys.size(); i++)
	{
		Engine::Instance().m_enemys[i]->Render(Engine::Instance().m_pRenderer, Engine::Instance().m_pEnemyTexture);
	}

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
	for (int i = 0; i < Engine::Instance().m_caltrops.size(); i++)
	{
		delete Engine::Instance().m_caltrops[i];
		Engine::Instance().m_caltrops[i] = nullptr;
	}
	Engine::Instance().m_caltrops.clear();
	Engine::Instance().m_caltrops.shrink_to_fit();
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
