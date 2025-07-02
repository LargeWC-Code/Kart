// #version 130
uniform sampler2D Texture0;
uniform sampler2D Texture1;

varying vec2 TexCoord0;
varying vec4 iPos;
varying vec3 iNor;

uniform vec4    UCVAR_FactorColor;

uniform vec3    UCVAR_SunDir;
uniform vec3    UCVAR_SunBase;
uniform vec3    UCVAR_SunBala;
uniform vec3    UCVAR_CameraPos;
uniform vec3    UCVAR_CameraDir;
uniform float	UCVAR_CameraFarPlane;

uniform vec2	UCVAR_FogRange;
uniform vec4	UCVAR_FogColor;

//vec3 HighLight = {0.4, 0.4, 0.0};
void main()
{
	vec4 Color = texture2D(Texture0,TexCoord0);
	if (Color.a < 0.1)
		discard;
	
	vec3 FinalNormal = normalize(iNor.xyz);
	{
		vec3 HighLight = texture2D(Texture0, TexCoord0).rgb;

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
 		//vec3 indirect_specular = texture2D(Texture1, ref_dir.xz, roughness.y).rgb;
 		//indirect_specular *= (HighLight.z + (max(HighLight.x, HighLight.z) - HighLight.z) * ndh_vdh_vdn.z * HighLight.z);

		Color.xyz = ((UCVAR_SunBase + sun_diffuse * UCVAR_SunBala) * (Color.xyz * Color.xyz) + (vec3(sun_specular,sun_specular,sun_specular)));
	}
	gl_FragColor = vec4(sqrt(Color.rgb),Color.a * UCVAR_FactorColor.a);
	

	/*
 	Color.rgb = Color.rgb * Color.rgb;

	float fDiffuse = max(dot(UCVAR_SunDir, iNor), -0.2);
	
// 	if (fDiffuse > -0.2)
// 		fDiffuse = 1.0;
// 	else
// 		fDiffuse = 0.0;

	vec3 Diffuse = (UCVAR_SunBase + UCVAR_SunBala * fDiffuse);
	gl_FragColor = vec4(sqrt(Color.rgb * Diffuse),Color.a * UCVAR_FactorColor.a);
	*/
}
