#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"
#include "BChara.h"

namespace Player {

	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤー");	//グループ名
	const  string  defName("卵");	//タスク名
	//-------------------------------------------------------------------
	class Resource : public BResource
	{
		bool Initialize() override;
		bool Finalize() override;
		Resource();
	public:
		~Resource();
		typedef shared_ptr<Resource> SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP  img;
		DG::Image::SP chargeimg;

	};
	//-------------------------------------------------------------------
	class Object : public BChara
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//BCharaに持たせていない変数・メソッドのみここに書く
		XI::GamePad::SP  controller;
		int hp;

		//接触時の応答処理(必ず受け身の処理として実装する)
		void Received(BChara* from_, AttackInfo at_);
		bool CheckHit(const  ML::Box2D&  hit_);
		bool Check_Head(const ML::Box2D& head_);
	};	
}