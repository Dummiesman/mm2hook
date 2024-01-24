#include "static2.h"
#include <modules\gfx\rstate.h>
#include <modules\gfx\packet.h>
#include <modules\gfx\pipeline.h>
#include <modules\model\shader.h>

using namespace MM2;

static gfxMaterial ColorMaterialTmp[2]; // Keep two around since AGE compares material changes by ptr
static int ColorMaterialTmpIdx = 0;
static gfxMaterial ShadowMaterial;

AGE_API int modStatic::GetTriCount(void) const	                    
{
	return hook::Thunk<0x4A4DE0>::Call<int>(this); 
}

AGE_API int modStatic::GetAdjunctCount(void) const	                
{
	return hook::Thunk<0x4A4DB0>::Call<int>(this);
}

AGE_API void modStatic::CopyFrom(const modStatic *mod)
{ 
	return hook::Thunk<0x4A4D60>::Call<void>(this, mod); 
}

AGE_API modStatic * modStatic::Clone(void) const	                
{ 
	return hook::Thunk<0x4A4CA0>::Call<modStatic *>(this); 
}

AGE_API void modStatic::Optimize(modShader *shader)	            
{
	return hook::Thunk<0x4A49A0>::Call<void>(this, shader); 
}

AGE_API void modStatic::Draw(modShader *shaders) const	            
{ 
	gfxRenderState::FlushMasked();
	bool lastAlphaEnable = gfxRenderState::GetAlphaEnabled();
	bool alphaEnabled = false;

	for (int i = 0; i < this->PacketCount; i++)
	{
		if (shaders != nullptr)
		{
			auto shader = shaders[this->ShaderIndices[i]];
			gfxRenderState::SetMaterial(shader.GetMaterial());
			gfxRenderState::SetTexture(0, shader.GetTexture());
			
			/*if ((this->Flags & 2) != 0
				&& !alphaEnabled
				&& (shader.GetMaterial()->Diffuse.W != 1.0 || shader.GetTexture() && (shader.GetTexture()->TexEnv & 0x20000) != 0))*/
			if (!alphaEnabled && (shader.GetTexture() && (shader.GetTexture()->TexEnv & 0x20000) != 0 || shader.GetMaterial()->Diffuse.W != 1.0))
			{
				gfxRenderState::SetAlphaEnabled(true);
				alphaEnabled = true;
			}

			gfxRenderState::FlushMasked();
		}

		auto list = this->ppPacketLists[i];
		gfxPacket::DrawList(list);
	}

	gfxRenderState::SetAlphaEnabled(lastAlphaEnable);
}

AGE_API void modStatic::DrawShadowed(modShader* shaders, float intensity) const
{
	gfxRenderState::FlushMasked();
	bool lastAlphaEnable = gfxRenderState::GetAlphaEnabled();

	ShadowMaterial.Diffuse = Vector4(0.0f, 0.0f, 0.0f, intensity);
	ShadowMaterial.Ambient = Vector4(0.0f, 0.0f, 0.0f, intensity);
	ShadowMaterial.Emissive = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	ShadowMaterial.Specular = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	ShadowMaterial.Shininess = 0.0f;
	gfxRenderState::SetMaterial(&ShadowMaterial);

	for (int i = 0; i < this->PacketCount; i++)
	{
		if (shaders != nullptr)
		{
			auto shader = shaders[this->ShaderIndices[i]];
			gfxRenderState::SetTexture(0, shader.GetTexture());
			gfxRenderState::FlushMasked();
		}

		auto list = this->ppPacketLists[i];
		gfxPacket::DrawList(list);
	}

	gfxRenderState::SetAlphaEnabled(lastAlphaEnable);
}

