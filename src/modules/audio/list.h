#pragma once
#include <mm2_common.h>

namespace MM2
{
    // Forward declarations
    class audList;

    // External declarations

    // Class definitions
    class audList {
    private:
        audList* m_Next;
        audList* m_Prev;
    public:
        void Init()                 { m_Next = this; m_Prev = this; }
        audList* GetNext() const    { return m_Next;  }
        audList* GetPrev() const    { return m_Prev; }
        void SetNext(audList* list) { m_Next = list; }
        void SetPrev(audList* list) { m_Prev = list; }
    };
}