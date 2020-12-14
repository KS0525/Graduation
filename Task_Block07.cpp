//-------------------------------------------------------------------
//�e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block07.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "Task_EffectHit.h"
#include  "Task_EffectBomb.h"



namespace  Block07
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Block/Block_07.jpg");
		se = DM::Sound::CreateSE("./data/sound/shot.wav");
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
		i = 0;

		this->render2D_Priority[1] = 1.0f;

		//�ړ����Ԑݒ�
		RightTime = 100;
		DownTime = 40;
		LeftTime = 100;
		UpTime = 40;

		this->maxFallSpeed = 10.0f;	//�ő嗎�����x
		this->gensoku = 0.2f;		//���Ԃɂ�錸����

		ge->serial++;
		this->serial = ge->serial;


		this->gravity = ML::Gravity(32) * 5; //�d�͉����x�����ԑ��x�ɂ����Z��
		//���^�X�N�̐���
		//this->res->se->Play_Normal(false);

		se::LoadFile("shot", "./data/sound/shot.wav");
		se::Play("shot");
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
		//pos.x += moveVec.x;
		//pos.y += moveVec.y;
		auto key = ge->in1->GetState();
		this->moveCnt++;
		//�d�͕ύX
		switch (this->direction[i])
		{
		case 0:
			if (this->RightTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::right;
			}
			else
			{
				i++;
				this->moveCnt = 0;
			}
			break;
		case 1:
			if (this->DownTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::down;
			}
			else
			{
				i++;
				this->moveCnt = 0;
			}
			break;
		case 2:
			if (this->LeftTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::left;
			}
			else
			{
				i++;
				this->moveCnt = 0;
			}
			break;
		case 3:
			if (this->UpTime > this->moveCnt)
			{
				this->MoveGravity = Gravity::up;
			}
			else
			{
				i ++;
				this->moveCnt = 0;
			}
			break;
			
		}
		if (i > 3)i = 0;
		this->GravityMotion("�u���b�N");


		//this->pos += this->moveVec;

		//��ʊO�֏o�Ȃ��悤��
		if (this->pos.y < -this->hitBase.h) { pos.y = ge->screen2DHeight; }
		
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
			//ge->effectCreator->CreateEffect(EffectCreate::Object::BOMB, this->pos,0.5f);
			//++ge->score;
			//this->Kill();
		}
		else {
			//ge->effectCreator->CreateEffect(EffectCreate::Object::BOMBMINI, this->pos);
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