//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSelector.h"
#include  "Task_Game.h"

#include "easing.h"

namespace  MapSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Block/Block_00.jpg");
		this->choosingimg = DG::Image::Create("./data/image/UI_choosing.png");
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

		this->render2D_Priority[1] = 1.0f;

		//Button::set_allNum(0);
		buttons[0].set_StagePass("./data/Map/Map.txt");
		buttons[1].set_StagePass("./data/Map/Map_Title.txt");
		//���f�[�^������
		//easing::Init();
		choosing = 1;
		choiceMax_ = 6;
		//���^�X�N�̐���
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

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
		
		Carsol();
		
		if (ms.LB.down) {
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
#define choiceMax 6
		ML::Box2D draw[choiceMax] = {
			ML::Box2D(100,100,100,100),
			ML::Box2D(200,100,100,100),
			ML::Box2D(300,100,100,100),
			ML::Box2D(400,100,100,100),
			ML::Box2D(500,100,100,100),
			ML::Box2D(600,100,100,100),
		};
		ML::Box2D  draw1(100,100, 100, 100);
		ML::Box2D  src1(0, 0, 128, 128);
		this->res->img->Draw(draw1, src1);
		
		this->res->img->Draw(draw[1], src1);

		ML::Box2D src2(0, 0, 132, 132);
		this->res->choosingimg->Draw(draw[choosing], src2);
		
	}
	//------------------------------------------------------------------
	void Object::Carsol() 
	{
		auto key = ge->in1->GetState();
		//�J�[�\���ړ�
		if (key.B1.down) {
			choosing++;
		}
		if (key.B2.down) {
			choosing--;
		}
		//�����Ə��
		if (choosing < 0) {
			choosing = 0;
		}
		if (choosing > this->choiceMax_) {
			choosing = choiceMax_;
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