#include "pch.h"
#include "LTModel.h"

void Light::LTModel::Draw(render::RenderDevice * renderer, const float * model, const float * mvp)
{
	for (std::size_t i = 0; i<Meshs.size(); i++)
	{
		Materials[i]->Apply(renderer, model, mvp);
		if (Textures[i]) renderer->SetTexture(0, Textures[i]);
		Meshs[i]->Draw(renderer);
	}
}