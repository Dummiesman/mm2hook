#include "static2.h"
using namespace MM2;

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

AGE_API void modStatic::Draw(modShader *shader) const	            
{ 
	return hook::Thunk<0x4A4550>::Call<void>(this, shader); 
}

AGE_API void modStatic::DrawNoAlpha(modShader *shader) const	    
{ 
	return hook::Thunk<0x4A4A20>::Call<void>(this, shader); 
}

AGE_API void modStatic::DrawEnvMapped(modShader *shader, gfxTexture *tex, float a3) const
{
	return hook::Thunk<0x4A4A50>::Call<void>(this, shader, tex, a3); 
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
		.addFunction("GetTriCount", &GetTriCount)
		.addFunction("GetAdjunctCount", &GetAdjunctCount)
		.endClass();
}