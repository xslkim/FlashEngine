
const char* ccPositionTextureGray_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision mediump float;
\n#endif\n

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main()
{
	vec4 col = texture2D(CC_Texture0, v_texCoord);
	float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114));
	gl_FragColor = v_fragmentColor * vec4(grey, grey, grey, col.a);
}
);

