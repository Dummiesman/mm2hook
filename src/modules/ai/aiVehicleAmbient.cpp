#pragma once
#include "aiVehicleAmbient.h"
#include <modules\rgl.h>

using namespace MM2;

void aiVehicleAmbient::DrawBBox(bool selected)
{
    auto matrix = this->GetMatrix();
    Vector3 frontleft = Vector3(-LSideDistance(), 1.0f, -FrontBumperDistance());
    Vector3 frontright = Vector3(RSideDistance(), 1.0f, -FrontBumperDistance());
    Vector3 backleft = Vector3(-LSideDistance(), 1.0f, BackBumperDistance());
    Vector3 backright = Vector3(RSideDistance(), 1.0f, BackBumperDistance());

    if (selected)
        vglCurrentColor = mkfrgba(0.0f, 0.0f, 1.0f, 1.0f);
    else
        vglCurrentColor = mkfrgba(1.0f, 0.0f, 0.0f, 1.0f);

    vglBegin(gfxDrawMode::DRAWMODE_LINESTRIP, 5);
    vglVertex3f(matrix.Transform(frontleft));
    vglVertex3f(matrix.Transform(frontright));
    vglVertex3f(matrix.Transform(backright));
    vglVertex3f(matrix.Transform(backleft));
    vglVertex3f(matrix.Transform(frontleft));
    vglEnd();
}