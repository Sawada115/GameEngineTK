//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   .h
//!
//! @brief  �Ǐ]�J�����̃w�b�_�[�t�@�C��
//!
//! @date   ���t�@2017//
//!
//! @author ����Җ� �V�c
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once
#include "Camera.h"

class FollowCamera :public Camera
{
public:
	FollowCamera(int w, int h);
	~FollowCamera();

	void Update() override;
	void SetTargetPos(const DirectX::SimpleMath::Vector3& target);
	void SetTargetAngle(float angle);

	// �J��������
	static const float CAMERA_DISTANCE;
private:
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;

	// �Ǐ]�Ώۂ̊p�x
	float m_targetAngle;
};

