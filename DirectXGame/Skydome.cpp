#include "Skydome.h"

void Skydome::Initialize(Model* model, uint32_t textureHandle) {
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_.Initialize();
}

void Skydome::Update() { worldTransform_.TransferMatrix(); }

void Skydome::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}