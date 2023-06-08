#include "mm2_vehicle.h"
using namespace MM2;

/*
    vehPoliceCarAudio
*/
declfield(vehPoliceCarAudio::iNumCopsPursuingPlayer)(0x6AFFE4);

/*
    vehCarDamage
*/
declfield(vehCarDamage::EngineSmokeRule)(0x6AFED4);

/*
    vehCarModel
*/
bool vehCarModel::EnableSpinningWheels = true;
bool vehCarModel::EnableHeadlightFlashing = true;
bool vehCarModel::MWStyleTotaledCar = false;
bool vehCarModel::PartReflections = false;
bool vehCarModel::WheelReflections = false;
int vehCarModel::SirenType = 0;
int vehCarModel::HeadlightType = 0;
float vehCarModel::SirenCycle = 0.25f;
float vehCarModel::HeadlightFlashingSpeed = 42.411503f;

bool vehCarModel::ShowHeadlights = false;
bool vehCarModel::HazardLightsState = false;
bool vehCarModel::LeftSignalLightState = false;
bool vehCarModel::RightSignalLightState = false;

bool vehCarModel::mm1StyleTransmission = false;
bool vehCarModel::breakableRenderTweak = false;

/*
   vehBreakableMgr
*/
bool vehBreakableMgr::EnableReflections = false;

/*
    vehSiren
*/
float vehSiren::SirenRotationSpeed = 3.1415927f;