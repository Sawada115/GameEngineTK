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

	// cキーでカメラの切り替え
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		// フラグを切り替える
		m_cameraFlug = !m_cameraFlug;
	}

	// 視点・参照点
	Vector3 eyepos, refpos;
	if (m_cameraFlug)
	{
		// tpsカメラ ====================================================================================
		// 参照点の計算
		refpos = m_targetPos + Vector3(0, 2, 0);
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);

		// カメラが背後に来るように
		Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// カメラ座標の計算
		eyepos = refpos + cameraV;

		// 視点を現在位置から補間する
		eyepos = m_eyepos + (eyepos - m_eyepos)*0.05f;
		// 参照点を現在位置から補間する
		refpos = m_refpos + (refpos - m_refpos)*0.2f;
	}
	// ===============================================================================================
	else
	{
		// 参照点の計算
		eyepos = m_targetPos + Vector3(0, 0.2, 0);
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);

		// カメラが背後に来るように
		Matrix rotmat = Matrix::CreateRotationY(XMConvertToRadians(m_targetAngle));
		cameraV = Vector3::TransformNormal(cameraV, rotmat);

		// カメラ座標の計算
		refpos = eyepos + cameraV;
	}
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

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}

bool FollowCamera::GetCameraFlug()
{
	return m_cameraFlug;
}
