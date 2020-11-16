//-------------------------------------------------------------------
//�e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Block00.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "Task_EffectHit.h"
#include  "Task_EffectBomb.h"



namespace  Block00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/map/CastleTown-B.png");
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

		this->maxFallSpeedDown = 10.0f;	//�ő嗎�����x
		this->maxFallSpeedUp = -10.0f;
		this->maxFallSpeedLeft = -10.0f;
		this->maxFallSpeedRight = 10.0f;

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

		//�d�͕ύX
		if (key.B1.on) { this->MoveGravity = Gravity::up; }
		if (key.B2.on) { this->MoveGravity = Gravity::left; }
		if (key.B3.on) { this->MoveGravity = Gravity::down; }
		if (key.B4.on) { this->MoveGravity = Gravity::right; }

		this->GravityMotion();

		this->pos += this->moveVec;

		//��ʊO�֏o�Ȃ��悤��
		if (this->pos.x < 0) { pos.x = 0; }
		if (this->pos.y < 0) { pos.y = 0; }
		if (this->pos.x > ge->screen2DWidth - this->hitBase.w) { pos.x = ge->screen2DWidth - this->hitBase.w; }
		if (this->pos.y > ge->screen2DHeight - this->hitBase.h) { pos.y = ge->screen2DHeight - this->hitBase.h; }
		
		//�G�Ƃ̓����蔻��
		if (this->Attack_Std("�v���C���[", atk)) { //���ʉ��ɂ��
			//�ڐG���Ă����ꍇ�A�����ɑ΂��ĉ����������Ȃ�
			
		   //this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = hitBase;
		ML::Box2D src[3] = { ML::Box2D(321,64,30,32),
							ML::Box2D(289,64,30,32),
							ML::Box2D(257, 64, 30, 32)						
							};
		draw.Offset(this->pos);

		res->img->Draw(draw, src[this->hp-1]);
	}
	//------------------------------------------------------------------
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	void  Object::Received(BChara*  from_, AttackInfo  at_)
	{
		this->hp -= at_.power;

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
	bool Object::Attack_Std(const string& GName,AttackInfo at_)
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
			(*it)->Received(this, at_);
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



class ChipInfo
{
private:
	ChipInfo()
	{
		num = chip;
		chip++;
		mChips.push_back(this);
	}
	static int chip;
	int num;

	string srcName;

	std::vector<ChipInfo*> mChips;
	void Create()
	{
		for (auto chips : mChips)
		{
			auto bl = Block00::Object::Create(true);
			//���Ƃ̓`�b�v����n��

		}
	}
public:
	enum class Chiptype {
		broken, //�󂹂�I�u�W�F�N�g
		hardbroken, //�󂵂ɂ����I�u�W�F�N�g
		Unbroken //�󂹂Ȃ��I�u�W�F�N�g
	};

	ML::Box2D src;
};