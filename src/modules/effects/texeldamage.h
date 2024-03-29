#pragma once
#include <algorithm>
#include <modules\gfx\texture.h>
#include <modules\gfx\gettex.h>
#include <modules\gfx\packet.h>
#include <modules\model\shader.h>
#include <modules\model\static2.h>

namespace MM2
{
    // Forward declarations
    struct TexelDamageTri;
    struct fxTexelDamage;

    // External declarations


    // Class definitions
    const auto ApplyBirdPoopDamage = (void(*)(gfxTexture*, gfxTexture*, float, float))(0x5923C0);

    struct TexelDamageTri
    {
        ushort Indices[3];
        ushort Texture;
    };

    struct fxTexelDamage
    {
        int AdjunctCount;
        int TriCount;
        int TextureCount;
        Vector3 *Positions;
        Vector2 *TexCoords;
        TexelDamageTri *DamageTris;

        // Referenced by vehCarModel::Draw @ 0x4CE0CB, offset 0x18
        modShader *CurrentShaders;
        modShader *CleanShaders;
        gfxTexture **DamageTextures;

        ANGEL_ALLOCATOR

        fxTexelDamage()
        { 
            this->AdjunctCount = 0;
            this->TriCount = 0;
            this->TextureCount = 0;
            this->Positions = nullptr;
            this->TexCoords = nullptr;
            this->DamageTris = nullptr;
            this->CurrentShaders = nullptr;
            this->CleanShaders = nullptr;
            this->DamageTextures = nullptr;
        }

        int Init(modStatic* model, modShader* shaders, int shaderCount)
        {
            if (model->PacketCount <= 0)
            {
                return 0;
            }

            TextureCount = shaderCount;
            CurrentShaders = new modShader[shaderCount];
            CleanShaders = new modShader[shaderCount];
            DamageTextures = new gfxTexture * [shaderCount];

            for (int i = 0; i < shaderCount; ++i)
            {
                CleanShaders[i].SetTexture(shaders[i].GetTexture());
                CleanShaders[i].SetMaterial(shaders[i].GetMaterial());

                CurrentShaders[i].SetTexture(CleanShaders[i].GetTexture());
                CurrentShaders[i].SetMaterial(CleanShaders[i].GetMaterial());

                DamageTextures[i] = nullptr;
            }

            bool hasDamageTextures = false;

            for (int i = 0; i < model->PacketCount; ++i)
            {
                int shaderIndex = model->ShaderIndices[i];
                modShader* cleanShader = &CleanShaders[shaderIndex];
                cleanShader->PreLoad();

                if (cleanShader->GetTexture())
                {
                    char textureName[128];

                    strcpy_s(textureName, cleanShader->GetTexture()->Name);

                    char* find = strrchr(textureName, '_');

                    if (find && !_strcmpi(find, "_dmg"))
                    {
                        *find = '\0';

                        DamageTextures[shaderIndex] = cleanShader->GetTexture();

                        gfxTexture* cleanTexture = gfxGetTexture(textureName, 1);

                        if (cleanTexture)
                        {
                            cleanShader->SetTexture(cleanTexture);
                        }

                        hasDamageTextures = true;
                    }
                    else
                    {
                        strcat_s(textureName, "_dmg");

                        gfxTexture* damageTexture = gfxGetTexture(textureName, 1);

                        DamageTextures[shaderIndex] = damageTexture;

                        if (damageTexture)
                        {
                            hasDamageTextures = true;
                        }
                    }

                    modShader* dirtyShader = &CurrentShaders[shaderIndex];

                    dirtyShader->SetTexture(cleanShader->GetTexture()->Clone());
                    dirtyShader->SetMaterial(new gfxMaterial(*cleanShader->GetMaterial()));
                }
            }

            if (hasDamageTextures)
            {
                int adjunctCount = model->GetAdjunctCount();

                AdjunctCount = adjunctCount;

                Positions = new Vector3[adjunctCount];
                TexCoords = new Vector2[adjunctCount];

                int currentPos = 0;

                for (int i = 0; i < model->PacketCount; ++i)
                {
                    for (gfxPacket* packet = model->ppPackets[i]; packet; packet = packet->GetNext())
                    {
                        if (packet->StartVertex >= 0)
                        {
                            packet->DoLock();
                        }

                        for (int j = 0; j < packet->AdjunctCount; ++j)
                        {
                            packet->GetPosition(Positions[currentPos], j);
                            packet->GetTexCoord(TexCoords[currentPos], j);

                            ++currentPos;
                        }

                        if (packet->StartVertex >= 0)
                        {
                            packet->DoUnlock();
                        }
                    }
                }

                int triCount = model->GetTriCount();

                TriCount = triCount;
                DamageTris = new TexelDamageTri[triCount];

                int currentTri = 0;
                int currentAdjunct = 0;

                for (int i = 0; i < model->PacketCount; ++i)
                {
                    uint8_t shaderIndex = model->ShaderIndices[i];

                    for (gfxPacket* packet = model->ppPackets[i]; packet; packet = packet->GetNext())
                    {
                        for (int j = 0; j < (packet->TriCount / 3); ++j)
                        {
                            int tri[3];
                            packet->GetTri(tri, j);

                            DamageTris[currentTri].Indices[0] = (uint16_t)(currentAdjunct + tri[0]);
                            DamageTris[currentTri].Indices[1] = (uint16_t)(currentAdjunct + tri[1]);
                            DamageTris[currentTri].Indices[2] = (uint16_t)(currentAdjunct + tri[2]);
                            DamageTris[currentTri].Texture = shaderIndex;

                            ++currentTri;
                        }

                        currentAdjunct += packet->AdjunctCount;
                    }
                }

                std::sort(DamageTris, DamageTris + triCount, [](const TexelDamageTri& lhs, const TexelDamageTri& rhs)
                {
                    return lhs.Texture < rhs.Texture;
                });

                return 1;
            }

            Kill();
            return 0;
        }

