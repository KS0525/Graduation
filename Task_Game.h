#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

namespace  Game
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("本編");	//グループ名
	const  string  defName(		"統括");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP grab_UI_img;
		DG::Image::SP grab_UI_img2;
		DG::Image::SP grab_up_01;
		DG::Image::SP grab_up_02;
		DG::Image::SP grab_under_01;
		DG::Image::SP grab_under_02;
		DG::Image::SP grab_right_01;
		DG::Image::SP grab_right_02;
		DG::Image::SP grab_left_01;
		DG::Image::SP grab_left_02;

		DG::Image::SP grab_horizontal[7];
		DG::Image::SP grab_vertical[7];
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		int resetCoolCount;
		int bcount;
		int animCnt;
		enum Gravity { up, down, left, right, non };
		Gravity MoveGravity;

		void Grab_Anim();
	};
}