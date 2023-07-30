#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy;
class Player;
class EnemyBullet;
class Boss;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GetAllColisions();

	void AddEnemyBullet(EnemyBullet* enemyBullet);
	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets_; }
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	void CreateEnemy(Vector3 pos);
	bool GetSpawnEnd();

private: // メンバ変数
	bool isSpawn = false;
	DirectXCommon* dxCommon_ = nullptr;
	ViewProjection viewPlojection_;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Player* player_ = nullptr;
	Boss *boss_=nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	// Enemy* enemy_=nullptr;
	uint32_t textuerHandle_ = 0;
	uint32_t textureHandleEnemy_ = 0;
	uint32_t textureHandleSkydome_ = 0;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	int intervalTimer;
	bool waitFlag = false;
	int32_t waitTimer;

	std::list<EnemyBullet*> enemyBullets_;
	std::stringstream enemyPopCommands;
	std::list<Enemy*> enemys_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};