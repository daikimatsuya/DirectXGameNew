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
		delete sprite2DReticle_;
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
	uint32_t textureReticle_ = TextureManager::Load("picture/spikeneedle.png");
	sprite2DReticle_ = Sprite::Create(
	    textureReticle_, {1280 / 2, 720 / 2}, {0xff, 0xff, 0xff, 0xff}, {0.5f, 0.5f});
}

void Player::Update(const ViewProjection& viewProjection) {

	worldTrasform_.TransferMatrix();
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

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

	////3Dレティクル
	const float kDistancePlayerTo3DReticle = 70.0f;
	// Vector3 offset = {0, 0, 0.1f};
	// offset = MF_->TransformNormal(offset, worldTrasform_.matWorld_);
	// offset = VF_->Multiply(VF_->Normalize(offset), kDistancePlayerTo3DReticle);
	// worldTransform3DReticle_.translation_ = VF_->Add(GetWorldPosition(), offset);
	// worldTransform3DReticle_.UpdateMatrix();

	// 2Dレティクル
	// Vector3 positionReticle = {
	//     worldTransform3DReticle_.matWorld_.m[3][0],
	//	worldTransform3DReticle_.matWorld_.m[3][1],
	//     worldTransform3DReticle_.matWorld_.m[3][2]
	// };
	// Matrix4x4 matViewport =
	//     RPF_->MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// Matrix4x4 matViewProjectionViewPort = MF_->Multiply(
	//     viewProjection.matView, MF_->Multiply(viewProjection.matProjection, matViewport));
	// positionReticle = MF_->Transform(positionReticle, matViewProjectionViewPort);
	// sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	// レティクル->マウス->コントローラー

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(spritePosition);
	}

	/*POINT mousePosition;*/
	/*GetCursorPos(&mousePosition);*/

	/*HWND hwnd = WinApp::GetInstance()->GetHwnd();*/
	// ScreenToClient(hwnd, &mousePosition);
	/*sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));*/
	Matrix4x4 matViewport =
	    RPF_->MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = MF_->Multiply(
	    viewProjection.matView, MF_->Multiply(viewProjection.matProjection, matViewport));
	Matrix4x4 matInverseVPV = MF_->Inverse(matVPV);
	// Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
	// Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);

	Vector3 posNear = Vector3(
	    float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 0);
	Vector3 posFar = Vector3(
	    float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 1);
	posNear = MF_->Transform(posNear, matInverseVPV);
	posFar = MF_->Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = VF_->Subtract(posFar, posNear);
	mouseDirection = VF_->Normalize(mouseDirection);
	const float kDistanceTestObject = kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ =
	    VF_->Add(posNear, VF_->Multiply(mouseDirection, kDistanceTestObject));
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

	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) || input_->TriggerKey(DIK_Z)) {
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

void Player::DrawUI() { sprite2DReticle_->Draw(); }