
const char* ccPositionTextureBright_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main()
{
    vec4 col = texture2D(CC_Texture0, v_texCoord);\n
	float r = col.r * 0.5; \n
	float g = col.g * 0.5; \n
	float b = col.b * 0.5; \n
	gl_FragColor = vec4(r, g, b, col.a);\n
}
);

