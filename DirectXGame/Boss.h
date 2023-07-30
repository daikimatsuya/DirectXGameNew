#pragma once
#include "AffineMatrixFunctions.h"
#include "EnemyBullet.h"
#include "Model.h"
#include "Vector3.h"
#include "VectorFanctions.h"
#include "WorldTransform.h"
#include "list"
#include "BossBullet.h"

class Player;
class GameScene;
class Boss {
public:
	Boss();
	~Boss();
	void Initialize(Model* model, uint32_t textureHandle,Vector3 pos);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	Vector3 GetWorldPosition();
	void SetPlayer(Player* player) { player_ = player; };
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	void OnCollision();
	void Attack();
	void Rest();
	void SetPhase();


 private:
	Model* model_ = nullptr;
	 Player* player_ = nullptr;
	 GameScene* gameScene_ = nullptr;
	 WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;
	 AffineMatrixFunctions amf_;
	VectorFanctions vf_;
	 int hp_;
	bool isDead_;
	 static void (Boss::*MovePhase[])();
	int phase_ = 0;

};
