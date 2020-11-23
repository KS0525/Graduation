//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Player.h"
#include  "Task_BackGround.h"
#include  "Task_Enemy.h"
#include  "Task_Block00.h"
#include  "Task_EffectBomb.h"
#include  "Task_EffectHit.h"
#include  "Task_Enemy01.h"
#include  "Task_Map2D.h"
#include  "Task_Block01.h"
#include  "Task_Block02.h"
#include  "Task_MapGenerator.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->score = DG::Image::Create("./data/image/font_number.png");
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
		this->count = 0;
		this->bcount = 0;

		ge->camera2D = ML::Box2D(0,0, ge->screen2DWidth, ge->screen2DHeight);

		//スコア初期化
		ge->score = 0;

	   //★タスクの生成
		BackGround::Object::Create(true);

		/*auto pl = Player::Object::Create(true);
		pl->pos.x = 480 / 2;
		pl->pos.y = 270 * 2 / 3;

		auto blo = Block00::Object::Create(true);
		blo->pos.x = 500;
		blo->pos.y = 500;

		auto blo2 = Block01::Object::Create(true);
		blo2->pos.x = 1000;
		blo2->pos.y = 700;

		auto blo3 = Block02::Object::Create(true);
		blo3->pos.x = 700;
		blo3->pos.y = 700;


		if (auto map = Generator::Object::Create_Mutex()) {
			map->Set("./data/Map/Map.txt");
		}

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

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto nextTask = Ending::Object::Create(true);

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto inp = ge->in1->GetState();

		if (ms.LB.down) {
			ge->KillAll_G("プレイヤー");
			ge->KillAll_G("ブロック");
			if (auto map = Generator::Object::Create_Mutex()) {
				map->Set("./data/Map/Map.txt");
			}
		}
		if (ms.CB.down) {
			//自身に消滅要請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

		//スコア描画
		//char msg[10];
		//sprintf(msg, "%5d", ge->score);
		////SPRINTF　S　String　Print Format　フォーマットに合わせて出力
		////　Intからchar配列への変換

		//for (int i = 0; i < 5; i++) {
		//	if (msg[i] != ' ') {
		//		ML::Box2D  draw2(-(48 / 2), (-60 / 2), 48, 60);
		//		ML::Box2D  src2((msg[i] - '0') * 19, 32, 19, 32);
		//		draw2.Offset((50 * i) + 500, 50);

		//		this->res->score->Draw(draw2, src2);
		//	}
		//}
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