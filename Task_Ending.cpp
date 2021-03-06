//-------------------------------------------------------------------
//GfBO
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include  "Task_StageSelector.h"
#include  "Task_MapSelector.h"
#include  "Task_BackGround.h"
#include "easing.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->appear[0] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_01_01.png");
		this->appear[1] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_01_02.png");
		this->appear[2] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_01_03.png");
		this->appear[3] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_02_01.png");
		this->appear[4] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_02_02.png");
		this->appear[5] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_02_03.png");
		this->appear[6] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_03_01.png");
		this->appear[7] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_03_02.png");
		this->appear[8] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_03_03.png");
		this->appear[9] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_04_01.png");
		this->appear[10] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_04_02.png");
		this->appear[11] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_04_03.png");
		this->appear[12] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_05_01.png");
		this->appear[13] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_05_02.png");
		this->appear[14] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_05_03.png");
		this->appear[15] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_06_01.png");
		this->appear[16] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_06_02.png");
		this->appear[17] = DG::Image::Create("./data/image/Select/nyoki/Select_Egg_06_03.png");
		
		this->fall[0] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_01.png");
		this->fall[1] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_02.png");
		this->fall[2] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_03.png");
		this->fall[3] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_04.png");
		this->fall[4] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_05.png");
		this->fall[5] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_06.png");
		this->fall[6] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_07.png");
		this->fall[7] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_08.png");
		this->fall[8] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_09.png");
		this->fall[9] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_10.png");
		this->fall[10] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_11.png");
		this->fall[11] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_12.png");
		this->fall[12] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_13.png");
		this->fall[13] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_14.png");
		this->fall[14] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_15.png");
		this->fall[15] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_16.png");
		this->fall[16] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_17.png");
		this->fall[17] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_18.png");
		this->fall[18] = DG::Image::Create("./data/image/Select/fall/Select_Eggfall_19.png");

		this->eggCapsule = DG::Image::Create("./data/image/Select/Select_body.png");

		this->nextimg = DG::Image::Create("./data/image/UI/next3-5.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		for (int i = 0;i < 18;++i) {
			this->appear[i].reset();
		}
		for (int i = 0;i < 19;++i) {
			this->fall[i].reset();
		}
		this->eggCapsule.reset();

		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		BackGround::Object::Create(true);
		animCnt = 0;
		isFall = false;
		//^XNΜΆ¬
		se::LoadFile("clear", "./data/sound/[v (online-audio-converter.com).wav");
		se::LoadFile("ok", "./data/sound/se_maoudamashii_system46.wav");
		se::Play("clear");
		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		ge->KillAll_G("wiζ");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
			auto  nextTask = StageSelector::Object::Create(true);
			nextTask->mapNumber = ge->mapNum;
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto key = ge->in1->GetState();

		animCnt++;
		if (!isFall) {
			if (animCnt >= 114) { animCnt = 114;  isFall = true; }
		}
		else {
			if (animCnt >= 17) { animCnt = 17;}
		}

		if (key.B1.down) {
			se::Play("ok");
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 720);
		ML::Box2D src(0, 0, 1280, 720);

		int cuts_fall = 19;
		int cuts_appear = 3;
		int offset_stage = ge->stageNum;
		int frameRate = 6;

		this->res->eggCapsule->Draw(draw, src);

		if (!isFall) {
			this->res->fall[animCnt / frameRate % cuts_fall]->Draw(draw, src);
		}
		else {
			this->res->appear[animCnt / frameRate % cuts_appear + (offset_stage * cuts_appear)]->Draw(draw, src);
		}

		ML::Box2D selectdraw(ge->screen2DWidth / 2, 600, 170, 128);
		ML::Box2D selectsrc(0, 0, 170, 128);
		this->res->nextimg->Draw(selectdraw, selectsrc);
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack_ABCDEFGHIJKLMN(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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