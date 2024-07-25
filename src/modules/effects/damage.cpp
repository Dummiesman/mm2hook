#include "damage.h"
#include <modules/model/shader.h>
#include <modules/gfx/gettex.h>
#include <modules/gfx/packet.h>

namespace MM2
{
	mmDamage::mmDamage()
	{
		TextureCount = 0;
		VertexCount = 0;
		BodyModel = nullptr;
		CleanModel = nullptr;
		DamageModel = nullptr;
		CleanShaders = nullptr;
		DamageShaders = nullptr;
		DamageTextures = nullptr;
	}

	mmDamage::~mmDamage()
	{
		if (TextureCount)
		{
			delete[] CleanShaders;
			CleanShaders = nullptr;

			delete[] DamageShaders;
			DamageShaders = nullptr;

			delete[] DamageTextures;
			DamageTextures = nullptr;

			TextureCount = 0;
		}

		if (VertexCount)
		{
			delete[] VertexPositions;
			VertexPositions = nullptr;

			VertexCount = 0;
		}
	}

	void mmDamage::Init(modStatic* model, modShader* shaders, int shaderCount)
	{
		if (model == nullptr)
			return;

		BodyModel = model;
		CleanModel = model->Clone();
		DamageModel = model->Clone();

		TextureCount = shaderCount;
		CleanShaders = new modShader[shaderCount];
		DamageShaders = new modShader[shaderCount];
		DamageTextures = new gfxTexture*[shaderCount];

		for (int i = 0; i < shaderCount; i++)
		{
			CleanShaders[i].SetTexture(shaders[i].GetTexture());
			CleanShaders[i].SetMaterial(shaders[i].GetMaterial());

			DamageShaders[i].SetTexture(shaders[i].GetTexture());
			DamageShaders[i].SetMaterial(shaders[i].GetMaterial());

			DamageTextures[i] = nullptr;
		}

		bool hasDamageTextures = false;

		for (int i = 0; i < CleanModel->GetPacketCount(); ++i)
		{
			int shaderIndex = CleanModel->GetShaderIndex(i);
			modShader* cleanShader = &CleanShaders[shaderIndex];
			cleanShader->PreLoad();

			if (cleanShader->GetTexture())
			{
				char textureName[128];

				strcpy_s(textureName, cleanShader->GetTexture()->GetName());

				char* find = strrchr(textureName, '_');

				if (find && !_strcmpi(find, "_dmg"))
				{
					*find = '\0';

					gfxTexture* cleanTexture = gfxGetTexture(textureName, 1);

					if (cleanTexture)
					{
						cleanShader->SetTexture(cleanTexture);
						DamageTextures[shaderIndex] = cleanTexture;
						hasDamageTextures = true;
					}
				}
			}
		}

		for (int i = 0; i < DamageModel->GetPacketCount(); ++i)
		{
			int shaderIndex = DamageModel->GetShaderIndex(i);
			modShader* damageShader = &DamageShaders[shaderIndex];
			damageShader->PreLoad();

			if (damageShader->GetTexture())
			{
				char textureName[128];

				strcpy_s(textureName, damageShader->GetTexture()->GetName());

				char* find = strrchr(textureName, '_');

				if (!find || _strcmpi(find, "_dmg"))
				{
					strcat_s(textureName, "_dmg");

					gfxTexture* damageTexture = gfxGetTexture(textureName, 1);

					if (damageTexture)
					{
						damageShader->SetTexture(damageTexture);
						DamageTextures[shaderIndex] = damageTexture;
						hasDamageTextures = true;
					}
				}
			}
		}

		if (hasDamageTextures)
		{
			int adjunctCount = model->GetAdjunctCount();

			VertexCount = adjunctCount;

			VertexPositions = new Vector3[adjunctCount];

			int currentVertex = 0;

			for (int i = 0; i < model->GetPacketCount(); i++)
			{
				auto packet = model->GetPacket(i);
				while (packet)
				{
					packet->DoLock();

					for (unsigned int j = 0; j < packet->GetAdjunctCount(); j++)
					{
						packet->GetPosition(VertexPositions[currentVertex], j);
						currentVertex++;
					}

					packet->DoUnlock();
					packet = packet->GetNext();
				}
			}
		}

		Reset(0);
	}

	void mmDamage::Reset(bool invertDamage)
	{
		for (int i = 0; i < CleanModel->GetPacketCount(); i++)
		{
			auto bodyPacket = BodyModel->GetPacket(i);
			auto cleanPacket = CleanModel->GetPacket(i);
			auto damagePacket = DamageModel->GetPacket(i);

			while (bodyPacket && cleanPacket && damagePacket)
			{
				auto shaderIndex = CleanModel->GetShaderIndex(i);
				gfxTexture* damageTexture = DamageTextures[shaderIndex];
				if (damageTexture != nullptr)
				{
					for (unsigned int j = 0; j < cleanPacket->GetTriangleCount() / 3; j++)
					{
						int tri[3];
						bodyPacket->GetTri(tri, j);

						if (invertDamage)
						{
							damagePacket->SetTri(tri[0], tri[1], tri[2], j);
							cleanPacket->SetTri(0, 0, 0, j);
						}
						else
						{
							cleanPacket->SetTri(tri[0], tri[1], tri[2], j);
							damagePacket->SetTri(0, 0, 0, j);
						}
					}
				}

				bodyPacket = bodyPacket->GetNext();
				cleanPacket = cleanPacket->GetNext();
				damagePacket = damagePacket->GetNext();
			}
		}
	}

	void mmDamage::Apply(Vector3& impactPosition, float radius, bool invertDamage)
	{
		int currentVertex = 0;

		for (int i = 0; i < DamageModel->GetPacketCount(); i++)
		{
			auto bodyPacket = BodyModel->GetPacket(i);
			auto cleanPacket = CleanModel->GetPacket(i);
			auto damagePacket = DamageModel->GetPacket(i);

			while (bodyPacket && cleanPacket && damagePacket)
			{
				auto shaderIndex = DamageModel->GetShaderIndex(i);
				gfxTexture* damageTexture = DamageTextures[shaderIndex];
				if (damageTexture != nullptr)
				{
					for (unsigned int j = 0; j < damagePacket->GetTriangleCount() / 3; j++)
					{
						int tri[3];
						bodyPacket->GetTri(tri, j);

						float dist0 = (VertexPositions[currentVertex + tri[0]] - impactPosition).Mag2();
						float dist1 = (VertexPositions[currentVertex + tri[1]] - impactPosition).Mag2();
						float dist2 = (VertexPositions[currentVertex + tri[2]] - impactPosition).Mag2();

						if (dist0 < radius || dist1 < radius || dist2 < radius)
						{
							if (invertDamage)
							{
								cleanPacket->SetTri(tri[0], tri[1], tri[2], j);
								damagePacket->SetTri(0, 0, 0, j);
							}
							else
							{
								damagePacket->SetTri(tri[0], tri[1], tri[2], j);
								cleanPacket->SetTri(0, 0, 0, j);
							}
						}
					}
				}
				currentVertex += damagePacket->GetAdjunctCount();

				bodyPacket = bodyPacket->GetNext();
				cleanPacket = cleanPacket->GetNext();
				damagePacket = damagePacket->GetNext();
			}
		}
	}

	modStatic* mmDamage::GetCleanModel()
	{
		return this->CleanModel;
	}

	modStatic* mmDamage::GetDamageModel()
	{
		return this->DamageModel;
	}

	modShader* mmDamage::GetCleanShaders()
	{
		return this->CleanShaders;
	}

	modShader* mmDamage::GetDamageShaders()
	{
		return this->DamageShaders;
	}
}