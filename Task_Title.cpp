//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_MapSelector.h"
#include "easing.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->back = DG::Image::Create("./data/image/Title/title.png");
		//this->titlelogo = DG::Image::Create("./data/image/title.png");
		
		//bgm::LoadFile("title", "./data/sound/�����̐��E-Real_promenade-.mp3");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//this->back.reset();
		//this->titlelogo.reset();
		//bgm::Pause("title");
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
		//���f�[�^������
		//easing::Init();

		//���^�X�N�̐���
		//bgm::Play("title");
		//easing::Init();
		//���ォ��o�E���h�ŗ����Ă���
		//easing::Create("titlelogoY", easing::EASINGTYPE::BOUNCEOUT, -100, 300, 100);
		//easing::Start("titlelogoY");
		//easing::Create("titlelogoX", easing::EASINGTYPE::CIRCOUT, -100, 1280 / 2, 100);
		//easing::Start("titlelogoX");

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto nextTask = MapSelector::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();

		//easing::UpDate();

		if (ms.LB.down) {
			//���S�̏o�����I����Ă��邩�H
		//	if(easing::GetState("titlelogoX") == easing::EQ_STATE::EQ_END){
				//���g�ɏ��ŗv��
				this->Kill();
			}
		//}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw1(0, 0, 1280, 720);
		ML::Box2D  src1(0, 0, 1280, 720);
		this->res->back->Draw(draw1, src1);

		//ML::Box2D  draw2(-792 / 2, 89 / 2, 792, 89);
	//	ML::Box2D  src2(0, 0, 792, 89);
		//int x = easing::GetPos("titlelogoX");
	//	int y = easing::GetPos("titlelogoY");

	//	draw2.Offset(x, y);
		//this->res->titlelogo->Draw(draw2, src2);
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