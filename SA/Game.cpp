//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.h"
#include "SideBlock.h"
#include "MeshComponent.h"
#include "Block.h"

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f)) {
		return false;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadLevel(std::string filename, float xPos) {

	//The block in the top left corner is centered at (16, 16), Each block is 32x32
	//o y=-237.5 and z=237.5. As you increase the column, the y increases by 25, 
	//and increasing the row decreases the z by 25.

	char x;
	std::ifstream inFile;

	inFile.open(filename);
	if (!inFile) {
		exit(1); // terminate with error
	}
	//float xPos = 3000;
	float zPos = 237.5;
	float yPos = -237.5;
	int row = 0;
	bool exploding = false;
	std::string line = "";
	while (inFile >> line) {
		for (int i = 0; i < line.length(); i++) {
			char type = line[i];
			if (type == 'A') {
				Block* b = new Block(this);
				Vector3 blockPos = Vector3(xPos, yPos + (i* 25), zPos - (row*25));
				b->SetPosition(blockPos);
				b->meshc->SetTextureIndex(2);
			}
			else if (type == 'B') {
				Block* b = new Block(this);
				exploding = true;
				Vector3 blockPos = Vector3(xPos, yPos + (i * 25), zPos - (row * 25));
				b->SetPosition(blockPos);
				b->meshc->SetTextureIndex(4);
			}
			else if (type == '.') {
			}
		}
		row++;
	}
	inFile.close();
}


void Game::LoadData()
{
	int mChannel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/ShipLoop.wav"), -1);

	mPlayer = new Player(this);

	Matrix4 projection = Matrix4::CreatePerspectiveFOV(1.22f, 1024.0f, 768.0f, 10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(projection);

	Matrix4 view = Matrix4::CreateLookAt(Vector3(-300, 0, 100), Vector3(20, 0, 0), Vector3::UnitZ);
	mRenderer->SetViewMatrix(view);

	
	SideBlock* s1 = new SideBlock(this);
	s1->SetPosition(Vector3(0, 500, 0));
	s1->meshc->SetTextureIndex(1);
	SideBlock* s2 = new SideBlock(this);
	s2->SetPosition(Vector3(0, -500, 0));
	s1->meshc->SetTextureIndex(1);
	SideBlock* s3 = new SideBlock(this);
	s3->SetPosition(Vector3(0, 1000, 0));
	s1->meshc->SetTextureIndex(2);
	SideBlock* s4 = new SideBlock(this);
	s4->SetPosition(Vector3(0, -1000, 0));
	s1->meshc->SetTextureIndex(2);
	SideBlock* s5 = new SideBlock(this);
	s5->SetPosition(Vector3(0, 1500, 0));
	s1->meshc->SetTextureIndex(3);
	SideBlock* s6 = new SideBlock(this);
	s6->SetPosition(Vector3(0, -1500, 0));
	s1->meshc->SetTextureIndex(3);
	SideBlock* s7 = new SideBlock(this);
	s7->SetPosition(Vector3(0, 2000, 0));
	s1->meshc->SetTextureIndex(4);
	SideBlock* s8 = new SideBlock(this);
	s8->SetPosition(Vector3(0, -2000, 0));
	s1->meshc->SetTextureIndex(4);

	LoadLevel("Assets/Blocks/1.txt", 1000);
	LoadLevel("Assets/Blocks/2.txt", 2000);
	LoadLevel("Assets/Blocks/3.txt", 3000);
	
	/*
	while (mPlayer->GetState() != ActorState::Paused) {
		std::string path = "Assets/Blocks/";
		int type = rand() % 20;
		path += std::to_string(type);
		path += ".txt";
		LoadLevel(path, type * 1000);
	}*/
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

void Game::AddBlock(Block* b)
{
	mBlocks.emplace_back(b);
}

void Game::RemoveBlock(Block* b)
{
	auto iter = std::find(mBlocks.begin(), mBlocks.end(), b);
	if (iter != mBlocks.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mBlocks.end() - 1;
		std::iter_swap(iter, iter2);
		mBlocks.pop_back();
	}
}
