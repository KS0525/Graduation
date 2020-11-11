//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Player.h"
#include  "Task_BackGround.h"
#include  "Task_Enemy.h"
#include  "Task_Block00.h"
#include  "Task_EffectBomb.h"
#include  "Task_EffectHit.h"
#include  "Task_Enemy01.h"
#include  "Task_Map2D.h"
#include  "Task_Block01.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->score = DG::Image::Create("./data/image/font_number.png");
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
		this->count = 0;
		this->bcount = 0;

		ge->camera2D = ML::Box2D(0,0, ge->screen2DWidth, ge->screen2DHeight);

		//�X�R�A������
		ge->score = 0;

	   //���^�X�N�̐���
		BackGround::Object::Create(true);
		auto pl = Player::Object::Create(true);
		pl->pos.x = 480 / 2;
		pl->pos.y = 270 * 2 / 3;
		auto map = Map2D::Object::Create(true);
		map->Load("./data/Map/Map2.txt");
	

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

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto nextTask = Ending::Object::Create(true);

		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto inp = ge->in1->GetState();

		//++count;
		//++bcount;

		//if (count >=120)
		//{
		//	count = 0;
		//	auto ene = Enemy::Object::Create(true);
		//	ene->pos.x = rand() % (ge->screen2DWidth - 100);
		//}

		//if (bcount >= 160) {
		//	bcount = 0;
		//	auto blo = Block01::Object::Create(true);
		//	blo->pos.x = rand() % (ge->screen2DWidth - 100);
		//}
		if (ms.B1.down) {
			
		}
		if (ms.CB.down) {
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

		//�X�R�A�`��
		//char msg[10];
		//sprintf(msg, "%5d", ge->score);
		////SPRINTF�@S�@String�@Print Format�@�t�H�[�}�b�g�ɍ��킹�ďo��
		////�@Int����char�z��ւ̕ϊ�

		//for (int i = 0; i < 5; i++) {
		//	if (msg[i] != ' ') {
		//		ML::Box2D  draw2(-(48 / 2), (-60 / 2), 48, 60);
		//		ML::Box2D  src2((msg[i] - '0') * 19, 32, 19, 32);
		//		draw2.Offset((50 * i) + 500, 50);

		//		this->res->score->Draw(draw2, src2);
		//	}
		//}
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