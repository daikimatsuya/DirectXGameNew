#pragma once
#include "WorldTransform.h"
#include "AffineMatrixFunctions.h"
#include "input.h"
#include "MatrixFunctions.h"
#include "ViewProjection.h"

class RailCamera {
public:
	void Initialize();
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; };
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	AffineMatrixFunctions amf_;
	MatrixFunctions mf_;
	Input* input_ = nullptr;
	Vector3 velocity_ = {};
	Vector3 rotate_ = {};
};
