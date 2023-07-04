#pragma once
#include "AffineMatrixFunctions.h"
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
class EnemyBullet {
public:
	EnemyBullet();
	~EnemyBullet();
	void Initialize(Model* model, const Vector3 position, const Vector3 velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	Vector3 GetWorldPosition();
	bool Isdead() const { return isdead_; }
	void OnCollision();

private:
	WorldTransform worldTransform_;
	AffineMatrixFunctions amf_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_ = {};
	static const int32_t klifeTimer = 60 * 5;
	int32_t deathTimer_ = klifeTimer;
	bool isdead_ = false;
};