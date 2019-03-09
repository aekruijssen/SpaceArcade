#pragma once
#include "Actor.h"
#include "Game.h"

class Bullet : public Actor
{
public:
	Bullet(class Game* game);
	void OnUpdate(float deltaTime) override;

	class MeshComponent* meshc;
	class CollisionComponent* cc;
	class MoveComponent* mc;

private:
	float lifetime;
};