#pragma once

//クラス宣言
class GameCamera;

class Player :public IGameObject
{
public:
	/// <summary>
	/// プレイヤーステート
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,   //待機ステート
		enPlayerState_Run,    //走りステート
		enPlayerState_Shot,
		enPlayerState_Back
	};

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
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// 右座標を取得
	/// </summary>
	/// <returns>右座標</returns>
	const Vector3& GetRight() const
	{
		return m_right;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

private:
	void MakeBullet();

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

	void ProcessBackStateTransition();
	//アニメーションクリップの番号を表す
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //待機アニメーション
		enAnimClip_Run,    //走りアニメーション
		enAnimClip_Shot,
		enAnimClip_Back,
		enAnimClip_Num     //アニメーションの数
	};

	EnPlayerState        m_playerState = enPlayerState_Idle;     //プレイヤーステート
	Animation            m_animation;                            //アニメーション
	AnimationClip        m_animClips[enAnimClip_Num];            //アニメーションクリップ
	ModelRender          m_modelRender;                          //モデルレンダー
	ModelRender          m_rightHandRender;
	ModelRender          m_leftHandRender;
	CharacterController  m_charaCon;                             //キャラクターコントローラー
	Vector3              m_position;                             //座標
	Vector3              m_scale;                                //大きさ
	Vector3              m_moveSpeed;                            //移動速度
	Vector3              m_forward = Vector3::AxisZ;             //プレイヤーの前ベクトル
	Vector3              m_right = Vector3::AxisX;
	Quaternion           m_rotation;                             //回転
	GameCamera*          m_gameCamera;                           //ゲームカメラ
};

