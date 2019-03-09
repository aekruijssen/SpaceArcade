#pragma once
#include "Actor.h"
#include "Game.h"

class SideBlock : public Actor
{
public:
	SideBlock(class Game* game);
	void OnUpdate(float deltaTime) override;

	class MeshComponent* meshc;
	class CollisionComponent* cc;


private:
};
