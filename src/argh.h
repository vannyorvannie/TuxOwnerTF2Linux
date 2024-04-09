#include "SDK.h"

class CKillSay : public IGameEventListener2
{
public:

	//void Run(CBaseEntity * pLocal, CUserCmd * pCommand);

	void InitKillSay();

	void FireGameEvent(IGameEvent * event);

	bool wasEventInitialized = false;
    /*
	std::string repeat(int n, const char* str)
	{
		std::ostringstream os;
		for (int i = 0; i < n; i++)
			os << str;
		return os.str();
	};
    */
};

extern CKillSay gKillSay;