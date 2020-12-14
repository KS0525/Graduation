//-------------------------------------------------------------------
//弾
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block07.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "Task_EffectHit.h"
#include  "Task_EffectBomb.h"



namespace  Block07
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Block/Block_07.jpg");
		se = DM::Sound::CreateSE("./data/sound/shot.wav");
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
		hitBase = ML::Box2D(0, 0, 128, 128);
		hp = 3;
		moveVec = { 0,2 };
		atk = { 0 };
		i = 0;

		this->render2D_Priority[1] = 1.0f;

		//移動時間設定
		RightTime = 100;
		DownTime = 40;
		LeftTime = 100;
		UpTime = 40;

		this->maxFallSpeed = 10.0f;	//最大落下速度
		this->gensoku = 0.2f;		//時間による減速量

		ge->serial++;
		this->serial = ge->serial;


		this->gravity = ML::Gravity(32) * 5; //重力加速度＆時間速度による加算量
		//★タスクの生成
		//this->res->se->Play_Normal(false);

		se::LoadFile("shot", "./data/sound/shot.wav");
		se::Play("shot");
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
		//pos.x += moveVec.x;
		//pos.y += moveVec.y;
		auto key = ge->in1->GetState();
		this->moveCnt++;
		//重力変更
		switch (this->direction[i])
		{
		case 0:
			if (this->RightTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::right;
			}
			else
			{
				i++;
				this->moveCnt = 0;
			}
			break;
		case 1:
			if (this->DownTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::down;
			}
			else
			{
				i++;
				this->moveCnt = 0;
			}
			break;
		case 2:
			if (this->LeftTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::left;
			}
			else
			{
				i++;
				this->moveCnt = 0;
			}
			break;
		case 3:
			if (this->UpTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::up;
			}
			else
			{
				i ++;
				this->moveCnt = 0;
			}
			break;
			
		}
		if (i > 3)i = 0;
		this->GravityMotion("ブロック");


		//this->pos += this->moveVec;

		//画面外へ出ないように
		if (this->pos.y < -this->hitBase.h) { pos.y = ge->screen2DHeight; }
		
		//敵との当たり判定
		if (this->Attack_Std("プレイヤー")) { //共通化により
			//接触していた場合、自分に対して何かしたいなら
			
		   //this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = hitBase;
		ML::Box2D src(0, 0, 128, 128);
		draw.Offset(this->pos);

		res->img->Draw(draw, src);
	}
	//------------------------------------------------------------------
	//接触時の応答処理（これ自体はダミーのようなモノ）
	void  Object::Received(BChara*  from_)
	{
		if (this->hp <= 0)
		{
			//ge->effectCreator->CreateEffect(EffectCreate::Object::BOMB, this->pos,0.5f);
			//++ge->score;
			//this->Kill();
		}
		else {
			//ge->effectCreator->CreateEffect(EffectCreate::Object::BOMBMINI, this->pos);
		}
	}
	//------------------------------------------------------------------
	bool Object::Check_bottom()
	{
		ML::Box2D bottom(this->hitBase.x, this->hitBase.y + this->hitBase.h, this->hitBase.w, 1);
		bottom.Offset(this->pos);

		auto pl = ge->GetTask_One_GN<Player::Object>(Player::defGroupName,Player::defName);
		if (nullptr == pl) {return false;}

		return pl->CheckHit(bottom);
	}
	//------------------------------------------------------------------
	bool Object::Attack_Std(const string& GName)
	{
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	
		ML::Box2D bottom(this->hitBase.x, this->hitBase.y + this->hitBase.h, this->hitBase.w, 1);
		bottom.Offset(this->pos);

	auto targets = ge->GetTask_Group_G<BChara>(GName);
	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//相手に接触の有無を確認させる
		if ((*it)->CheckHit(bottom)) {
			//相手にダメージの処理を行わせる
			(*it)->Received(this);
			return true;
		}
	}
	return false;
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