        void Reset()
        {
            for (int i = 0; i < TextureCount; ++i)
            {
                if (CleanShaders[i].GetTexture() != CurrentShaders[i].GetTexture())
                {
                    CurrentShaders[i].GetTexture()->Blit(
                        0, 0,
                        CleanShaders[i].GetTexture(),
                        0, 0,
                        CleanShaders[i].GetTexture()->Width, CleanShaders[i].GetTexture()->Height
                    );
                }

                if (CleanShaders[i].GetMaterial() != CurrentShaders[i].GetMaterial())
                {
                    CleanShaders[i].SetMaterial(CurrentShaders[i].GetMaterial());
                }
            }
        }

        void ApplyDamage(Vector3* damagePosition, float maxDist)
        {
            // maxDist = 1.0f;
            // LogFile::Printf(1, "Texel Damage @ %f, %f, %f - Radius: %f", damagePosition->X, damagePosition->Y, damagePosition->Z, maxDist);

            DisableGlobalSeed();

            int lastTex = -1;

            for (int i = 0; i < TriCount; ++i)
            {
                TexelDamageTri* tri = &DamageTris[i];

                int texIndex = tri->Texture;

                gfxTexture* damageTexture = DamageTextures[texIndex];

                if (damageTexture)
                {
                    int tri0 = tri->Indices[0];
                    int tri1 = tri->Indices[1];
                    int tri2 = tri->Indices[2];

                    float dist0 = damagePosition->Dist(Positions[tri0]);
                    float dist1 = damagePosition->Dist(Positions[tri1]);
                    float dist2 = damagePosition->Dist(Positions[tri2]);

                    float distMagniture = 1.0f / (dist0 + dist1 + dist2);

                    if ((dist0 < maxDist) || (dist1 < maxDist) || (dist2 < maxDist))
                    {
                        float randX = frand();
                        float randY = frand();
                        float randZ = frand();

                        float randMagnitude = 1.0f / (randX + randY + randZ);

                        randX *= randMagnitude;
                        randY *= randMagnitude;
                        randZ *= randMagnitude;

                        float texDamageX
                            = randX * TexCoords[tri0].X
                            + randY * TexCoords[tri1].X
                            + randZ * TexCoords[tri2].X;

                        float texDamageY
                            = randX * TexCoords[tri0].Y
                            + randY * TexCoords[tri1].Y
                            + randZ * TexCoords[tri2].Y;

                        if (CleanShaders[texIndex].GetTexture() != CurrentShaders[texIndex].GetTexture())
                        {
                            ApplyBirdPoopDamage(
                                CurrentShaders[texIndex].GetTexture(),
                                damageTexture,
                                texDamageX,
                                texDamageY);
                        }

                        /*if (texIndex > lastTex)
                        {
                            // Completely made up
                            float matChange = min(min(min(dist0, dist1), dist2) * 2.0f, 0.9f);

                            shader->Material->Shininess *= matChange;

                            shader->Material->Specular.X *= matChange;
                            shader->Material->Specular.Y *= matChange;
                            shader->Material->Specular.Z *= matChange;
                            shader->Material->Specular.W *= matChange;

                            lastTex = texIndex;
                        }*/
                    }
                }
            }

            EnableGlobalSeed();
        }

        void Kill()
        {
            if (TextureCount)
            {
                // TODO: Handle texture ref count

                for (int i = 0; i < TextureCount; ++i)
                {
                    if (CleanShaders[i].GetMaterial() != CurrentShaders[i].GetMaterial())
                    {
                        delete CurrentShaders[i].GetMaterial();
                    }
                }

                delete[] CurrentShaders;
                CurrentShaders = nullptr;

                delete[] CleanShaders;
                CleanShaders = nullptr;

                delete[] DamageTextures;
                DamageTextures = nullptr;

                TextureCount = 0;
            }

            if (AdjunctCount)
            {
                delete[] Positions;
                Positions = nullptr;

                delete[] TexCoords;
                TexCoords = nullptr;

                AdjunctCount = 0;
            }

            if (TriCount)
            {
                delete[] DamageTris;
                DamageTris = nullptr;

                TriCount = 0;
            }
        }
    };

    ASSERT_SIZEOF(TexelDamageTri, 8);

    // Allocated by vehCarModel::Init @ 0x4CD408, size 0x24
    ASSERT_SIZEOF(fxTexelDamage, 0x24);

    // Lua initialization

}