#include "Boss.h"
#include <cassert>
#include "Player.h"
#include "GameScene.h"

Boss::Boss() {}

Boss::~Boss() {}

void Boss::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	//textureHandle_ = textureHandle;
	worldTransform_.translation_ = pos;
	worldTransform_.scale_.x = 5;
	worldTransform_.scale_.y = 5;
	worldTransform_.scale_.z = 5;
	hp_ = 50;
}

void Boss::Update() {
	(this->*MovePhase[phase_])();
	worldTransform_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 Boss::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Boss::OnCollision() {
	hp_--; 
	if (hp_ <= 0) {
		isDead_ = true;
	}
}
void Boss::Attack() {

	if (hp_ == 30) {
		SetPhase();
	}
	if (hp_ == 15) {
		SetPhase();
	}
	if (hp_ == 2) {
		SetPhase();
	}
}
void Boss::Rest() {
	phase_ = 0;
}
void Boss::SetPhase() { phase_ = 1; }


void (Boss::*Boss::MovePhase[])() = {
    &Boss::Attack,
    &Boss::Rest,
};
