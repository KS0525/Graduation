#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_82.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2    pos;		//�L�����N�^�ʒu
	ML::Box2D   hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^
	//���E�̌����i2D�����_�Q�[����p�j
	enum Angle_LR { Left, Right };
	Angle_LR	angle_LR;


	float maxFallSpeed;		//�ő嗎�����x


	float gravity;			//�d�͉����x�����ԑ��x�ɂ����Z��
	float gensoku;			//���Ԃɂ�錸����
	int serial;				//�u���b�N�i���o�[
	//�d�͂̕���
	enum Gravity { up, down, left, right };
	Gravity MoveGravity;
	
	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, angle_LR(Right)
		, MoveGravity(down)
		, maxFallSpeed(0)
		,gravity(0)
		,serial(0)
	{
    }
	virtual  ~BChara(){}

	//	�A�j���[�V�������\����
	struct DrawInfo {
		ML::Box2D		draw, src;
		ML::Color		color;
	};
	//  �U�����
	struct  AttackInfo {
		int  power;			//�U���̈З�
		void operator=(int pow) { power = pow; }
										//���̑��K�v�ɉ�����
	};
	//---------------------------------------------------------
	enum class Motion
	{
		Unnon = 01, //����
		Stand, //��~
		Walk, //���s
		Attack, //�U��
		Damage, //����
	};

	Motion motion; //���݂̍s���������t���O
	int animCnt; //�A�j���[�V�����J�E���^

	float maxSpeed; //���E�����ւ̈ړ��̍ő��
	float addSpeed; //���E�����ւ̈ړ��̉��Z��
	//---------------------------------------------------------

	//�L�����N�^���ʃ��\�b�h
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	virtual  void  Received(BChara*  from_);
	//�ڐG����
	virtual  bool  CheckHit(const  ML::Box2D&  hit);
	//���[�V�������X�V(�ύX�Ȃ��̏ꍇ false)
	bool UpdateMotion(Motion nm_);

	void GravityMotion(const string& GName);

	//�����蔻��
	virtual bool Attack_Std(const string& GName);
	virtual bool Check_Bottom();
	//virtual bool Check_Head(const ML::Box2D& hit_);
};
