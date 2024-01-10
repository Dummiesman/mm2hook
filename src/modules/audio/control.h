#pragma once
#include <mm2_common.h>
#include "list.h"
#include "manager.h"

namespace MM2
{
    // Forward declarations
    class audControl;

    // External declarations

    // Class definitions
    class audControl  {
    private:
        char _buf[0xCC]; // verified
    public:
        audControl* Create(audManager::AUDTYPE type, audManager* manager)
        {
            auto control = manager->AllocControl(type);
            if (!control)
            {
                Quitf("audManager::AllocControl(%i): Ran out of controls. Increase the number of controls allocated to type %i.", type, type);
            }
            control->Init(type, manager);
            return control;
        }


        void Init(audManager::AUDTYPE type, audManager* manager)
        {
            hook::Thunk<0x59FF40>::Call<void>(this, type, manager);
        }

    };
    ASSERT_SIZEOF(audControl, 0xCC);
}