#include "landmark.h"
#include <modules\model\getpivot.h>
#include <modules\effects\waterspout.h>

using namespace MM2;

AGE_API int lvlLandmark::Init(const char* a1, const Matrix34& a2, int a3)
{
    int retVal = lvlFixedRotY::Init(a1, a2, a3);

    char fountainName[16] = { 0 };
    char fountainFullName[128] = { 0 };

    Matrix34 fountainPivot;
    Matrix34 fountainMatrix;

    this->m_Spout = nullptr;
    for (int i = 0; i < 10; i++)
    {
        sprintf_s(fountainName, "FOUNTAIN%d", i);
        if (GetPivot(fountainPivot, a1, fountainName))
        {
            // create waterspout
            auto spout = new fxWaterSpout();

            fountainMatrix.Identity();
            fountainMatrix.SetRow(3, fountainPivot.GetRow(2));
            spout->SetMatrix(fountainMatrix);

            spout->SetNext(this->m_Spout);
            this->m_Spout = spout;

            sprintf_s(fountainFullName, "%s_%s", a1, fountainName);
            spout->SetName(fountainFullName);
            spout->Load();
            
            this->SetFlag(0x80); // Set glow flag
        }
    }
    return retVal;
}

AGE_API void lvlLandmark::DrawGlow()
{
    auto spout = this->m_Spout;
    while (spout != nullptr)
    {
        spout->Update(datTimeManager::Seconds);
        spout->Draw();
        spout = spout->GetNext();
    }
}
