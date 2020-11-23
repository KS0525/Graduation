#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//2�����z��}�b�v
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

namespace  Generator
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("����");	//�O���[�v��
	const  string  defName("����");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		static Object::SP Create(bool flagEnemyGeneretorPushBuck);
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		static WP instance;
		ifstream evFile;

		int genCnt;
		int genSpan;

		static Object::SP Object::Create_Mutex();
		bool Set(const string& fpath_);
		bool ReadLine(string& lineT_);
		bool Execute(string& hs_, string& ds_);

		void Output(stringstream& ds_);
		void Generate();
	};
}