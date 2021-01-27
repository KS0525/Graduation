#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"
#include "Button.h"

namespace  StageSelector
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�X�e�[�W�Z���N�^");	//�O���[�v��
	const  string  defName("NoName");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP  eggCapsule;
		DG::Image::SP nonClearEgg[6];
		DG::Image::SP clearEgg[6];
		DG::Image::SP selectEgg[6];
	
		DG::Image::SP  choosingimg;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		int choosing;
		int choiceMax_;
		void Carsol();
		bool back;
		
		int animCnt;
		int mapNumber;

		class StageButton : public Button {
			DG::Image::SP clearEgg;
			DG::Image::SP nonClearEgg;
			string stagePass;
		public:


			StageButton(string inPass) :Button() {
				stagePass = inPass;
			};
			StageButton() :Button() { isClear = false; };

			string get_StagePass() const  { return stagePass; }
			void set_StagePass(const string pass) { stagePass = pass; }

			bool isClear;
		};

		StageButton buttons[6];
	};
}

