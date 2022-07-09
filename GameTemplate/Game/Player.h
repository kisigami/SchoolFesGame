#pragma once

//クラス宣言
class GameCamera;  //ゲームカメラクラス
class Enemy;       //エネミークラス

/// <summary>
/// プレイヤークラス
/// </summary>
class Player :public IGameObject
{
public:
	Player();
	~Player();
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
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
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
	/// 回転を取得
	/// </summary>
	/// <returns>回転</returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns>HP</returns>
	const int& GetHP() const
	{
		return m_hp;
	}
	const Vector3& GetForward() const
	{
		return m_forward;
	}
private:
	/// <summary>
	/// プレイヤーステート
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,            //待機ステート
		enPlayerState_Run,             //走りステート
		enPlayerState_Shot,            //射撃ステート
		enPlayerState_ReceiveDamage,   //被ダメージステート
		enPlayerState_Down,            //ダウンステート
	};
	/// <summary>
	/// アニメーション初期化
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
	/// 攻撃の当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// 射撃処理
	/// </summary>
	void Shot();
	/// <summary>
	/// 弾作成処理
	/// </summary>
	void MakeBullet();
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
	/// 射撃ステートの遷移処理
	/// </summary>
	void ProcessShotStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// エフェクトテスト
	/// </summary>
	void EffectPlay();
	/// <summary>
	///アニメーションのキーを設定
	/// </summary>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	//アニメーションクリップの番号を表す
	enum EnAnimationClip
	{
		enAnimClip_RifleIdle,   //待機アニメーション
		enAnimClip_RifleRun,    //走りアニメーション
		enAnimClip_RifleShot,   //射撃アニメーション
		enAnimClip_Num          //アニメーションの数
	};

	FontRender           m_fontRender;                           //フォントレンダー
	EnPlayerState        m_playerState = enPlayerState_Idle;     //プレイヤーステート
	Animation            m_animation;                            //アニメーション
	AnimationClip        m_animClips[enAnimClip_Num];            //アニメーションクリップ
	ModelRender          m_modelRender;                          //モデルレンダー
	CharacterController  m_charaCon;                             //キャラクターコントローラー
	Vector3              m_position;                             //座標
	Vector3              m_scale;                                //大きさ
	Vector3              m_moveSpeed;                            //移動速度
	Vector3              m_forward = Vector3::AxisZ;             //プレイヤーの前ベクトル
	Quaternion           m_rotation;                             //回転
	GameCamera*          m_gameCamera = nullptr;                 //ゲームカメラ
	Enemy*               m_enemy = nullptr;                      //エネミー
	float                m_bulletNum = 30;                       //残弾数
	int                  m_hp = 100;                             //ヒットポイント
	float                m_receiveDamageTimer = 1.0f;            //無敵タイマー
	float x = 0.0f;
	Vector3 m_toCameraPos;
};

