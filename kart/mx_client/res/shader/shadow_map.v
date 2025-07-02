attribute vec4 UCVAR_Position;
attribute vec4 UCVAR_Normal;
attribute vec2 UCVAR_TexCoord0;
attribute vec4 UCVAR_BoneIndex;
attribute vec4 UCVAR_BoneWeight;

uniform mat4    UCVAR_World;
uniform mat4    UCVAR_CameraProj;
uniform mat4    UCVAR_CameraView;

varying vec4	iPos;
varying vec2	iTexCoord0;

uniform float  UCVAR_BoneSize;
uniform mat4   UCVAR_BoneMatrix[50];

void main()
{
	vec4 NewPos;
	if (UCVAR_BoneWeight.x>0.0)
	{
		NewPos = UCVAR_BoneWeight.x * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.x*255.0+0.5)] * UCVAR_Position;
		if (UCVAR_BoneWeight.y>0.0)
		{
			NewPos += UCVAR_BoneWeight.y * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.y*255.0+0.5)] * UCVAR_Position;
			if (UCVAR_BoneWeight.z>0.0)
			{
				NewPos += UCVAR_BoneWeight.z * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.z*255.0+0.5)] * UCVAR_Position;
				if (UCVAR_BoneWeight.w>0.0)
					NewPos += UCVAR_BoneWeight.w * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.w*255.0+0.5)] * UCVAR_Position;
			}
		}
	}
	else
		NewPos = UCVAR_Position;

	gl_Position = UCVAR_CameraProj * UCVAR_CameraView * UCVAR_World * NewPos;
	iPos = UCVAR_CameraProj * UCVAR_CameraView * UCVAR_World * NewPos;

	iTexCoord0 = UCVAR_TexCoord0;
}