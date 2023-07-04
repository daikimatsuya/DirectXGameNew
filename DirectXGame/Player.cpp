#include "Player.h"
#include "AffineMatrixFunctions.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <cassert>

Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t tectureHandle, Vector3 position) {
	assert(model);
	model_ = model;
	tectureHandle_ = tectureHandle;
	worldTrasform_.Initialize();
	worldTransform3DReticle_.Initialize();
	worldTrasform_.translation_ = position;
	// viewProjection_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() {

	worldTrasform_.TransferMatrix();
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTrasform_.translation_.y += move.y;
	worldTrasform_.translation_.x += move.x;

	inputFloat[0] = worldTrasform_.translation_.x;
	inputFloat[1] = worldTrasform_.translation_.y;
	inputFloat[2] = worldTrasform_.translation_.z;

	const float MoveLimitX = 30;
	const float MoveLimitY = 15;
	Rotate();

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Player", inputFloat, -100, 100);
	ImGui::End();

	worldTrasform_.translation_.x = inputFloat[0];
	worldTrasform_.translation_.y = inputFloat[1];
	worldTrasform_.translation_.z = inputFloat[2];

	worldTrasform_.translation_.x =
	    std::clamp(worldTrasform_.translation_.x, -MoveLimitX, MoveLimitX);

	worldTrasform_.translation_.y =
	    std::clamp(worldTrasform_.translation_.y, -MoveLimitY, MoveLimitY);

	worldTrasform_.UpdateMatrix();

	const float kDistancePlayerTo3DReticle = 25.0f;
	Vector3 offset = {0, 0, 0.1f};
	offset = MF_->TransformNormal(offset, worldTrasform_.matWorld_);
	offset = VF_->Multiply(VF_->Normalize(offset), kDistancePlayerTo3DReticle);
	worldTransform3DReticle_.translation_ = VF_->Add(GetWorldPosition(), offset);
	worldTransform3DReticle_.UpdateMatrix();

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->Isdead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform3DReticle_, viewProjection, tectureHandle_);
	model_->Draw(worldTrasform_, viewProjection, tectureHandle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Rotate() {
	const float KRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTrasform_.rotation_.y += KRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTrasform_.rotation_.y -= KRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_Z)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = VF_->Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = VF_->Multiply(VF_->Normalize(velocity), kBulletSpeed);
		// velocity = AMF_->TransformNormal(velocity, worldTrasform_.matWorld_);
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTrasform_.matWorld_, velocity);
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTrasform_.matWorld_.m[3][0];
	worldPos.y = worldTrasform_.matWorld_.m[3][1];
	worldPos.z = worldTrasform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::Setparent(const WorldTransform* parent) { worldTrasform_.parent_ = parent; }