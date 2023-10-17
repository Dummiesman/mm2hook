#include "mm2_model.h"
using namespace MM2;

modModel * modGetModel(LPCSTR filename, uint a2, bool a3, bool a4) {
    return hook::StaticThunk<0x597A70>::Call<modModel *>(filename, a2, a3, a4);
}

void modConvertModel(LPCSTR filename, LPCSTR newFilename, bool a3) {
    return hook::StaticThunk<0x597BD0>::Call<void>(filename, newFilename, a3);
}

/*
    modShader
*/
declfield(modShader::sm_Materials)(0x66191C);
declfield(modShader::sm_StaticMaterialCount)(0x661920);