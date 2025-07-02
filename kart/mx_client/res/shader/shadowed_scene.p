#version 130
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

varying vec2 TexCoord0;
varying vec2 TexCoord1;
varying vec4 iShadow;
varying vec4 iPos;
varying vec3 iNor;
varying float iFog;

uniform vec3    UCVAR_SunDir;
uniform vec3    UCVAR_SunBase;
uniform vec3    UCVAR_SunBala;
uniform vec3    UCVAR_CameraPos;
uniform vec4	UCVAR_FogColor;
uniform vec4	UCVAR_FactorColor;

uniform float   g_Bias = 0.002;
uniform vec3	HighLight = vec3(0.32, 0.32, 0.0);

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
 	if (Color.a<0.1)
 		discard;

	vec3 FinalNormal = normalize(iNor.xyz);
	{
		//vec3 HighLight = texture2D(Texture1, TexCoord).rgb;
		float ENV_MIP = 8.0;
		vec2 roughness = vec2(1.0, ENV_MIP) * (1.0 - HighLight.x);
		float roughness2 = roughness.x * roughness.x;
		float sun_specular_power = 2.0 / (roughness2 * roughness2) - 2.0;

		//sun diffuse
		float sun_diffuse = max(dot(UCVAR_SunDir, FinalNormal), 0.0);

		//specular helper
		vec4 ndh_vdh_vdn = vec4(0.0,0.0,0.0,0.0);
		vec3 view_dir = normalize(UCVAR_CameraPos - iPos.xyz);
		vec3 half_dir = normalize(view_dir + UCVAR_SunDir);

		ndh_vdh_vdn.x = dot(FinalNormal, half_dir);
		ndh_vdh_vdn.y = dot(view_dir, half_dir);
		ndh_vdh_vdn.z = dot(FinalNormal, view_dir);
		vec3 ref_dir = 2.0 * FinalNormal * ndh_vdh_vdn.z - view_dir;
		ndh_vdh_vdn = max(ndh_vdh_vdn, 0.0);
		ndh_vdh_vdn.yz = 1.0 - ndh_vdh_vdn.yz;
		ndh_vdh_vdn = pow(ndh_vdh_vdn, vec4(sun_specular_power, 5.0, 5.0, 5.4));

		//sun specular
		float direct_spe = HighLight.y;
		float sun_specular = (direct_spe + (1.0 - direct_spe) * ndh_vdh_vdn.y)
			* (sun_specular_power + 2.0) * 0.125 * ndh_vdh_vdn.x
			* sun_diffuse;

		//indirect_specular

 		vec3 indirect_specular = texture2D(Texture1, ref_dir.xz, roughness.y).rgb;
// 		vec3 indirect_specular = texture2D(Texture1, ref_dir.xz, roughness.y).rgb;
//		texCubeLod(Texture1, vec4(ref_dir, roughness.y)).xyz;

 		indirect_specular *= (HighLight.z + (max(HighLight.x, HighLight.z) - HighLight.z) * ndh_vdh_vdn.z * HighLight.z);

		float fLight = 1.0;
 		if (iShadow.x>=0.0&&iShadow.x<=1.0&&iShadow.y>=0.0&&iShadow.y<=1.0)
 			fLight = GetLight3(iShadow);
		Color.xyz = ((UCVAR_SunBase + sun_diffuse * UCVAR_SunBala * fLight) * (Color.xyz * Color.xyz) + (vec3(sun_specular,sun_specular,sun_specular) + indirect_specular) * fLight);

		gl_FragColor = vec4(sqrt(Color.rgb),Color.a * UCVAR_FactorColor.a);
	}
}
