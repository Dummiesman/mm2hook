#include "network.h"
#include "playerdata.h"

using namespace MM2;

mmNetworkPlayerData asNetwork::getPlayerDataLua(int id)
{
    mmNetworkPlayerData data;
    memset(&data, 0x00, sizeof(mmNetworkPlayerData));
    GetPlayerData(id, &data, sizeof(mmNetworkPlayerData));
    return data;
}
