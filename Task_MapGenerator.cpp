//-------------------------------------------------------------------
//2�����z��}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapGenerator.h"
#include  "Task_Enemy.h"
#include  "Task_Enemy01.h"
#include  "Task_Player.h"
#include  "Task_Block00.h"
#include  "Task_Block01.h"
#include  "Task_Block02.h"
#include  "Task_Block03.h"
#include  "Task_Block04.h"
#include  "Task_Block05.h"
#include  "Task_Block06.h"
#include  "Task_Block07.h"
#include  "Task_Block08.h"
#include  "Task_Block09.h"
#include  "Task_Block10.h"
#include  "Task_Switch.h"
#include  "Task_Goal.h"
namespace  Generator
{
	Object::WP Object::instance;
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
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
		this->res = Resource::Create();
		this->genCnt = 0;
		this->genSpan = 1;

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
		this->genCnt++;

		if (!this->evFile) { this->Kill(); } //�t�@�C���������Ȃ����

		Generate();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}
	//-------------------------------------------------------------------
	bool Object::ReadLine(string& lineT_)
	{
		//1�s�ǂݍ���
		bool rtv = false;
		while (getline(this->evFile, lineT_)) {
			//�������`�F�b�N���s��
			//�s���ɋ�(�S�p��)�ȊO�̕������Ȃ�
			if (string::npos == lineT_.find_first_not_of("�@")) { continue; }
			//�ŏ��̕�����/�̏ꍇ�R�����g�Ƃ݂Ȃ�
			if ('/' == lineT_.at(0)) { continue; }
			//�w�b�_���ƃf�[�^�̋��ڂł���>���܂܂�Ă��Ȃ�
			if (string::npos == lineT_.find(">")) { continue; }
			//�������`�F�b�N
			rtv = true; //�L���ȍs�Ƃ݂Ȃ�
			break;
		}
		return rtv;
	}
	//-------------------------------------------------------------------
	bool Object::Execute(string& hs_, string& ds_)
	{
		//�f���~�^(��؂�L��)�폜(���܂������@�`�F�b�N�͂��ĂȂ�)
		string::size_type t;
		while ((t = ds_.find_first_of("(,);")) != string::npos)
		{
			ds_[t] = ' ';
		}
		//
		stringstream ss;
		ss << ds_;
		//�C�x���g�I��
		if (hs_ == "end") {
			
			this->Kill();
		}
		//�G�̐���
		else if (hs_ == "crt") {
			Output(ss);
		}
		else {
			return false;
		}
		return true;
	}

	//-------------------------------------------------------------------
	bool Object::Set(const string& fpath_)
	{
		//���������̃t�@�C�������
		if (this->evFile.is_open()) {
			this->evFile.close();
		}

		//�C�x���g�t�@�C���ǂݍ���
		this->evFile.open(fpath_);
		//�t�@�C�����J���Ȃ�
		if (!this->evFile) {
			return false;
		}
		return true;

	}
	//-------------------------------------------------------------------
	Object::SP Object::Create_Mutex()
	{
		//�V�K�쐬���X�V���̔���
		if (auto p = instance.lock()) {
			return nullptr;
		}
		else {
			p = Object::Create(true);
			instance = p;
			return p;
		}
	}
	//-------------------------------------------------------------------
	void Object::Output(stringstream& ds_)
	{
		ML::Box2D pos;
		string kind;
		ds_ >> kind >> pos.x >> pos.y >> pos.w >> pos.h;
		
		if (kind == "player") {
			auto pl = Player::Object::Create(true);
			pl->pos = ML::Vec2(pos.x,pos.y);
			pl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block00") {
			auto bl = Block00::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block01") {
			auto bl = Block01::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block02") {
			auto bl = Block02::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block03") {
			auto bl = Block03::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block04") {
			auto bl = Block04::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block05") {
			auto bl = Block05::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block06") {
			auto bl = Block06::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block07") {
			auto bl = Block07::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block08") {
			auto bl = Block08::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block09") {
			auto bl = Block09::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Block10") {
			auto bl = Block10::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Switch") {
			auto sw = Switch::Object::Create(true);
			sw->pos = ML::Vec2(pos.x, pos.y);
			sw->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
		if (kind == "Goal") {
			auto bl = Goal::Object::Create(true);
			bl->pos = ML::Vec2(pos.x, pos.y);
			bl->hitBase = ML::Box2D(0, 0, pos.w, pos.h);
		}
	}
	//-------------------------------------------------------------------
	void Object::Generate()
	{
		if (genCnt % genSpan == 0) {
			//�V�����s��ǂݍ���
			string lineText;
			string headerText;
			string dataText;
			if (this->ReadLine(lineText)) {
				//�s�����
				string::size_type t = lineText.find(">");
				headerText = lineText.substr(0, t);
				dataText = lineText.substr(t + 1);
				//�w�b�_���ɑΉ������C�x���g�������s��
				bool rtv = this->Execute(headerText, dataText);
				//�C�x���g�����Ɏ��s�������͎��g�̏�Ԃ��A�N�e�B�u�łȂ��Ȃ��Ă����ꍇ���[�v�𔲂���
				if (false == rtv ||
					BTask::State::Active != this->CheckState()) {

				}
			}

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
	//-------------------------------------------------------------------


}

