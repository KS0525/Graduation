//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

#include "easing.h"
namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Ending.bmp");
		//this->back = DG::Image::Create("./data/image/UI/back1.png");
		//this->back2 = DG::Image::Create("./data/image/UI/back2.png");
		//this->back3 = DG::Image::Create("./data/image/UI/back3.png");
		//this->back4 = DG::Image::Create("./data/image/UI/back4.png");


		this->back = DG::Image::Create("./data/image/UI/select3-2.png");
		this->back2 = DG::Image::Create("./data/image/UI/select3-3.png");
		this->back3 = DG::Image::Create("./data/image/UI/select3-4.png");
		this->back4 = DG::Image::Create("./data/image/UI/select3-5.png");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
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

		//���f�[�^������

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
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		if (key.B1.down) {
			this->Kill();
		}

		if (ms.RB.down) {

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
		ML::Box2D  draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D  src(0, 0, 240, 135);

		draw.Offset(0, this->logoPosY);
		this->res->img->Draw(draw, src);

	/*	for (int i = 0; i < 4; ++i) {
			ML::Box2D draw2(i * 200, 640, 128, 128);
			ML::Box2D src2(0, 0, 128, 128);
			this->res->back->Draw(draw2, src2);
		}*/
		ML::Box2D draw1(200, 600, 170, 128);
		ML::Box2D draw2(400, 600, 170, 128);
		ML::Box2D draw3(600, 600, 170, 128);
		ML::Box2D draw4(800, 600, 170, 128);
		ML::Box2D src2(0, 0, 170, 128);

		this->res->back->Draw(draw1, src2);
		this->res->back2->Draw(draw2, src2);
		this->res->back3->Draw(draw3, src2);
		this->res->back4->Draw(draw4, src2);

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