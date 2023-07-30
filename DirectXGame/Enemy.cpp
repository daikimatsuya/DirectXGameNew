#include "Enemy.h"
#include "GameScene.h"
#include "Player.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	// for (EnemyBullet* bullet : gameScene_->enemyBullets_) {
	//	delete bullet;
	// }
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = pos;
	AproachVelocity_ = {0, 0, -0.2f};
	LeaveVelocity_ = {0.0f, 0.0f, +0.2f};
	InitializeBulletPhase();
	hp_ = 6;
}
void (Enemy::*Enemy::MovePhase[])() = {
    &Enemy::Approach,
    &Enemy::Leave,
};
void Enemy::Update() {

	(this->*MovePhase[phase_])();
	worldTransform_.UpdateMatrix();

	// intervalTimer -= 1;
	// if (intervalTimer == 0) {
	//	Fire();
	//	intervalTimer = kInterval;
	// }
	// bullets_.remove_if([](EnemyBullet* bullet) {
	//	if (bullet->Isdead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	// });
	// for(EnemyBullet*bullet : bullets_) {
	//	bullet->Update();
	// }
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// for (EnemyBullet* bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	// }
}

void Enemy::Fire() {
	const float kBulletSpeed = -1.0f;

	Vector3 tmp = GetWorldPosition();
	Vector3 tmp2 = player_->GetWorldPosition();
	Vector3 tmp3 = amf_.Subtract(tmp, tmp2);
	Vector3 tmp4 = vf_.Normalize(tmp3);

	Vector3 velocity = vf_.Multiply(tmp4, kBulletSpeed);
	velocity = amf_.TransformNormal(velocity, worldTransform_.matWorld_);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::InitializeBulletPhase() { intervalTimer = kInterval; }

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::Leave() {
	Vector3 tmp2 = amf_.Add(worldTransform_.translation_, LeaveVelocity_);
	worldTransform_.translation_ = tmp2;
	if (worldTransform_.translation_.z > 50.0f) {
		phase_ = 0;
	}
}

void Enemy::OnCollision() { 
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

void Enemy::Approach() {
	Vector3 tmp = amf_.Add(worldTransform_.translation_, AproachVelocity_);
	worldTransform_.translation_ = tmp;
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = 1;
	}
}