#pragma once
#include "AffineMatrixFunctions.h"
#include "EnemyBullet.h"
#include "Model.h"
#include "Vector3.h"
#include "VectorFanctions.h"
#include "WorldTransform.h"
#include "list"
class Player;
class GameScene;
class Enemy {
public:
	Enemy();
	~Enemy();
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Fire();
	void InitializeBulletPhase();
	void SetPlayer(Player* player) { player_ = player; };
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	Vector3 GetWorldPosition();
	// const std::list<EnemyBullet*>& GetBullets() { return bullets_; }
	void OnCollision();
	void Approach();
	void Leave();
	bool IsDead() { return isDead_; }
	static const int kInterval = 60;

private:
	// enum Phase {
	//	Approach,	//接近
	//	Leave,		//離脱
	// };
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
	Vector3 AproachVelocity_ = {0, 0, 0};
	Vector3 LeaveVelocity_ = {0, 0, 0};
	uint32_t textureHandle_ = 0;
	AffineMatrixFunctions amf_;
	VectorFanctions vf_;
	// std::list<EnemyBullet*> bullets_;
	uint32_t intervalTimer = 0;
	static void (Enemy::*MovePhase[])();
	int phase_ = 0;
	int hp_;
	bool isDead_;
};