AGE_API void modStatic::DrawColored(modShader* shaders, const Vector4& color) const
{
	gfxRenderState::FlushMasked();
	bool lastAlphaEnable = gfxRenderState::GetAlphaEnabled();
	bool alphaEnabled = false;

	for (int i = 0; i < this->PacketCount; i++)
	{
		if (shaders != nullptr)
		{
			auto shader = shaders[this->ShaderIndices[i]];
			if (shader.GetMaterial() != nullptr)
			{
				ColorMaterialTmpIdx = 1 - ColorMaterialTmpIdx;
				ColorMaterialTmp[ColorMaterialTmpIdx] = *shader.GetMaterial();
				Vector4& ambient = ColorMaterialTmp[ColorMaterialTmpIdx].Ambient;
				Vector4& diffuse = ColorMaterialTmp[ColorMaterialTmpIdx].Diffuse;
				ambient.X *= color.X; ambient.Y *= color.Y; ambient.Z *= color.Z; ambient.W *= color.W;
				diffuse.X *= color.X; diffuse.Y *= color.Y; diffuse.Z *= color.Z; diffuse.W *= color.W;
				gfxRenderState::SetMaterial(&ColorMaterialTmp[ColorMaterialTmpIdx]);
			}
			else
			{
				gfxRenderState::SetMaterial(gfxMaterial::FlatWhite.ptr());
			}
			gfxRenderState::SetTexture(0, shader.GetTexture());

			/*if ((this->Flags & 2) != 0
				&& !alphaEnabled
				&& (shader.GetMaterial()->Diffuse.W != 1.0 || shader.GetTexture() && (shader.GetTexture()->TexEnv & 0x20000) != 0))*/
			if (!alphaEnabled && (shader.GetTexture() && (shader.GetTexture()->TexEnv & 0x20000) != 0 || shader.GetMaterial()->Diffuse.W != 1.0))
			{
				gfxRenderState::SetAlphaEnabled(true);
				alphaEnabled = true;
			}

			gfxRenderState::FlushMasked();
		}

		auto list = this->ppPacketLists[i];
		gfxPacket::DrawList(list);
	}

	gfxRenderState::SetAlphaEnabled(lastAlphaEnable);
}

AGE_API void modStatic::DrawNoAlpha(modShader *shader) const	    
{ 
	return hook::Thunk<0x4A4A20>::Call<void>(this, shader); 
}

AGE_API void modStatic::DrawEnvMapped(modShader* shaders, gfxTexture* envMap, float intensity) const
{
	gfxRenderState::SetTexture(0, envMap);
	gfxRenderState::SetMaterial(gfxMaterial::FlatWhite.ptr());
	gfxRenderState::FlushMasked();
	gfxPacket::BeginRef();

	for (int i = 0; i < this->PacketCount; i++)
	{
		auto shader = shaders[this->ShaderIndices[i]];
		int shininess = (int)(shader.GetMaterial()->Shininess * intensity * 255.0f);
		if (shininess > 0)
		{
			lpD3DDev->SetRenderState(D3DRENDERSTATE_AMBIENT, shininess | (((unsigned int)shininess | (((unsigned __int8)shininess | 0xFFFFFF00) << 8)) << 8));
			auto list = this->ppPacketLists[i];
			gfxPacket::DrawList(list);
		}
	}
}

AGE_API void modStatic::DrawOrthoMapped(modShader *shader, gfxTexture *tex, float a3, uint a4) const
{ 
	return hook::Thunk<0x4A4B30>::Call<void>(this, shader, tex, a3, a4); 
}

AGE_API void modStatic::DrawWithTexGenAndTexMatrix(void) const	    
{
	return hook::Thunk<0x4A4C50>::Call<void>(this); 
}

int MM2::modStatic::GetPacketCount() const
{
	return this->PacketCount;
}

gfxPacket* MM2::modStatic::GetPacket(int num)
{
	if (num < 0 || num >= GetPacketCount())
		return nullptr;
	return this->ppPackets[num];
}

int MM2::modStatic::GetShaderIndex(int num) const
{
	if (num < 0 || num >= GetPacketCount())
		return -1;
	return this->ShaderIndices[num];
}

void modStatic::BindLua(LuaState L) 
{
	LuaBinding(L).beginClass<modStatic>("modStatic")
		//functions
		.addPropertyReadOnly("NumPackets", &GetPacketCount)
		.addFunction("GetPacket", &GetPacket)
		.addFunction("GetShaderIndex", &GetShaderIndex)
		.addFunction("GetTriCount", &GetTriCount)
		.addFunction("GetAdjunctCount", &GetAdjunctCount)
		.endClass();
}