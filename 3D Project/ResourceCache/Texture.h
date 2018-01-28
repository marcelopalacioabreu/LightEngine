#pragma once
#include "pch.h"
class Texture: public GLBO
{
public:
	virtual void Init();
	virtual void Shutdown();
	string Name;
	GLuint iIndex;
	GLuint iWidth;
	GLuint iHeight;
	float scale = 1.0f;
	/*
	inline void Bind(GLuint tex_unit) {
		glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_2D, iIndex);
	};
	inline void UnBind() { glBindTexture(GL_TEXTURE_2D, 0); };
	*/
};