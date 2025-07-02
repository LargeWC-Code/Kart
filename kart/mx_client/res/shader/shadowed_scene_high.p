#version 130
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

varying vec2 TexCoord0;
varying vec2 TexCoord1;
varying vec4 iShadow;
varying vec3 iNor;

uniform vec3    UCVAR_SunDir;
uniform vec3    UCVAR_SunBase;
uniform vec3    UCVAR_SunBala;

uniform vec2	UCVAR_FogRange;
uniform vec4	UCVAR_FogColor;

uniform float   g_Bias = 0.0012;
uniform vec2	ShadowTexSize = vec2( 2048.0, 2048.0 );

#define SCALE 255.0
vec4 pack( float depth )
{
    const vec4 bitSh = vec4( SCALE * SCALE * SCALE,
                             SCALE * SCALE,
                             SCALE,
                             1.0 );
    const vec4 bitMsk = vec4( 0.0,
                              1.0 / SCALE,
                              1.0 / SCALE,
                              1.0 / SCALE );
    vec4 comp = fract( depth * bitSh );
    comp -= comp.xxyz * bitMsk;
    return comp;
}

float unpack (vec4 colour)
{
    const vec4 bitShifts = vec4(1.0 / (SCALE * SCALE * SCALE),
                                1.0 / (SCALE * SCALE),
                                1.0 / (SCALE),
                                1);
    return dot(colour , bitShifts);
}

float GetLight3(vec4 iShadow)
{
	float fExtentTag = iShadow.z / iShadow.w;

	float fLight = 0.0;
	for (float y=-1.0;y<=1.0;y+=1.0)
	{
		for (float x=-1.0;x<=1.0;x+=1.0)
		{
			vec4 vShw = texture2D(Texture2, iShadow.xy + vec2(x,y) / ShadowTexSize);
		 	float fExtentSrc = unpack(vShw);

			if (fExtentTag - fExtentSrc < g_Bias)
				fLight += 1.0;
		}
	}

	fLight /= 9.0;
	return fLight;
}

void main()
{
	vec4 Color = texture2D(Texture0,TexCoord0);
	vec4 Light = texture2D(Texture1,TexCoord1);
	if (Color.a<0.1)
		discard;
 	Color.rgb = Color.rgb * Color.rgb;
/*
 	vec4 vShw = texture2D(Texture1, iShadow.xy);
 	float fExtentSrc = unpack(vShw);
	float fExtentTag = iShadow.z/iShadow.w;
	fExtentTag = (fExtentTag + 1.0) /2.0;
 	float Distance = abs(fExtentTag - fExtentSrc);
 	if (Distance<0.002)
 		Distance = 0.0;
 	gl_FragColor = Color*0.01 + pack(Distance);//
*/

	float fLight = 1.0;
	if (iShadow.x>=0.0&&iShadow.x<=1.0&&iShadow.y>=0.0&&iShadow.y<=1.0)
	{
		fLight = GetLight3(iShadow);
		/*
		vec4 vShw = texture2D(Texture1, iShadow.xy);
		float fExtentSrc = unpack(vShw);
		float fExtentTag = iShadow.z / iShadow.w;

		if (fExtentTag - fExtentSrc > g_Bias)
			fLight = 0.0;
		*/
	}

	//fLight = fLight * Light.r;
	float fAO = Light.a;
	float fDiffuse = max(dot(UCVAR_SunDir, iNor),0.0) * fLight * 1.4;
	vec3 Diffuse = (UCVAR_SunBase + UCVAR_SunBala * 1.4 * fDiffuse);

	gl_FragColor = vec4(sqrt(Color.rgb * Diffuse) * fAO,Color.a);
}
