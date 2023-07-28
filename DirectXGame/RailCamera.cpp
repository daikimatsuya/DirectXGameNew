#include "RailCamera.h"


void RailCamera::Initialize() { 
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	velocity_ = {};
	rotate_ = {};

}

void RailCamera::Update() { 
	Vector3 tmp = amf_.Add(worldTransform_.translation_, velocity_);
	worldTransform_.translation_ = tmp;

	Vector3 tmp2 = amf_.Add(worldTransform_.rotation_, rotate_);
	worldTransform_.rotation_ = tmp2;

	worldTransform_.UpdateMatrix();
	viewProjection_.matView = mf_.Inverse(worldTransform_.matWorld_);

}
