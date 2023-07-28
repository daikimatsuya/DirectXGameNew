#include "Skydome.h"

void Skydome::Initialize(Model* model, uint32_t textureHandle) { 
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	size_ = 30;

	worldTransform_.matWorld_.m[0][0] = size_;
	worldTransform_.matWorld_.m[1][1] = size_;
	worldTransform_.matWorld_.m[2][2] = size_;
}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
	