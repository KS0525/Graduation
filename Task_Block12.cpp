//-------------------------------------------------------------------
//�u���b�N(�ړ��s��)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block12.h"
#include  "Task_Player.h"

namespace  Block12
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Block/main/Block_floor_01.png");
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
		hitBase = ML::Box2D(0, 0, 128, 128);
		moveVec = { 0,2 };
		atk = { 0 };

		this->render2D_Priority[1] = 1.0f;

		this->maxFallSpeed = 10.0f;	//�ő嗎�����x
		this->gensoku = 0.2f;		//���Ԃɂ�錸����
		this->gravity = ML::Gravity(32) * 5; //�d�͉����x�����ԑ��x�ɂ����Z��
		this->reverse = false;
		ge->serial++;
		this->serial = ge->serial;

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
		auto key = ge->in1->GetState();

		//��ʊO�֏o�Ȃ��悤��
		if (this->pos.x < 0) { pos.x = 0; this->moveVec.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; this->moveVec.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; this->moveVec.x = 0; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; this->moveVec.y = 0; }

		////�G�Ƃ̓����蔻��
		if (this->Attack_Std("�v���C���[")) { //���ʉ��ɂ��
			//�ڐG���Ă����ꍇ�A�����ɑ΂��ĉ����������Ȃ�
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = hitBase;
		float pie(3.1415f);
		if (this->reverse) {
			this->res->img->Rotation(pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
		}
		else {
			this->res->img->Rotation(0 * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
		}
		ML::Box2D src(0, 0, 1280, 46);
		draw.Offset(this->pos);

		res->img->Draw(draw, src);
	}
	//------------------------------------------------------------------
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	void  Object::Received(BChara* from_)
	{

	}
	//------------------------------------------------------------------
	bool Object::Attack_Std(const string& GName)
	{
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);

		auto targets = ge->GetTask_Group_G<BChara>(GName);
		for (auto it = targets->begin();
			it != targets->end();
			++it) {
			//����ɐڐG�̗L�����m�F������
			if ((*it)->CheckHit(me) && this->serial != (*it)->serial) {
				//����Ƀ_���[�W�̏������s�킹��
				(*it)->Received(this);
				return true;
			}
		}
		return false;
	}

	bool  Object::CheckHit(const  ML::Box2D& hit_)
	{
		ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
		return  me.Hit(hit_);
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