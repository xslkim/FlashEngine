if(DISPLAY)

set(COCOS_RENDERER_SRC
	renderer/CCBatchCommand.cpp
	renderer/CCCustomCommand.cpp
	renderer/CCMeshCommand.cpp
	renderer/CCGLProgramCache.cpp
	renderer/CCGLProgram.cpp
	renderer/CCGLProgramStateCache.cpp
	renderer/CCGLProgramState.cpp
	renderer/ccGLStateCache.cpp
	renderer/CCGroupCommand.cpp
	renderer/CCQuadCommand.cpp
	renderer/CCRenderCommand.cpp
	renderer/CCRenderer.cpp
	renderer/ccShaders.cpp
	renderer/CCTexture2D.cpp
	renderer/CCTextureAtlas.cpp
	renderer/CCTextureCache.cpp
)


else()

set(COCOS_RENDERER_SRC
	renderer/CCBatchCommand_ex.cpp
	renderer/CCCustomCommand_ex.cpp
	renderer/CCMeshCommand_ex.cpp
	renderer/CCGLProgramCache_ex.cpp
	renderer/CCGLProgram_ex.cpp
	renderer/CCGLProgramStateCache_ex.cpp
	renderer/CCGLProgramState_ex.cpp
	renderer/ccGLStateCache_ex.cpp
	renderer/CCGroupCommand_ex.cpp
	renderer/CCQuadCommand_ex.cpp
	renderer/CCRenderCommand_ex.cpp
	renderer/CCRenderer_ex.cpp
	renderer/ccShaders_ex.cpp
	renderer/CCTexture2D_ex.cpp
	renderer/CCTextureAtlas_ex.cpp
	renderer/CCTextureCache_ex.cpp
)

endif()
