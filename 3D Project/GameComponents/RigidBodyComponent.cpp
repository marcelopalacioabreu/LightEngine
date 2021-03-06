#include <pch.h>
#include "RigidBodyComponent.h"
#include "collidercomponent.h"
#include "..\Core\Physic.h"
#include "..\Interface\IActor.h"
#include "..\Utilities\PhysicsUtilities.h"
#include <glm\gtx\matrix_decompose.hpp>

namespace Light
{
namespace physics
{
RigidBodyComponent::RigidBodyComponent(void):m_pRigidBody(nullptr),m_RigidBodyLocation(0),m_RigidBodyOrientation(),m_CustomGravity(0),m_bUseGravity(true)
{
}

RigidBodyComponent::~RigidBodyComponent(void)
{
	m_pPhysic->VRemoveActor(m_pOwner->VGetId());
}

tinyxml2::XMLElement * RigidBodyComponent::VDeserialize(tinyxml2::XMLDocument * doc)
{
	return nullptr;
}

bool RigidBodyComponent::VSerialize(IContext* pContext,const tinyxml2::XMLElement* pData)
{
	m_pPhysic = pContext->GetSystem<IGamePhysic>();
	if (!pData) return false;
	// mass
	const tinyxml2::XMLElement* pMass = pData->FirstChildElement("Mass");
	if (pMass)
		this->m_fMass = pMass->DoubleAttribute("value");

	// material
	const tinyxml2::XMLElement* pMaterial = pData->FirstChildElement("PhysicsMaterial");
	if (pMaterial)
		this->m_material = pMaterial->FirstChild()->Value();

	// initial transform
	const tinyxml2::XMLElement* pRigidBodyTransform = pData->FirstChildElement("RigidBodyTransform");
	if (pRigidBodyTransform)
		BuildRigidBodyTransform(pRigidBodyTransform);

	PostInit();
	return true;
}

void RigidBodyComponent::PostInit(void)
{
	ITransformComponent* pTransformComponent = m_pOwner->GetComponent<ITransformComponent>();
	ColliderComponent* pCollider = static_cast<ColliderComponent*>(m_pOwner->GetComponent<IColliderComponent>());
	
	btCollisionShape* pShape = pCollider->m_pCollisionShape;
	ActorMotionState * myMotionState = nullptr;

	glm::mat4 transform;

	
	transform = glm::translate(glm::mat4(), pTransformComponent->GetPos()) * glm::mat4_cast(pTransformComponent->GetOrientation());
	transform = glm::translate(glm::mat4(), m_RigidBodyLocation) * glm::mat4_cast(m_RigidBodyOrientation) * transform;

	if (pCollider->GetType() == SHAPE_TERRAIN)
	{
		vec2 mm = pCollider->GetMinMax();
		float off = (mm.x + mm.y) / 2.0;
		mat4 m;
		m = glm::translate(m, vec3(0, off, 0));
		myMotionState = DEBUG_NEW ActorMotionState((transform*m));
	}
	
	else myMotionState = DEBUG_NEW ActorMotionState(transform);
	

	// localInertia defines how the object's mass is distributed
	btVector3 localInertia(0.f, 0.f, 0.f);
	if (m_fMass > 0.001f)
		pShape->calculateLocalInertia(m_fMass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_fMass, myMotionState, pShape, localInertia);

	// lookup the material
	MaterialData material(static_cast<BulletPhysics*>(m_pPhysic)->LookupMaterialData(m_material));

	// set up the materal properties
	rbInfo.m_restitution = material.m_restitution;
	rbInfo.m_friction = material.m_friction;

	m_pRigidBody = new btRigidBody(rbInfo);
	m_pRigidBody->setUserPointer(this);

	if (pCollider->GetType() == SHAPE_CHARACTER)
	{
		this->SetAngularFactor(vec3(0));
		m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
	}
	else if (pCollider->GetType() == SHAPE_TRIANGLEMESH)
	{
		//enable custom material callback
		m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	}
	ActorId actorID = m_pOwner->VGetId();
	m_pPhysic->AddRigidBody(actorID, this);

}


btMotionState * RigidBodyComponent::GetMotionState()
{
	return m_pRigidBody->getMotionState();
}

btRigidBody * RigidBodyComponent::GetRigidBody()
{
	return m_pRigidBody;
}

const btTransform & RigidBodyComponent::GetTransform()
{
	return m_pRigidBody->getCenterOfMassTransform();
}

vec3 RigidBodyComponent::GetLinearVelocity()
{
	return ToVector3(m_pRigidBody->getLinearVelocity());
}

void RigidBodyComponent::SetLinearVelocity(const vec3 & velocity)
{
	Activate();
	m_pRigidBody->setLinearVelocity(ToBtVector3(velocity));
}

void RigidBodyComponent::SetLinearFactor(const vec3 & factor)
{
	m_pRigidBody->setLinearFactor(ToBtVector3(factor));
}

void RigidBodyComponent::SetLinearDamping(float damping)
{
	m_pRigidBody->setDamping(damping, m_pRigidBody->getAngularDamping());
}

void RigidBodyComponent::SetAngularVelocity(const vec3 & angularVelocity)
{
	Activate();
	m_pRigidBody->setAngularVelocity(ToBtVector3(angularVelocity));

}

void RigidBodyComponent::SetAngularFactor(const vec3 & factor)
{
	m_pRigidBody->setAngularFactor(ToBtVector3(factor));
}

void RigidBodyComponent::SetAngularDamping(float factor)
{
	m_pRigidBody->setDamping(m_pRigidBody->getLinearDamping(), factor);
}

void RigidBodyComponent::SetFriction(float friction)
{
	m_pRigidBody->setFriction(friction);
}

void RigidBodyComponent::SetAnisotropicFriction(const vec3 & friction)
{
	m_pRigidBody->setAnisotropicFriction(ToBtVector3(friction));
}

void RigidBodyComponent::SetRestitution(float restitution)
{
	m_pRigidBody->setRestitution(restitution);
}

void RigidBodyComponent::SetContactProcessingThreshold(float threshold)
{
	m_pRigidBody->setContactProcessingThreshold(threshold);
}

void RigidBodyComponent::SetCcdRadius(float radius)
{
	m_pRigidBody->setCcdSweptSphereRadius(radius);
}

void RigidBodyComponent::SetCcdMotionThreshold(float threshold)
{
	m_pRigidBody->setCcdMotionThreshold(threshold);
}

void RigidBodyComponent::SetUseGravity(bool enable)
{
	if (m_bUseGravity != enable)
	{
		m_bUseGravity = enable;
		UpdateGravity();
	}
}

void RigidBodyComponent::SetCustomGravity(const vec3 & gravity)
{
	if (m_CustomGravity != gravity)
	{
		m_CustomGravity = gravity;
		UpdateGravity();
	}
}

void RigidBodyComponent::SetTrigger(bool enable)
{
	int flags = m_pRigidBody->getCollisionFlags();
	if (enable) flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
	else flags &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;
	m_pRigidBody->setCollisionFlags(flags);
}



void RigidBodyComponent::BuildRigidBodyTransform(const tinyxml2::XMLElement * pTransformElement)
{
	const tinyxml2::XMLElement* pPositionElement = pTransformElement->FirstChildElement("Position");
	if (pPositionElement)
	{
		double x = pPositionElement->DoubleAttribute("x");
		double y = pPositionElement->DoubleAttribute("y");
		double z = pPositionElement->DoubleAttribute("z");
		m_RigidBodyLocation = vec3(x, y, z);
	}

	const tinyxml2::XMLElement* pOrientationElement = pTransformElement->FirstChildElement("Orientation");
	if (pOrientationElement)
	{
		double yaw = pPositionElement->DoubleAttribute("yaw");
		double pitch = pPositionElement->DoubleAttribute("pitch");
		double roll = pPositionElement->DoubleAttribute("roll");


		m_RigidBodyOrientation = glm::toQuat(glm::orientate3(vec3((float)glm::radians(yaw), (float)glm::radians(pitch), (float)glm::radians(roll))));
	}

}

void RigidBodyComponent::ApplyForce(const vec3 & force)
{
	Activate();
	m_pRigidBody->applyCentralForce(ToBtVector3(force));
}

void RigidBodyComponent::ApplyForce(const vec3 & force, const vec3 & position)
{
	Activate();
	m_pRigidBody->applyForce(ToBtVector3(force), ToBtVector3(position - m_CenterOfMass));
}

void RigidBodyComponent::ApplyTorque(const vec3 & torque)
{
	Activate();
	m_pRigidBody->applyTorque(ToBtVector3(torque));
}

void RigidBodyComponent::ApplyImpulse(const vec3 & impulse)
{
	Activate();
	m_pRigidBody->applyCentralImpulse(ToBtVector3(impulse));
}

void RigidBodyComponent::ApplyImpulse(const vec3 & impulse, const vec3 & position)
{
	Activate();
	m_pRigidBody->applyImpulse(ToBtVector3(impulse), ToBtVector3(position - m_CenterOfMass));
}

void RigidBodyComponent::ApplyTorqueImpulse(const vec3 & torque)
{
	Activate();
	m_pRigidBody->applyTorqueImpulse(ToBtVector3(torque));
}

void RigidBodyComponent::ResetForces()
{
	m_pRigidBody->clearForces();
}

void RigidBodyComponent::Activate()
{
	m_pRigidBody->activate(true);
}

void RigidBodyComponent::UpdateGravity()
{
	

	int flag = m_pRigidBody->getFlags();

	// first set flag for bullet physic
	if (m_bUseGravity && m_CustomGravity == vec3(0))
		flag &= ~BT_DISABLE_WORLD_GRAVITY;		// turn on gravity
	else flag |= BT_DISABLE_WORLD_GRAVITY;		// turn off gravity
	m_pRigidBody->setFlags(flag);

	// second set gravity
	if (m_bUseGravity)
	{
		if (m_CustomGravity == vec3(0))
			m_pRigidBody->setGravity(ToBtVector3(m_pPhysic->VGetGravity()));
		else m_pRigidBody->setGravity(ToBtVector3(m_CustomGravity));
	}
	else m_pRigidBody->setGravity(btVector3(0, 0, 0));
}
}
}