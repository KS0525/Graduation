//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Player.h"
#include  "Task_BackGround.h"
#include  "Task_Block00.h"
#include  "Task_EffectBomb.h"
#include  "Task_EffectHit.h"
#include  "Task_Map2D.h"
#include  "Task_Block01.h"
#include  "Task_MapGenerator.h"
#include  "Task_MapSelector.h"
#include  "Task_StageSelector.h"
#include  "Task_Block02.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->grab_horizontal[0] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0001.png");
		this->grab_horizontal[1] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0002.png");
		this->grab_horizontal[2] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0003.png");
		this->grab_horizontal[3] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0004.png");
		this->grab_horizontal[4] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0005.png");
		this->grab_horizontal[5] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0006.png");
		this->grab_horizontal[6] = DG::Image::Create("./data/image/effect/gravity/yoko/Gravity_effect_yoko_0007.png");

		this->grab_vertical[0] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0001.png");
		this->grab_vertical[1] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0002.png");
		this->grab_vertical[2] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0003.png");
		this->grab_vertical[3] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0004.png");
		this->grab_vertical[4] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0005.png");
		this->grab_vertical[5] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0006.png");
		this->grab_vertical[6] = DG::Image::Create("./data/image/effect/gravity/tate/Gravity_effect_tate_0007.png");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0;i < 7;++i) {
			this->grab_horizontal[i].reset();
			this->grab_vertical[i].reset();
		}
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
		this->resetCoolCount = 0;
		this->bcount = 0;
		this->animCnt = 0;
		this->render2D_Priority[1] = 0.5f;
		ge->camera2D = ML::Box2D(0,0, ge->screen2DWidth, ge->screen2DHeight);

		//�X�R�A������
		ge->score = 0;
		ge->gameClearFlag = false;
		animFin = false;
		animCnt = 0;
	   //���^�X�N�̐���
		BackGround::Object::Create(true);

		if (auto map = Generator::Object::Create_Mutex()) {
			map->Set(ge->nowStage);
		}
		bgm::LoadFile("bgm", "./data/sound/bgm_maoudamashii_ethnic16.mp3");
		bgm::Play("bgm");
		bgm::VolumeControl("bgm", 70);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�v���C���[");
		ge->KillAll_G("�w�i�摜");
		ge->KillAll_G("�G");
		ge->KillAll_G("�e");
		ge->KillAll_G("�u���b�N");
		ge->KillAll_G("�X�C�b�`");
		ge->KillAll_G("�S�[��");


		bgm::Stop("bgm");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			if (ge->gameClearFlag) {
				auto nextTask = Ending::Object::Create(true);
			}
			else {
				auto nextTask = MapSelector::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto key = ge->in1->GetState();

		//animCnt++;
		//�A�j���[�V��������
		Grab_Anim();

		if (key.LStick.BU.on) { this->MoveGravity = Gravity::up; }
		if (key.LStick.BL.on) { this->MoveGravity = Gravity::left; }
		if (key.LStick.BD.on) { this->MoveGravity = Gravity::down; }
		if (key.LStick.BR.on) { this->MoveGravity = Gravity::right; }


		if (ge->isReady) { //�}�b�v�̐������������Ă��邩
			if (ge->isDead) { //�v���C���͎���ł��邩
				resetCoolCount++;
				if (resetCoolCount > 60) { 
					//�}�b�v�֌W�̃^�X�N�����
					ge->KillAll_G("�v���C���[");
					ge->KillAll_G("�u���b�N");
					ge->KillAll_G("�Œ�u���b�N");
					ge->KillAll_G("�X�C�b�`");
					ge->KillAll_G("�S�[��");

					ge->isDead = false;
					//�N�[���J�E���g��0�ɖ߂�
					resetCoolCount = 0;
					ge->isReady = false;

					//�ăZ�b�g
					if (auto map = Generator::Object::Create_Mutex()) {
						map->Set(ge->nowStage);
					}
				}
			}

			bgm::EndCheck();

			if (key.B2.down) {
				//���g�ɏ��ŗv��
				this->Kill();
			}
			if (ms.CB.down) {
				//���g�ɏ��ŗv��
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1280, 720);
		ML::Box2D src(0, 0, 1280, 720);

		if (!animFin) {
			if (this->MoveGravity == Gravity::up ||
				this->MoveGravity == Gravity::down) {
				this->res->grab_vertical[animCnt / 4 % 7]->Draw(draw, src);
			}
			else if (this->MoveGravity == Gravity::left ||
				this->MoveGravity == Gravity::right) {
				this->res->grab_horizontal[animCnt / 4 % 7]->Draw(draw, src);
			}
			else {

			}
		}
	}
	//-------------------------------------------------------------------
	void Object::Grab_Anim()
	{
		int animMax = 28;
		switch (this->MoveGravity) {
		case 0: //up
			animCnt--;
			animFin = false;
			if (animCnt < 0) { animCnt = 0;  animFin = true; }
			break;
		case 1: // down
			animCnt++;
			animFin = false;
			if (animCnt > animMax) { animCnt = animMax; animFin = true; }
			break;
		case 2: //left	
			animCnt--;
			animFin = false;
			if (animCnt < 0) { animCnt = 0; animFin = true; }
			break;
		case 3: //right
			animCnt++;
			animFin = false;
			if (animCnt > animMax) { animCnt = animMax;  animFin = true; }
			break;
		default:
			break;
		}

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