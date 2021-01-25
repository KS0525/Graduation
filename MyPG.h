#pragma once
#include "GameEngine_Ver3_82.h"
#include <fstream>
#include <sstream>

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------

#include "sound.h"
#include "fpscounter.h"
#include "BChara.h"
#include "Task_Map2D.h"
#include "Task_EffectCreate.h"
//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
	//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	�J�����𐶐�����
		static SP Create(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
	//	�J�����̐ݒ�
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_);
		~MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate( );

	//3DPG1�Ή��ɂ��ǉ�
		//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);

		//�J�����̌X�����x���ɑ΂���p�x�Ŏ擾����
		//X���{������0�x�Ƃ����J�����̌�����Ԃ��B
		float GetCameraDirectionY_XPlus0(UINT cn_);

	//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
	//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	�J����
		
		int						score;
		bool					gameClearFlag;
		bool                    isReady;        //�}�b�v�������I�������
		int						serial;			//�u���b�N�i���o�[

		XI::Mouse::SP  mouse;
		XI::GamePad::SP  in1, in2, in3, in4;	//��荇�����S�{
																					//�Q�c�J������`
		ML::Box2D				camera2D;	//  �Q�c�X�N���[������p
		string nowStage;

		//�G���o���������Ȃ����邽�߂�
		shared_ptr< vector< BChara::SP > >    qa_Enemys;
		EffectCreate::Object::SP effectCreator;

		Map2D::Object::SP               qa_Map;
		//----------------------------------------------
		void Dbg_ToConsole(const char *str, ...);
		void Dbg_ToDisplay(int x, int y, const char * str, ...);
		void Dbg_ToTitle(const char * str, ...);
		void Dbg_ToBox(const char *str, ...);
		void Dbg_ToFileOut(const char *str, ...);

        FPSCounter *c;
	};
}

extern MyPG::MyGameEngine* ge;


