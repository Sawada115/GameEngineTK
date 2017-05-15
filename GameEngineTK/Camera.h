//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   .h
//!
//! @brief  �J�����̃w�b�_�[�t�@�C��
//!
//! @date   ���t�@2017//
//!
//! @author ����Җ� �V�c
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�[�t�@�C���̃C���N���[�h==============================================
#include <d3d11.h>
#include <SimpleMath.h>


//�N���X�̐錾
class Camera
{
public:
	Camera(int w, int h);
	virtual ~Camera();
	virtual void Update();

	// �r���[�s����擾
	DirectX::SimpleMath::Matrix GetViewMatrix();

	// �v���W�F�N�V�����s����擾
	DirectX::SimpleMath::Matrix GetProjMatrix();

	// �����o�[�ϐ��̕ύX�֐� ==========================================================
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);
	void SetFovY(const float& fovY);
	void SetAspect(const float& aspect);
	void SetNearClip(const float& nearClip);
	void SetFarClip(const float& farClip);

	// �����o�[�ϐ��̎擾�֐� ==========================================================
	const DirectX::SimpleMath::Matrix& GetView();
	const DirectX::SimpleMath::Matrix& GetProj();
	DirectX::SimpleMath::Vector3 GetEyePos();
	DirectX::SimpleMath::Vector3 GetRefPos();
	DirectX::SimpleMath::Vector3 GetUpVec();
	float GetFovY();
	float GetAspect();
	float GetNearClip();
	float GetFarClip();

protected:
	DirectX::SimpleMath::Matrix m_view;		// �r���[�s��
	DirectX::SimpleMath::Matrix m_proj;		// �v���W�F�N�V�����s��

	DirectX::SimpleMath::Vector3 m_eyepos;	// ���_
	DirectX::SimpleMath::Vector3 m_refpos;	// �Q�Ɠ_
	DirectX::SimpleMath::Vector3 m_upvec;	// ������x�N�g��

	float m_fovY;							// ������������p
	float m_aspect;							// �A�X�y�N�g��
	float m_nearClip;						// ��O�`�拗������
	float m_farClip;						// ���`�拗������
};