#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

//クラス宣言
class Player;     //プレイヤークラス
class Bullet;     //弾クラス
class SpawnEnemy; //エネミー出現クラス

/// <summary>
/// スピードエネミークラス
/// </summary>
class SpeedEnemy :public IGameObject
{
public:
	SpeedEnemy();
	~SpeedEnemy();
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
	/// マイナンバーを設定
	/// </summary>
	/// <param name="position">番号</param>
	void SetMyNumber(const int& position)
	{
		m_myNumber = position;
	}
	/// <summary>
	/// マイナンバーを取得
	/// </summary>
	/// <returns>番号</returns>
	const int& GetMyNumber() const
	{
		return m_myNumber;
	}
	/// <summary>
	/// 活動フラグを設定
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetActiveFlag(const bool& flag)
	{
		m_isActive = flag;
	}
	/// <summary>
	/// 活動フラグを取得
	/// </summary>
	/// <returns>フラグ</returns>
	const bool& GetActiveFlag() const
	{
		return m_isActive;
	}

private:
	/// <summary>
	/// エネミーステート
	/// </summary>
	enum EnEnemyState
	{
		enEnemyState_Idle,         //待機ステート
		enEnemyState_Run,          //走りステート
		enEnemyState_Chase,        //追跡ステート
		enEnemyState_PathChase,    //パス移動ステート
		enEnemyState_Attack,       //攻撃ステート
		enEnemyState_Down,         //ダウンステート
		enEnemyState_DeActive,     //非活動ステート
	};
	
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

	void DeActive();

	/// <summary>
	/// アニメーションの初期化処理
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// パス移動処理
	/// </summary>
	void PathMove();
	/// <summary>
	/// 追跡処理
	/// </summary>
	void Chase();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// 攻撃用コリジョン生成処理
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// アニメーションの再生処理
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
	/// プレイヤーから見えているか？
	/// </summary>
	/// <returns>trueなら見えている</returns>
	void CantLookMe();
	/// <summary>
	/// 攻撃できるか？
	/// </summary>
	/// <returns>trueならできる</returns>
	const bool CanAttack() const;

	/// <summary>
    /// 鳴き声再生処理
    /// </summary>
	void PlayNeowingSE();
	/// <summary>
    /// 攻撃音の再生
    /// </summary>
	void PlayAttackSE();

	//クラス定義
	Bullet*             m_bullet = nullptr;               //弾
	Player*             m_player = nullptr;               //プレイヤー
	SpawnEnemy*         m_spawnEnemy = nullptr;           //エネミー出現
	//ステータス
	EnEnemyState        m_enemyState = enEnemyState_Idle; //エネミーステート
	int                 m_hp = 3;                         //HP
	int                 m_myNumber = 0;                   //マイナンバー
	//フラグ
	bool                m_mitukatta = false;              //見つかったか？
	bool                m_attacking = false;              //攻撃中か？
	bool                m_isActive = false;               //活動中か？
	//経路探索
	TknFile             m_tknFile;                        //tkmファイル
	nsAI::NaviMesh      m_nvmMesh;                        //ナビメッシュ
	nsAI::Path          m_path;                           //パス
	nsAI::PathFinding   m_pathFiding;                     //パス検索
	float               m_pathTimer = 1.0f;               //パス検索タイマー

	ModelRender         m_modelRender;                    //モデルレンダー
	Animation           m_animation;                      //アニメーション
	AnimationClip       m_animClips[enAnimClip_Num];      //アニメーションクリップ
	Vector3             m_position;                       //座標
	Vector3             m_scale;                          //大きさ
	Vector3             m_moveSpeed;                      //移動速度
	Vector3             m_forward = Vector3::AxisZ;       //エネミーの前方向のベクトル
	Quaternion          m_rotation;                       //回転
	CharacterController m_charaCon;                       //キャラコン
	SphereCollider	    m_sphereCollider;                 //スフィアコライダー
	int				    m_pumchBoneId = -1;               //パンチのボーン
};