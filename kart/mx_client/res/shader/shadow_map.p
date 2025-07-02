varying vec4 iPos;
varying vec2 iTexCoord0;

uniform float	UCVAR_CameraFarPlane;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
	vec4 Color  = texture2D(Texture0,iTexCoord0);
	if (Color.a == 0.0)
		discard;

	float shadowMapPosition = iPos.z / iPos.w;
    gl_FragColor = vec4(shadowMapPosition, 0.0, 0.0, 1.0);
}
