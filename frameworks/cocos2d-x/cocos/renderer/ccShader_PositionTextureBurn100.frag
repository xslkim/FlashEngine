
const char* ccPositionTextureBurn100_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main()
{
	vec4 col = texture2D(CC_Texture0, v_texCoord);
	float r = (col.r + ((40.0/255.0)*(1.0-col.r)*0.8)) * col.a;
	float g = (col.g - ((80.0/255.0)*(col.g)*0.8)) * col.a;
	float b = 0.0;
	gl_FragColor = v_fragmentColor * vec4(r, g, b, col.a);
}
);

