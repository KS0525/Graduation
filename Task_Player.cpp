//-----------------------------------------------------------------------------
//�v���C���[�i���j
//-----------------------------------------------------------------------------
#include "Task_Player.h"
#include "Task_Ending.h"
#include "Task_EffectHit.h"
#include "Task_MapGenerator.h"
#include  "MyPG.h"

namespace Player 
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara/Egg_anim_01.png");
		this->break_img = DG::Image::Create("./data/image/chara/Egg_break_anim_01.png");
		
		//this->se = DM::Sound::Create("./data/sound/�ʎq�E�ׂ�01.mp3");

		
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->break_img.reset();
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
		pos.y = 0;
		this->hitBase = ML::Box2D(8,8,34, 34);
		this->angle_LR = Angle_LR::Right;
		this->controller = ge->in1;
		this->hp = 10;
		ge->isDead = false;
		this->isDead = false;
		this->animCnt = 0;
		ge->serial++;
		this->serial = ge->serial;

		this->maxFallSpeed = 10.0f;	//�ő嗎�����x
		this->gensoku = 0.4f;		//���Ԃɂ�錸����
		this->gravity = ML::Gravity(32) * 7; //�d�͉����x�����ԑ��x�ɂ����Z��
		//���^�X�N�̐���
		se::LoadFile("gravity", "./data/sound/�W�����v (online-audio-converter.com) (1).wav");
		bgm::LoadFile("grav", "./data/sound/�W�����v.mp3");
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		bgm::Stop("grav");
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			//auto nextTask = Ending::Object::Create(true);
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto mouse = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		//�d�͕ύX
		if (ge->isReady) {
			if (!ge->isDead) {
				if (key.LStick.BU.on) { this->MoveGravity = Gravity::up; }
				if (key.LStick.BL.on) { this->MoveGravity = Gravity::left; }
				if (key.LStick.BD.on) { this->MoveGravity = Gravity::down; }
				if (key.LStick.BR.on) { this->MoveGravity = Gravity::right; }

				this->GravityMotion("�u���b�N");
				//�d�͑����se
				if (key.LStick.axis.Length() > 0) {
					se::Play("gravity");
					//bgm::Play("grav");
					//se::LoadFile("gravity", "./data/sound/GravityChangeSE12.wav");
					//se::Play("gravity");
				}
			}

		}

		if (ge->isDead) {
			animCnt++;
			se::LoadFile("tamago", "./data/sound/���E�ׂ�.wav");
		
			if (animCnt > 3) {
				animCnt = 3;
			}
			else if (animCnt == 1)
			{				
				se::Play("tamago");
			}
		}
		//��ʊO�֏o�Ȃ��悤��
		if (this->pos.x < 0) { pos.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; }
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
		{
			ML::Box2D draw(0, 0, 50, 50);
			ML::Box2D src(0, 0, 100, 100);

			float pie(3.1415f);
			switch (this->MoveGravity) {
			case 0: //up
				this->res->img->Rotation(pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			case 1: // down
				this->res->img->Rotation(0 * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(0 * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			case 2: //left
				this->res->img->Rotation(0.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(0.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			case 3: //right
				this->res->img->Rotation(1.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				this->res->break_img->Rotation(1.5f * pie, ML::Vec2((draw.x + draw.w) / 2, (draw.y + draw.h) / 2));
				break;
			default:
				break;
			}
			draw.Offset(this->pos);
			//�����Ă���Ƃ�
			if (isDead == false) {
				this->res->img->Draw(draw, src);
			}
			//����ł���Ƃ�
			else {
				ML::Box2D anim[] = {
					{0,0,200,200},
					{200,0,200,200 },
					{400,0,200,200},
					{600,0,200,200}
				};
				//�`������炵�āA���g������Ă���悤��
				ML::Box2D draw_ = draw;
				int dif = 25;
				switch (this->MoveGravity) {					
				case 0: //up
					draw_.y -= dif;
					break;
				case 1: //down
					draw_.y += dif;
					break;
				case 2: //left
					draw_.x -= dif;
					break;
				case 3: //right
					draw_.x += dif;
					break;
				default:
					break;
				}

				this->res->break_img->Draw(draw_, anim[animCnt%4]);
			}
		}
		
	}
	//-----------------------------------------------------------------------------
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	void  Object::Received(BChara*  from_)
	{
		this->isDead = true;
		ge->isDead = true;
	}
	//------------------------------------------------------------------
	bool Object::Attack_Std(const string& GName)
	{
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);

		auto targets = ge->GetTask_Group_G<BChara>(GName);
		for (auto it = targets->begin();
			it != targets->end();
			++it)
		{
			//����ɐڐG�̗L�����m�F������
			if ((*it)->CheckHit(me) && this->serial != (*it)->serial)
			{
				//����Ƀ_���[�W�̏������s�킹��
				(*it)->Received(this);
				this->isDead = true;
				ge->isDead = true;
				//this->Kill();
				return true;
			}

		}
		return false;
	}
	//-----------------------------------------------------------------------------
	//�ڐG����
	bool  Object::CheckHit(const  ML::Box2D&  hit_)
	{
		ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
		return  me.Hit(hit_);
	}
	//-----------------------------------------------------------------------------
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
	Object::Object() { }
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


