#include "aiPoliceForce.h"
#include <modules\vehicle\car.h>

using namespace MM2;

int aiPoliceForce::s_MaxCops = 3;

int aiPoliceForce::GetNumChasers(vehCar* perp) const
{
    for (int i = 0; i < NUM_TARGETS; i++)
    {
        if (PerpCars[i] == perp)
        {
            return this->NumChasers[i];
        }
    }
    return 0;
}

vehCar* aiPoliceForce::GetPerp(int perpIndex) const
{
    if (perpIndex < 0 || perpIndex >= NumPerps)
        return nullptr;
    return PerpCars[perpIndex];
}

vehCar* aiPoliceForce::GetChaser(vehCar* perp, int chaserIndex) const
{
    if (chaserIndex < 0 || chaserIndex >= NUM_COPS)
        return nullptr;

    for (int i = 0; i < NUM_TARGETS; i++)
    {
        if (PerpCars[i] == perp)
        {
            if (chaserIndex < NumChasers[i])
                return this->CopCars[i][chaserIndex];
            else
                return nullptr;
        }
    }
    return nullptr;
}

bool MM2::aiPoliceForce::IsCopChasingPerp(vehCar* cop, vehCar* perp) const
{
    int chaserCount = this->GetNumChasers(perp);
    for (int i = 0; i < chaserCount; i++)
    {
        auto chaser = this->GetChaser(perp, i);
        if (chaser == cop) 
        {
            return true;
        }
    }
    return false;
}

int aiPoliceForce::GetNumPerps() const
{
    return this->NumPerps;
}

BOOL aiPoliceForce::UnRegisterCop(vehCar* cop, vehCar* perp)
{
    // Locate indices of cop and perp
    int perpIndex = -1;
    int copIndex = -1;
    for (int i = 0; i < NUM_TARGETS; i++)
    {
        if (PerpCars[i] == perp)
        {
            perpIndex = i;
            for (int j = 0; j < NumChasers[i]; j++)
            {
                if (CopCars[i][j] == cop)
                {
                    copIndex = j;
                    break;
                }
            }
            break;
        }
    }

    if (copIndex < 0 || perpIndex < 0)
    {
        return FALSE; // Could not find one or the other
    }

    NumChasers[perpIndex]--;
    if (NumChasers[perpIndex] == 0)
    {
        // shift everything down from next perp
        NumPerps--;
        for (int i = perpIndex; i < NumPerps - 1; i++)
        {
            NumChasers[i] = NumChasers[i + 1];
            PerpCars[i] = PerpCars[i + 1];
            for (int j = 0; j < NumChasers[i + 1]; j++)
            {
                CopCars[i][j] = CopCars[i + 1][j];
            }
        }
    }
    else
    {
        // shift cop cars down
        for (int i = copIndex; i < NumChasers[perpIndex] - 1; i++)
        {
            CopCars[perpIndex][i] = CopCars[perpIndex][i + 1];
        }
    }
    return TRUE;
}

BOOL aiPoliceForce::RegisterPerp(vehCar* cop, vehCar* perp)
{
    // check if perp is already being chased
    int perpIndex = -1;
    for (int i = 0; i < this->NumPerps; i++)
    {
        if (PerpCars[i] == perp)
        {
            perpIndex = i;
            break;
        }
    }

    // if not, attempt to register a new one
    if (perpIndex < 0 && NumPerps < NUM_TARGETS)
    {
        perpIndex = NumPerps;
        PerpCars[perpIndex] = perp;
        NumPerps++;
    }

    // failed to find/ no available cop/perp slots / reached soft limit for cops
    if (perpIndex < 0 || NumChasers[perpIndex] >= NUM_COPS || NumChasers[perpIndex] >= s_MaxCops)
    {
        return FALSE;
    }

    // check if this cop is already chasing this perp
    for (int i = 0; i < NumChasers[perpIndex]; i++)
    {
        if (CopCars[perpIndex][i] == cop)
        {
            return TRUE;
        }
    }

    CopCars[perpIndex][NumChasers[perpIndex]] = cop;
    NumChasers[perpIndex]++;
    timer.TickCount = Timer::Ticks();
    return TRUE;
}

int aiPoliceForce::Find(vehCar* cop, vehCar* perp)
{
    for (int i = 0; i < NUM_TARGETS; i++)
    {
        if (PerpCars[i] == perp)
        {
            for (int j = 0; j < NumChasers[i]; j++)
            {
                if (CopCars[i][j] == cop)
                {
                    return j;
                }
            }
            break;
        }
    }
    return -1;
}

int aiPoliceForce::State(vehCar* cop, vehCar* perp, float distance)
{
    // (these probably correspond to aiPoliceState enum values)
    // check if perp is acutally being chased
    int perpIndex = -1;
    for (int i = 0; i < this->NumPerps; i++)
    {
        if (PerpCars[i] == perp)
        {
            perpIndex = i;
            break;
        }
    }

    if (perpIndex < 0)
    {
        return 5;
    }

    // perp is being chased, continue
    Vector3 perpPos = perp->GetICS()->GetPosition();
    float closestDistance = 1000000000.0f;
    int closestCopIndex = 0;

    for (int i = 0; i < NumChasers[perpIndex]; i++)
    {
        auto copCar = CopCars[perpIndex][i];
        Vector3 copPos = copCar->GetICS()->GetPosition();

        float distanceSquared = (copPos - perpPos).Mag2();
        if (distanceSquared < closestDistance)
        {
            closestDistance = distanceSquared;
            closestCopIndex = i;
        }
    }

    if (CopCars[perpIndex][closestCopIndex] == cop && distance < 25.0f)
        return 1;
    else
        return 2;
}

void aiPoliceForce::Reset() {
    timer.TickCount = Timer::Ticks();
    NumPerps = 0;

    for (int i = 0; i < NUM_TARGETS; i++)
    {
        NumChasers[i] = 0;
        PerpCars[i] = nullptr;
        for (int j = 0; j < NUM_COPS; j++)
        {
            CopCars[i][j] = nullptr;
        }
    }
}

void aiPoliceForce::BindLua(LuaState L) {
    LuaBinding(L).beginClass<aiPoliceForce>("aiPoliceForce")
        .addPropertyReadOnly("NumPerps", &GetNumPerps)
        .addFunction("GetNumChasers", &GetNumChasers)
        .addFunction("GetPerp", &GetPerp)
        .addFunction("GetChaser", &GetChaser)
        .addFunction("IsCopChasingPerp", &IsCopChasingPerp)
        .addFunction("UnRegisterCop", &UnRegisterCop)
        .addFunction("RegisterPerp", &RegisterPerp)
        .addFunction("Find", &Find)
        .addFunction("State", &State)
        .addFunction("Reset", &Reset)
        .endClass();
}