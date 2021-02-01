//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSelector.h"
#include  "Task_BackGround.h"
#include  "Task_Game.h"
#include  "Task_StageSelector.h"
#include "easing.h"

namespace  MapSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Select/Map_Select_body.png");
		this->choosingimg = DG::Image::Create("./data/image/UI/UI_choosing.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		img.reset();
		choosingimg.reset();
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
		//easing::Init();
		BackGround::Object::Create(true);
		choosing = 0;
		choiceMax_ = sizeof(buttons)/sizeof(buttons[0]);
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
			auto nextTask = StageSelector::Object::Create(true);
			nextTask->mapNumber = this->choosing + 1;
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();
		Carsol();
		
		if (key.B1.down) {
			//ge->nowStage = buttons[choosing].get_StagePass();
				//自身に消滅要請
				this->Kill();
			}
		//}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 720);
		ML::Box2D src(0, 0, 352, 202);

		this->res->img->Draw(draw, src);

		ML::Box2D chooseDraw[3] = {
			ML::Box2D(100,0,300,400),
			ML::Box2D(500,0,300,400),
			ML::Box2D(900,0,300,400)
		};
		ML::Box2D chooseSrc(0,0,132,132);
		this->res->choosingimg->Draw(chooseDraw[choosing], chooseSrc);
	}
	//------------------------------------------------------------------
	void Object::Carsol() 
	{
		auto key = ge->in1->GetState();
		//カーソル移動
		if (key.LStick.BR.down) {
			choosing++;
		}
		if (key.LStick.BL.down) {
			choosing--;
		}
		//下限と上限
		if (choosing < 0) {
			choosing = 0;
		}
		if (choosing > this->choiceMax_) {
			choosing = choiceMax_;
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