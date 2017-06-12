#pragma once
#include "..\pch.h"
class Actor;
class ActorComponent;

//class TiXmlElement;

class Actor: public ISceneNode
{
public:
	typedef std::vector<ISceneNode*> ActorList;
	typedef std::map<ComponentId, ActorComponent*> ActorComponents;
protected:
	ActorList				m_Children;
	Actor*					m_pParent;
	Shader*					m_pShader;
	string					m_Name;

private:
	ActorId m_id;					// unique id for the actor
	ActorComponents m_components;	// all components this actor has

public:
	Actor(ActorId id);
	~Actor();

	bool Init(tinyxml2::XMLElement* pData);
	void PostInit(void);
	void Destroy(void);

	virtual void VSetName(string name) { m_Name = name; }
	virtual string VGetName() {	return m_Name;	};
	virtual void VSetTransform(const mat4 *toWorld);
	virtual mat4 VGetTransform();
	virtual Shader* VGetShader() ;
	virtual void VSetShader(Shader* p) ;
	virtual HRESULT VOnUpdate(Scene *, DWORD const elapsedMs);

	virtual HRESULT VPreRender(Scene *pScene);
	virtual bool VIsVisible(Scene *pScene) const { return true; };
	virtual HRESULT VRenderChildren(Scene *pScene);
	virtual HRESULT VPostRender(Scene *pScene);
	virtual HRESULT VRender(Scene *pScene);

	virtual bool VAddChild(Actor* kid);
	virtual bool VRemoveChild(ActorId id);
	virtual Actor* VGetParent();
	// accessors
	ActorId GetId(void) const { return m_id; }
	template<class ComponentType>ComponentType* GetComponent(ComponentId id);
	template<class ComponentType>ComponentType* GetComponent(const char*  name);

	const ActorComponents* GetComponents() { return &m_components; }

	void AddComponent(ActorComponent* pComponent);

	
	
};

template<class ComponentType>
inline ComponentType * Actor::GetComponent(ComponentId id)
{
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second);
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}

template<class ComponentType>
inline ComponentType * Actor::GetComponent(const char * name)
{
	ComponentId id = ActorComponent::GetIdFromName(name);
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second);
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}

