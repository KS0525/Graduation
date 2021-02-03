#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

namespace  Game
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{��");	//�O���[�v��
	const  string  defName(		"����");	//�^�X�N��
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
		DG::Image::SP grab_UI_img;
		DG::Image::SP grab_UI_img2;
		DG::Image::SP grab_up_01;
		DG::Image::SP grab_up_02;
		DG::Image::SP grab_under_01;
		DG::Image::SP grab_under_02;
		DG::Image::SP grab_right_01;
		DG::Image::SP grab_right_02;
		DG::Image::SP grab_left_01;
		DG::Image::SP grab_left_02;

		DG::Image::SP grab_horizontal[7];
		DG::Image::SP grab_vertical[7];
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
		int resetCoolCount;
		int bcount;
		int animCnt;
		enum Gravity { up, down, left, right, non };
		Gravity MoveGravity;

		void Grab_Anim();
	};
}