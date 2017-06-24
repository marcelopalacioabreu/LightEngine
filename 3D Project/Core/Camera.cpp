#include "..\pch.h"



void Camera::UpdateVector()
{

	vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	

	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	
}

Camera::Camera():m_pCameraC(nullptr)
{
	m_Position = vec3(0, 0, 200);
	m_Front = glm::normalize(vec3(0, 0, 0) - m_Position);
	WorldUp = vec3(0, 1, 0);
	m_Speed = 50.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	MouseSensitivity = 0.25;
	UpdateVector();
}

Camera::Camera(const vec3 & pos, const vec3 & target, const vec3 & up) :m_pCameraC(nullptr)
{
	m_Position = pos;
	m_Front = glm::normalize(target - m_Position);;
	WorldUp = up;
	m_Speed = 50.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	MouseSensitivity = 0.25;
	UpdateVector();
}


Camera::~Camera()
{
}

void Camera::Update(float deltaTIme)
{
	// Camera controls
	if (gInput()->KeyDown(DIK_W)) m_Position += m_Front*m_Speed*deltaTIme;
	if (gInput()->KeyDown(DIK_S)) m_Position -= m_Front*m_Speed*deltaTIme;
	if (gInput()->KeyDown(DIK_D)) m_Position += m_Right*m_Speed*deltaTIme;
	if (gInput()->KeyDown(DIK_A)) m_Position -= m_Right*m_Speed*deltaTIme;

	m_Pitch += gInput()->mouseDY()*MouseSensitivity;
	m_Yaw += gInput()->mouseDX()*MouseSensitivity;
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	UpdateVector();

	//cout << m_Position << endl;
}



mat4 Camera::GetViewMatrix()
{
	mat4 view;
	//mat4 view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	if(m_pCameraC!=nullptr) view = m_pCameraC->GetViewMatrix();
	else view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	return view;
}

void Camera::SetCameraActor(Actor * p)
{
	m_pCameraC = p->GetComponent<CameraComponent>(CameraComponent::Name);
}
