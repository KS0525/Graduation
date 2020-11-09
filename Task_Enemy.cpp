//-----------------------------------------------------------------------------
//敵クラス
//-----------------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_Enemy.h"
#include "Task_Player.h"
#include "Task_EffectBomb.h"
#include "Task_EffectCreate.h"

namespace Enemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara/mobチップ/pipo-charachip018.png");
		this->hp = DG::Image::Create("./data/image/font_number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		//this->render2D_Priority[1] = 0.5f;
		this->pos.x = (float)ge->screenWidth;

		this->hitBase = ML::Box2D(0, 0,64, 64);
		this->angle_LR = Angle_LR::Right;
		this->controller = ge->in1;
		this->animCnt = 0;
		this->motion = Motion::Walk;		//キャラ初期状態
		this->maxSpeed = 2.0f;		//最大移動速度（横）
		this->addSpeed = 1.0f;		//歩行加速度（地面の影響である程度打ち消される
		this->hp = 1;
		this->angle = 0;
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
		
	
		angle++;
		moveVec.x = sin(angle/10) * 5;
		pos += moveVec;
		this->moveCnt++;
		this->animCnt++;
		//思考・状況判断
		this->Think();
		//現モーションに対応した制御
		this->Move();
		//めり込まない移動
		//ML::Vec2  est = this->moveVec;
		//this->CheckMove(est);


		if (pos.y > ge->screenHeight) {
			Kill();
		}
		//プレイヤーとの当たり判定
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		auto player = ge->GetTask_Group_G<Player::Object>("プレイヤー");
		for (auto it = player->begin(); it != player->end(); ++it)
		{
			ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
			if (you.Hit(me))
			{
				//Eff_dead();
				auto eff = EffectCreate::Object::Create(true);
				eff->CreateEffect(EffectCreate::Object::EFFECTTYPE::BOMB, this->pos);
				//EffectCreate::Object::CreateEffect()
				this->Kill();
				(*it)->Kill();
				
				ge->KillAll_G("本編");
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		
		this->res->img->Draw(di.draw, di.src);

		//HP描画
		char msg[10];
		sprintf(msg, "%04d", this->hp);
		//SPRINTF　S　String　Print Format　フォーマットに合わせて出力
		//　Intからchar配列への変換

		for (int i = 0; i < 4; i++) {
			if (msg[i] != ' ') {
				ML::Box2D  draw2 (this->pos.x+64, this->pos.y+100, 19, 32);
				ML::Box2D  src2((msg[i] - '0') * 19, 32, 19, 32);
				draw2.Offset((19 * i), 0);

				//this->res->hp->Draw(draw2, src2);
			}
		}
	}
	//-----------------------------------------------------------------------------
	//思考＆状況判断　モーション決定
	void  Object::Think()
	{
		auto  inp = ge->in1->GetState();
		BChara::Motion  nm = this->motion;	//とりあえず今の状態を指定

		//思考（入力）や状況に応じてモーションを変更する事を目的としている。
		//モーションの変更以外の処理は行わない
		switch (nm) {
		default:
			if (inp.B2.down) { nm = Motion::Walk; }
			break;
		case  Motion::Walk:		//歩いている
			//if (inp.B1.down) { nm = Motion::Attack; }
			//if (inp.B3.down) { nm = Motion::Damage; }
			break;
		case  Motion::Attack:	//攻撃中

			break;
		case  Motion::Damage:
			if (inp.B3.down) { nm = Motion::Walk; }
			break;
		}
		//モーション更新
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	//モーションに対応した処理
	//(モーションは変更しない）
	void  Object::Move()
	{


		//-----------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->motion)
		{
		default:

		case  Motion::Walk:		//歩いている
			this->moveVec.y = maxSpeed;
			break;
		case  Motion::Attack:	//攻撃中
			break;
		case  Motion::Damage:   //負傷
			break;
		}

	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo  Object::Anim()
	{
		ML::Color  defColor(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ ML::Box2D(0 , 0, 64, 64), ML::Box2D(32 * 0 ,0, 32, 32), defColor },	//歩行１
			{ ML::Box2D(0, 0, 64, 64), ML::Box2D(32 * 1, 0, 32, 32), defColor },	//歩行２
			{ ML::Box2D(0, 0, 64, 64), ML::Box2D(32 * 2, 0, 32, 32), defColor },	//歩行３
			{ ML::Box2D(0, 0, 64, 64), ML::Box2D(32 * 3, 0, 32, 32), defColor },	//歩行４
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 0, 224, 224, 224), defColor },	//攻撃１
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 1, 224, 224, 224), defColor },	//攻撃２
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 2 ,224, 224, 224), defColor },	//攻撃３
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 0, 224 * 3, 224, 224), defColor },	//負傷１
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 1, 224 * 3, 224, 224), defColor },	//負傷２
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 2, 224 * 3, 224, 224), defColor },	//負傷3


		};
		BChara::DrawInfo  rtv;
		int  work;
		switch (this->motion) {
		default:		rtv = imageTable[0];	break;
			//	停止----------------------------------------------------------------------------
		case  Motion::Walk:
			work = this->animCnt / 8;
			work %= 3;
			rtv = imageTable[work];
			break;

		case  Motion::Attack:
			work = this->animCnt / 8;
			work %= 3;
			rtv = imageTable[work + 4];
			break;
		case  Motion::Damage:
			work = this->animCnt / 8;
			work %= 3;
			rtv = imageTable[work + 7];
			break;
		}
		//	向きに応じて画像を左右反転する
		if (Angle_LR::Left == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//接触時の応答処理（これ自体はダミーのようなモノ）
	void  Object::Received(BChara*  from_, AttackInfo  at_)
	{
		this->hp -= at_.power;

		if (this->hp <= 0)
		{
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMBMINI, this->pos);
			++ge->score;
			this->Kill();
		}
		else {
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMB, this->pos, 0.5f);
		}
	}
	//-----------------------------------------------------------------------------
	////接触判定
	//bool  Object::CheckHit(const  ML::Box2D&  hit_)
	//{
	//	ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
	//	return  me.Hit(hit_);
	//}
	//
	//
	
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




