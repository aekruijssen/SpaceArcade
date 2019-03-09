#include "Bullet.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "MoveComponent.h"
#include "Block.h"
#include "PlayerMove.h"

Bullet::Bullet(Game* game)
	:Actor(game)
{
	mGame = game;

	meshc = new MeshComponent(this);
	meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
	//meshc->SetTextureIndex(1);
	cc = new CollisionComponent(this);
	cc->SetSize(10.0f, 10.0f, 10.0f);
	mc = new MoveComponent(this);
	//mc->SetForwardSpeed(900.0f);
	//mc->SetForwardSpeed(500.0f + mGame->mPlayer->pm->speed.x);
	mc->SetForwardSpeed(400 * mGame->mPlayer->pm->speedMultiplier + 500);
	mScale = 0.5f;
	lifetime = 0.0f;

}



void Bullet::OnUpdate(float deltaTime) {

	lifetime += deltaTime;
	for (auto block : (mGame->mBlocks)) {
		if (block->cc != nullptr && GetState() != ActorState::Destroy && this->cc != nullptr) {
			if ((this->cc)->Intersect(block->cc)) {
				if (block->exploding == true) {
					block->explode();
				}
				else {
					block->SetState(ActorState::Destroy);
				}
				this->SetState(ActorState::Destroy);
				break;
			}
		}
	}

	/*
	for (auto b : mGame->mBlocks) {
		if (this->cc->Intersect(b->cc)) {
			b->SetState(ActorState::Destroy);
			//this->SetState(ActorState::Destroy);
			//break;
		}
	}*/

	if (lifetime >= 1.0f && GetState() != ActorState::Destroy) {
		this->SetState(ActorState::Destroy);
	}
}