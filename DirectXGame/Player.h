#pragma once

#include "AffineMatrixFunctions.h"
#include "Input.h"
#include "MatrixFunctions.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "RenderingPipelineFunctions.h"
#include "Sprite.h"
#include "VectorFanctions.h"
#include "WorldTransform.h"
#include "list"

class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, uint32_t tectureHandle, Vector3 position);
	void Update(const ViewProjection& viewProjection);
	void Draw(const ViewProjection& viewProjection);
	void Rotate();
	void Attack();
	Vector3 GetWorldPosition();
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	void Setparent(const WorldTransform* parent);
	void OnCollision();
	void DrawUI();

private:
	WorldTransform worldTrasform_;
	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DReticle_ = nullptr;
	// ViewProjection viewProjection_;
	Model* model_ = nullptr;
	uint32_t tectureHandle_ = 0u;
	Input* input_ = nullptr;
	AffineMatrixFunctions* AMF_ = nullptr;
	MatrixFunctions* MF_ = nullptr;
	VectorFanctions* VF_ = nullptr;
	RenderingPipelineFunctions* RPF_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	float inputFloat[3] = {};
};