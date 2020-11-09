//-----------------------------------------------------------------------------
//�G�N���X
//-----------------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_Enemy.h"
#include "Task_Player.h"
#include "Task_EffectBomb.h"
#include "Task_EffectCreate.h"

namespace Enemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara/mob�`�b�v/pipo-charachip018.png");
		this->hp = DG::Image::Create("./data/image/font_number.png");
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
		//this->render2D_Priority[1] = 0.5f;
		this->pos.x = (float)ge->screenWidth;

		this->hitBase = ML::Box2D(0, 0,64, 64);
		this->angle_LR = Angle_LR::Right;
		this->controller = ge->in1;
		this->animCnt = 0;
		this->motion = Motion::Walk;		//�L�����������
		this->maxSpeed = 2.0f;		//�ő�ړ����x�i���j
		this->addSpeed = 1.0f;		//���s�����x�i�n�ʂ̉e���ł�����x�ł��������
		this->hp = 1;
		this->angle = 0;
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
		
	
		angle++;
		moveVec.x = sin(angle/10) * 5;
		pos += moveVec;
		this->moveCnt++;
		this->animCnt++;
		//�v�l�E�󋵔��f
		this->Think();
		//�����[�V�����ɑΉ���������
		this->Move();
		//�߂荞�܂Ȃ��ړ�
		//ML::Vec2  est = this->moveVec;
		//this->CheckMove(est);


		if (pos.y > ge->screenHeight) {
			Kill();
		}
		//�v���C���[�Ƃ̓����蔻��
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		auto player = ge->GetTask_Group_G<Player::Object>("�v���C���[");
		for (auto it = player->begin(); it != player->end(); ++it)
		{
			ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
			if (you.Hit(me))
			{
				//Eff_dead();
				auto eff = EffectCreate::Object::Create(true);
				eff->CreateEffect(EffectCreate::Object::EFFECTTYPE::BOMB, this->pos);
				//EffectCreate::Object::CreateEffect()
				this->Kill();
				(*it)->Kill();
				
				ge->KillAll_G("�{��");
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		
		this->res->img->Draw(di.draw, di.src);

		//HP�`��
		char msg[10];
		sprintf(msg, "%04d", this->hp);
		//SPRINTF�@S�@String�@Print Format�@�t�H�[�}�b�g�ɍ��킹�ďo��
		//�@Int����char�z��ւ̕ϊ�

		for (int i = 0; i < 4; i++) {
			if (msg[i] != ' ') {
				ML::Box2D  draw2 (this->pos.x+64, this->pos.y+100, 19, 32);
				ML::Box2D  src2((msg[i] - '0') * 19, 32, 19, 32);
				draw2.Offset((19 * i), 0);

				//this->res->hp->Draw(draw2, src2);
			}
		}
	}
	//-----------------------------------------------------------------------------
	//�v�l���󋵔��f�@���[�V��������
	void  Object::Think()
	{
		auto  inp = ge->in1->GetState();
		BChara::Motion  nm = this->motion;	//�Ƃ肠�������̏�Ԃ��w��

		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���鎖��ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm) {
		default:
			if (inp.B2.down) { nm = Motion::Walk; }
			break;
		case  Motion::Walk:		//�����Ă���
			//if (inp.B1.down) { nm = Motion::Attack; }
			//if (inp.B3.down) { nm = Motion::Damage; }
			break;
		case  Motion::Attack:	//�U����

			break;
		case  Motion::Damage:
			if (inp.B3.down) { nm = Motion::Walk; }
			break;
		}
		//���[�V�����X�V
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	//���[�V�����ɑΉ���������
	//(���[�V�����͕ύX���Ȃ��j
	void  Object::Move()
	{


		//-----------------------------------------------------------------
		//���[�V�������ɌŗL�̏���
		switch (this->motion)
		{
		default:

		case  Motion::Walk:		//�����Ă���
			this->moveVec.y = maxSpeed;
			break;
		case  Motion::Attack:	//�U����
			break;
		case  Motion::Damage:   //����
			break;
		}

	}
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo  Object::Anim()
	{
		ML::Color  defColor(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ ML::Box2D(0 , 0, 64, 64), ML::Box2D(32 * 0 ,0, 32, 32), defColor },	//���s�P
			{ ML::Box2D(0, 0, 64, 64), ML::Box2D(32 * 1, 0, 32, 32), defColor },	//���s�Q
			{ ML::Box2D(0, 0, 64, 64), ML::Box2D(32 * 2, 0, 32, 32), defColor },	//���s�R
			{ ML::Box2D(0, 0, 64, 64), ML::Box2D(32 * 3, 0, 32, 32), defColor },	//���s�S
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 0, 224, 224, 224), defColor },	//�U���P
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 1, 224, 224, 224), defColor },	//�U���Q
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 2 ,224, 224, 224), defColor },	//�U���R
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 0, 224 * 3, 224, 224), defColor },	//�����P
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 1, 224 * 3, 224, 224), defColor },	//�����Q
			{ ML::Box2D(-20, -40, 224, 224), ML::Box2D(224 * 2, 224 * 3, 224, 224), defColor },	//����3


		};
		BChara::DrawInfo  rtv;
		int  work;
		switch (this->motion) {
		default:		rtv = imageTable[0];	break;
			//	��~----------------------------------------------------------------------------
		case  Motion::Walk:
			work = this->animCnt / 8;
			work %= 3;
			rtv = imageTable[work];
			break;

		case  Motion::Attack:
			work = this->animCnt / 8;
			work %= 3;
			rtv = imageTable[work + 4];
			break;
		case  Motion::Damage:
			work = this->animCnt / 8;
			work %= 3;
			rtv = imageTable[work + 7];
			break;
		}
		//	�����ɉ����ĉ摜�����E���]����
		if (Angle_LR::Left == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	void  Object::Received(BChara*  from_, AttackInfo  at_)
	{
		this->hp -= at_.power;

		if (this->hp <= 0)
		{
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMBMINI, this->pos);
			++ge->score;
			this->Kill();
		}
		else {
			ge->effectCreator->CreateEffect(EffectCreate::Object::BOMB, this->pos, 0.5f);
		}
	}
	//-----------------------------------------------------------------------------
	////�ڐG����
	//bool  Object::CheckHit(const  ML::Box2D&  hit_)
	//{
	//	ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
	//	return  me.Hit(hit_);
	//}
	//
	//
	
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




