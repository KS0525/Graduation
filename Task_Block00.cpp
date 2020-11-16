//-------------------------------------------------------------------
//弾
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block00.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "Task_EffectHit.h"
#include  "Task_EffectBomb.h"



namespace  Block00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/map/CastleTown-B.png");
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

		this->maxFallSpeedDown = 10.0f;	//最大落下速度
		this->maxFallSpeedUp = -10.0f;
		this->maxFallSpeedLeft = -10.0f;
		this->maxFallSpeedRight = 10.0f;

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

		//重力変更
		if (key.B1.on) { this->MoveGravity = Gravity::up; }
		if (key.B2.on) { this->MoveGravity = Gravity::left; }
		if (key.B3.on) { this->MoveGravity = Gravity::down; }
		if (key.B4.on) { this->MoveGravity = Gravity::right; }

		this->GravityMotion();

		this->pos += this->moveVec;

		//画面外へ出ないように
		if (this->pos.x < 0) { pos.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; }
		
		//敵との当たり判定
		if (this->Attack_Std("プレイヤー", atk)) { //共通化により
			//接触していた場合、自分に対して何かしたいなら
			
		   //this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = hitBase;
		ML::Box2D src[3] = { ML::Box2D(321,64,30,32),
							ML::Box2D(289,64,30,32),
							ML::Box2D(257, 64, 30, 32)						
							};
		draw.Offset(this->pos);

		res->img->Draw(draw, src[this->hp-1]);
	}
	//------------------------------------------------------------------
	//接触時の応答処理（これ自体はダミーのようなモノ）
	void  Object::Received(BChara*  from_, AttackInfo  at_)
	{
		this->hp -= at_.power;

		if (this->hp <= 0)
		{
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMB, this->pos,0.5f);
			++ge->score;
			this->Kill();
		}
		else {
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMBMINI, this->pos);
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
	bool Object::Attack_Std(const string& GName,AttackInfo at_)
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
			(*it)->Received(this, at_);
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



class ChipInfo
{
private:
	ChipInfo()
	{
		num = chip;
		chip++;
		mChips.push_back(this);
	}
	static int chip;
	int num;

	string srcName;

	std::vector<ChipInfo*> mChips;
	void Create()
	{
		for (auto chips : mChips)
		{
			auto bl = Block00::Object::Create(true);
			//あとはチップ情報を渡す

		}
	}
public:
	enum class Chiptype {
		broken, //壊せるオブジェクト
		hardbroken, //壊しにくいオブジェクト
		Unbroken //壊せないオブジェクト
	};

	ML::Box2D src;
};