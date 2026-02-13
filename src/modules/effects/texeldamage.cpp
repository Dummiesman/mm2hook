#include "texeldamage.h"
#include <algorithm>
#include <modules\gfx\texture.h>
#include <modules\gfx\gettex.h>
#include <modules\gfx\packet.h>
#include <modules\model\shader.h>
#include <modules\model\static2.h>

using namespace MM2;

const auto ApplyBirdPoopDamage = (void(*)(gfxTexture*, gfxTexture*, float, float))(0x5923C0);

fxTexelDamage::fxTexelDamage()
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

float fxTexelDamage::Dot(Vector3 a, Vector3 b)
{
    Vector3 vec;
    vec.Set(a);
    return vec.Dot(b);
}

void ProjectToTri(Vector3& p, Vector3 a, Vector3 b, Vector3 c)
{
    Vector3 edge1 = b - a;
    Vector3 edge2 = c - a;
    Vector3 normal;
    normal.Set(edge1);
    normal.Cross(edge2);
    normal.Normalize();
    
    Vector3 pointToA = p - a;
    float distanceToPlane = pointToA.Dot(normal);
    p.Set(p - normal * distanceToPlane);
}

void fxTexelDamage::Barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c, float& u, float& v, float& w)
{
    Vector3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = Dot(v0, v0);
    float d01 = Dot(v0, v1);
    float d11 = Dot(v1, v1);
    float d20 = Dot(v2, v0);
    float d21 = Dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}

int fxTexelDamage::Init(modStatic* model, modShader* shaders, int shaderCount)
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

void fxTexelDamage::Reset()
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

void RadialBlit(gfxTexture* dst, int destX, int destY, gfxTexture* src, int srcX, int srcY, int radius) {
    int minX = srcX - radius;
    int maxX = srcX + radius;
    int minY = srcY - radius;
    int maxY = srcY + radius;

    int radiusSquared = radius * radius;

    for (int y = minY; y <= maxY; ++y) {
        int dy = y - srcY;
        int dySquared = dy * dy;

        for (int x = minX; x <= maxX; ++x) {
            int dx = x - srcX;
            int distanceSquared = dx * dx + dySquared;

            if (distanceSquared <= radiusSquared) {
                // Calculate the distance from the center
                float distance = sqrtf(static_cast<float>(distanceSquared));

                // Normalize the distance to the radius
                float normalizedDistance = distance / radius;

                // Calculate blit probability based on distance (e.g., linear interpolation)
                float blitProbability = 1.0f - normalizedDistance;
                blitProbability += 0.1f;
                
                // Dither based on distance and probability
                if (frand() <= blitProbability) {
                    dst->Blit(destX + dx, destY + dy, src, x, y, 1, 1);
                }
            }
        }
    }
}

void fxTexelDamage::ApplyDamage(Vector3 const & damagePosition, float maxDist)
{
    // maxDist = 1.0f;
    // LogFile::Printf(1, "Texel Damage @ %f, %f, %f - Radius: %f", damagePosition->X, damagePosition->Y, damagePosition->Z, maxDist);

    int lastTex = -1;

#ifndef MM2HOOK_SHIPPING
    if (false || true)
    {
        Warningf("new damage");
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

                if (CleanShaders[texIndex].GetTexture() != CurrentShaders[texIndex].GetTexture())
                {
                    Vector3 damgaePositionCopy = damagePosition;
                    ProjectToTri(damgaePositionCopy, Positions[tri0], Positions[tri1], Positions[tri2]);

                    float bcu, bcv, bcw;
                    Barycentric(damgaePositionCopy, Positions[tri0], Positions[tri1], Positions[tri2], bcu, bcv, bcw);

                    bool isNonNegative = (bcu >= 0.0f) && (bcv >= 0.0f) && (bcw >= 0.0f);
                    const float epsilon = 1e-5f;
                    bool sumsToOne = fabs(bcu + bcv + bcw - 1.0f) < epsilon;

                    if (isNonNegative && sumsToOne)
                    {
                        Warningf("BARYCENTRIC new!!");
                        float  texDamageX = bcu * TexCoords[tri0].X + bcv * TexCoords[tri1].X + bcw * TexCoords[tri2].X;
                        float texDamageY = bcu * TexCoords[tri0].Y + bcv * TexCoords[tri1].Y + bcw * TexCoords[tri2].Y;

                        int texDamageXPix = static_cast<int>(texDamageX * damageTexture->GetWidth());
                        int texDamageYPix = static_cast<int>(texDamageY * damageTexture->GetHeight());

                        RadialBlit(CurrentShaders[texIndex].GetTexture(), texDamageXPix, texDamageYPix, damageTexture, texDamageXPix, texDamageYPix, 48);
                        /*
                        ApplyBirdPoopDamage(
                            CurrentShaders[texIndex].GetTexture(),
                            damageTexture,
                            texDamageX,
                            texDamageY);
                            */
                        return; // we found our hit triangle, no need to contin
                    }
                }
            }
        }
        return;
    }
#endif

    DisableGlobalSeed();
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

            float dist0 = damagePosition.Dist(Positions[tri0]);
            float dist1 = damagePosition.Dist(Positions[tri1]);
            float dist2 = damagePosition.Dist(Positions[tri2]);

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
#ifndef MM2HOOK_SHIPPING
                    float bcu, bcv, bcw;
                    Barycentric(damagePosition, Positions[tri0], Positions[tri1], Positions[tri2], bcu, bcv, bcw);

                    bool isNonNegative = (bcu >= 0.0f) && (bcv >= 0.0f) && (bcw >= 0.0f);
                    const float epsilon = 1e-5f;
                    bool sumsToOne = fabs(bcu + bcv + bcw - 1.0f) < epsilon;

                    if (isNonNegative && sumsToOne) 
                    {
                        Warningf("BARYCENTRIC!!");
                        texDamageX = bcu * TexCoords[tri0].X + bcv * TexCoords[tri1].X + bcw * TexCoords[tri2].X;
                        texDamageY = bcu * TexCoords[tri0].Y + bcv * TexCoords[tri1].Y + bcw * TexCoords[tri2].Y;
                    }
#endif

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

void fxTexelDamage::Kill()
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