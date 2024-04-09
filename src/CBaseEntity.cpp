#include "SDK.h"

CBaseCombatWeapon* CBaseEntity::GetActiveWeapon()
{
		//DYNVAR(pHandle, uintptr_t, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	    //return (CBaseCombatWeapon *)gInts.EntList->GetClientEntityFromHandle(pHandle.GetValue(this));
    typedef CBaseCombatWeapon *(*OriginalFn)(void *); // from 8dcc
    return getvfunc<OriginalFn>(this, 291)(this);
}

#define VEC_ZERO ((Vector){ 0.f, 0.f, 0.f })
#define HITBOX_SET 0
#define MAXSTUDIOBONES 128
#define BONE_USED_BY_HITBOX 0x00000100
void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut)
{
	for (auto i = 0; i < 3; i++)
		vOut[i] = vSome.Dot((Vector&)vMatrix[i]) + vMatrix[i][3];
}


Vector center_of_hitbox(studiohdr_t* studio, matrix3x4* bonemat, int set, int idx) 
{
    studiobbox_t* bbox = studiohdr_pHitbox(studio, set, idx);
    if (!bbox)
        return VEC_ZERO;

    Vector min, max;
    VectorTransform(bbox->bbmin, bonemat[bbox->bone], min);
    VectorTransform(bbox->bbmax, bonemat[bbox->bone], max);

    return (Vector){
        (min.x + max.x) * 0.5f,
        (min.y + max.y) * 0.5f,
        (min.z + max.z) * 0.5f,
    };
}


Vector CBaseEntity::get_hitbox_pos(int hitbox_idx) 
{
    static matrix3x4 bones[MAXSTUDIOBONES];
    if (!this->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
        return VEC_ZERO;

    const auto model = this->GetModel();
    if (!model)
        return VEC_ZERO;

    studiohdr_t* hdr = gInts.ModelInfo->GetStudiomodel(model);
    if (!hdr)
        return VEC_ZERO;

    return center_of_hitbox(hdr, bones, HITBOX_SET, hitbox_idx);
    
}

