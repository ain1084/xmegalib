/*
 * IIntervalTimerNotify.h
 *
 * Created: 2012/06/10 3:55:40
 *  Author: Seiji Ainoguchi
 */ 

namespace Device
{
	class IIntervalTimerNotify
	{
	public:
		virtual ~IIntervalTimerNotify(void) { }
		virtual void OnTimer(void) = 0;
	};
}

