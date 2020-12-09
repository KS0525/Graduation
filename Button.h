#pragma once
#include "BChara.h"

class Button
{
	ML::Box2D hitBox;
	int buttonNum;
	//static int allNum;
public:
	Button() {
		//allNum++;
		//buttonNum = allNum;
	}
	ML::Box2D get_hitBox() const { return hitBox; }
	void set_hitBox(const ML::Box2D inBox) { hitBox = inBox; }
	int get_buttonNum() const { return buttonNum; }
	void set_buttonNum(const int inNum) { buttonNum = inNum; }
	//static void set_allNum(int inNum) { allNum = inNum; }
};