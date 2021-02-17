//-------------------------------------------------------------------
//�^�C�g�����
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
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Goal/Goal02.png");
		this->choosingimg = DG::Image::Create("./data/image/UI/UI_choosing.png");
		this->imgselect = DG::Image::Create("./data/image/UI/select3-3.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		img.reset();
		choosingimg.reset();
		imgselect.reset();
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

		//���f�[�^������
		//easing::Init();
		BackGround::Object::Create(true);
		choosing = 0;
		choiceMax_ = sizeof(buttons)/sizeof(buttons[0]);
		//���^�X�N�̐���
		se::LoadFile("ok", "./data/sound/se_maoudamashii_system46.wav");
		se::SetVolume("ok", 70);
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
			auto nextTask = StageSelector::Object::Create(true);
			return  true;
		}
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();
		Carsol();
		
		if (key.B1.down) {
			//ge->nowStage = buttons[choosing].get_StagePass();
			ge->mapNum = this->choosing + 1;
			se::Play("ok");
				//���g�ɏ��ŗv��
				this->Kill();
			}
		//}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
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
		//�J�[�\���ړ�
		if (key.LStick.BR.down) {
			choosing++;
		}
		if (key.LStick.BL.down) {
			choosing--;
		}
		//�����Ə��
		if (choosing < 0) {
			choosing = 0;
		}
		if (choosing > this->choiceMax_) {
			choosing = choiceMax_ -1 ;
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