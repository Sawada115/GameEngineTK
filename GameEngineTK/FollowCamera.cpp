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
	// 視点・参照点
	Vector3 eyepos, refpos;

	// 参照点の計算
	refpos = m_targetPos + Vector3(0, 2, 0);
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);

	// カメラが背後に来るように
	Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	// カメラ座標の計算
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);
	SetRefPos(refpos);

	// 基底クラスの更新
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
