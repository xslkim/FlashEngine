
const char* ccPositionTextureRed20_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main()
{
	vec4 col = texture2D(CC_Texture0, v_texCoord);
	float r = (col.r + ((1.0-col.r)*0.2)) * col.a;
	float g = (col.g - (col.g*0.2)) * col.a;
	float b = (col.b - (col.b*0.2)) * col.a;
	gl_FragColor = v_fragmentColor * vec4(r, g, b, col.a);
}
);

