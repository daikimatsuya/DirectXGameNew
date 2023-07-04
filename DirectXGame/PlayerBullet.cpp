#include "PlayerBullet.h"
#include "Model.h"
#include "WorldTransform.h"
#include "assert.h"

void PlayerBullet::Initialize(Model* model, const Matrix4x4& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("picture/black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = {position.m[3][0], position.m[3][1], position.m[3][2]};
}

void PlayerBullet::Update() {
	Vector3 tmp = amf_.Add(worldTransform_.translation_, velocity_);
	worldTransform_.translation_ = tmp;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isdead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPos() {

	Vector3 tmp = amf_.Add(worldTransform_.translation_, velocity_);

	return tmp;
}

void PlayerBullet::OnCollision() { isdead_ = true; }