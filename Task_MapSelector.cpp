//-------------------------------------------------------------------
//^Cgๆส
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSelector.h"
#include  "Task_BackGround.h"
#include  "Task_Game.h"
#include  "Task_StageSelector.h"
#include "easing.h"

namespace  MapSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Goal/Goal02.png");
		this->choosingimg = DG::Image::Create("./data/image/UI/UI_choosing.png");
		this->imgselect = DG::Image::Create("./data/image/UI/select3-3.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		img.reset();
		choosingimg.reset();
		imgselect.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		this->render2D_Priority[1] = 0.5f;

		//f[^๚ป
		//easing::Init();
		BackGround::Object::Create(true);
		choosing = 0;
		choiceMax_ = sizeof(buttons)/sizeof(buttons[0]);
		//^XNฬถฌ
		se::LoadFile("ok", "./data/sound/se_maoudamashii_system46.wav");
		se::SetVolume("ok", 70);
		se::LoadFile("carsol", "./data/sound/se_maoudamashii_system48.wav");
		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚
		ge->KillAll_G("wiๆ");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
			auto nextTask = StageSelector::Object::Create(true);
			return  true;
		}
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();
		Carsol();
		
		if (key.B1.down) {
			//ge->nowStage = buttons[choosing].get_StagePass();
			ge->mapNum = this->choosing + 1;
			se::Play("ok");
				//ฉgษมลvฟ
				this->Kill();
			}
		//}

	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 720);
		ML::Box2D draws[5] = { 
			ML::Box2D(50, 40, 264, 264),
			ML::Box2D(279, 400,264, 264),
			ML::Box2D(508, 40, 264, 264),
			ML::Box2D(737, 400, 264, 264),
			ML::Box2D(966, 40, 264, 264),		
		};

		ML::Box2D src(0, 0, 330, 330);
		for (int i = 0; i < 5; ++i) {
			this->res->img->Draw(draws[i], src);
		}
		//this->res->img->Draw(draw, src);

		ML::Box2D chooseSrc(0,0,132,132);
		this->res->choosingimg->Draw(draws[choosing], chooseSrc);

		ML::Box2D selectdraw(ge->screen2DWidth / 2, 600, 170, 128);
		ML::Box2D selectsrc(0, 0, 170, 128);
		this->res->imgselect->Draw(selectdraw, selectsrc);
	}
	//------------------------------------------------------------------
	void Object::Carsol() 
	{
		auto key = ge->in1->GetState();
		//J[\ฺฎ
		if (key.LStick.BR.down) {
			choosing++;
			se::Play("carsol");
		}
		if (key.LStick.BL.down) {
			choosing--;
			se::Play("carsol");
		}
		//บภฦใภ
		if (choosing < 0) {
			choosing = 0;
		}
		if (choosing > this->choiceMax_) {
			choosing = choiceMax_ -1 ;
		}
	}
	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack_ABCDEFGHIJKLMN(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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