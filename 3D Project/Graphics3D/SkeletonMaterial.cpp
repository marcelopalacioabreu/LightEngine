#include "pch.h"
#include "SkeletonMaterial.h"

#include "Scene.h"
#include "..\Interface\IRenderSystem.h"
#include "Interface\IResourceManager.h"
#include "..\Interface\IScene.h"
#include "..\Interface\IEventManager.h"
#include "..\Core\Events.h"
Light::render::SkeletonMaterial::SkeletonMaterial(IContext * pContext):m_pModelUniform(nullptr),m_pMVPUniform(nullptr)
{
	auto pRenderer = pContext->GetSystem<render::IRenderSystem>()->GetRenderDevice();
	auto pResources = pContext->GetSystem<resources::IResourceManager>();

	auto pVertexShader = pRenderer->CreateVertexShader(pResources->VGetShaderCode("Skeleton.vs")->Get());
	auto pPixelShader = pRenderer->CreatePixelShader(pResources->VGetShaderCode("Default.fs")->Get());

	m_Pipeline = std::unique_ptr<Pipeline>(pRenderer->CreatePipeline(pVertexShader, pPixelShader));
	this->GetUniform();
	pContext->GetSystem<IEventManager>()->VAddListener(DEBUG_NEW EventDelegate<SkeletonMaterial>(this, &SkeletonMaterial::OnSceneCreate), events::EvtSceneCreate::StaticType);
}

void Light::render::SkeletonMaterial::Apply(RenderDevice * renderer, const MatrixParam& matrixParam,const MaterialData& matData)
{
	const float* model = matrixParam.at(render::uMODEL);
	const float* mvp = matrixParam.at(render::uMVP);
	const float* skeTrannsform = matrixParam.at(render::uSkeTransform);
	const float* cameraPos = matrixParam.at(uCameraPos);
	const int numNode = (int)matrixParam.at("numNode");
	renderer->SetPipeline(m_Pipeline.get());
	
	m_uNumLight->SetAsInt(m_pLightManager->GetNumPointLight());
	m_pLightManager->SetupDirLight(&m_uDirLight);
	m_pLightManager->SetupPointLight(m_uPointLight);

	//m_uAmbient->SetAsInt(UNIT_AMBIENT);
	m_uDiffuse->SetAsInt(UNIT_DIFFUSE);
	//m_uSpecular->SetAsInt(UNIT_SPECULAR);
	m_uCubeTex->SetAsInt(UNIT_SKYBOX);

	m_uKa->SetAsVec3(glm::value_ptr(matData.Ka));
	m_uKd->SetAsVec3(glm::value_ptr(matData.Kd));
	m_uKs->SetAsVec3(glm::value_ptr(matData.Ks));
	m_uShiness->SetAsFloat(matData.exp.x);

	//renderer->SetTexture(UNIT_SKYBOX,renderer->GetSkyBoxTexture());
	m_uCameraPos->SetAsVec3(cameraPos);
	m_pModelUniform->SetAsMat4(model);
	m_pMVPUniform->SetAsMat4(mvp);

	m_uSkeTransform->SetAsMat4V(skeTrannsform, numNode);
}

std::shared_ptr<Light::render::Material> Light::render::SkeletonMaterial::Clone()
{
	return std::shared_ptr<Material>(new SkeletonMaterial());
}

void Light::render::SkeletonMaterial::GetUniform()
{
	assert(m_Pipeline != nullptr);
	m_pModelUniform = m_Pipeline->GetParam(uMODEL);
	m_pMVPUniform = m_Pipeline->GetParam(uMVP);


	m_uDiffuse = m_Pipeline->GetParam("mat.diffuse");
	m_uAmbient = m_Pipeline->GetParam("mat.ambient");
	m_uSpecular = m_Pipeline->GetParam("mat.specular");
	m_uCubeTex = m_Pipeline->GetParam(uCubeTex);
	m_uCameraPos = m_Pipeline->GetParam(uCameraPos);

	m_uKa = m_Pipeline->GetParam("mat.Ka");
	m_uKd = m_Pipeline->GetParam("mat.Kd");
	m_uKs = m_Pipeline->GetParam("mat.Ks");
	m_uShiness = m_Pipeline->GetParam("mat.shininess");
	m_uNumLight = m_Pipeline->GetParam("uNumLight");

	m_uDirLight.AddParam("Ia", m_Pipeline->GetParam("dLight.Ia"));
	m_uDirLight.AddParam("Id", m_Pipeline->GetParam("dLight.Id"));
	m_uDirLight.AddParam("Is", m_Pipeline->GetParam("dLight.Is"));
	m_uDirLight.AddParam("Direction", m_Pipeline->GetParam("dLight.Direction"));

	m_uSkeTransform = m_Pipeline->GetParam(uSkeTransform);
}

void Light::render::SkeletonMaterial::OnSceneCreate(std::shared_ptr<IEvent> event)
{
	events::EvtSceneCreate* pEvent = static_cast<events::EvtSceneCreate*>(event.get());
	Scene* pScene = static_cast<Scene*>(pEvent->m_pScene);

	m_pLightManager = pScene->GetLightManager();

	int numLight = m_pLightManager->GetNumPointLight();
	for (int i = 0; i < numLight; i++)
	{
		LightParam param;
		string num = { char('0' + i) };

		param.AddParam("Ia", m_Pipeline->GetParam(("pLights[" + num + "].Ia").c_str()));
		param.AddParam("Id", m_Pipeline->GetParam(("pLights[" + num + "].Id").c_str()));
		param.AddParam("Is", m_Pipeline->GetParam(("pLights[" + num + "].Is").c_str()));
		param.AddParam("Position", m_Pipeline->GetParam(("pLights[" + num + "].Pos").c_str()));
		param.AddParam("Constant", m_Pipeline->GetParam(("pLights[" + num + "].constant").c_str()));
		param.AddParam("Linear", m_Pipeline->GetParam(("pLights[" + num + "].linear").c_str()));
		param.AddParam("Quadratic", m_Pipeline->GetParam(("pLights[" + num + "].quadratic").c_str()));
		m_uPointLight.push_back(param);

	}
}
