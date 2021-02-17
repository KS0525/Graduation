//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Player.h"
#include  "Task_BackGround.h"
#include  "Task_Block00.h"
#include  "Task_EffectBomb.h"
#include  "Task_EffectHit.h"
#include  "Task_Map2D.h"
#include  "Task_Block01.h"
#include  "Task_MapGenerator.h"
#include  "Task_MapSelector.h"
#include  "Task_StageSelector.h"
#include  "Task_Block02.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->grab_horizontal[0] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0001.png");
		this->grab_horizontal[1] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0002.png");
		this->grab_horizontal[2] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0003.png");
		this->grab_horizontal[3] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0004.png");
		this->grab_horizontal[4] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0005.png");
		this->grab_horizontal[5] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0006.png");
		this->grab_horizontal[6] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0007.png");

		this->grab_vertical[0] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0001.png");
		this->grab_vertical[1] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0002.png");
		this->grab_vertical[2] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0003.png");
		this->grab_vertical[3] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0004.png");
		this->grab_vertical[4] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0005.png");
		this->grab_vertical[5] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0006.png");
		this->grab_vertical[6] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0007.png");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0;i < 7;++i) {
			this->grab_horizontal[i].reset();
			this->grab_vertical[i].reset();
		}
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
		this->resetCoolCount = 0;
		this->bcount = 0;
		this->animCnt = 0;
		this->render2D_Priority[1] = 0.5f;
		ge->camera2D = ML::Box2D(0,0, ge->screen2DWidth, ge->screen2DHeight);

		//スコア初期化
		ge->score = 0;
		ge->gameClearFlag = false;
		animFin = false;
		animCnt = 0;
	   //★タスクの生成
		BackGround::Object::Create(true);

		if (auto map = Generator::Object::Create_Mutex()) {
			map->Set(ge->nowStage);
		}
		bgm::LoadFile("bgm", "./data/sound/bgm_maoudamashii_ethnic16.mp3");
		bgm::Play("bgm");
		bgm::VolumeControl("bgm", 70);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("プレイヤー");
		ge->KillAll_G("背景画像");
		ge->KillAll_G("敵");
		ge->KillAll_G("弾");
		ge->KillAll_G("ブロック");
		ge->KillAll_G("スイッチ");
		ge->KillAll_G("ゴール");


		bgm::Stop("bgm");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			if (ge->gameClearFlag) {
				auto nextTask = Ending::Object::Create(true);
			}
			else {
				auto nextTask = MapSelector::Object::Create(true);
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

		//animCnt++;
		//アニメーション制御
		Grab_Anim();

		if (key.LStick.BU.on) { this->MoveGravity = Gravity::up; }
		if (key.LStick.BL.on) { this->MoveGravity = Gravity::left; }
		if (key.LStick.BD.on) { this->MoveGravity = Gravity::down; }
		if (key.LStick.BR.on) { this->MoveGravity = Gravity::right; }


		if (ge->isReady) { //マップの生成が完了しているか
			if (ge->isDead) { //プレイヤは死んでいるか
				resetCoolCount++;
				if (resetCoolCount > 60) { 
					//マップ関係のタスクを解放
					ge->KillAll_G("プレイヤー");
					ge->KillAll_G("ブロック");
					ge->KillAll_G("固定ブロック");
					ge->KillAll_G("スイッチ");
					ge->KillAll_G("ゴール");

					ge->isDead = false;
					//クールカウントを0に戻す
					resetCoolCount = 0;
					ge->isReady = false;

					//再セット
					if (auto map = Generator::Object::Create_Mutex()) {
						map->Set(ge->nowStage);
					}
				}
			}

			bgm::EndCheck();

			if (key.B2.down) {
				//自身に消滅要請
				this->Kill();
			}
			if (ms.CB.down) {
				//自身に消滅要請
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 720);
		ML::Box2D src(0, 0, 1280, 720);

		if (!animFin) {
			if (this->MoveGravity == Gravity::up ||
				this->MoveGravity == Gravity::down) {
				this->res->grab_vertical[animCnt / 4 % 7]->Draw(draw, src);
			}
			else if (this->MoveGravity == Gravity::left ||
				this->MoveGravity == Gravity::right) {
				this->res->grab_horizontal[animCnt / 4 % 7]->Draw(draw, src);
			}
			else {

			}
		}
	}
	//-------------------------------------------------------------------
	void Object::Grab_Anim()
	{
		int animMax = 28;
		switch (this->MoveGravity) {
		case 0: //up
			animCnt--;
			animFin = false;
			if (animCnt < 0) { animCnt = 0;  animFin = true; }
			break;
		case 1: // down
			animCnt++;
			animFin = false;
			if (animCnt > animMax) { animCnt = animMax; animFin = true; }
			break;
		case 2: //left	
			animCnt--;
			animFin = false;
			if (animCnt < 0) { animCnt = 0; animFin = true; }
			break;
		case 3: //right
			animCnt++;
			animFin = false;
			if (animCnt > animMax) { animCnt = animMax;  animFin = true; }
			break;
		default:
			break;
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
	Object::Object() { }
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