//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "Task_Player.h"
#include "Task_Shot00.h"
#include "Task_EffectHit.h"
#include  "MyPG.h"

namespace Player 
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara/maou_64_64.png");
		this->chargeimg = DG::Image::Create("./data/image/bar.png");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->chargeimg.reset();
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
		pos.y = 300;
		//this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(0, 0, 128, 128);
		this->angle_LR = Angle_LR::Right;
		this->controller = ge->in1;
		this->chargeX = 0;
		this->hp = 10;
		this->dist = 0;
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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto mouse = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		//移動実装(ナナメ入力等未考慮  後々実装)
		int speed = 7;
		if (key.B1.on) { this->pos.y -= speed; }
		if (key.B2.on) { this->pos.x -= speed; }
		if (key.B3.on) { this->pos.y += speed; }
		if (key.B4.on) { this->pos.x += speed; }
		if (this->pos.x < 0) { pos.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; } 
		
		if (mouse.LB.on)
		{
			dist++;
			if (dist % 7 == 0) {
				auto shot = Shot00::Object::Create(true);
				shot->pos.y = this->pos.y;
				ML::Box2D shotpos = this->hitBase.OffsetCopy(pos);
				shot->pos.x = (shotpos.x + (shotpos.x +shotpos.w)) /2;
				shot->moveVec = { 0,-5 };

				shot->atk = 1;

				this->chargeX = 0;
			}
		}
		
		//カメラの位置を再調整
		{
			//プレイヤを画面のどこに置くか（今回は画面中央）
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//プレイヤを画面中央に置いた時のカメラの左上座標を求める
			int cpx = int(this->pos.x) - px;
			int cpy = int(this->pos.y) - py;
			//カメラの座標を更新
			//ge->camera2D.x = cpx;
			//ge->camera2D.y = cpy;
			//以下はスクロール停止の節まで入力しない
			if (auto map = ge->qa_Map) {
				//map->AdjustCameraPos();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = this->hitBase;
			ML::Box2D src(0, 0, 64, 64);
			draw.Offset(this->pos);

			this->res->img->Draw(draw, src);
		} 
		
	}
	//-----------------------------------------------------------------------------
	//接触時の応答処理（これ自体はダミーのようなモノ）
	void  Object::Received(BChara*  from_, AttackInfo  at_)
	{
		hp -= at_.power;
		if (from_->groupName == "ブロック") {
			//if(from_->Check_Bottom()){
				this->pos.y = from_->pos.y;
				if (this->pos.y >ge->screenHeight- this->hitBase.h) {
					this->Kill();
					ge->KillAll_G("本編");
				}
			}
		//}
		if (this->hp <= 0)
		{
			this->Kill();
			ge->KillAll_G("本編");
		}
	}
	//-----------------------------------------------------------------------------
	//接触判定
	bool  Object::CheckHit(const  ML::Box2D&  hit_)
	{
		ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
		return  me.Hit(hit_);
	}
	//-----------------------------------------------------------------------------
	//
	/*bool Object::Check_Head(const ML::Box2D& hit_)
	{
		ML::Box2D me(this->hitBase.x, this->hitBase.y, this->hitBase.w, -1);
		me.Offset(this->pos);
		return me.Hit(hit_);
	}*/
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
				//（メソッド名が変なのは旧バージョンのコピーによるバグを回避するため
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


