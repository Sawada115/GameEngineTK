//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   .cpp
//!
//! @brief  �̃\�[�X�t�@�C��
//!
//! @date   ���t�@2017//
//!
//! @author ����Җ� �V�c
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�[�t�@�C���̃C���N���[�h==============================================
#include "pch.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

Camera::Camera(int w, int h)
{
	// �����o�ϐ��̏����� ===============================================================================
	// �r���[�s��̌v�Z
	m_eyepos =Vector3(0, 0, 5);
	m_refpos = Vector3(0, 0, -100);
	m_upvec = Vector3(0, 1, 0);
	m_upvec.Normalize();
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	// �v���W�F�N�V�����s��̌v�Z
	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)w / h;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}


Camera::~Camera()
{
}

void Camera::Update()
{
	// �r���[�s��̌v�Z
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	// �v���W�F�N�V�����s��̌v�Z
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}

DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

DirectX::SimpleMath::Matrix Camera::GetProjMatrix()
{
	return m_proj;
}

void Camera::SetEyePos(const DirectX::SimpleMath::Vector3& eyepos)
{
	m_eyepos = eyepos;
}

void Camera::SetRefPos(const DirectX::SimpleMath::Vector3& refpos)
{
	m_refpos = refpos;
}

void Camera::SetUpVec(const DirectX::SimpleMath::Vector3& upvec)
{
	m_upvec = upvec;
}

void Camera::SetFovY(const float& fovY)
{
	m_fovY = fovY;
}

void Camera::SetAspect(const float& aspect)
{
	m_aspect = aspect;
}

void Camera::SetNearClip(const float& nearClip)
{
	m_nearClip = nearClip;
}

void Camera::SetFarClip(const float& farClip)
{
	m_farClip = farClip;
}

const DirectX::SimpleMath::Matrix & Camera::GetView()
{
	return m_view;
}

const DirectX::SimpleMath::Matrix & Camera::GetProj()
{
	return m_proj;
}

DirectX::SimpleMath::Vector3 Camera::GetEyePos()
{
	return m_eyepos;
}

DirectX::SimpleMath::Vector3 Camera::GetRefPos()
{
	return m_refpos;
}

DirectX::SimpleMath::Vector3 Camera::GetUpVec()
{
	return m_upvec;
}

float Camera::GetFovY()
{
	return m_fovY;
}

float Camera::GetAspect()
{
	return m_aspect;
}

float Camera::GetNearClip()
{
	return m_nearClip;
}

float Camera::GetFarClip()
{
	return m_farClip;
}
