#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmViewMgr;

    // External declarations
	extern class mmHUD;
	extern class mmHudMap;
	extern class mmMirror;

    // Class definitions
	class mmViewMgr : public asNode
	{
	private:
	  mmHUD *HUD;
	  mmHudMap *Hudmap;
	  mmMirror *Mirror;
	public:
		mmHUD* GetHUD() const {
			return this->HUD;
		}

		mmHudMap* GetHudMap() const {
			return this->Hudmap;
		}

		mmMirror* GetMirror() const {
			return this->Mirror;
		}

		static void BindLua(LuaState L) {
			LuaBinding(L).beginExtendClass<mmViewMgr, asNode>("mmViewMgr")
				.addPropertyReadOnly("HUD", &GetHUD)
				.addPropertyReadOnly("Map", &GetHudMap)
				.addPropertyReadOnly("Mirror", &GetMirror)
				.endClass();
		}
	};

}