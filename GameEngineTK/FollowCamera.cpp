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
}


FollowCamera::~FollowCamera()
{

}

void FollowCamera::Update()
{
	// ���_�E�Q�Ɠ_
	Vector3 eyepos, refpos;

	// �Q�Ɠ_�̌v�Z
	refpos = m_targetPos + Vector3(0, 2, 0);
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);

	// �J�������w��ɗ���悤��
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	// �J�������W�̌v�Z
	eyepos = refpos + cameraV;

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
