#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

namespace  EffectBomb
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"effect");	//�O���[�v��
	const  string  defName(				"bomb");	//�^�X�N��
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
		DG::Image::SP  img;
		DM::Sound::SP bomSe;
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
		ML::Vec2	pos;		//�L�����N�^�ʒu
		ML::Vec2	moveVec;	//�ړ������x�N�g��
		float		alpha;		//��
		float		sizeW,sizeH;//�T�C�Y
		int			count;		//�����܂ł̃J�E���g
		int			animCnt;	//�A�j���؂�ւ��J�E���g
		int			animWait;	//�A�j���̐؂�ւ��҂�
		int			animMax;	//�A�j���̃R�}��
		int			animNo;		//���̃A�j���R�}
		int			animXCount;	//X�R�}��

		float		scale;		//�{��
	};
}