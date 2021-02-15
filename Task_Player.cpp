//-----------------------------------------------------------------------------
//プレイヤー（卵）
//-----------------------------------------------------------------------------
#include "Task_Player.h"
#include "Task_Ending.h"
#include "Task_EffectHit.h"
#include "Task_MapGenerator.h"
#include  "MyPG.h"

namespace Player 
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara/Egg_anim_01.png");
		this->break_img = DG::Image::Create("./data/image/chara/Egg_break_anim_01.png");
		
		//this->se = DM::Sound::Create("./data/sound/玉子・潰す01.mp3");

		
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->break_img.reset();
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
		pos.y = 0;
		this->hitBase = ML::Box2D(8,8,34, 34);
		this->angle_LR = Angle_LR::Right;
		this->controller = ge->in1;
		this->hp = 10;
		ge->isDead = false;
		this->isDead = false;
		this->animCnt = 0;
		ge->serial++;
		this->serial = ge->serial;

		this->maxFallSpeed = 10.0f;	//最大落下速度
		this->gensoku = 0.4f;		//時間による減速量
		this->gravity = ML::Gravity(32) * 7; //重力加速度＆時間速度による加算量
		//★タスクの生成
		se::LoadFile("gravity", "./data/sound/ジャンプ (online-audio-converter.com) (1).wav");
		bgm::LoadFile("grav", "./data/sound/ジャンプ.mp3");
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		bgm::Stop("grav");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			//auto nextTask = Ending::Object::Create(true);
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto mouse = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		//重力変更
		if (ge->isReady) {
			if (!ge->isDead) {
				if (key.LStick.BU.on) { this->MoveGravity = Gravity::up; }
				if (key.LStick.BL.on) { this->MoveGravity = Gravity::left; }
				if (key.LStick.BD.on) { this->MoveGravity = Gravity::down; }
				if (key.LStick.BR.on) { this->MoveGravity = Gravity::right; }

				this->GravityMotion("ブロック");
				//重力操作のse
				if (key.LStick.axis.Length() > 0) {
					se::Play("gravity");
					//bgm::Play("grav");
					//se::LoadFile("gravity", "./data/sound/GravityChangeSE12.wav");
					//se::Play("gravity");
				}
			}

		}

		if (ge->isDead) {
			animCnt++;
			se::LoadFile("tamago", "./data/sound/卵・潰す.wav");
		
			if (animCnt > 3) {
				animCnt = 3;
			}
			else if (animCnt == 1)
			{				
				se::Play("tamago");
			}
		}
		//画面外へ出ないように
		if (this->pos.x < 0) { pos.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; }
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		
		{
			ML::Box2D draw(0, 0, 50, 50);
			ML::Box2D src(0, 0, 100, 100);

			float pie(3.1415f);
			switch (this->MoveGravity) {
			case 0: //up
				this->res->img->Rotation(pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			case 1: // down
				this->res->img->Rotation(0 * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(0 * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			case 2: //left
				this->res->img->Rotation(0.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(0.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			case 3: //right
				this->res->img->Rotation(1.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(1.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			default:
				break;
			}
			draw.Offset(this->pos);
			//生きているとき
			if (isDead == false) {
				this->res->img->Draw(draw, src);
			}
			//死んでいるとき
			else {
				ML::Box2D anim[] = {
					{0,0,200,200},
					{200,0,200,200 },
					{400,0,200,200},
					{600,0,200,200}
				};
				//描画をずらして、白身が垂れているように
				ML::Box2D draw_ = draw;
				int dif = 25;
				switch (this->MoveGravity) {					
				case 0: //up
					draw_.y -= dif;
					break;
				case 1: //down
					draw_.y += dif;
					break;
				case 2: //left
					draw_.x -= dif;
					break;
				case 3: //right
					draw_.x += dif;
					break;
				default:
					break;
				}

				this->res->break_img->Draw(draw_, anim[animCnt%4]);
			}
		}
		
	}
	//-----------------------------------------------------------------------------
	//接触時の応答処理（これ自体はダミーのようなモノ）
	void  Object::Received(BChara*  from_)
	{
		this->isDead = true;
		ge->isDead = true;
	}
	//------------------------------------------------------------------
	bool Object::Attack_Std(const string& GName)
	{
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);

		auto targets = ge->GetTask_Group_G<BChara>(GName);
		for (auto it = targets->begin();
			it != targets->end();
			++it)
		{
			//相手に接触の有無を確認させる
			if ((*it)->CheckHit(me) && this->serial != (*it)->serial)
			{
				//相手にダメージの処理を行わせる
				(*it)->Received(this);
				this->isDead = true;
				ge->isDead = true;
				//this->Kill();
				return true;
			}

		}
		return false;
	}
	//-----------------------------------------------------------------------------
	//接触判定
	bool  Object::CheckHit(const  ML::Box2D&  hit_)
	{
		ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
		return  me.Hit(hit_);
	}
	//-----------------------------------------------------------------------------
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


