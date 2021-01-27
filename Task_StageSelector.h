#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"
#include "Button.h"

namespace  StageSelector
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ステージセレクタ");	//グループ名
	const  string  defName("NoName");		//タスク名
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
		DG::Image::SP  eggCapsule;
		DG::Image::SP nonClearEgg[6];
		DG::Image::SP clearEgg[6];
		DG::Image::SP selectEgg[6];
	
		DG::Image::SP  choosingimg;
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
		int choosing;
		int choiceMax_;
		void Carsol();
		bool back;
		
		int animCnt;
		int mapNumber;

		class StageButton : public Button {
			DG::Image::SP clearEgg;
			DG::Image::SP nonClearEgg;
			string stagePass;
		public:


			StageButton(string inPass) :Button() {
				stagePass = inPass;
			};
			StageButton() :Button() { isClear = false; };

			string get_StagePass() const  { return stagePass; }
			void set_StagePass(const string pass) { stagePass = pass; }

			bool isClear;
		};

		StageButton buttons[6];
	};
}

