/*
 * RepeatButton.cpp
 *
 * Created: 2012/05/26 12:35:05
 *  Author: Seiji Ainoguchi
 */ 

#include "PushButton.h"
#include "RepeatButton.h"

using namespace Device;

RepeatButton::RepeatButton(void)
 : _pLatestButton(nullptr)
 , _pPushedButton(nullptr)
 , _repeatPhase(false)
 , _repeatCount(0)
 , _repeatTime(30 + 1)
 , _intervalTime(0 + 1)
{
	
}

void RepeatButton::Add(PushButton& button)
{
	_buttons.Add(&button);
}

void RepeatButton::SetTime(int repeatTime, int intervalTime)
{
	_repeatTime = repeatTime + 1;
	_intervalTime = intervalTime + 1;
}

void RepeatButton::Update(void)
{
	PushButton* pPushedButton = nullptr;
	PushButton* pLatestButton = nullptr;
	for (Container::IndexType i = 0; i < _buttons.GetCount(); ++i)
	{
		PushButton* pButton = _buttons[i];
		pButton->Update();
		if (pButton->GetCurrentState())
		{
			pLatestButton = pButton;
		}
	}
	if (pLatestButton != nullptr)
	{
		if (pLatestButton == _pLatestButton)
		{
			if (_repeatPhase)
			{
				if (--_intervalTimeCount == 0)
				{
					_intervalTimeCount = _intervalTime;
					++_repeatCount;
					pPushedButton = _pLatestButton;
				}
			}
			else
			{
				if (--_repeatTimeCount == 0)
				{
					_intervalTimeCount = 1;
					_repeatPhase = true;
				}
			}
		}
		else
		{
			//push first
			_repeatTimeCount = _repeatTime;
			_repeatPhase = false;
			_repeatCount = 0;
			pPushedButton = pLatestButton;
		}
	}
	else
	{
		_repeatPhase = false;
	}
	_pLatestButton = pLatestButton;
	_pPushedButton = pPushedButton;
}

PushButton* RepeatButton::GetPushed(void) const
{
	return _pPushedButton;
}

int RepeatButton::GetRepeatCount(void) const
{
	return _repeatCount;
}