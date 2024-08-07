#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmInterface;

    // External declarations
	extern class Vehicle;
	extern class UIMenu;

    // Class definitions
	class mmInterface : public asNode
	{
	private:
		char _buffer[0x7738];
	protected:
		static hook::Field<0x18, UIMenu*> _mainMenu;
		static hook::Field<0x24, Vehicle*> _vehiclesMenu;
		static hook::Field<0x30, UIMenu*> _aboutMenu;
		static hook::Field<0x40, UIMenu*> _graphicsOptionsMenu;

		static mmInterface* Instance; // not in MM2 in actuality but since we're allowing things like custom menus
									  // we'll define this
	public:
		static mmInterface* GetInstance()
		{
			return mmInterface::Instance;
		}

		/// <summary>
		/// INTERNAL: Do not use
		/// </summary>
		static void ClearInstance()
		{
			mmInterface::Instance = nullptr;
		}

		/// <summary>
		/// INTERNAL: Do not use
		/// </summary>
		static void SetInstance(mmInterface* instance)
		{
			mmInterface::Instance = instance;
		}
	public:

        /*
            asNode virtuals
        */

        virtual AGE_API void Update(void) override          { hook::Thunk<0x40A6C0>::Call<void>(this); };
        virtual AGE_API void Reset(void) override           { hook::Thunk<0x40A130>::Call<void>(this); };

		/*
			mmInterface members
		*/
		AGE_API void BeDone()								{ hook::Thunk<0x40A5B0>::Call<void>(this); }
		AGE_API void MultiStartGame()						{ hook::Thunk<0x4107B0>::Call<void>(this); }

		UIMenu* GetMainMenu()								{ return _mainMenu.get(this); }
		Vehicle* GetVehiclesMenu()						    { return _vehiclesMenu.get(this); }
		UIMenu* GetAboutMenu()								{ return _aboutMenu.get(this); }
		UIMenu* GetGraphicsOptionsMenu()					{ return _graphicsOptionsMenu.get(this); }

		static void BindLua(LuaState L) {
			LuaBinding(L).beginExtendClass<mmInterface, asNode>("mmInterface")
				.addPropertyReadOnly("MainMenu", &GetMainMenu)
				.addPropertyReadOnly("VehiclesMenu", &GetVehiclesMenu)
				.addPropertyReadOnly("AboutMenu", &GetAboutMenu)
				.addPropertyReadOnly("GraphicsOptionsMenu", &GetGraphicsOptionsMenu)
				.addFunction("MultiStartGame", &MultiStartGame)
				.addFunction("BeDone", &BeDone)
				.endClass();
		}
	};
	ASSERT_SIZEOF(mmInterface, 0x7750);
}