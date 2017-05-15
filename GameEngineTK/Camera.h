//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   .h
//!
//! @brief  カメラのヘッダーファイル
//!
//! @date   日付　2017//
//!
//! @author 制作者名 澤田
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダーファイルのインクルード==============================================
#include <d3d11.h>
#include <SimpleMath.h>


//クラスの宣言
class Camera
{
public:
	Camera(int w, int h);
	virtual ~Camera();
	virtual void Update();

	// ビュー行列を取得
	DirectX::SimpleMath::Matrix GetViewMatrix();

	// プロジェクション行列を取得
	DirectX::SimpleMath::Matrix GetProjMatrix();

	// メンバー変数の変更関数 ==========================================================
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);
	void SetFovY(const float& fovY);
	void SetAspect(const float& aspect);
	void SetNearClip(const float& nearClip);
	void SetFarClip(const float& farClip);

	// メンバー変数の取得関数 ==========================================================
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
	DirectX::SimpleMath::Matrix m_view;		// ビュー行列
	DirectX::SimpleMath::Matrix m_proj;		// プロジェクション行列

	DirectX::SimpleMath::Vector3 m_eyepos;	// 視点
	DirectX::SimpleMath::Vector3 m_refpos;	// 参照点
	DirectX::SimpleMath::Vector3 m_upvec;	// 上方向ベクトル

	float m_fovY;							// 垂直方向視野角
	float m_aspect;							// アスペクト比
	float m_nearClip;						// 手前描画距離制限
	float m_farClip;						// 奥描画距離制限
};