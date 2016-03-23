#include "struct.h"

Vec3 GLM_createVec3(float, float, float);
Vec4 GLM_createVec4(float, float, float, float);
Vec3 GLM_addVec3(Vec3, Vec3);
Vec3 GLM_subVec3(Vec3, Vec3);
Vec4 GLM_addVec4(Vec4, Vec4);
Vec4 GLM_subVec4(Vec4, Vec4);
float GLM_dotVec3(Vec3, Vec3);
float GLM_dotVec4(Vec4, Vec4);
Vec3 GLM_unitVec3(Vec3);
float GLM_magVec3(Vec3);
Vec4 GLM_unitVec4(Vec4);
float GLM_magVec4(Vec4);
Vec4 GLM_convertVec3Vec4(Vec3, float);
Vec3 GLM_convertVec4Vec3(Vec4);
Vec3 GLM_crossVec3(Vec3, Vec3);
Vec3 GLM_projectVec3(Vec3, Vec3);
Mat4 GLM_identityMat4(void);
Vec3 GLM_mulScalarVec3(float, Vec3);
Vec4 GLM_mulScalarVec4(float, Vec4);
Vec4 GLM_mulMat4Vec4(Mat4, Vec4);
Mat4 GLM_mulMat4Mat4(Mat4, Mat4);
Mat4 GLM_viewMat4(Vec3, Vec3, Vec3);
Mat4 GLM_translateMat4(Vec3);
Mat4 GLM_rotateMat4(Vec3, float);
Mat4 GLM_orthoMat4(float, float, float, float);
Mat4 GLM_perspMat4(float, float, float, float);
float GLM_toDegree(float);
float GLM_toRadian(float);
