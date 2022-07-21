#include "stdafx.h"
#include "SpeedEnemy.h"

#include "Player.h"
#include "Bullet.h"
#include "SpawnEnemy.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float CHARACON_RADIUS = 22.0f;             //キャラコンの半径
	const float CHARACON_HEIGHT = 20.0f;             //キャラコンの高さ
	const int   REACT_DAMAGE_VALUE = 1;              //受けるダメージの値
	const float MOVE_SPEED = 125.0f;                 //移動速度
	const int   HP_DOWN_VALUE = 0;                   //死亡時のHPの値
	const float MODEL_SCALE = 1.4f;                  //エネミーの大きさ
	const float ATTACK_COLLISION_SIZE_X = 30.0f;     //攻撃の当たり判定のX軸大きさ
	const float ATTACK_COLLISION_SIZE_Y = 30.0f;	 //攻撃の当たり判定のY軸大きさ
	const float ATTACK_COLLISION_SIZE_Z = 30.0f;	 //攻撃の当たり判定のZ軸大きさ
	const float IDLE_ANIMATION_SPEED = 1.0f;         //待機アニメーションの再生速度
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //待機アニメーションの補完時間
	const float RUN_ANIMATION_SPEED = 1.8f;          //走りアニメーションの再生速度
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //走りアニメーションの補完時間
	const float DOWN_ANIMATION_SPEED = 1.5f;         //ダウンアニメーションの再生時間
	const float DOWN_ANIMATION_INTERPOLATE = 0.1f;   //ダウンアニメーションの補完時間
	const float ATTACK_ANIMATION_SPEED = 1.0f;       //攻撃アニメーションの再生時間
	const float ATTACK_ANIMATION_INTERPOLATE = 0.3f; //攻撃アニメーションの補完時間
	const float UNDERGROUND_POSITION = 1000.0f;      //地下の座標
	const int   PROBABILITY = 100;                   //確率の数
	const int   ATTACK_PROBABILITY = 50;             //攻撃できる確率
	const float ATTACK_DIFF = 48.0f;                 //攻撃できる距離
	const float COLLIDER_RADIUS = 3.0f;              //スフィアコライダーの半径
	const float COLLIDER_HEIGHT = 60.0f;             //コライダーの発射時の高
	const float LOOKME_ANGLE = 0.3f;                 //プレイヤーを見つける角度
}

SpeedEnemy::SpeedEnemy()
{
}

SpeedEnemy::~SpeedEnemy()
{
	DeleteGO(m_walkse);
}

