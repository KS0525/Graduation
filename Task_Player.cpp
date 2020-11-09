//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "Task_Player.h"
#include "Task_Shot00.h"
#include "Task_EffectHit.h"
#include  "MyPG.h"

namespace Player 
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara/maou_64_64.png");
		this->chargeimg = DG::Image::Create("./data/image/bar.png");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->chargeimg.reset();
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
		pos.y = 300;
		//this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(0, 0, 128, 128);
		this->angle_LR = Angle_LR::Right;
		this->controller = ge->in1;
		this->chargeX = 0;
		this->hp = 10;
		this->dist = 0;
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
		auto mouse = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		//�ړ�����(�i�i�����͓����l��  ��X����)
		int speed = 7;
		if (key.B1.on) { this->pos.y -= speed; }
		if (key.B2.on) { this->pos.x -= speed; }
		if (key.B3.on) { this->pos.y += speed; }
		if (key.B4.on) { this->pos.x += speed; }
		if (this->pos.x < 0) { pos.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; } 
		
		if (mouse.LB.on)
		{
			dist++;
			if (dist % 7 == 0) {
				auto shot = Shot00::Object::Create(true);
				shot->pos.y = this->pos.y;
				ML::Box2D shotpos = this->hitBase.OffsetCopy(pos);
				shot->pos.x = (shotpos.x + (shotpos.x +shotpos.w)) /2;
				shot->moveVec = { 0,-5 };

				shot->atk = 1;

				this->chargeX = 0;
			}
		}
		
		//�J�����̈ʒu���Ē���
		{
			//�v���C������ʂ̂ǂ��ɒu�����i����͉�ʒ����j
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//�v���C������ʒ����ɒu�������̃J�����̍�����W�����߂�
			int cpx = int(this->pos.x) - px;
			int cpy = int(this->pos.y) - py;
			//�J�����̍��W���X�V
			//ge->camera2D.x = cpx;
			//ge->camera2D.y = cpy;
			//�ȉ��̓X�N���[����~�̐߂܂œ��͂��Ȃ�
			if (auto map = ge->qa_Map) {
				//map->AdjustCameraPos();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = this->hitBase;
			ML::Box2D src(0, 0, 64, 64);
			draw.Offset(this->pos);

			this->res->img->Draw(draw, src);
		} 
		
	}
	//-----------------------------------------------------------------------------
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	void  Object::Received(BChara*  from_, AttackInfo  at_)
	{
		hp -= at_.power;
		if (from_->groupName == "�u���b�N") {
			//if(from_->Check_Bottom()){
				this->pos.y = from_->pos.y;
				if (this->pos.y >ge->screenHeight- this->hitBase.h) {
					this->Kill();
					ge->KillAll_G("�{��");
				}
			}
		//}
		if (this->hp <= 0)
		{
			this->Kill();
			ge->KillAll_G("�{��");
		}
	}
	//-----------------------------------------------------------------------------
	//�ڐG����
	bool  Object::CheckHit(const  ML::Box2D&  hit_)
	{
		ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
		return  me.Hit(hit_);
	}
	//-----------------------------------------------------------------------------
	//
	/*bool Object::Check_Head(const ML::Box2D& hit_)
	{
		ML::Box2D me(this->hitBase.x, this->hitBase.y, this->hitBase.w, -1);
		me.Offset(this->pos);
		return me.Hit(hit_);
	}*/
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


