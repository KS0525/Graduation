//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include  "Task_StageSelector.h"
#include  "Task_MapSelector.h"
#include  "Task_BackGround.h"
#include "easing.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->appear[0] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_01_01.png");
		this->appear[1] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_01_02.png");
		this->appear[2] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_01_03.png");
		this->appear[3] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_02_01.png");
		this->appear[4] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_02_02.png");
		this->appear[5] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_02_03.png");
		this->appear[6] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_03_01.png");
		this->appear[7] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_03_02.png");
		this->appear[8] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_03_03.png");
		this->appear[9] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_04_01.png");
		this->appear[10] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_04_02.png");
		this->appear[11] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_04_03.png");
		this->appear[12] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_05_01.png");
		this->appear[13] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_05_02.png");
		this->appear[14] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_05_03.png");
		this->appear[15] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_06_01.png");
		this->appear[16] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_06_02.png");
		this->appear[17] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_06_03.png");
		
		this->fall[0] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_01.png");
		this->fall[1] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_02.png");
		this->fall[2] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_03.png");
		this->fall[3] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_04.png");
		this->fall[4] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_05.png");
		this->fall[5] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_06.png");
		this->fall[6] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_07.png");
		this->fall[7] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_08.png");

		this->eggCapsule = DG::Image::Create("./data/image/Select/Select_body.png");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		BackGround::Object::Create(true);
		animCnt = 0;
		isFall = false;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("背景画像");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto  nextTask = MapSelector::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		animCnt++;
		if (!isFall) {
			if (animCnt >= 47) { animCnt = 0;  isFall = true; }
		}
		else {
			if (animCnt >= 17) { animCnt = 17;}
		}

		if (key.B1.down) {
			this->Kill();
		}
		if (ms.LB.down) {
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 720);
		ML::Box2D src(0, 0, 1280, 720);
		
		int cuts_fall = 8; 
		int cuts_appear = 3;

		int frameRate = 6;

		this->res->eggCapsule->Draw(draw, src);

		if (!isFall) {
			this->res->fall[animCnt / frameRate % cuts_fall]->Draw(draw, src);
		}
		else {
			this->res->appear[animCnt /frameRate % cuts_appear]->Draw(draw, src);
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack_ABCDEFGHIJKLMN(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}