void SpeedEnemy::InitAnimation()
{
	//アニメーションクリップをロードする
	m_animClips[enAnimClip_Idle].Load("Assets/animData/speedenemy/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/speedenemy/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
	m_animClips[enAnimClip_Attack].Load("Assets/animData/speedenemy/attack.tka");
	m_animClips[enAnimClip_Attack].SetLoopFlag(false);
	m_animClips[enAnimClip_Down].Load("Assets/animData/speedenemy/down.tka");
	m_animClips[enAnimClip_Down].SetLoopFlag(false);

}

bool SpeedEnemy::Start()
{
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/enemydownsound.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/enemy/walksound2.wav");
	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/enemy/pumch.wav");
	//エフェクトを読み込んで登録する
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/effect/efk/blood.efk");

	//アニメーションの初期化
	InitAnimation();
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm", m_animClips, enAnimClip_Num);
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
	//スフィアコライダーを初期化。
	m_sphereCollider.Create(4.0f);
	//パンチのボーンを探す
	m_pumchBoneId = m_modelRender.FindBoneID(L"mixamorig1:LeftHand");
	//キャラコンを初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	//大きさを設定
	m_modelRender.SetScale(Vector3::One * MODEL_SCALE);
	//モデルの更新
	m_modelRender.Update();
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");
	m_spawnEnemy = FindGO<SpawnEnemy>("spawnenemy");

	m_nvmMesh.Init("Assets/nvm/test.tkn");
	return true;
}

void SpeedEnemy::Update()
{
	if (m_isActive == true)
	{
		MoveSound();
		PathMove();
		//プレイヤーから見られいるか？
		CantLookMe();
		//攻撃処理
		Attack();
		//回転処理
		Rotation();
		//追跡処理
		Chase();
		//アニメーションの再生
		PlayAnimation();
		//当たり判定処理
		Collision();
		//各ステートの遷移処理
		ManageState();
	}

	m_charaCon.SetPosition(m_position);
	m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//座標の設定
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(Vector3::One * MODEL_SCALE);
	m_modelRender.Update();
}

void SpeedEnemy::PathMove()
{
	if (m_enemyState != enEnemyState_PathChase)
	{
		return;
	}

	m_pathTimer += g_gameTime->GetFrameDeltaTime();

	Vector3 nextPosition = Vector3::Zero;
	bool isEnd;
	if (m_pathTimer >= 0.5f)
	{
		// パス検索
		m_pathFiding.Execute(
			m_path,							// 構築されたパスの格納先
			m_nvmMesh,						// ナビメッシュ
			m_position,						// 開始座標
			m_player->GetPosition(),		// 移動目標座標
			PhysicsWorld::GetInstance(),	// 物理エンジン	
			50.0f,							// AIエージェントの半径
			200.0f							// AIエージェントの高さ。
		);
		m_pathTimer = 0.0f;
	}

	// パス上を移動する。
	nextPosition = m_path.Move(
		m_position,
		5.0f,
		isEnd
	);

	Vector3 vector = nextPosition - m_position;
	vector.Normalize();

	m_moveSpeed = vector * 125.0f;
	m_charaCon.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

}

void SpeedEnemy::DeActive()
{
	//アクティブではなかったら
	if (m_isActive == false)
	{
		//待機ステートに移行する
		m_enemyState = enEnemyState_Idle;
		//待機座標に移動する
		m_position = m_spawnEnemy->GetStayPoint();
		//キャラコンを移動させる
		m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
		//HPを元に戻す
		m_hp = 3;
		m_pathTimer = 1.0f;
	}
}

void SpeedEnemy::Rotation()
{
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//回転の設定
	m_modelRender.SetRotation(m_rotation);
}

void SpeedEnemy::Chase()
{
	//追跡ステートではなかったら
	if (m_enemyState != enEnemyState_Chase)
	{
		//何もしない
		return;
	}
	//プレイヤーに向かうベクトル
	Vector3 vector = m_player->GetPosition() - m_position;
	//正規化
	vector.Normalize();
	//移動速度＝プレイヤーに向かうベクトル×速度
	m_moveSpeed = vector * MOVE_SPEED;
	m_charaCon.SetPosition(m_position);
	//キャラコンを使って座標を移動させる
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void SpeedEnemy::Attack()
{
	//攻撃ステートではなかったら
	if (m_enemyState != enEnemyState_Attack)
	{
		//何もしない
		return;
	}
	//攻撃中なら
	if (m_attacking == true)
	{

		//攻撃の当たり判定を作成する
		MakeAttackCollision();
	}
}

void SpeedEnemy::MakeAttackCollision()
{
	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_pumchBoneId)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position,
		Quaternion::Identity,
		Vector3(ATTACK_COLLISION_SIZE_X,
			ATTACK_COLLISION_SIZE_Y,
			ATTACK_COLLISION_SIZE_Z));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

void SpeedEnemy::Collision()
{
	//ダウンステートだったら
	if (m_enemyState == enEnemyState_Down)
	{
		//何もしない
		return;
	}

	{
		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet");
		//配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				m_mitukatta = true;
				//HPを減らす。
				m_hp -= REACT_DAMAGE_VALUE;

				//コリジョンを非アクティブにする
				collision->Dead();
				collision->Deactivate();
				//HPが0になったら。
				if (m_hp <= HP_DOWN_VALUE)
				{
					//ダウンステートに移行する
					m_enemyState = enEnemyState_Down;

					//銃声SEを作成する
					m_se = NewGO<SoundSource>(0);
					//銃声SEを初期化する
					m_se->Init(5);
					//銃声SEの大きさを設定
					m_se->SetVolume(0.2f);
					m_se->SetPosition(m_position);
					//銃声SEを再生する（ループなし）
					m_se->Play(false);


					m_charaCon.RemoveRigidBoby();

					//カウントを進める
					int counter = m_player->GetKillEnemyCount();
					counter += 1;
					m_player->SetKillEnemyCount(counter);
				}
				return;
			}
		}
	}
}

void SpeedEnemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case SpeedEnemy::enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//追跡ステートの時
	case SpeedEnemy::enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
	case SpeedEnemy::enEnemyState_PathChase:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//走りステートの時
	case SpeedEnemy::enEnemyState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//ダウンステートの時
	case SpeedEnemy::enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, DOWN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(DOWN_ANIMATION_SPEED);
		break;
		//攻撃ステートの時
	case SpeedEnemy::enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimClip_Attack, ATTACK_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(ATTACK_ANIMATION_SPEED);
		break;
	}
}

void SpeedEnemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case SpeedEnemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case SpeedEnemy::enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	case SpeedEnemy::enEnemyState_PathChase:
		ProcessChaseStateTransition();
		break;
		//走りステートの時
	case SpeedEnemy::enEnemyState_Run:
		ProcessRunStateTransition();
		break;
		//ダウンステートの時
	case SpeedEnemy::enEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//攻撃ステートの時
	case SpeedEnemy::enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void SpeedEnemy::ProcessCommonStateTransition()
{
	//プレイヤーを見つけたなら
	if (m_mitukatta == true)
	{
		//プレイヤーを攻撃できるなら
		if (CanAttack() == true)
		{
			//確率で攻撃ステートか待機ステートに移行する
			int randm = rand() % PROBABILITY;
			if (randm >= ATTACK_PROBABILITY)
			{
				//攻撃ステートに移行する
				m_enemyState = enEnemyState_Attack;
			}
			else
			{
				//待機ステートに移行する
				m_enemyState = enEnemyState_Idle;
			}
			return;
		}
		//攻撃できないなら
		else
		{
			//追跡ステート移行する
			m_enemyState = enEnemyState_Chase;
			return;
		}
	}
	//プレイヤーを見つけてなかったら
	else
	{
		//待機ステートに移行する
		m_enemyState = enEnemyState_PathChase;
	}
}

void SpeedEnemy::ProcessIdleStateTransition()
{
	//共通のステートの遷移処理へ
	ProcessCommonStateTransition();
}

void SpeedEnemy::ProcessChaseStateTransition()
{
	//共通のステートの遷移処理へ
	ProcessCommonStateTransition();
}

void SpeedEnemy::ProcessRunStateTransition()
{
	//共通のステートの遷移処理へ
	ProcessCommonStateTransition();
}

void SpeedEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステートの遷移処理へ
		ProcessCommonStateTransition();
	}
}

void SpeedEnemy::ProcessDownStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//非アクティブにする
		m_isActive = false;
		//ステータスを初期化する
		DeActive();
	}
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}
		
		//壁とぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};

void SpeedEnemy::CantLookMe()
{
	m_mitukatta = false;

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));

	//プレイヤーが視界内に居なかったら。
	if (Math::PI * LOOKME_ANGLE <= fabsf(angle))
	{
		//プレイヤーは見つかっていない。
		return;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y + COLLIDER_HEIGHT, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + COLLIDER_HEIGHT, playerPosition.z));

	SweepResultWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		//プレイヤーは見つかっていない。
		return;
	}

	//壁と衝突してない！！
	//プレイヤー見つけたフラグをtrueに。
	m_mitukatta = true;
}

void SpeedEnemy::MoveSound()
{

}

const bool SpeedEnemy::CanAttack() const
{
	//プレイヤーを見つけたなら
	if (m_mitukatta == true)
	{
		//プレイヤーとの距離を計算する
		Vector3 diff = m_player->GetPosition() - m_position;
		//ある程度近かったら
		if (diff.LengthSq() <= ATTACK_DIFF * ATTACK_DIFF)
		{
			//攻撃できる
			return true;
		}
	}
	//攻撃できない
	return false;
}

void SpeedEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「attack_start」なら
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃中
		SoundSource* se;
		se = NewGO<SoundSource>(0);
		se->Init(22);
		se->SetVolume(0.4f);
		se->Play(false);
		m_attacking = true;
	}
	//キーの名前が「attack_end」なら
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃中じゃない
		m_attacking = false;
	}

}

void SpeedEnemy::Render(RenderContext& rc)
{
	//アクティブだったら
	if (m_isActive == true)
	{
		//モデルを描画する
		m_modelRender.Draw(rc);
	}
}
