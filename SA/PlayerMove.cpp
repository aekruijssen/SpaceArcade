#include "PlayerMove.h"
#include "Game.h"
#include "MoveComponent.h"
#include "Player.h"
#include "Renderer.h"
#include "SideBlock.h"
#include "MeshComponent.h"
#include "Bullet.h"
#include "Block.h"
#include "Player.h"
#include "CollisionComponent.h"

PlayerMove::PlayerMove(class Player* owner)
	:MoveComponent(owner)
	, mXSpeed(400.0f)
	, mYSpeed(0.0f)
	, mZSpeed(0.0f)
{
	mPlayer = owner;

	mSpacePressed = false;
	mGame = mPlayer->GetGame();

	speed = Vector3(400.0f, 0.0f, 0.0f);

	attacking = 0.0f;
	invulnerability = 0.0f;

	lastBlock = 3000;
	lastSideBlock = 0;
	textureCounter = 5;

	speedMultiplier = 1.0f;
	speedTime = 0.0f;
}

// Use the left and right arrow keys 
void PlayerMove::ProcessInput(const Uint8 *keyState) {
	//float move = 0;

	if (keyState[SDL_SCANCODE_W]) {
		if (!keyState[SDL_SCANCODE_S]) {
			speed.z = 300.0f;
			mZSpeed = (speedMultiplier + 1) *300.0f;
		}
	}
	else if (keyState[SDL_SCANCODE_S]) {
		speed.z = -300.0f;
		mYSpeed = (speedMultiplier + 1) * -300.0f;
	}
	else if (keyState[SDL_SCANCODE_A]) {
		if (!keyState[SDL_SCANCODE_D]) {
			speed.y = -300.0f;
			mYSpeed = (speedMultiplier + 1) * -300.0f;
		}
	}
	else if (keyState[SDL_SCANCODE_D]) {
		speed.y = 300.0f;
		mYSpeed = (speedMultiplier + 1) *300.0f;
	}
	else {
		speed.y = 0.0f;
		speed.z = 0.0f;
		mYSpeed = 0.0f;
		mZSpeed = 0.0f;
	}
	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed) {
		//attacking = 0.25f;
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Shoot.wav"), 0);
		Bullet* b = new Bullet(mGame);
		b->SetPosition(mPlayer->GetPosition());
	}
	// At the end of ProcessInput, save the current keyState value in
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}

void PlayerMove::Update(float deltaTime) {
	speedTime += deltaTime;
	if (speedTime >= 10.0f) {
		speedMultiplier += 0.15f;
		speedTime = 0.0f;
	}
	for (auto block : (mGame->mBlocks)) {
		//if (block->cc != nullptr && GetState() != ActorState::Destroy && this->cc != nullptr) {
			if ((mPlayer->cc)->Intersect(block->cc)) {
				//block->SetState(ActorState::Destroy);
				Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/ShipDie.wav"), 0);
				mPlayer->SetState(ActorState::Paused);
				break;
			}
		//}
	}

	Vector3 cur = mPlayer->GetPosition();

	if (cur.x + 3000 - lastBlock >= 1000) {
		std::string path = "Assets/Blocks/";
		if (cur.x <= 18000) {
			path += std::to_string((int)(lastBlock + 1000) / 1000);
		}
		else {
			int rando = (int)((rand() % 20) + 1);
			path += std::to_string(rando);
		}
		path += ".txt";
		mGame->LoadLevel(path, lastBlock + 1000);
		lastBlock += 1000;
	}

	// Side Blocks
	if (cur.x + 3000 - lastSideBlock >= 500) {
		SideBlock* s = new SideBlock(mGame);
		s->SetPosition(Vector3(lastSideBlock+500, 500, 0));
		SideBlock* s2 = new SideBlock(mGame);
		s2->SetPosition(Vector3((lastSideBlock+500) , -500, 0));
		lastSideBlock += 500;
		s->meshc->SetTextureIndex(textureCounter);
		s2->meshc->SetTextureIndex(textureCounter);
		if (textureCounter == 5) {
			textureCounter = 1;
		}
		else if(textureCounter == 3) {
			textureCounter += 2;
		}
		else {
			textureCounter++;
		}
	}
	
	cur.x += speed.x * deltaTime + (speedMultiplier + 1);
	SetForwardSpeed(speed.x + (speedMultiplier + 1));
	cur.y += speed.y * deltaTime;
	cur.z += speed.z * deltaTime;

	
	if (mPlayer->GetPosition().y < -195) {
		cur.y = -195;
	}
	else if (mPlayer->GetPosition().y > 195) {
		cur.y = 195;
	}
	if (mPlayer->GetPosition().z < -225) {
		cur.z = -225;
	}
	else if (mPlayer->GetPosition().z > 225) {
		cur.z = 225;
	}
	
	mPlayer->SetPosition(cur);

	Vector3 cam = Vector3(mPlayer->GetPosition() -
		(mPlayer->GetForward() * 300) + ((Vector3::UnitZ * 100)));

	Vector3 target = mPlayer->GetPosition() + (mPlayer->GetForward() * 20);
	mGame->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(cam, target, Vector3::UnitZ));
}