//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"

//-----------------------------------------------------------------------------
//接触時の応答処理（これ自体はダミーのようなモノ）
void  BChara::Received(BChara*  from_, AttackInfo  at_)
{
	ML::MsgBox("Received 実装されていません");
}
//-----------------------------------------------------------------------------
//接触判定
bool  BChara::CheckHit(const  ML::Box2D&  hit_)
{
	ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
	return  me.Hit(hit_);
}
//-----------------------------------------------------------------------------
//モーションを更新(変更なしの場合 false)
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
//当たり判定
bool BChara::Attack_Std(const string& GName, AttackInfo at_)
{
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	auto targets = ge->GetTask_Group_G<BChara>(GName);
	for (auto it = targets->begin();
		it != targets->end();
		++it) {
		//相手に接触の有無を確認させる
		if ((*it)->CheckHit(me)) {
			//相手にダメージの処理を行わせる
			(*it)->Received(this, at_);
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
//重力移動
void BChara::GravityMotion()
{
	//重力による落下
	if (this->MoveGravity == Gravity::up)
	{
		this->moveVec.y = max(this->moveVec.y - this->gravity, this->maxFallSpeedUp);
	}
	else if (this->MoveGravity == Gravity::down)
	{
		this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeedDown);
	}
	else if (this->MoveGravity == Gravity::left)
	{
		this->moveVec.x = max(this->moveVec.x - this->gravity, this->maxFallSpeedLeft);
	}
	else if (this->MoveGravity == Gravity::right)
	{
		this->moveVec.x = min(this->moveVec.x + this->gravity, this->maxFallSpeedRight);
	}

	if (this->MoveGravity == Gravity::up || this->MoveGravity == Gravity::down)
	{
		if (this->moveVec.x < 0.0f)
		{
			this->moveVec.x += 0.2f;
			if (this->moveVec.x > 0.0f) { this->moveVec.x = 0.0f; }
		}
		else if (this->moveVec.x > 0.0f)
		{
			this->moveVec.x -= 0.2f;
			if (this->moveVec.x < 0.0f) { this->moveVec.x = 0.0f; }
		}
	}
	if (this->MoveGravity == Gravity::left || this->MoveGravity == Gravity::right)
	{
		if (this->moveVec.y < 0.0f)
		{
			this->moveVec.y += 0.2f;
			if (this->moveVec.y > 0.2f) { this->moveVec.y = 0.0f; }
		}
		else if (this->moveVec.y > 0.0f)
		{
			this->moveVec.y -= 0.2f;
			if (this->moveVec.y < 0.2f) { this->moveVec.y = 0.0f; }
		}
	}
}
//------------------------------------------------------------------------------
bool BChara::Check_Bottom()
{
	return false;
}


