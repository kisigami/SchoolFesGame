#pragma once

//クラス宣言
class Bullet;   //弾
class Enemy;
class Player;

/// <summary>
/// エネミークラス
/// </summary>
class Enemy:public IGameObject
{
public:
	/// <summary>
	/// エネミーステート
	/// </summary>
	enum EnEnemyState
	{
		enEnemyState_Idle,         //待機ステート
		enEnemyState_Run,          //走りステート
		enEnemyState_Chase,        //追跡ステート
		enEnemyState_Attack,       //攻撃ステート
		enEnemyState_Down,         //ダウンステート
	};

	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//アニメーションクリップの番号を表す
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //待機アニメーション
		enAnimClip_Run,    //走りアニメーション
		enAnimClip_Attack, //攻撃アニメーション
		enAnimClip_Down,   //ダウンアニメーション
		enAnimClip_Num     //アニメーションの数
	};

	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 追跡処理
	/// </summary>
	void Chase();
	/// <summary>
	/// 当たり判定
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステートの遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 走りステートの遷移処理
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	///	攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition();

	ModelRender          m_modelRender;                         //モデルレンダー
	Vector3              m_position;                            //座標
	Vector3              m_scale;                               //大きさ
	Vector3              m_moveSpeed;                           //移動速度
	Vector3              m_forward = Vector3::AxisZ;            //エネミーの前方向のベクトル
	Quaternion           m_rotation;                            //回転
	EnEnemyState         m_enemyState = enEnemyState_Idle;      //エネミーステート
	Animation            m_animation;                           //アニメーション
	AnimationClip        m_animClips[enAnimClip_Num];           //アニメーションクリップ
	CharacterController  m_charaCon;                            //キャラコン
	int                  m_hp = 2;                              //HP
	Bullet*              m_bullet = nullptr;                    //弾
	Player*              m_player = nullptr;                    //プレイヤー
};

