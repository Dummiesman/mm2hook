#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmWaypoints;

    // External declarations

    // Class definitions
	class mmWaypoints : public asNode
	{
	private:
		char _buffer[0x78];
	private:
		Vector3 getWaypointLua(int index)
		{
			Vector3 out;
			GetWaypoint(index, out);
			return out;
		}

		Vector3 getStartLua()
		{
			Vector3 out;
			GetStart(out);
			return out;
		}
	protected:
		static hook::Field<0x2C, int> _currentGoal;
		static hook::Field<0x30, int> _waypointCount;
		static hook::Field<0x3C, int> _lapCount;
		static hook::Field<0x40, int> _currentLap;
		static hook::Field<0x50, int> _numCleared;
	public:
        /*
            asNode virtuals
        */

		virtual AGE_API void Cull(void) override					{ hook::Thunk<0x435240>::Call<void>(this); };
        virtual AGE_API void Update(void) override					{ hook::Thunk<0x435280>::Call<void>(this); };
        virtual AGE_API void Reset(void) override					{ hook::Thunk<0x4357E0>::Call<void>(this); };

		/*
			mmWaypoints
		*/

		AGE_API void DeactivateFinish()								{ hook::Thunk<0x4357D0>::Call<void>(this); }
		AGE_API void GetWaypoint(int index, Vector3 & out) const	{ hook::Thunk<0x435930>::Call<void>(this, index, &out); }
		AGE_API float GetHeading(int index) const					{ return hook::Thunk<0x435970>::Call<float>(this, index); }
		AGE_API void GetStart(Vector3 & out) const					{ hook::Thunk<0x4358F0>::Call<void>(this, &out); }
		AGE_API float GetStartAngle() const							{ return hook::Thunk<0x434960>::Call<float>(this); }
		AGE_API int GetHitRoom(int index) const						{ return hook::Thunk<0x435A00>::Call<int>(this, index); }
		AGE_API float GetCurrentDistance() const					{ return hook::Thunk<0x435A50>::Call<float>(this); }

		int GetWaypointCount() const 
		{
			return _waypointCount.get(this);
		}

		int GetCurrentGoal() const
		{
			return _currentGoal.get(this);
		}

		int GetClearedWaypointCount() const
		{
			return _numCleared.get(this);
		}

		int GetCurrentLap() const
		{
			// lap count starts at zero and ends at GetLapCount() only when the race is finished
			// which is a bit unintuitive so we do it like aiVehiclePhysics instead
			int lapCount = _currentLap.get(this);
			return min(GetLapCount(), lapCount + 1);
		}

		int GetLapCount() const 
		{
			return _lapCount.get(this);
		}
		
		static void BindLua(LuaState L) {
			LuaBinding(L).beginExtendClass<mmWaypoints, asNode>("mmWaypoints")
				.addPropertyReadOnly("NumWaypoints", &GetWaypointCount)
				.addPropertyReadOnly("NumCleared", &GetClearedWaypointCount)
				.addPropertyReadOnly("CurrentGoal", &GetCurrentGoal)
				.addPropertyReadOnly("CurrentLap", &GetCurrentLap)
				.addPropertyReadOnly("NumLaps", &GetLapCount)
				.addFunction("DeactivateFinish", &DeactivateFinish)
				.addFunction("GetWaypoint", &getWaypointLua)
				.addFunction("GetHeading", &GetHeading)
				.addFunction("GetStart", &getStartLua)
				.addFunction("GetStartAngle", &GetStartAngle)
				.addFunction("GetHitRoom", &GetHitRoom)
				.addFunction("GetCurrentDistance", &GetCurrentDistance)
				.endClass();
		}
	};
	ASSERT_SIZEOF(mmWaypoints, 0x90);

}