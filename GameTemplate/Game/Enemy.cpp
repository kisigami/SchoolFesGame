#include "stdafx.h"
#include "Enemy.h"

#include "Bullet.h"
#include "Player.h"
#include "collision/CollisionObject.h"

#include "SpawnEnemy.h"

namespace
{
	const float CHARACON_RADIUS = 12.0f;             //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;             //キャラコンの高さ
	const int   REACT_DAMAGE = 1;                    //受けるダメージ量
	const float MOVE_SPEED = 70.0f;                  //移動速度
	const int   HP_ZERO = 0;                         //０
	const float ENEMY_SCALE = 1.0f;                  //エネミーの大きさ
	const float ATTACK_COLLISION_SIZE_X = 20.0f;     //攻撃の当たり判定のX軸大きさ
	const float ATTACK_COLLISION_SIZE_Y = 20.0f;	 //攻撃の当たり判定のY軸大きさ
	const float ATTACK_COLLISION_SIZE_Z = 20.0f;	 //攻撃の当たり判定のZ軸大きさ
	const float IDLE_ANIMATION_SPEED = 1.0f;         //待機アニメーションの再生速度
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //待機アニメーションの補完時間
	const float RUN_ANIMATION_SPEED = 1.0f;          //走りアニメーションの再生速度
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //走りアニメーションの補完時間
	const float DOWN_ANIMATION_SPEED = 1.0f;         //ダウンアニメーションの再生時間
	const float DOWN_ANIMATION_INTERPOLATE = 0.1f;   //ダウンアニメーションの補完時間
	const float ATTACK_ANIMATION_SPEED = 1.0f;       //攻撃アニメーションの再生時間
	const float ATTACK_ANIMATION_INTERPOLATE = 0.3f; //攻撃アニメーションの補完時間
	const float UNDERGROUND_POSITION = 1000.0f;      //地下の座標
	const int   PROBABILITY = 100;                   //確率の数
	const int   ATTACK_PROBABILITY = 50;             //攻撃できる確率
	const float ATTACK_DIFF = 50.0f;                 //攻撃できる距離
	const float COLLIDER_HEIGHT = 70.0f;             //コライダーの高さ
	const float LOOKME_ANGLE = 0.5f;                 //プレイヤーを見つける角度
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::InitAnimation()
{
	//アニメーションクリップをロードする
	m_animClips[enAnimClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
	m_animClips[enAnimClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animClips[enAnimClip_Down].SetLoopFlag(false);
	m_animClips[enAnimClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animClips[enAnimClip_Attack].SetLoopFlag(false);
}

bool Enemy::Start()
{
	//アニメーションの初期化
	InitAnimation();
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animClips, enAnimClip_Num);
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);
	//パンチのボーンを探す
	m_pumchBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//キャラコンを初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT,m_position);
	//モデルの更新
	m_modelRender.Update();
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");
	m_spawnEnemy = FindGO<SpawnEnemy>("spawnenemy");

	return true;
}

void Enemy::Update()
{
	//プレイヤーから見られいるか？
	//CantLookMe();
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

	//座標の設定
	m_modelRender.SetPosition(m_position);
	//大きさの設定
	m_modelRender.SetScale(Vector3::One*ENEMY_SCALE);
	//モデルの更新
	m_modelRender.Update();
}

void Enemy::Rotation()
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

void Enemy::Chase()
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
	//座標の設定
	m_modelRender.SetPosition(m_position);
}

void Enemy::Attack()
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

void Enemy::MakeAttackCollision()
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

void Enemy::Collision()
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
				//HPを減らす。
				m_hp -= REACT_DAMAGE;
				//コリジョンを非アクティブにする
				collision->Dead();
				collision->Deactivate();
				//HPが0になったら。
				if (m_hp <= HP_ZERO)
				{
					//キャラコンをエンジンから削除
					m_charaCon.RemoveRigidBoby();
					//ダウンステートに移行する
					m_enemyState = enEnemyState_Down;
				}
				return;
			}
		}
	}
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case Enemy::enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//追跡ステートの時
	case Enemy::enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//走りステートの時
	case Enemy::enEnemyState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//ダウンステートの時
	case Enemy::enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, DOWN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(DOWN_ANIMATION_SPEED);
		break;
		//攻撃ステートの時
	case Enemy::enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimClip_Attack, ATTACK_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(ATTACK_ANIMATION_SPEED);
		break;
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case Enemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case Enemy::enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//走りステートの時
	case Enemy::enEnemyState_Run:
		ProcessRunStateTransition();
		break;
		//ダウンステートの時
	case Enemy::enEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//攻撃ステートの時
	case Enemy::enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Enemy::ProcessCommonStateTransition()
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
		m_enemyState = enEnemyState_Idle;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//共通のステートの遷移処理へ
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
{
	//共通のステートの遷移処理へ
	ProcessCommonStateTransition();
}

void Enemy::ProcessRunStateTransition()
{
	//共通のステートの遷移処理へ
	ProcessCommonStateTransition();
}

void Enemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステートの遷移処理へ
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessDownStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_isActive = false;
		m_hp = 6;
		m_position = m_spawnEnemy->GetStayPoint();
		m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
		m_enemyState = enEnemyState_Idle;
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

void Enemy::CantLookMe()
{
	m_mitukatta = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	if (diff.LengthSq() >= 500.0f * 500.0f)
	{
		return;
	}
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

const bool Enemy::CanAttack() const
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

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「attack_start」なら
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃中
		m_attacking = true;
	}
	//キーの名前が「attack_end」なら
	else if (wcscmp(eventName, L"attack_end") == 0) 
	{
		//攻撃中じゃない
		m_attacking = false;
	}
}

void Enemy::Render(RenderContext& rc)
{
	if (m_isActive == true)
	{
		//モデルの更新
		m_modelRender.Draw(rc);
	}
}