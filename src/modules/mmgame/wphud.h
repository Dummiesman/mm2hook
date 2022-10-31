#pragma once
#include <modules\node\node.h>
#include <modules\mmeffects.h>

namespace MM2
{
    // Forward declarations
    class mmGameHUD;
    class mmCRHUD;
    class mmWPHUD;

    // Class definitions
    class mmGameHUD : public asNode 
    {
    public:
        AGE_API virtual void PostUpdate() PURE;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmGameHUD, asNode>("mmGameHUD")
                .addFunction("PostUpdate", &PostUpdate)
                .endClass();
        }
    };

    class mmWPHUD : public mmGameHUD
    {
    private:
        mmTextNode* tCheck;
        mmNumber* nWaypointCounter;
        int dword_20;
        mmNumber* nStandings;
        mmTextNode* tPlace;
        int dword_2c;
        mmNumberFont dword_30;
        mmNumberFont dword_40;
        HFONT font_50;
        int WaypointCount;
        int WaypointsCleared;
        int dword_5c;
        int* CurrentWaypointPtr;
        int LastWaypoint; // the last value of currentwaypointptr. if currentwaypointptr != this, text is updated
    public:
        AGE_API virtual void PostUpdate() override          { hook::Thunk<0x438960>::Call<void>(this); }
    };
    ASSERT_SIZEOF(mmWPHUD, 0x68);

    class mmCRHUD : public mmGameHUD
    {
    private:
        mmNumberFont ScoreFont;
        mmNumberFont dword_28;
        HFONT font_38;
        HFONT font_3c;
        mmTextNode tPlayerName;
        mmNumber nScore;
        mmTextNode* tPlayerNames;
        mmNumber* nPlayerScores;
        mmTextNode* tTeam1Name;
        mmTextNode* tTeam2Name;
        mmNumber* nTeam1Score;
        mmNumber* nTeam2Score;
        mmTextNode* tPlayerDollarSigns;
        int* PlayerIds;
        asNode ScoresParent;
        int NumPlayers;
        asMeshSetForm* GoldMeshForm;
        Matrix34 GoldMatrix;
        Matrix34* mtx_178;
    public:
        AGE_API virtual void PostUpdate() override          { hook::Thunk<0x4382E0>::Call<void>(this); }

        mmTextNode& GetPlayerNameText() {
            return this->tPlayerName;
        }

        mmTextNode* GetTeamNameText(int teamNumber) {
            if (teamNumber < 0 || teamNumber > 1)
                return nullptr;
            if (teamNumber == 0) {
                return tTeam1Name;
            } else {
                return tTeam2Name;
            }
        }

        mmNumber* GetTeamScoreText(int teamNumber) {
            if (teamNumber < 0 || teamNumber > 1)
                return nullptr;
            if (teamNumber == 0) {
                return nTeam1Score;
            } else {
                return nTeam2Score;
            }
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmCRHUD, mmGameHUD>("mmCRHUD")
                .addPropertyReadOnly("PlayerNameText", &GetPlayerNameText)
                .addFunction("GetTeamNameText", &GetTeamNameText)
                .addFunction("GetTeamScoreText", &GetTeamScoreText)
                .endClass();
        }
    };
    ASSERT_SIZEOF(mmCRHUD, 0x17C);
}