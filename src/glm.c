#include "glm.h"

Vec3 GLM_createVec3(float x, float y, float z){
	Vec3 toret;
	toret.x=x;
	toret.y=y;
	toret.z=z;
	return toret;
}

Vec4 GLM_createVec4(float x, float y, float z, float w){
	Vec4 toret;
	toret.x=x;
	toret.y=y;
	toret.z=z;
	toret.w=w;
	return toret;
}

Vec3 GLM_addVec3(Vec3 a,Vec3 b){
        Vec3 c;
        c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;
        return c;
}

Vec3 GLM_subVec3(Vec3 a,Vec3 b){
        Vec3 c;
        c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;
        return c;
}

Vec4 GLM_addVec4(Vec4 a,Vec4 b){
        Vec4 c;
        c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;c.w=a.w+b.w;
        return c;
}

Vec4 GLM_subVec4(Vec4 a,Vec4 b){
        Vec4 c;
        c.x=a.x-b.x;c.y=a.y-b.y;c.z=a.z-b.z;c.w=a.w-b.w;
        return c;
}

float GLM_dotVec3(Vec3 a, Vec3 b){
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

float GLM_dotVec4(Vec4 a, Vec4 b){
	return a.x*b.x+a.y*b.y+a.z*b.z+a.w+b.w;
}

Vec3 GLM_unitVec3(Vec3 a){
        float mod=sqrt(GLM_dotVec3(a,a));
        if(mod==0){
                fprintf(stderr,"Null vector cannot be converted to unit vector\n");
                return a;
        }
        a.x/=mod;a.y/=mod;a.z/=mod;
        return a;
}

float GLM_magVec3(Vec3 a){
	return sqrtf(a.x*a.x+a.y*a.y+a.z*a.z);
}

Vec4 GLM_unitVec4(Vec4 a){
        float mod=sqrt(GLM_dotVec4(a,a));
        if(mod==0){
                fprintf(stderr,"Null vector cannot be converted to unit vector\n");
                return a;
        }
        a.x/=mod;a.y/=mod;a.z/=mod;a.w/=mod;
        return a;
}

float GLM_magVec4(Vec4 a){
	return sqrtf(a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w);
}

Vec4 GLM_convertVec3Vec4(Vec3 a,float w){
        return GLM_createVec4(a.x,a.y,a.z,w);
}

Vec3 GLM_convertVec4Vec3(Vec4 a){
        return GLM_createVec3(a.x,a.y,a.z);
}

Vec3 GLM_crossVec3(Vec3 a,Vec3 b){
        Vec3 c;
        c.x=a.y*b.z-a.z*b.y;
        c.y=a.z*b.x-a.x*b.z;
        c.z=a.x*b.y-a.y*b.x;
        return c;
}

Vec3 GLM_projectVec3(Vec3 source, Vec3 base){
	Vec3 n=GLM_unitVec3(base);
	n=GLM_mulScalarVec3(GLM_dotVec3(n, source), n);
	return n;
}

Mat4 GLM_identityMat4(void){
	Mat4 toret;
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(i==j)
				toret.e[i][j]=1;
			else
				toret.e[i][j]=0;
	return toret;
}

Vec3 GLM_mulScalarVec3(float s, Vec3 v){
	v.x*=s;v.y*=s;v.z*=s;
	return v;
}

Vec4 GLM_mulScalarVec4(float s, Vec4 v){
	v.x*=s;v.y*=s;v.z*=s;v.w*=s;
	return v;
}

Vec4 GLM_mulMat4Vec4(Mat4 mat,Vec4 vec){
        Vec4 toret;
        float *src,*dest;
        int i,j;
        src=&vec.x;dest=&toret.x;
        for(i=0;i<4;i++){
                dest[i]=0;
                for(j=0;j<4;j++)
                        dest[i]+=mat.e[i][j]*src[j];
        }
        return toret;
}

Mat4 GLM_mulMat4Mat4(Mat4 mat1,Mat4 mat2){
        Mat4 toret;
        int i,j,k;
        for(i=0;i<4;i++)
                for(j=0;j<4;j++){
                        toret.e[i][j]=0;
                        for(k=0;k<4;k++)
                                toret.e[i][j]+=mat1.e[i][k]*mat2.e[k][j];
                }
        return toret;
}

Mat4 GLM_viewMat4(Vec3 pos,Vec3 back,Vec3 up){
        Mat4 toret;
        Vec3 newX,newY,newZ;
        newY=GLM_unitVec3(up);
        newZ=GLM_unitVec3(back);
        newX=GLM_crossVec3(newY,newZ);
        toret.e[0][0]=newX.x;
        toret.e[0][1]=newX.y;
        toret.e[0][2]=newX.z;
        toret.e[0][3]=-GLM_dotVec3(newX,pos);
        toret.e[1][0]=newY.x;
        toret.e[1][1]=newY.y;
        toret.e[1][2]=newY.z;
        toret.e[1][3]=-GLM_dotVec3(newY,pos);
        toret.e[2][0]=newZ.x;
        toret.e[2][1]=newZ.y;
        toret.e[2][2]=newZ.z;
        toret.e[2][3]=-GLM_dotVec3(newZ,pos);
        toret.e[3][0]=0;
        toret.e[3][1]=0;
        toret.e[3][2]=0;
        toret.e[3][3]=1;
        return toret;
}

Mat4 GLM_translateMat4(Vec3 pos){
	Mat4 toret;
	toret=GLM_identityMat4();
	toret.e[0][3]=pos.x;
	toret.e[1][3]=pos.y;
	toret.e[2][3]=pos.z;
	return toret;
}

Mat4 GLM_rotateMat4(Vec3 axis,float angle){
        Mat4 toret;
        float sina,cosa,x,y,z;
        sina=sinf(angle);cosa=cosf(angle);
        axis=GLM_unitVec3(axis);
        x=axis.x;y=axis.y;z=axis.z;
        toret.e[0][0]=x*x+(1-x*x)*cosa;
        toret.e[0][1]=x*y*(1-cosa)-z*sina;
        toret.e[0][2]=x*z*(1-cosa)+y*sina;
        toret.e[0][3]=0;
        toret.e[1][0]=x*y*(1-cosa)+z*sina;
        toret.e[1][1]=y*y+(1-y*y)*cosa;
        toret.e[1][2]=y*z*(1-cosa)-x*sina;
        toret.e[1][3]=0;
        toret.e[2][0]=x*z*(1-cosa)-y*sina;
        toret.e[2][1]=y*z*(1-cosa)+x*sina;
        toret.e[2][2]=z*z+(1-z*z)*cosa;
        toret.e[2][3]=0;
        toret.e[3][0]=0;
        toret.e[3][1]=0;
        toret.e[3][2]=0;
        toret.e[3][3]=1;
        return toret;
}

Mat4 GLM_orthoMat4(float xrange, float yrange, float zmin, float zmax){
	Mat4 toret=GLM_identityMat4();
	toret.e[0][0]=1/xrange;
	toret.e[1][1]=1/yrange;
	toret.e[2][2]=2/(zmax-zmin);
	toret.e[2][3]=(zmax+zmin)/(zmin-zmax);
	return toret;
}

Mat4 GLM_perspMat4(float fov,float aspectRatio,float zmin,float zmax){
        Mat4 toret=GLM_identityMat4();
        toret.e[0][0]=1/(aspectRatio*tan(GLM_toRadian(fov/2)));
        toret.e[1][1]=1/tan(GLM_toRadian(fov/2));
        toret.e[2][2]=(zmin+zmax)/(zmin-zmax);
        toret.e[2][3]=2*zmin*zmax/(zmin-zmax);
        toret.e[3][2]=-1;
        toret.e[3][3]=0;
        return toret;
}

float GLM_toDegree(float rad){
        return rad*180/PI;
}

float GLM_toRadian(float deg){
        return deg*PI/180;
}
