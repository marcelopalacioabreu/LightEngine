#include "..\pch.h"

void TexShader::SetupRender(Scene * pScene, Actor * pActor)
{
	this->Use();
	// ----- Transform Matricies ------
	mat4 transform = pActor->GetComponent<TransformComponent>("TransformComponent")->GetTransform();
	mat4 parentTransform = pActor->VGetParent()->GetComponent<TransformComponent>("TransformComponent")->GetTransform();
	mat4 globalTransform = parentTransform*transform;
	SetUniformMatrix("Model", globalTransform.ToFloatPtr());
	mat4 MVP = globalTransform* pScene->GetViewProj();
	SetUniformMatrix("MVP", MVP.ToFloatPtr());

	// ----- Lighting ------
	const Light& dirLight = pScene->GetDirLight();
	SetUniform("gLight.La", dirLight.La);
	SetUniform("gLight.Ld", dirLight.Ld);
	SetUniform("gLight.Ls", dirLight.Ls);
	SetUniform("gLight.direction", dirLight.direction);

	// ----- Material ------

	MeshRenderComponent* mrc =  pActor->GetComponent<MeshRenderComponent>("MeshRenderComponent");
	if (!mrc) return;
	const Material& mat = mrc->GetMaterial();
	SetUniform("gMaterial.Ka", mat.Ka);
	SetUniform("gMaterial.Kd", mat.Kd);
	SetUniform("gMaterial.Ks", mat.Ks);
	SetUniform("gMaterial.exp", mat.exp);

}
