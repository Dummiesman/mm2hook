#pragma once
#include "aiGoalAvoidPlayer.h"
#include <modules/ai/aiMap.h>
#include <modules/vehicle/car.h>
#include <modules/rgl.h>

using namespace MM2;

/*
void MM2::aiGoalAvoidPlayer::AvoidPlayer()
{
    // Reference code from the vanilla game
    // The xDot > 0 check was flipped
    // and it seems like they were working on some unfinished tweaks to the code

    auto aimap = aiMap::GetInstance();
    auto player = aimap->Player(0);

    auto playerMtx = player->GetMatrix();
    auto ourMtx = Vehicle->GetMatrix();

    Vector3 playerPosition = playerMtx.GetRow(3);
    Vector3 ourPosition = ourMtx.GetRow(3);

    Vector3 positionDifference = playerPosition - ourPosition;

    float xDot = positionDifference.Dot(ourMtx.GetRow(0));
    float zDot = positionDifference.Dot(ourMtx.GetRow(2) * -1.0f);

    auto railSet = Vehicle->getRailSet();
    float chance = frand();

    float rotationAmount = 0.0f;

    if ((railSet->GetCurrentLink()->GetFlags() & 1) != 0 && railSet->GetCurrentLane() == 0)
    {
        // weird special freeway case
        if (chance < 0.5f)
        {
            Vehicle->setCurSpeed(Vehicle->getCurSpeed() - 0.5f);
            rotationAmount = aimap->avoidThing_1;
        }
        else
        {
            rotationAmount = atan2(xDot + PlayerSideReactDist, zDot);
        }
    }
    else 
    {
        if (pdotFlag) 
        {
            railSet->SetAccelFactor(railSet->GetAccelFactor() - 10.0f;
            if (xDot < 0.0f)
            {
                rotationAmount = atan2(xDot + PlayerSideReactDist, zDot);
            }
            else
            {
                rotationAmount = atan2(xDot - PlayerSideReactDist, zDot);
            }
        }
        else 
        {
            if (xDot >= -aimap->avoidThing_1 && xDot <= aimap->avoidThing_1)
            {
                // we're still within avoidance tolerance, keep going!
                rotationAmount = aimap->avoidThing_1;
            }
            else
            {
                // done the important part of the avoid, but we could do more
                bool playerIsHorny = player->getCar()->GetCarAudioContainerPtr()->GetIsHornPlaying();
                if (playerIsHorny && ((xDot > 0 && chance >= 0.7f) || (xDot < 0 && chance <= 0.7f))
                {
                    rotationAmount = atan2(xDot + PlayerSideReactDist, zDot);
                }
            }
        }
    }

    // applied mathematics
    float maxSpeed = RailSet->GetCurrentLink()->GetBaseSpeedLimit() + RailSet->GetExheedLimit();
    float rotationRateMul = Vehicle->getCurSpeed() / maxSpeed;

    if (pdotFlag && Vehicle->getCurSpeed() > 5.0f)
    {
        rotationRateMul = 1.0f;
    }
    
    if (rotationAmount < -0.02f) rotationAmount = -0.02f;
    if (rotationAmount > 0.02f) rotationAmount = 0.02f;

    Heading += rotationAmount * aimap->avoidThing_2 * rotationRateMul;
    ourMtx.MakeRotateY(Heading);

    Vehicle->setCurSpeed((RailSet->GetAccelFactor() * datTimeManager::Seconds) + Vehicle->getCurSpeed());
    if (Vehicle->getCurSpeed() < 0.25f)
    {
        Vehicle->setCurSpeed(0.0f);
    }
    else if (Vehicle->getCurSpeed() > maxSpeed)
    {
        Vehicle->setCurSpeed(maxSpeed);
    }

    float velocity = Vehicle->getCurSpeed() * datTimeManager::Seconds;
    RailSet->SetRoadDist(RailSet->GetRoadDist() + velocity);
    ourMtx.SetRow(3, ourMtx.GetRow(3) - (ourMtx.GetRow(2) * velocity));
    Vehicle->SetMatrix(ourMtx);

    Vehicle->SolveYPositionAndOrientation();
}*/

void MM2::aiGoalAvoidPlayer::AvoidPlayer()
{
    // A dumbed down but more predictable approach
    auto aimap = aiMap::GetInstance();
    auto player = aimap->Player(0);

    auto playerMtx = player->GetMatrix();
    auto ourMtx = Vehicle->GetMatrix();

    Vector3 playerPosition = playerMtx.GetRow(3);
    Vector3 playerPositionPredicted = player->GetCar()->GetICS()->GetVelocity() + playerPosition;
    Vector3 ourPosition = ourMtx.GetRow(3);

    Vector3 positionDifference = playerPositionPredicted - ourPosition;

    float xDot = positionDifference.Dot(ourMtx.GetRow(0));
    float zDot = positionDifference.Dot(ourMtx.GetRow(2) * -1.0f);

    auto railSet = Vehicle->GetRailSet();
    float chance = frand();

    float rotationAmount = 0.0f;

    railSet->SetAccelFactor(railSet->GetAccelFactor() - (10.0f * datTimeManager::Seconds));
    if (xDot > 0.0f)
    {
        rotationAmount = atan2f(xDot + PlayerSideReactDist, zDot);
    }
    else
    {
        rotationAmount = atan2f(xDot - PlayerSideReactDist, zDot);
    }

    // applied mathematics
    float maxSpeed = RailSet->GetCurrentLink()->GetBaseSpeedLimit() + RailSet->GetExheedLimit();
    float rotationRateMul = Vehicle->GetSpeed() / maxSpeed;

    if (rotationAmount < -0.02f) rotationAmount = -0.02f;
    if (rotationAmount > 0.02f) rotationAmount = 0.02f;

    Heading += rotationAmount * aimap->avoidThing_2 * rotationRateMul * (datTimeManager::Seconds * 30.0f);
    ourMtx.MakeRotateY(Heading);

    Vehicle->SetSpeed((RailSet->GetAccelFactor() * datTimeManager::Seconds) + Vehicle->GetSpeed());
    if (Vehicle->GetSpeed() < 0.25f)
    {
        Vehicle->SetSpeed(0.0f);
    }
    else if (Vehicle->GetSpeed() > maxSpeed)
    {
        Vehicle->SetSpeed(maxSpeed);
    }

    float velocity = Vehicle->GetSpeed() * datTimeManager::Seconds;
    RailSet->SetRoadDist(RailSet->GetRoadDist() + velocity);
    ourMtx.SetRow(3, ourMtx.GetRow(3) - (ourMtx.GetRow(2) * velocity));
    Vehicle->SetMatrix(ourMtx);

    Vehicle->SolveYPositionAndOrientation();
}
