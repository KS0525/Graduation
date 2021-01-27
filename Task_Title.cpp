//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_MapSelector.h"
#include  "Task_StageSelector.h"
#include  "Task_MapGenerator.h"
#include  "Task_BackGround.h"
#include "easing.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->back = DG::Image::Create("./data/image/Title/title.png");
		this->presskey = DG::Image::Create("./data/image/Title/pressKey02.png");
		
		//bgm::LoadFile("title", "./data/sound/夢幻の世界-Real_promenade-.mp3");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->back.reset();
		this->presskey.reset();
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

		this->render2D_Priority[1] = 0.5f;
		//★データ初期化
		BackGround::Object::Create(true);

		//★タスクの生成
		if (auto map = Generator::Object::Create_Mutex()) {
			map->Set("./data/Map/Map_Title.txt");
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("プレイヤー");
		ge->KillAll_G("ブロック");
		ge->KillAll_G("固定ブロック");
		ge->KillAll_G("スイッチ");
		ge->KillAll_G("ゴール");
		ge->KillAll_G("背景画像");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto nextTask = MapSelector::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();
		//easing::UpDate();

		//if (ms.RB.down || key.B2.down) {
		//	ge->KillAll_G("プレイヤー");
		//	ge->KillAll_G("ブロック");
		//	ge->KillAll_G("固定ブロック");
		//	ge->KillAll_G("スイッチ");
		//	ge->KillAll_G("ゴール");

		//	if (auto map = Generator::Object::Create_Mutex()) {
		//		map->Set("./data/Map/Map_Title.txt");
		//	}
		//}

		if (ge->isReady) {
			if (key.B1.down) {
				this->Kill();
				ge->isReady = false;
			}
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
		ML::Box2D  draw1(0, 0, 1280, 720);
		ML::Box2D  src1(0, 0, 1280, 720);
		this->res->back->Draw(draw1, src1);
		this->res->presskey->Draw(draw1, src1);
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