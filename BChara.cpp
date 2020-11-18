//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"

//-----------------------------------------------------------------------------
//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
void  BChara::Received(BChara*  from_)
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
bool BChara::Attack_Std(const string& GName)
{
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	auto targets = ge->GetTask_Group_G<BChara>(GName);
	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//����ɐڐG�̗L�����m�F������
		if ((*it)->CheckHit(me)) {
			//����Ƀ_���[�W�̏������s�킹��
			(*it)->Received(this);
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
//�d�͈ړ�
void BChara::GravityMotion(const string& GName)
{
	//�d�͂ɂ�闎��
	if (this->MoveGravity == Gravity::up)
	{
		this->moveVec.y = max(this->moveVec.y - this->gravity, -this->maxFallSpeed);
	}
	else if (this->MoveGravity == Gravity::down)
	{
		this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);
	}
	else if (this->MoveGravity == Gravity::left)
	{
		this->moveVec.x = max(this->moveVec.x - this->gravity, -this->maxFallSpeed);
	}
	else if (this->MoveGravity == Gravity::right)
	{
		this->moveVec.x = min(this->moveVec.x + this->gravity, this->maxFallSpeed);
	}

	//X�ړ�
	float moveX = this->moveVec.x;
	while (moveX > 1);
	{
		float savePosX = this->pos.x;
		this->pos.x++;
		moveX--;
		if (this->Attack_Std(GName))
		{
			this->pos.x = savePosX;
			moveX = 0;
		}
	}
	while (moveX < 1);
	{
		float savePosX = this->pos.x;
		this->pos.x--;
		moveX++;
		if (this->Attack_Std(GName))
		{
			moveX = 0;
			this->pos.x = savePosX;
		}
	}
	if (moveX != 0) { this->pos.x += moveX; }

	//Y�ړ�
	float moveY = this->moveVec.y;
	while (moveY > 1);
	{
		float savePosY = this->pos.y;
		this->pos.y++;
		moveY--;
		if (this->Attack_Std(GName))
		{
			this->pos.y = savePosY;
			moveY = 0;
		}
	}
	while (moveY < 1);
	{
		float savePosY = this->pos.y;
		this->pos.y--;
		moveY++;
		if (this->Attack_Std(GName))
		{
			moveY = 0;
			this->pos.y = savePosY;
		}
	}
	if (moveY != 0) { this->pos.y += moveY; }

	//����
	if (this->MoveGravity == Gravity::up || this->MoveGravity == Gravity::down)
	{
		if (this->moveVec.x < 0.0f)
		{
			this->moveVec.x += this->gensoku;
			if (this->moveVec.x > 0.0f) { this->moveVec.x = 0.0f; }
		}
		else if (this->moveVec.x > 0.0f)
		{
			this->moveVec.x -= this->gensoku;
			if (this->moveVec.x < 0.0f) { this->moveVec.x = 0.0f; }
		}
	}
	if (this->MoveGravity == Gravity::left || this->MoveGravity == Gravity::right)
	{
		if (this->moveVec.y < 0.0f)
		{
			this->moveVec.y += this->gensoku;
			if (this->moveVec.y > 0.2f) { this->moveVec.y = 0.0f; }
		}
		else if (this->moveVec.y > 0.0f)
		{
			this->moveVec.y -= this->gensoku;
			if (this->moveVec.y < 0.2f) { this->moveVec.y = 0.0f; }
		}
	}
}
//------------------------------------------------------------------------------
bool BChara::Check_Bottom()
{
	return false;
}


