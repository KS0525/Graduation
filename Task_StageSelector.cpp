//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_StageSelector.h"
#include  "Task_BackGround.h"
#include  "Task_Game.h"
#include  "Task_MapSelector.h"

#include "easing.h"

namespace  StageSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->eggCapsule = DG::Image::Create("./data/image/Select/Select_body.png");

		this->nonClearEgg[0] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_01_.png");
		this->nonClearEgg[1] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_02_.png");
		this->nonClearEgg[2] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_03_.png");
		this->nonClearEgg[3] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_04_.png");
		this->nonClearEgg[4] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_05_.png");
		this->nonClearEgg[5] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_06_.png");

		this->clearEgg[0] = DG::Image::Create("./data/image/Select/Select_Egg_01.png");
		this->clearEgg[1] = DG::Image::Create("./data/image/Select/Select_Egg_02.png");
		this->clearEgg[2] = DG::Image::Create("./data/image/Select/Select_Egg_03.png");
		this->clearEgg[3] = DG::Image::Create("./data/image/Select/Select_Egg_04.png");
		this->clearEgg[4] = DG::Image::Create("./data/image/Select/Select_Egg_05.png");
		this->clearEgg[5] = DG::Image::Create("./data/image/Select/Select_Egg_06.png");

		this->selectEgg[0] = DG::Image::Create("./data/image/Select/Select_select_01.png");
		this->selectEgg[1] = DG::Image::Create("./data/image/Select/Select_select_02.png");
		this->selectEgg[2] = DG::Image::Create("./data/image/Select/Select_select_03.png");
		this->selectEgg[3] = DG::Image::Create("./data/image/Select/Select_select_04.png");
		this->selectEgg[4] = DG::Image::Create("./data/image/Select/Select_select_05.png");
		this->selectEgg[5] = DG::Image::Create("./data/image/Select/Select_select_06.png");

		this->selectNum[0] = DG::Image::Create("./data/image/Select/Select_num_01.png");
		this->selectNum[1] = DG::Image::Create("./data/image/Select/Select_num_02.png");
		this->selectNum[2] = DG::Image::Create("./data/image/Select/Select_num_03.png");
		this->selectNum[3] = DG::Image::Create("./data/image/Select/Select_num_04.png");
		this->selectNum[4] = DG::Image::Create("./data/image/Select/Select_num_05.png");
		this->selectNum[5] = DG::Image::Create("./data/image/Select/Select_num_06.png");

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

		this->render2D_Priority[1] = 0.5f;

		//★データ初期化
		//easing::Init();
		BackGround::Object::Create(true);
		back = false;
		choosing = 0;
		stageNumber = 0;
		animCnt = 0;
		choiceMax_ = sizeof(buttons)/sizeof(buttons[0]);
		mapNumber = ge->mapNum;
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
			if (back) { //マップ選択に戻る
				auto nextTask = MapSelector::Object::Create(true);
			}
			else { //選択したステージに進む
				auto nextTask = Game::Object::Create(true);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();
		//カーソルを動かす
		Carsol();
		//マップのパスを選択
		setPass();

		animCnt++;

		if (key.B1.down) {
			//ステージパスを入力
			ge->nowStage = buttons[choosing].get_StagePass();
			ge->stageNum = choosing;
				//自身に消滅要請
				this->Kill();
			}
		if (key.B2.down) {
			back = true;
			//自身に消滅要請
			this->Kill();
		}
		//}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw1(0, 0, 1280, 720);
		ML::Box2D  src1(0, 0, 1280, 720);
		this->res->eggCapsule->Draw(draw1, src1);

		for (int i = 0;i < 6;++i) {
			if (ge->isClear[mapNumber][i] == true) {
				this->res->clearEgg[i]->Draw(draw1, src1);
			}
			else {
				this->res->nonClearEgg[i]->Draw(draw1, src1);
			}
		}
		if (animCnt % 60 > 0 && animCnt % 60 < 30) {
			this->res->selectEgg[choosing]->Draw(draw1, src1);
		}
		else {

		}
		this->res->selectNum[choosing]->Draw(draw1, src1);
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
			choosing = this->choiceMax_ -1;
		}
		if (choosing >= this->choiceMax_) {
			choosing = 0;
		}
	}
	//-------------------------------------------------------------------
	void Object::setPass()
	{
		stageNumber = choosing + 1;
		this->buttons[choosing].set_StagePass("./data/Map/" + to_string(mapNumber) + "/Map" + to_string(stageNumber) + ".txt");

	}
	//-------------------------------------------------------------------
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