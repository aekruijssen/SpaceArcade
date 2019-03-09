#pragma once

#include "Actor.h"
#include "Game.h"

class Block : public Actor
{
public:
	Block(class Game* game);
	~Block();
	void OnUpdate(float deltaTime) override;

	class MeshComponent* meshc;
	class CollisionComponent* cc;

	bool exploding;
	void explode();

private:
};
