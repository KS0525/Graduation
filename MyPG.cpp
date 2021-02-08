#include "MyPG.h"
#include "Task_logo.h"

#define MYDEBUG
#define	REFRESHRATE   60


namespace MyPG
{
	//---------------------------------------------------------------------------------------
	//ウインドウへメッセージの表示
	//	引数
	//		x,y			表示位置
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToDisplay(int x, int y, const char * str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		ML::Box2D draw(x, y, 480, 30);
		//        DG::Font::SP font = DG::Font::Create("HGS行書体", 12, 24, 100);
		this->debugFont->Draw(draw, buf, ML::Color(1, 1, 1, 1));
#else
		//空実装
#endif
	}

	//---------------------------------------------------------------------------------------
	//タイトルへメッセージの表示
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToTitle(const char * str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		SetWindowText(GetActiveWindow(), (LPCSTR)buf);
#else
		//空実装
#endif
	}

	//---------------------------------------------------------------------------------------
	//メッセージボックスへの表示
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToBox(const char *str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		MessageBox(NULL, (LPCSTR)buf, "Message", MB_OK);
#else
		//空実装
#endif
	}
	//---------------------------------------------------------------------------------------
	//コンソールへの表示
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToConsole(const char *str, ...) {
#ifdef MYDEBUG
		char buf[256];
		vsprintf(buf, str, (char*)(&str + 1));
		OutputDebugString((LPCSTR)buf);
#else
		//空実装
#endif
	}


	//---------------------------------------------------------------------------------------
	//ファイルOUT関数
	//	引数
	//		str			フォーマット前文字列
	//		...			可変個引数指定
	//---------------------------------------------------------------------------------------
	void MyGameEngine::Dbg_ToFileOut(const char *str, ...) {
#ifdef MYDEBUG

		FILE*	fp;
		char buff[128];
		vsprintf(buff, (char*)str, (char*)(&str + 1));
		strcat(buff, "\n");
		if ((fp = fopen("debug.txt", "at")) != NULL) {
			fprintf(fp, "%s", buff);
			fclose(fp);
		}
#else
		//空実装
#endif
	}

	MyGameEngine::MyGameEngine()
		:GameEngine(
			//	環境情報
			1280,							//	画面サイズX
			720,							//	画面サイズY
			1,								//	表示倍率
			1,								//	マルチサンプル(1or2or4)
			false,							//	フルスクリーンモード
			480,							//	2D描画用解像度X(現在非対応）
			270,							//	2D描画用解像度Y(現在非対応）
			"防衛シューティングゲーム",	//	ウインドウタイトル
			"防衛シューティングゲーム",		//	ウィンドウクラス名
			1,//UpDate呼び出し頻度　↓のRender呼び出し頻度との比率で決まる１以上を指定すること
			1 //Render呼び出し頻度　↑のUpDate呼び出し頻度との比率で決まる１以上を指定すること
			//例えば、120Hz固定のPCで、60Hzと同等の状態で動かす場合、1U/2Rで設定すれば、UpDateの呼び出しが1/2になる
			) {
	}

