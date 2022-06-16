#include "stdafx.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

namespace
{
	const float CHARACON_RADIUS = 12.0f;             //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;             //キャラコンの高さ
	const int   REACT_DAMAGE = 1;                    //受けるダメージ量
	const int   HP_ZERO = 0;                         //０
	const float ENEMY_SCALE = 1.0f;
	const float IDLE_ANIMATION_SPEED = 1.0f;         //待機アニメーションの再生速度
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //待機アニメーションの補完時間
	const float RUN_ANIMATION_SPEED = 1.0f;          //走りアニメーションの再生速度
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //走りアニメーションの補完時間
	const float DOWN_ANIMATION_SPEED = 1.0f;         //ダウンアニメーションの再生時間
	const float DOWN_ANIMATION_INTERPOLATE = 0.1f;   //ダウンアニメーションの補完時間
	const float ATTACK_ANIMATION_SPEED = 1.0f;       //攻撃アニメーションの再生時間
	const float ATTACK_ANIMATION_INTERPOLATE = 0.3f; //攻撃アニメーションの補完時間
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
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");
	//アニメーションの初期化
	InitAnimation();
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm",m_animClips,enAnimClip_Num);
	//キャラコンを初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT,m_position);
	//モデルの更新
	m_modelRender.Update();

	return true;
}

void Enemy::Update()
{
	//アニメーションの再生
	PlayAnimation();
	//当たり判定処理
	Collision();
	//各ステートの遷移処理
	ManageState();

	//座標の設定
	m_modelRender.SetPosition(m_position);
	//回転の設定
	m_modelRender.SetRotation(m_rotation);
	//大きさの設定
	m_modelRender.SetScale(Vector3::One*ENEMY_SCALE);
	//モデルの更新
	m_modelRender.Update();
}

void Enemy::Rotation()
{
}

void Enemy::Move()
{
}

void Enemy:: Chase()
{
}

void Enemy::Attack()
{

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
	case Enemy::enAnimClip_Attack:
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

}

void Enemy::ProcessIdleStateTransition()
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
		//自身を削除する
		DeleteGO(this);
	}
}

void Enemy::Render(RenderContext& rc)
{
	//モデルの更新
	m_modelRender.Draw(rc);
}