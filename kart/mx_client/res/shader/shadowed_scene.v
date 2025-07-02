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

uniform vec2	UCVAR_FogRange;
uniform vec4	UCVAR_FogColor;

// Output
varying vec2 TexCoord0;
varying vec2 TexCoord1;
varying vec4 iShadow;
varying vec4 iFog;
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
				if (UCVAR_BoneWeight.z>0.0)
				{
					NewPos += UCVAR_BoneWeight.z * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.z*255.0)] * UCVAR_Position;
					NewNor += (UCVAR_BoneWeight.z * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.z*255.0)] * vec4(UCVAR_Normal.xyz,0.0)).rgb;
					if (UCVAR_BoneWeight.w>0.0)
					{
						NewPos += UCVAR_BoneWeight.w * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.w*255.0)] * UCVAR_Position;
						NewNor += (UCVAR_BoneWeight.w * UCVAR_BoneMatrix[int(UCVAR_BoneIndex.w*255.0)] * vec4(UCVAR_Normal.xyz,0.0)).rgb;
					}
				}
		}
	}
	else
	{
		NewPos = UCVAR_Position;
		NewNor = UCVAR_Normal.rgb;
	}


    vec4 vPos = UCVAR_CameraProj * UCVAR_CameraView * UCVAR_World * NewPos;
	gl_Position = vPos;
    iNor = normalize(UCVAR_World * vec4(NewNor,0.0)).xyz;
	
	float fAlpha = (vPos.z - UCVAR_FogRange.x) / (UCVAR_FogRange.y - UCVAR_FogRange.x);
	fAlpha = min(max(fAlpha, 0.0), 1.0);

	iFog = UCVAR_FogColor * (1.0 - fAlpha);

	TexCoord0 = UCVAR_TexCoord0;
	TexCoord1 = UCVAR_TexCoord1;

	iShadow = g_matLightViewProj * UCVAR_World * NewPos;

	iShadow.x = iShadow.x / 2.0 + 0.5;
	iShadow.y = iShadow.y / 2.0 + 0.5;
}
