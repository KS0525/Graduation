//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSelector.h"
#include  "Task_Game.h"

#include "easing.h"

namespace  MapSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Block/Block_00.jpg");
		this->choosingimg = DG::Image::Create("./data/image/UI/UI_choosing.png");
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

		this->render2D_Priority[1] = 1.0f;

		//Button::set_allNum(0);
		buttons[0].set_StagePass("./data/Map/Map.txt");
		buttons[1].set_StagePass("./data/Map/Map4.txt");
		buttons[2].set_StagePass("./data/Map/Map3.txt");
		buttons[3].set_StagePass("./data/Map/Map5.txt");
		buttons[4].set_StagePass("./data/Map/Map6.txt");
		buttons[5].set_StagePass("./data/Map/Map7.txt");
		buttons[6].set_StagePass("./data/Map/Map8.txt");
		buttons[7].set_StagePass("./data/Map/Map9.txt");
		buttons[8].set_StagePass("./data/Map/Map10.txt");
		buttons[9].set_StagePass("./data/Map/Map11.txt");
		buttons[10].set_StagePass("./data/Map/Map12.txt");
		buttons[11].set_StagePass("./data/Map/Map13.txt");
		buttons[12].set_StagePass("./data/Map/Map14.txt");
		buttons[13].set_StagePass("./data/Map/Map15.txt");


		//★データ初期化
		//easing::Init();
		choosing = 1;
		choiceMax_ = sizeof(buttons)/sizeof(buttons[0]);
		//★タスクの生成
		
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto nextTask = Game::Object::Create(true);
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
			ge->nowStage = buttons[choosing].get_StagePass();
				//自身に消滅要請
				this->Kill();
			}
		//}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
#define choiceMax 14
		ML::Box2D draw[choiceMax] = {
			ML::Box2D(100,100,100,100),
			ML::Box2D(200,100,100,100),
			ML::Box2D(300,100,100,100),
			ML::Box2D(400,100,100,100),
			ML::Box2D(500,100,100,100),
			ML::Box2D(600,100,100,100),
			ML::Box2D(700,100,100,100),
			ML::Box2D(800,100,100,100),
			ML::Box2D(100,300,100,100),
			ML::Box2D(200,300,100,100),
			ML::Box2D(300,300,100,100),
			ML::Box2D(400,300,100,100),
			ML::Box2D(500,300,100,100),
			ML::Box2D(600,300,100,100),
		};
		ML::Box2D  draw1(100,100, 100, 100);
		ML::Box2D  src1(0, 0, 128, 128);
		this->res->img->Draw(draw1, src1);
		
		for (int i = 1; i < choiceMax; ++i) {
			this->res->img->Draw(draw[i], src1);
		}
		/*this->res->img->Draw(draw[1], src1);
		this->res->img->Draw(draw[2], src1);
		this->res->img->Draw(draw[3], src1);
		this->res->img->Draw(draw[4], src1);*/

		ML::Box2D src2(0, 0, 132, 132);
		this->res->choosingimg->Draw(draw[choosing], src2);
		
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