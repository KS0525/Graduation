//-------------------------------------------------------------------
//�e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot00.h"
#include  "Task_Enemy.h"
#include  "Task_EffectHit.h"
#include  "Task_EffectBomb.h"

namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/chara/devil_anim_32_32.png");
		//se = DM::Sound::CreateSE("./data/sound/shot.wav");
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

		//���f�[�^������
		angle = 0;
		hitBase = ML::Box2D(-32, -32, 32, 32);

		//���^�X�N�̐���
		//this->res->se->Play_Normal(false);

		//se::LoadFile("shot", "./data/sound/shot.wav");
		//se::Play("shot");
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
		pos.x += moveVec.x;
		pos.y += moveVec.y;
		moveCnt++;
		
		/*if (chargeMax) {
			auto eff = EffectCreate::Object::Create(true);
			eff->CreateEffect(EffectCreate::Object::EFFECTTYPE::HIT, this->pos);
				
		}*/
		if (moveCnt >= 120)
		{
			chargeMax = false;
			this->Kill();
		}

		//�G�Ƃ̓����蔻��
		if (this->Attack_Std("�G")) { //���ʉ��ɂ��
			//�ڐG���Ă����ꍇ�A�����ɑ΂��đ���ɉ����������Ȃ�
			
			this->Kill();
		}
		if (this->Attack_Std("�u���b�N")) {

			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = hitBase;
		ML::Box2D src(0, 0, 32, 32);
		draw.Offset(this->pos);

		res->img->Rotation(angle, ML::Vec2(28, 28));
		res->img->Draw(draw, src);
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
				//�i���\�b�h�����ςȂ̂͋��o�[�W�����̃R�s�[�ɂ��o�O��������邽��
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