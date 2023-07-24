#include "Skydome.h"

void Skydome::Initialize(Model* model, uint32_t textureHandle) { 
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	size_ = 1;
}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
	