#pragma once

class Player;
class Bullet;

class SpeedEnemy :public IGameObject
{
public:
	SpeedEnemy();
	~SpeedEnemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
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
	/// 攻撃のコリジョンを作成する
	/// </summary>
	void MakeAttackCollision();
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
	/// 追跡ステートの遷移処理
	/// </summary>
	void ProcessChaseStateTransition();
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
	/// <summary>
	/// アニメーションのキーを設定する処理
	/// </summary>
	/// <param name="clipName">クリップ名</param>
	/// <param name="eventName">イベント名</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// プレイヤーからみえていないか？
	/// </summary>
	/// <returns>trueなら見えた</returns>
	void CantLookMe();
	/// <summary>
	/// 攻撃できるか？
	/// </summary>
	const bool CanAttack() const;

	/// <summary>
	//アニメーションクリップの番号を表す
	/// </summary>
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //待機アニメーション
		enAnimClip_Run,    //走りアニメーション
		enAnimClip_Attack, //攻撃アニメーション
		enAnimClip_Down,   //ダウンアニメーション
		enAnimClip_Num     //アニメーションの数
	};


	ModelRender          m_modelRender;
	Animation            m_animation;                           //アニメーション
	AnimationClip        m_animClips[enAnimClip_Num];           //アニメーションクリップ
	int                  m_myNumber = 0;                        //マイナンバー
	Vector3              m_position;                            //座標
	Vector3              m_scale;                               //大きさ
	Vector3              m_moveSpeed;                           //移動速度
	Vector3              m_forward = Vector3::AxisZ;            //エネミーの前方向のベクトル
	Quaternion           m_rotation;                            //回転
	EnEnemyState         m_enemyState = enEnemyState_Idle;      //エネミーステート
	CharacterController  m_charaCon;                            //キャラコン
	int                  m_hp = 6;                              //HP
	Bullet* m_bullet = nullptr;                    //弾
	Player* m_player = nullptr;                    //プレイヤー
	bool                 m_mitukatta = false;                    //見つかったか
	SphereCollider	     m_sphereCollider;                       //スヒアコライダー
	bool                 m_attacking = false;                    //攻撃中か
	int				     m_pumchBoneId = -1;                     //パンチのボーン
	bool                 m_deadFlag = true;                      //死亡しているか
};