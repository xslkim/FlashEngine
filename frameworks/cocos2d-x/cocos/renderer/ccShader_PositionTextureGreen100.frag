
const char* ccPositionTextureGreen100_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main()
{
	vec4 col = texture2D(CC_Texture0, v_texCoord);
	//float r = 1.0 * col.a;
	//float g = 0 * col.a;
	//float b = 0 * col.a;
	gl_FragColor = v_fragmentColor * vec4(0.0, (1.0 * col.g), 0.0, col.a);
}
);

