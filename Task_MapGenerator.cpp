//-------------------------------------------------------------------
//2次元配列マップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapGenerator.h"
#include  "Task_Enemy.h"
#include  "Task_Enemy01.h"
#include  "Task_Player.h"
#include  "Task_Block00.h"
#include  "Task_Block01.h"
#include  "Task_Block02.h"
#include  "Task_Block03.h"
#include  "Task_Block04.h"
#include  "Task_Block05.h"
#include  "Task_Block06.h"
#include  "Task_Block07.h"
#include  "Task_Block08.h"
#include  "Task_Block09.h"
#include  "Task_Block10.h"
#include  "Task_Switch.h"
#include  "Task_Goal.h"
namespace  Generator
{
	Object::WP Object::instance;
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
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
		this->res = Resource::Create();
		this->genCnt = 0;
		this->genSpan = 1;

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
		this->genCnt++;

		if (!this->evFile) { this->Kill(); } //ファイルが無いなら消滅

		Generate();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

	}
	//-------------------------------------------------------------------
	bool Object::ReadLine(string& lineT_)
	{
		//1行読み込む
		bool rtv = false;
		while (getline(this->evFile, lineT_)) {
			//整合性チェックを行う
			//行内に空白(全角も)以外の文字がない
			if (string::npos == lineT_.find_first_not_of("　")) { continue; }
			//最初の文字が/の場合コメントとみなす
			if ('/' == lineT_.at(0)) { continue; }
			//ヘッダ名とデータの境目である>が含まれていない
			if (string::npos == lineT_.find(">")) { continue; }
			//整合性チェック
			rtv = true; //有効な行とみなす
			break;
		}
		return rtv;
	}
	//-------------------------------------------------------------------
	bool Object::Execute(string& hs_, string& ds_)
	{
		//デリミタ(区切り記号)削除(こまかい文法チェックはしてない)
		string::size_type t;
		while ((t = ds_.find_first_of("(,);")) != string::npos)
		{
			ds_[t] = ' ';
		}
		//
		stringstream ss;
		ss << ds_;
		//イベント終了
		if (hs_ == "end") {
			
			this->Kill();
		}
		//敵の生成
		else if (hs_ == "crt") {
			Output(ss);
		}
		else {
			return false;
		}
		return true;
	}

	//-------------------------------------------------------------------
	bool Object::Set(const string& fpath_)
	{
		//今処理中のファイルを閉じる
		if (this->evFile.is_open()) {
			this->evFile.close();
		}

		//イベントファイル読み込み
		this->evFile.open(fpath_);
		//ファイルが開けない
		if (!this->evFile) {
			return false;
		}
		return true;

	}
	//-------------------------------------------------------------------
	Object::SP Object::Create_Mutex()
	{
		//新規作成か更新かの判別
		if (auto p = instance.lock()) {
			return nullptr;
		}
		else {
			p = Object::Create(true);
			instance = p;
			return p;
		}
	}
	//-------------------------------------------------------------------
	void Object::Output(stringstream& ds_)
	{
		ML::Box2D pos;
		string kind;
		ds_ >> kind >> pos.x >> pos.y >> pos.w >> pos.h;
		
		if (kind == "player") {
			auto pl = Player::Object::Create(true);
			pl->pos = ML::Vec2(pos.x,pos.y);
			pl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block00") {
			auto bl = Block00::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block01") {
			auto bl = Block01::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block02") {
			auto bl = Block02::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block03") {
			auto bl = Block03::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block04") {
			auto bl = Block04::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block05") {
			auto bl = Block05::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block06") {
			auto bl = Block06::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block07") {
			auto bl = Block07::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block08") {
			auto bl = Block08::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block09") {
			auto bl = Block09::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block10") {
			auto bl = Block10::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Switch") {
			auto sw = Switch::Object::Create(true);
			sw->pos = ML::Vec2(pos.x, pos.y);
			sw->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Goal") {
			auto bl = Goal::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
	}
	//-------------------------------------------------------------------
	void Object::Generate()
	{
		if (genCnt % genSpan == 0) {
			//新しい行を読み込む
			string lineText;
			string headerText;
			string dataText;
			if (this->ReadLine(lineText)) {
				//行を解読
				string::size_type t = lineText.find(">");
				headerText = lineText.substr(0, t);
				dataText = lineText.substr(t + 1);
				//ヘッダ名に対応したイベント処理を行う
				bool rtv = this->Execute(headerText, dataText);
				//イベント処理に失敗もしくは自身の状態がアクティブでなくなっていた場合ループを抜ける
				if (false == rtv ||
					BTask::State::Active != this->CheckState()) {

				}
			}

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
	//-------------------------------------------------------------------


}

