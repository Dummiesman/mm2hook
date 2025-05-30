#include "mm2_vehicle.h"
using namespace MM2;

/*
    vehPoliceCarAudio
*/
declfield(vehPoliceCarAudio::iNumCopsPursuingPlayer)(0x6AFFE4);

/*
    vehCarModel
*/
bool vehCarModel::EnableSpinningWheels = true;
bool vehCarModel::EnableHeadlightFlashing = true;
bool vehCarModel::MWStyleTotaledCar = false;
bool vehCarModel::PartReflections = false;
bool vehCarModel::WheelReflections = false;
bool vehCarModel::DamageReflections = false;
int vehCarModel::SirenType = 0;
int vehCarModel::HeadlightType = 0;
float vehCarModel::SirenCycle = 0.25f;
float vehCarModel::HeadlightFlashingSpeed = 42.411503f;

bool vehCarModel::ShowHeadlights = false;
bool vehCarModel::HazardLightsState = false;
bool vehCarModel::LeftSignalLightState = false;
bool vehCarModel::RightSignalLightState = false;

bool vehCarModel::mm1StyleTransmission = false;
bool vehCarModel::mm1StyleDamage = true;
bool vehCarModel::breakableRenderTweak = false;

bool vehCarModel::Enable3DDamage = true;
bool vehCarModel::Enable3DDynamicDamage = false;
bool vehCarModel::Enable3DShadows = true;

/*
   vehBreakableMgr
*/
bool vehBreakableMgr::EnableReflections = false;

/*
    vehSiren
*/
float vehSiren::SirenRotationSpeed = 3.1415927f;