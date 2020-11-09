//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EffectHit.h"

namespace  EffectHit
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/effect/exp.png");
//		this->bomSe = DM::Sound::CreateSE("./data/sound/shot.wav");

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
		//this->render2D_Priority[1] = 1.0f;
		//this->pos = ML::Vec2(100, 100);

		int r = ML::ToRadian(rand() % 180 + 100);
		this->moveVec = ML::Vec2(cos(r)*3, -sin(r) * 3);
		this->alpha = 1.f;

		this->animCnt = 0;	//切り替えのカウント
		this->animMax = 4;	//アニメーション数
		this->animWait = 12;//切り替え時間
		this->animNo = 0;	//現在のアニメ番号
		this->animXCount = 4;
		this->sizeH = 100;
		this->sizeW = 100;
		this->scale = 0.5f;

		this->count = 20;
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
		//座標の切り替え
		this->pos += ML::Vec2(this->moveVec.x-1, this->moveVec.y);

		this->count--;
		if (this->count == 0) {
			this->Kill();
		}
		//アニメカウント加算
		this->animCnt++;
		if ((this->animCnt >= this->animWait)) {
			this->animCnt = 0;
			//アニメ変化
			this->animNo++;
			if (this->animNo >= this->animMax) {
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw1(-this->sizeW/2 * this->scale, -this->sizeH / 2 * this->scale,
							this->sizeW * this->scale, this->sizeH* this->scale);
		draw1.Offset((int)this->pos.x, (int)this->pos.y);
		ML::Box2D  src1(animNo *40 , 0, 40, 40);
		this->res->img->Draw(draw1, src1);
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