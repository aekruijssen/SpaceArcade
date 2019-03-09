#include "Block.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"

Block::Block(Game* game)
	:Actor(game)
{
	mGame = game;

	meshc = new MeshComponent(this);
	meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	meshc->SetTextureIndex(2);
	cc = new CollisionComponent(this);
	cc->SetSize(1.0f, 1.0f, 1.0f);

	mScale = 25.0f;
	mGame->AddBlock(this);

	if (meshc->GetTextureIndex() == 4) {
		exploding = true;
	}
	else {
		exploding = false;
	}
}

Block::~Block() {
	mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime) {

	if (meshc->GetTextureIndex() == 4) {
		exploding = true;
	}
	else {
		exploding = false;
	}

	if (mGame->mPlayer->GetPosition().x - mPosition.x >= 2000) {
		SetState(ActorState::Destroy);
	}
}

void Block::explode() {
	Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
	if (this->GetState() == ActorState::Destroy) {
		return;
	}
	for (auto b : mGame->mBlocks) {
		Vector3 pos = this->GetPosition();
		this->SetState(ActorState::Destroy);
		if (b != nullptr) {
		//	if (b->GetState() != ActorState::Destroy) {
				if ((b->GetPosition() - pos).Length() <= 50.0f) {
					if (this->exploding == true) {
						b->explode();
						
					}
		//			if (b->GetState() != ActorState::Destroy) {
		//				b->SetState(ActorState::Destroy);
		//			}
				}
		//	}
		}
		else {
			return;
		}
	}
}