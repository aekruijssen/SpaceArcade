#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);

	// Update the move component
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8 *keyState) override;

	// Getters/setters
	float GetYSpeed() const { return mYSpeed; }
	float GetXSpeed() const { return mXSpeed; }
	float GetZSpeed(float speed) { return mZSpeed; }
	void SetYSpeed(float speed) { mYSpeed = speed; }
	void SetXSpeed(float speed) { mXSpeed = speed; }
	void SetZSpeed(float speed) { mZSpeed = speed; }

	Player* mPlayer;
	Vector2 mPlayerVel;

	Vector3 speed;
	float speedMultiplier;

private:

	// Angular speed (in radians/second)
	float mAngularSpeed;
	// Forward speed (in pixels/second)
	float mXSpeed;
	float mYSpeed;
	float mZSpeed;

	bool mSpacePressed;
	bool mInAir;
	const Uint8 *lastFrame;

	class Game* mGame;

	float attacking;
	float invulnerability;

	float lastSideBlock;
	float lastBlock;
	int textureCounter;

	
	float speedTime;
};
