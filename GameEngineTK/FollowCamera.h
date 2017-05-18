//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   .h
//!
//! @brief  追従カメラのヘッダーファイル
//!
//! @date   日付　2017//
//!
//! @author 制作者名 澤田
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once
#include "Camera.h"
#include <Keyboard.h>

class FollowCamera :public Camera
{
public:
	FollowCamera(int w, int h);
	~FollowCamera();

	void Update() override;
	void SetTargetPos(const DirectX::SimpleMath::Vector3& target);
	void SetTargetAngle(float angle);
	void SetKeyboard(DirectX::Keyboard* keyboard);
	bool GetCameraFlug();
	// カメラ距離
	static const float CAMERA_DISTANCE;
private:
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;

	// 追従対象の角度
	float m_targetAngle;

	// キーボード
	DirectX::Keyboard* m_keyboard;
	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	bool m_cameraFlug;
};

