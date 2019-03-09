#include "SideBlock.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"

SideBlock::SideBlock(Game* game)
	:Actor(game)
{
	mGame = game;

	meshc = new MeshComponent(this);
	meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	meshc->SetTextureIndex(1);
	cc = new CollisionComponent(this);
	cc->SetSize(100.0f, 40.0f, 60.0f);

	mScale = 500;
}


void SideBlock::OnUpdate(float deltaTime) {
	if (mGame->mPlayer->GetPosition().x - mPosition.x >= 2000) {
		SetState(ActorState::Destroy);
	}
}