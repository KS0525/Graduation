#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//2次元配列マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

namespace  Generator
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("生成");	//グループ名
	const  string  defName("生成");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
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
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		static Object::SP Create(bool flagEnemyGeneretorPushBuck);
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		static WP instance;
		ifstream evFile;

		int genCnt;
		int genSpan;

		static Object::SP Object::Create_Mutex();
		bool Set(const string& fpath_);
		bool ReadLine(string& lineT_);
		bool Execute(string& hs_, string& ds_);

		void Output(stringstream& ds_);
		void Generate();
	};
}