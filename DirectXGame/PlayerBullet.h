#pragma once
#include "AffineMatrixFunctions.h"
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Matrix4x4& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool Isdead() const { return isdead_; }
	Vector3 GetWorldPos();
	void OnCollision();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;
	AffineMatrixFunctions amf_;
	static const int32_t klifeTimer = 60 * 5;
	int32_t deathTimer_ = klifeTimer;
	bool isdead_ = false;
};