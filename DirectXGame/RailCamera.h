#pragma once
#include "AffineMatrixFunctions.h"
#include "Input.h"
#include "MatrixFunctions.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class RailCamera {
public:
	void Initialize(WorldTransform worldTransform);
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	AffineMatrixFunctions AMF_;
	MatrixFunctions Mf_;
	Input* input_ = nullptr;
	Vector3 velocity_ = {};
	Vector3 rotate_ = {};
};