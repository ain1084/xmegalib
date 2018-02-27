/*
 * OutputPin.h
 *
 * Created: 2011/11/28 1:21:12
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>
#include "Device/IOutputPin.h"
#include "GPIO.h"

namespace XMEGA
{
	///1bit 出力ピンのクラス
	class OutputPin : public Device::IOutputPin
	{
	public:
		///OutputPin のコンストラクタ
		///
		///指定された引数で OutputPin を構築する。
		///port と pinNumber の組み合わせでピンを指定する。
		///たとえば PA7 ならば port = PORTA, pinNumber = 7 とする。
		///initialStateIsHigh でピンの初期状態を指定できる。デフォルトは High レベルとなる。
		///
		///@param[in] port ポート(PORTA, PORTB, PORTC 等)
		///@param[in] pinNumber port 内のピン番号(0 - 7)
		///@param[in] initialStateIsHigh ピンの初期状態。true ならば High レベル。
		OutputPin(volatile PORT_t& port, uint8_t pinNumber, bool initialStateIsHigh = true);
	
		///ピンの状態を Low にする
		virtual void Low(void)
		{
			_gpio.SetLow();
		}

		///ピンの状態を High にする
		virtual void High(void)
		{
			_gpio.SetHigh();
		}

		virtual IOutputPin& operator=(bool isHigh)
		{
			if (isHigh)
			{
				High();
			}
			else
			{
				Low();
			}
			return *this;
		}

	private:
		GPIO _gpio;
	};
}