	//ゲームエンジンに追加したものの初期化と開放
	bool MyGameEngine::Initialize(HWND wnd_)
	{
		//　画像描画ライブラリの初期化
		{
			this->dgi = DG::DGObject::Create(wnd_, this->screenWidth, this->screenHeight, this->multiSample,
				this->screenMode, this->screen2DWidth, this->screen2DHeight);
			if (!this->dgi) {
				//	環境の初期化に失敗
				DestroyWindow(wnd_);
				return false;
			}
		}
		//	入力機器ライブラリの初期化
		{
			this->dii = XI::Obj::Create(wnd_);
			if (!this->dii) {
				//	環境の初期化に失敗
				DestroyWindow(wnd_);
				return false;
			}
		}
		//	サウンドライブラリの初期化
		{
			this->dmi = DM::Obj::Create(wnd_);
			if (!this->dmi) {
				//	環境の初期化に失敗
				DestroyWindow(wnd_);
				return false;
			}
		}
		//小柳サウンドライブラリ初期化
		{
			bgm::Init();
			se::Init(wnd_);
		}
		//変数の初期化
		/*{
			for (auto a : isClear) {
				*a = false;
			}
		}*/
		for (int i = 0;i < 3;++i) {
			for (int j = 0;j < 6;++j) {
				ge->isClear[i][j] = false;
			}
		}
		//FPS計測
		{
			c = new FPSCounter(REFRESHRATE);
		}

		//デバッグ用フォントの準備
		this->debugFont = DG::Font::Create("ＭＳ ゴシック", 6, 16);

		//デフォルトカメラ
		this->camera[0] = MyPG::Camera::Create(
			D3DXVECTOR3(1.0f, 0.0f, 0.0f),	//	ターゲット位置
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//	カメラ位置
			D3DXVECTOR3(0.0f, 1.0f, 0.0f),		//	カメラの上方向ベクトル
			ML::ToRadian(60.0f), 10.0f, 300.0f,	//	視野角・視野距離
			(float)this->screenWidth / (float)this->screenHeight);		//	画面比率
		this->dgi->EffectState().param.bgColor = ML::Color(1, 1, 1, 1);
		this->camera[3] = this->camera[2] = this->camera[1] = this->camera[0];

		//デバッグ用情報の表示ON/OFF
		DebugMode(false);

		//キーボードの入力を受け取るオブジェクトを生成する
		XI::AnalogAxisKB	ls = { DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_DOWN };
		XI::AnalogAxisKB	rs = { DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD2 };
		XI::AnalogTriggerKB  tg = { DIK_E, DIK_R };
		XI::KeyDatas_KB	 key = {
			{ DIK_W, XI::VGP::B4 },{ DIK_A, XI::VGP::B3 },{ DIK_S, XI::VGP::B1 },
			{ DIK_D, XI::VGP::B2 },{ DIK_Z, XI::VGP::SE },{ DIK_X, XI::VGP::ST },
			{ DIK_Q, XI::VGP::L1 },{ DIK_C, XI::VGP::R1 },
			{ DIK_V, XI::VGP::L3 },{ DIK_NUMPAD5, XI::VGP::R3 },
			{DIK_UP,XI::VGP::HU},{DIK_LEFT,XI::VGP::HL},{DIK_RIGHT,XI::VGP::HR},{DIK_DOWN,XI::VGP::HD}
		};
		//ゲームパッドパラメータ
		XI::KeyDatas_GP	but = {
			{ XI::RGP::B01, XI::VGP::B1 },{ XI::RGP::B02, XI::VGP::B2 },
			{ XI::RGP::B03, XI::VGP::B3 },{ XI::RGP::B04, XI::VGP::B4 },
			{ XI::RGP::B05, XI::VGP::L1 },{ XI::RGP::B06, XI::VGP::R1 },
			{ XI::RGP::B07, XI::VGP::SE },{ XI::RGP::B08, XI::VGP::ST },
			{ XI::RGP::B09, XI::VGP::L3 },{ XI::RGP::B10, XI::VGP::R3 },
		};
		this->in1 = XI::GamePad::CreateKB(ls, rs, tg, key);
		auto wgp = XI::GamePad::CreateGP(0, but);
		XI::GamePad::Link(this->in1, wgp);
		this->in2 = XI::GamePad::CreateGP(1, but);
		this->in3 = XI::GamePad::CreateGP(2, but);
		this->in4 = XI::GamePad::CreateGP(3, but);
		this->mouse = XI::Mouse::Create(ge->viewScaleW, ge->viewScaleH);

		this->isReady = false;
		this->gameClearFlag = false;
		//背景色の設定（デフォルト）
		this->dgi->EffectState().param.bgColor = ML::Color(0, .0f, .0f, .0f);

		//初期実行タスク生成＆ゲームエンジンに登録
		auto  ft = Logo::Object::Create(true);

		effectCreator = EffectCreate::Object::Create(true);
		//------------------------------------------------------------------------------------
		//レイヤー毎の描画のON/OFF
		//------------------------------------------------------------------------------------
		Layer_3D(false, false, false, false);
		Layer_2D(false, true);
		return true;
	}
	MyGameEngine::~MyGameEngine()
	{
		//小柳サウンドライブラリ初期化
		se::Del();
		bgm::Del();

		free(c);
	}
	//ゲームエンジンに追加したもののステップ処理
	void MyGameEngine::UpDate()
	{
		//	入力装置の更新
		this->dii->UpDate();
		//	サウンドの更新(ストリーミング監視）
		this->dmi->UpDate();
		
		//小柳サウンドライブラリ更新
		se::EndCheck();
		bgm::EndCheck();
	}

	//2D描画環境のデフォルトパラメータ設定
	void MyGameEngine::Set2DRenderState(DWORD l_)
	{
		//共通？
		this->dgi->EffectState().RS_Def2D();
		this->dgi->EffectState().BS_Alpha();
		if (l_ == 0) {
		}
		if (l_ == 1) {
		}
	}
	//3D描画環境のデフォルトパラメータ設定
	void MyGameEngine::Set3DRenderState(DWORD l_)
	{
		//出力マージャを初期状態に戻す
		this->dgi->EffectState().BS_Std();
		//	カメラの設定更新
		camera[l_]->UpDate();
		//レイヤー0番に対する設定
		if (l_ == 0) {
		}
		if (l_ == 1) {
		}
		if (l_ == 2) {
		}
		if (l_ == 3) {
		}
	}
	//------------------------------------------------------------
	Camera::Camera(const ML::Vec3&  tg_,	//	被写体の位置
		const ML::Vec3&  pos_,	//	カメラの位置
		const ML::Vec3&  up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		float            fov_,	//	視野角
		float            np_,	//	前クリップ平面（これより前は映らない）
		float            fp_,	//	後クリップ平面（これより後ろは映らない）
		float            asp_)	//	アスペクト比（画面の比率に合わせる　横÷縦）	
		: pos(pos_), target(tg_), up(up_), fov(fov_), nearPlane(np_), forePlane(fp_), aspect(asp_)
	{
		UpDate();
	}
	Camera::~Camera()
	{
	}
	//	カメラを生成する
	Camera::SP Camera::Create(const ML::Vec3&  tg_,	//	被写体の位置
		const ML::Vec3&  pos_,	//	カメラの位置
		const ML::Vec3&  up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
		float            fov_,	//	視野角
		float            np_,	//	前クリップ平面（これより前は映らない）
		float            fp_,	//	後クリップ平面（これより後ろは映らない）
		float            asp_)	//	アスペクト比（画面の比率に合わせる　横÷縦）	
	{
		return Camera::SP(new Camera(tg_, pos_, up_, fov_, np_, fp_, asp_));
	}
	//	カメラの設定
	void Camera::UpDate()
	{
		auto  dgi = DG::DGObject::GetInst();	if (dgi == 0) { return; }

		//ビュー行列を設定
		matView.LookAtLH(pos, target, up);
		//プロジェクション行列を設定
		matProj.PerspectiveFovLH(fov, aspect, nearPlane, forePlane);
		//カメラ関係の更新
		dgi->EffectState().param.matView = matView;
		dgi->EffectState().param.matProjection = matProj;
		dgi->EffectState().param.eyePos = pos;
	}
}

MyPG::MyGameEngine* ge;
