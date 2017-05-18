#include "pch.h"
#include "FollowCamera.h"

using namespace DirectX;
using namespace SimpleMath;

const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int w, int h)
	:Camera(w, h)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;
	m_cameraFlug = false;
}


FollowCamera::~FollowCamera()
{

}

void FollowCamera::Update()
{
	Keyboard::State keyboardstate = m_keyboard->GetState();
	m_keyboardTracker.Update(keyboardstate);

	// c�L�[�ŃJ�����̐؂�ւ�
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		// �t���O��؂�ւ���
		m_cameraFlug = !m_cameraFlug;
	}

	// ���_�E�Q�Ɠ_
	Vector3 eyepos, refpos;
	if (m_cameraFlug)
	{
		// tps�J���� ====================================================================================
		// �Q�Ɠ_�̌v�Z
		refpos = m_targetPos + Vector3(0, 2, 0);
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);

		// �J�������w��ɗ���悤��
		Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// �J�������W�̌v�Z
		eyepos = refpos + cameraV;

		// ���_�����݈ʒu�����Ԃ���
		eyepos = m_eyepos + (eyepos - m_eyepos)*0.05f;
		// �Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos = m_refpos + (refpos - m_refpos)*0.2f;
	}
	// ===============================================================================================
	else
	{
		// �Q�Ɠ_�̌v�Z
		eyepos = m_targetPos + Vector3(0, 0.2, 0);
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);

		// �J�������w��ɗ���悤��
		Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// �J�������W�̌v�Z
		refpos = eyepos + cameraV;
	}
	SetEyePos(eyepos);
	SetRefPos(refpos);

	// ���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & target)
{
	m_targetPos = target;
}

void FollowCamera::SetTargetAngle(float angle)
{
	m_targetAngle = angle;
}

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}

bool FollowCamera::GetCameraFlug()
{
	return m_cameraFlug;
}
