//-------------------------------------------------------------------
//�e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block06.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "Task_EffectHit.h"
#include  "Task_EffectBomb.h"

namespace  Block06
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Block/Block_06.jpg");
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
		hitBase = ML::Box2D(0, 0, 128, 128);
		hp = 3;
		moveVec = { 0,2 };
		atk = { 0 };

		this->render2D_Priority[1] = 1.0f;

		this->maxFallSpeed = 10.0f;	//�ő嗎�����x
		this->gensoku = 0.2f;		//���Ԃɂ�錸����

		ge->serial++;
		this->serial = ge->serial;

		this->gravity = ML::Gravity(32) * 5; //�d�͉����x�����ԑ��x�ɂ����Z��
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

		//�d�͕ύX
		if (key.LStick.BU.on) { this->MoveGravity = Gravity::up; }
		if (key.LStick.BL.on) { this->MoveGravity = Gravity::left; }
		if (key.LStick.BD.on) { this->MoveGravity = Gravity::down; }
		if (key.LStick.BR.on) { this->MoveGravity = Gravity::right; }

		this->GravityMotion("�u���b�N");

		//this->pos += this->moveVec;

		//��ʊO�֏o�Ȃ��悤��
		if (this->pos.x < -this->hitBase.w) { pos.x = ge->screen2DWidth + this->hitBase.w; }
		if (this->pos.y < -this->hitBase.h) { pos.y = ge->screen2DHeight + this->hitBase.h; }
		if (this->pos.x > ge->screen2DWidth + this->hitBase.w) { pos.x =  - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight + this->hitBase.h) { pos.y =  - this->hitBase.h; }
		
		//�G�Ƃ̓����蔻��
		if (this->Attack_Std("�v���C���[")) { //���ʉ��ɂ��
			//�ڐG���Ă����ꍇ�A�����ɑ΂��ĉ����������Ȃ�
			
		   //this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = hitBase;
		ML::Box2D src(0, 0, 128, 128);
		draw.Offset(this->pos);

		res->img->Draw(draw, src);
	}
	//------------------------------------------------------------------
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	void  Object::Received(BChara*  from_)
	{

		if (this->hp <= 0)
		{
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMB, this->pos,0.5f);
			++ge->score;
			this->Kill();
		}
		else {
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMBMINI, this->pos);
		}
	}
	//------------------------------------------------------------------
	bool Object::Check_bottom()
	{
		ML::Box2D bottom(this->hitBase.x, this->hitBase.y + this->hitBase.h, this->hitBase.w, 1);
		bottom.Offset(this->pos);

		auto pl = ge->GetTask_One_GN<Player::Object>(Player::defGroupName,Player::defName);
		if (nullptr == pl) {return false;}

		return pl->CheckHit(bottom);
	}
	//------------------------------------------------------------------
	bool Object::Attack_Std(const string& GName)
	{
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	
		ML::Box2D bottom(this->hitBase.x, this->hitBase.y + this->hitBase.h, this->hitBase.w, 1);
		bottom.Offset(this->pos);

	auto targets = ge->GetTask_Group_G<BChara>(GName);
	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//����ɐڐG�̗L�����m�F������
		if ((*it)->CheckHit(bottom)) {
			//����Ƀ_���[�W�̏������s�킹��
			(*it)->Received(this);
			return true;
		}
	}
	return false;
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