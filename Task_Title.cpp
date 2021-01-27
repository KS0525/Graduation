//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_MapSelector.h"
#include  "Task_StageSelector.h"
#include  "Task_MapGenerator.h"
#include  "Task_BackGround.h"
#include "easing.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->back = DG::Image::Create("./data/image/Title/title.png");
		this->presskey = DG::Image::Create("./data/image/Title/pressKey02.png");
		
		//bgm::LoadFile("title", "./data/sound/�����̐��E-Real_promenade-.mp3");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->back.reset();
		this->presskey.reset();
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
		BackGround::Object::Create(true);

		//���^�X�N�̐���
		if (auto map = Generator::Object::Create_Mutex()) {
			map->Set("./data/Map/Map_Title.txt");
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�v���C���[");
		ge->KillAll_G("�u���b�N");
		ge->KillAll_G("�Œ�u���b�N");
		ge->KillAll_G("�X�C�b�`");
		ge->KillAll_G("�S�[��");
		ge->KillAll_G("�w�i�摜");

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
		auto key = ge->in1->GetState();
		//easing::UpDate();

		//if (ms.RB.down || key.B2.down) {
		//	ge->KillAll_G("�v���C���[");
		//	ge->KillAll_G("�u���b�N");
		//	ge->KillAll_G("�Œ�u���b�N");
		//	ge->KillAll_G("�X�C�b�`");
		//	ge->KillAll_G("�S�[��");

		//	if (auto map = Generator::Object::Create_Mutex()) {
		//		map->Set("./data/Map/Map_Title.txt");
		//	}
		//}

		if (ge->isReady) {
			if (key.B1.down) {
				this->Kill();
				ge->isReady = false;
			}
		}
		if (ms.LB.down) {
			//���g�ɏ��ŗv��
			this->Kill();
			}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw1(0, 0, 1280, 720);
		ML::Box2D  src1(0, 0, 1280, 720);
		this->res->back->Draw(draw1, src1);
		this->res->presskey->Draw(draw1, src1);
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