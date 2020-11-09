//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"

//-----------------------------------------------------------------------------
//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
void  BChara::Received(BChara*  from_, AttackInfo  at_)
{
	ML::MsgBox("Received ��������Ă��܂���");
}
//-----------------------------------------------------------------------------
//�ڐG����
bool  BChara::CheckHit(const  ML::Box2D&  hit_)
{
	ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
	return  me.Hit(hit_);
}
//-----------------------------------------------------------------------------
//���[�V�������X�V(�ύX�Ȃ��̏ꍇ false)
bool BChara::UpdateMotion(Motion nm_)
{
	if (nm_ == this->motion) {
		return false;
	}
	else {
		this->motion = nm_;
		this->moveCnt = 0;
		this->animCnt = 0;
		return true;
	}
}
//------------------------------------------------------------------------------
//�����蔻��
bool BChara::Attack_Std(const string& GName, AttackInfo at_)
{
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	auto targets = ge->GetTask_Group_G<BChara>(GName);
	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//����ɐڐG�̗L�����m�F������
		if ((*it)->CheckHit(me)) {
			//����Ƀ_���[�W�̏������s�킹��
			(*it)->Received(this, at_);
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
bool BChara::Check_Bottom()
{
	return false;
}


