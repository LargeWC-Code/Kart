attribute vec4 UCVAR_Position;
attribute vec3 UCVAR_Normal;
attribute vec4 UCVAR_Diffuse;

attribute vec2 UCVAR_TexCoord0;
attribute vec2 UCVAR_TexCoord1;
attribute vec2 UCVAR_TexCoord2;
attribute vec2 UCVAR_TexCoord3;
attribute vec2 UCVAR_TexCoord4;
attribute vec2 UCVAR_TexCoord5;

uniform mat4    UCVAR_World;
uniform mat4    UCVAR_CameraProj;
uniform mat4    UCVAR_CameraView;

uniform mat4    g_matLightViewProj;

// Output
varying vec2 TexCoord0;
varying vec4 iPos;
varying vec3 iNor;

attribute vec4 UCVAR_BoneIndex;
attribute vec4 UCVAR_BoneWeight;

uniform float  UCVAR_BoneSize;
uniform mat4   UCVAR_BoneMatrix[50];

void main()
{
	vec4 NewPos;
	vec3 NewNor;
	
	if (UCVAR_BoneWeight.x>0.0)
	{
		NewPos = UCVAR_BoneWeight.x * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.x*255.0)] * UCVAR_Position;
		NewNor = (UCVAR_BoneWeight.x * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.x*255.0)] * vec4(UCVAR_Normal.xyz,0.0)).rgb;

		if (UCVAR_BoneWeight.y>0.0)
		{
				NewPos += UCVAR_BoneWeight.y * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.y*255.0)] * UCVAR_Position;
				NewNor += (UCVAR_BoneWeight.y * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.y*255.0)] * vec4(UCVAR_Normal.xyz,0.0)).rgb;
		}
	}
	else
	{
		NewPos = UCVAR_Position;
		NewNor = UCVAR_Normal.rgb;
	}

    gl_Position = UCVAR_CameraProj * UCVAR_CameraView * UCVAR_World * NewPos;
	iPos = UCVAR_World * NewPos;
    iNor = normalize(UCVAR_World * vec4(NewNor,0.0)).xyz;
	
	TexCoord0 = UCVAR_TexCoord0;
}
