//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EffectBomb.h"

namespace  EffectBomb
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/effect/explode.png");
	//	this->bomSe = DM::Sound::CreateSE("./data/sound/shot.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		img.reset();
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
		this->render2D_Priority[1] = 0.0f;
		this->pos = ML::Vec2(0, 0);
		this->moveVec = ML::Vec2(0.f, 0.f);
		this->alpha = 1.f;

		this->animCnt = 0;	//�؂�ւ��̃J�E���g
		this->animMax = 22;	//�A�j���[�V������
		this->animWait = 2;//�؂�ւ�����
		this->animNo = 0;	//���݂̃A�j���ԍ�
		this->animXCount = 22;
		this->sizeW = 1500;
		this->sizeH = 1500;

		this->scale = 0.2f;

//		this->res->bomSe->Play_Normal(false);

		se::LoadFile("bomb", "./data/sound/bom.wav");
		se::Play("bomb");

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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//���W�̐؂�ւ�
		this->pos += this->moveVec;
		//�A�j���J�E���g���Z
		this->animCnt++;
		if ((this->animCnt >= this->animWait)) {
			this->animCnt = 0;
			//�A�j���ω�
			this->animNo++;
			if (this->animNo >= this->animMax) {
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw1(-this->sizeW/2 * this->scale, -this->sizeH / 2 * this->scale,
							this->sizeW * this->scale, this->sizeH* this->scale);
		draw1.Offset((int)this->pos.x, (int)this->pos.y);
		ML::Box2D  src1(this->animNo%this->animXCount*96, this->animNo/this->animXCount*96, 96, 96);
		this->res->img->Draw(draw1, src1);
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