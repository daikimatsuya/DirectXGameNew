#include "GameScene.h"
#include "AxisIndicator.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "ImGuiManager.h"
#include "Player.h"
#include "Boss.h"
#include "TextureManager.h"
#include "fstream"
#include "sstream"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete model_;
	delete debugCamera_;
	for (Enemy* enemy_ : enemys_) {
		delete enemy_;
	}
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
	LoadEnemyPopData();
	
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	player_ = new Player();
	boss_ = new Boss();
	/*enemy_ = new Enemy();*/
	intervalTimer = 60;
	skydome_ = new Skydome();
	railCamera_ = new RailCamera();
	player_->Setparent(&railCamera_->GetWorldTransform());
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	
	/*enemy_->SetPlayer(player_);*/
	model_ = Model::Create();
	viewPlojection_.Initialize();
	textuerHandle_ = TextureManager::Load("Player/Player.png");
	textureHandleEnemy_ = TextureManager::Load("picture/enemy.png");
	textureHandleSkydome_ = TextureManager::Load("skydome/OIP.jpg");
	Vector3 playerPosition = {0, 0, 40};
	Vector3 bossPosition = {3, 0, 100};
	player_->Initialize(model_, textuerHandle_, playerPosition);
	boss_->Initialize(model_, textureHandleEnemy_, bossPosition);
	/*enemy_->Initialize(model_, textureHandleEnemy_, {20,5,50});*/
	/*enemy_->SetGameScene(this);*/
	skydome_->Initialize(modelSkydome_, textureHandleSkydome_);
	railCamera_->Initialize();
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewPlojection_);
}

void GameScene::Update() {
	UpdateEnemyPopCommands();
	player_->Update(viewPlojection_);
	boss_->Update();
	/*enemy_->Update();*/
	for (Enemy* enemy_ : enemys_) {
		enemy_->Update();
	}
	skydome_->Update();
	railCamera_->Update();
	viewPlojection_.matView = railCamera_->GetViewProjection().matView;
	viewPlojection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewPlojection_.TransferMatrix();

	intervalTimer -= 1;
	if (intervalTimer == 0) {
		/*enemy_->Fire();
		intervalTimer = enemy_->kInterval;*/
		for (Enemy* enemy_ : enemys_) {
			enemy_->Fire();
		}
		intervalTimer = 60;
	}
	enemys_.remove_if([](Enemy* enemy) { 
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->Isdead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	GetAllColisions();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewPlojection_.matView = debugCamera_->GetViewProjection().matView;
		viewPlojection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewPlojection_.TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw(viewPlojection_);
	player_->Draw(viewPlojection_);
	boss_->Draw(viewPlojection_);
	for (Enemy* enemy_ : enemys_) {
		enemy_->Draw(viewPlojection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewPlojection_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GetAllColisions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	posA = player_->GetWorldPosition(); // プレイヤーと敵の弾
	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();
		float length =
		    ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		     (posB.z - posA.z) * (posB.z - posA.z));
		if (length < (1 + 2) * (1 + 2)) {
			// player_->OnCollision();
			bullet->OnCollision();
		}
	}

	posA = boss_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPos();
		float length =
		    ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		     (posB.z - posA.z) * (posB.z - posA.z));
		if (length < (1 + 5) * (1 + 5)) {
			boss_->OnCollision();
			bullet->OnCollision();
		}
	}
	// 敵とプレイヤーの弾
	for (Enemy* enemy_ : enemys_) {

		posA = enemy_->GetWorldPosition();
		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPos();
			float length =
			    ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			     (posB.z - posA.z) * (posB.z - posA.z));
			if (length < (1 + 2) * (1 + 2)) {
				enemy_->OnCollision();
				bullet->OnCollision();
			}
		}
	}
}



void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());
	enemyPopCommands << file.rdbuf();
	file.close();
	waitFlag = false;
}

void GameScene::UpdateEnemyPopCommands() {
	
	std::string line;
	if (waitFlag != true) {
		while (getline(enemyPopCommands, line)) {
			std::istringstream line_stream(line);
			std::string word;
			getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}

			if (word.find("POP") == 0) {
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				CreateEnemy({x, y, z});
			}
			if (word.find("WAIT")) {
				getline(line_stream, word, ',');

				int32_t waitTime = atoi(word.c_str());

				waitFlag = true;
				waitTimer = waitTime;

				break;
			}
		}
	}
	if (waitFlag) {
		waitTimer--;

		if (waitTimer <= 0) {
			waitFlag = false;
		}

		return;
	}
}

void GameScene::CreateEnemy(Vector3 pos) {
	;
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, textureHandleEnemy_, pos);
	newEnemy->SetGameScene(this);
	newEnemy->SetPlayer(player_);
	enemys_.push_back(newEnemy);
}

bool GameScene::GetSpawnEnd() { return isSpawn; }
