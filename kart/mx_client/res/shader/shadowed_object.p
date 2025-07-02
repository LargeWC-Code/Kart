// #version 130
uniform sampler2D Texture0;
uniform sampler2D Texture1;

varying vec2 TexCoord0;
varying vec4 iShadow;
varying vec3 iNor;

uniform vec3    UCVAR_SunDir;
uniform vec3    UCVAR_SunBase;
uniform vec3    UCVAR_SunBala;
uniform vec3    UCVAR_CameraPos;
uniform vec3    UCVAR_CameraDir;
uniform float	UCVAR_CameraFarPlane;

uniform vec2	UCVAR_FogRange;
uniform vec4	UCVAR_FogColor;
uniform vec4	UCVAR_FactorColor;

uniform float   g_Bias;// = 0.0012;

float GetLight3(vec4 iShadow)
{
	float fLight = 0.0;
	for (float y=-1.0;y<=1.0;y+=1.0)
	{
		for (float x=-1.0;x<=1.0;x+=1.0)
		{
			float fExtentSrc = texture2D(Texture1, iShadow.xy + vec2(x,y) / vec2(4096.0,4096.0)).r;
			float fExtentTag = iShadow.z / iShadow.w;

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
	if (Color.a < 0.1)
		discard;
 	Color.rgb = Color.rgb * Color.rgb;
	/*
 	vec4 vShw = texture2D(Texture1, iShadow.xy);
 	float fExtentSrc = unpack(vShw);
	float fExtentTag = iShadow.z / iShadow.w;
 	float Distance = fExtentTag - fExtentSrc;
	
   	if (Distance > 0.0012)
   		Distance = 0.0;
 	//else
 		//Distance = 0.0;
 	//gl_FragColor = vec4(Distance,Distance,Distance,1.0);//
 	gl_FragColor = pack(Distance);//
	return;
	*/	

	float fLight = 1.0;
	if (iShadow.x>=0.0&&iShadow.x<=1.0&&iShadow.y>=0.0&&iShadow.y<=1.0)
	{
		fLight = GetLight3(iShadow);
		
		vec4 vShw = texture2D(Texture1, iShadow.xy);
		float fExtentSrc = unpack(vShw);
		float fExtentTag = iShadow.z / iShadow.w;

		if (fExtentTag - fExtentSrc > g_Bias)
			fLight = 0.0;		
	}

// 	float fCameraDiffuse = max(dot(UCVAR_CameraDir, iNor),0.0);
// 	if (fCameraDiffuse < 0.4)
// 		fCameraDiffuse = 5.0;
// 	else
// 		fCameraDiffuse = 1.0;
	
	float fDiffuse = 1.0;//max(dot(UCVAR_SunDir, iNor),0.0) * fLight;
// 	if (fDiffuse > 0.4)
// 		fDiffuse = 1.0;
// 	else
// 		fDiffuse = 0.0;

	vec3 Diffuse = (UCVAR_SunBase + UCVAR_SunBala * fLight * 1.4 * fDiffuse);// * fCameraDiffuse;
	gl_FragColor = vec4(sqrt(Color.rgb * Diffuse) , Color.a * UCVAR_FactorColor.a);
}
