#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2    pos;		//キャラクタ位置
	ML::Box2D   hitBase;	//あたり判定範囲
	ML::Vec2	moveVec;	//移動ベクトル
	int			moveCnt;	//行動カウンタ
	//左右の向き（2D横視点ゲーム専用）
	enum Angle_LR { Left, Right };
	Angle_LR	angle_LR;


	float maxFallSpeed;		//最大落下速度


	float gravity;			//重力加速度＆時間速度による加算量
	float gensoku;			//時間による減速量
	int serial;				//ブロックナンバー
	//重力の方向
	enum Gravity { up, down, left, right };
	Gravity MoveGravity;
	
	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, angle_LR(Right)
		, MoveGravity(down)
		, maxFallSpeed(0)
		,gravity(0)
		,serial(0)
	{
    }
	virtual  ~BChara(){}

	//	アニメーション情報構造体
	struct DrawInfo {
		ML::Box2D		draw, src;
		ML::Color		color;
	};
	//  攻撃情報
	struct  AttackInfo {
		int  power;			//攻撃の威力
		void operator=(int pow) { power = pow; }
										//その他必要に応じて
	};
	//---------------------------------------------------------
	enum class Motion
	{
		Unnon = 01, //無効
		Stand, //停止
		Walk, //歩行
		Attack, //攻撃
		Damage, //負傷
	};

	Motion motion; //現在の行動を示すフラグ
	int animCnt; //アニメーションカウンタ

	float maxSpeed; //左右方向への移動の最大量
	float addSpeed; //左右方向への移動の加算量
	//---------------------------------------------------------

	//キャラクタ共通メソッド
	//接触時の応答処理（これ自体はダミーのようなモノ）
	virtual  void  Received(BChara*  from_);
	//接触判定
	virtual  bool  CheckHit(const  ML::Box2D&  hit);
	//モーションを更新(変更なしの場合 false)
	bool UpdateMotion(Motion nm_);

	void GravityMotion(const string& GName);

	//当たり判定
	virtual bool Attack_Std(const string& GName);
	virtual bool Check_Bottom();
	//virtual bool Check_Head(const ML::Box2D& hit_);
};
