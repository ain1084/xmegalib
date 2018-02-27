/*
 * RepeatButton.h
 *
 * Created: 2012/05/26 12:29:58
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Collections/Vector.h"

namespace Device
{
	class PushButton;

	class RepeatButton
	{
	public:
		RepeatButton(void);
		void SetTime(int repeat, int interval);
		void Add(PushButton& button);
		PushButton* GetPushed(void) const;
		int GetRepeatCount(void) const;
		void Update(void);
			
	private:
		typedef Collections::Vector<PushButton*> Container;
		Container _buttons;

		PushButton* _pLatestButton;
		PushButton* _pPushedButton;
		bool _repeatPhase;
		int _repeatCount;
		int _repeatTime;
		int _repeatTimeCount;
		int _intervalTime;
		int _intervalTimeCount;
	};
}
