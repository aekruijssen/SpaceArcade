#include "Player.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "PlayerMove.h"

Player::Player(Game* game)
	:Actor(game)
{
	mGame = game;

	meshc = new MeshComponent(this);
	meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));

	cc = new CollisionComponent(this);
	cc->SetSize(100.0f, 40.0f, 60.0f);

	pm = new PlayerMove(this);
}