//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_StageSelector.h"
#include  "Task_BackGround.h"
#include  "Task_Game.h"

#include "easing.h"

namespace  StageSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->eggCapsule = DG::Image::Create("./data/image/Select/Select_body.png");

		this->nonClearEgg[0] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_01_.png");
		this->nonClearEgg[1] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_02_.png");
		this->nonClearEgg[2] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_03_.png");
		this->nonClearEgg[3] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_04_.png");
		this->nonClearEgg[4] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_05_.png");
		this->nonClearEgg[5] = DG::Image::Create("./data/image/Select/Select_toumeiEgg_06_.png");

		this->clearEgg[0] = DG::Image::Create("./data/image/Select/Select_Egg_01.png");
		this->clearEgg[1] = DG::Image::Create("./data/image/Select/Select_Egg_02.png");
		this->clearEgg[2] = DG::Image::Create("./data/image/Select/Select_Egg_03.png");
		this->clearEgg[3] = DG::Image::Create("./data/image/Select/Select_Egg_04.png");
		this->clearEgg[4] = DG::Image::Create("./data/image/Select/Select_Egg_05.png");
		this->clearEgg[5] = DG::Image::Create("./data/image/Select/Select_Egg_06.png");

		this->selectEgg[0] = DG::Image::Create("./data/image/Select/Select_select_01.png");
		this->selectEgg[1] = DG::Image::Create("./data/image/Select/Select_select_02.png");
		this->selectEgg[2] = DG::Image::Create("./data/image/Select/Select_select_03.png");
		this->selectEgg[3] = DG::Image::Create("./data/image/Select/Select_select_04.png");
		this->selectEgg[4] = DG::Image::Create("./data/image/Select/Select_select_05.png");
		this->selectEgg[5] = DG::Image::Create("./data/image/Select/Select_select_06.png");

		this->choosingimg = DG::Image::Create("./data/image/UI/UI_choosing.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		this->render2D_Priority[1] = 0.5f;

		//Button::set_allNum(0);
		buttons[0].set_StagePass("./data/Map/Map.txt");
		buttons[1].set_StagePass("./data/Map/Map4.txt");
		buttons[2].set_StagePass("./data/Map/Map3.txt");
		buttons[3].set_StagePass("./data/Map/Map5.txt");
		buttons[4].set_StagePass("./data/Map/Map6.txt");
		buttons[5].set_StagePass("./data/Map/Map7.txt");
	/*	buttons[6].set_StagePass("./data/Map/Map8.txt");
		buttons[7].set_StagePass("./data/Map/Map9.txt");
		buttons[8].set_StagePass("./data/Map/Map10.txt");
		buttons[9].set_StagePass("./data/Map/Map11.txt");
		buttons[10].set_StagePass("./data/Map/Map12.txt");
		buttons[11].set_StagePass("./data/Map/Map13.txt");
		buttons[12].set_StagePass("./data/Map/Map14.txt");
		buttons[13].set_StagePass("./data/Map/Map15.txt");*/


		//���f�[�^������
		//easing::Init();
		BackGround::Object::Create(true);
		choosing = 0;
		choiceMax_ = sizeof(buttons)/sizeof(buttons[0]);
		//���^�X�N�̐���
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�w�i�摜");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto nextTask = Game::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();
		Carsol();
		
		if (key.B1.down) {
			ge->nowStage = buttons[choosing].get_StagePass();
				//���g�ɏ��ŗv��
				this->Kill();
			}
		//}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw1(0,0, 1280, 720);
		ML::Box2D  src1(0, 0, 1280, 720);
		this->res->eggCapsule->Draw(draw1, src1);
		
		for(int i = 0;i<6;++i){
			this->res->nonClearEgg[i]->Draw(draw1, src1);
		}
		this->res->selectEgg[choosing]->Draw(draw1, src1);
	}
	//------------------------------------------------------------------
	void Object::Carsol() 
	{
		auto key = ge->in1->GetState();
		//�J�[�\���ړ�
		if (key.LStick.BR.down) {
			choosing++;
		}
		if (key.LStick.BL.down) {
			choosing--;
		}
		//�����Ə��
		if (choosing < 0) {
			choosing = this->choiceMax_ -1;
		}
		if (choosing >= this->choiceMax_) {
			choosing = 0;
		}
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack_ABCDEFGHIJKLMN(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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