#include "RailCamera.h"
#include "AffineMatrixFunctions.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "MatrixFunctions.h"
void RailCamera::Initialize(WorldTransform worldTransform) {
	input_ = Input::GetInstance();
	worldTransform_ = worldTransform;
	rotate_ = worldTransform.rotation_;
	viewProjection_.Initialize();
	velocity_.z = -0.01f;
	rotate_.z = 0.0f;
}

void RailCamera::Update() {
	;
	Vector3 tmp = AMF_.Add(worldTransform_.translation_, velocity_);
	worldTransform_.translation_ = tmp;

	Vector3 tmp2 = AMF_.Add(worldTransform_.rotation_, {0.0f, -0.00f, 0.0f});
	worldTransform_.rotation_ = tmp2;

	worldTransform_.UpdateMatrix();
	viewProjection_.matView = Mf_.Inverse(worldTransform_.matWorld_);
	float inputfloat[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float inputfloat2[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};
	ImGui::Begin("Camera");
	ImGui::InputFloat3("CameraTranslation", inputfloat);
	ImGui::InputFloat3("CameraRotate", inputfloat2);
	ImGui::End();
}