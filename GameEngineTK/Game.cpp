//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	// 変数の初期化 ================================================================================

	// プリミティブの初期化
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	// エフェクトと入力レイアウトの初期化

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	/*m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));*/

	// 3Dプロジェクションの作成
	/*Matrix proj = Matrix::CreateScale(2.f / float(m_outputWidth),
		-2.f / float(m_outputHeight), 1.f)
		* Matrix::CreateTranslation(-1.f, 1.f, 0.f);
	m_effect->SetProjection(proj);*/

	m_view = Matrix::CreateLookAt(Vector3(0.0f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight), 0.1f, 1000.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// デバッグカメラの生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// エフェクトファクトリー生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	// テクスチャの読み込みパス指定
	m_factory->SetDirectory(L"Resources");

	// モデルの読み込み
	m_modelGround = Model::CreateFromCMO(
		m_d3dDevice.Get(), 
		L"Resources\\ground2.cmo", 
		*m_factory);

	m_modelSkydome = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources\\skydome.cmo",
		*m_factory);

	m_modelHead = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources\\Head.cmo",
		*m_factory);

	/*m_modelSkydome2 = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources\\skydome2.cmo",
		*m_factory);*/

	m_modelTeapot = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		L"Resources\\Teapot.cmo",
		*m_factory);

	rot = 0.0f;

	for (int i = 0; i < 20; i++)
	{
		posRandom[i] = rand() % 100;
		rotRandom[i] = rand() % 360;
	}

	cnt = 0;

	// キーボードの初期化
	m_keyboard = std::make_unique<Keyboard>();

	m_tankRot = 0;

	// カメラの生成
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	// 毎フレーム更新処理
	//m_debugCamera->Update();

	m_camera->SetTargetPos(m_tankPos);
	m_camera->SetTargetAngle(m_tankRot);
	m_camera->Update();
	// 自機追従カメラ
	// ビュー行列の取得
	//m_view = m_debugCamera->GetCameraMatrix();
	m_view = m_camera->GetViewMatrix();
	// プロジェクション行列の取得
	m_proj = m_camera->GetProjMatrix();
	//m_camera->SetEyePos(m_tankPos);

	//// ビュー行列の計算
	//Vector3 eyepos(0, 0, 5.0f);		// 視点
	//Vector3 refpos(0, 0, 0);		// 参照点
	//static float angle = 0.0f;
	//angle += 0.1f;
	//Vector3 upvec(cosf(angle), sinf(angle), 0);			// 上方向ベクトル
	//upvec.Normalize();				// ベクトルの正規化(長さを１にする)
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);

	// プロジェクション行列の計算
	//float fovY = XMConvertToRadians(60.0f);						// 垂直方向視野角
	//float aspect = (float)m_outputWidth / m_outputHeight;		// アスペクト比
	//float nearClip = 0.1f;										// 手前描画距離制限
	//float farClip = 1000.0f;									// 奥描画距離制限
	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearClip, farClip);

	// 地面のワールド行列計算 ====================================================================
	for (int i = 0; i < 200 * 200; i++)
	{
		// 平行移動
		Matrix transmat = Matrix::CreateTranslation(i / 200 - 100, 0, i % 200 - 100);

		// 合成
		//m_worldGround[i] = transmat;
	}

	rot++;
	//// 球のワールド行列を計算 ====================================================================
	//for (size_t i = 0; i < 20; i++)
	//{
	//	// スケーリング
	//	Matrix scalemat;

	//	// 回転
	//	Matrix rotmatZ = Matrix::CreateRotationZ(XMConvertToRadians(0.0f));			// ロール
	//	Matrix rotmatX = Matrix::CreateRotationX(XMConvertToRadians(0.0f));			// ピッチ(仰角)
	//	Matrix rotmatY;																// ヨー(方位角)
	//	if (i < 10)
	//	{
	//		//scalemat = Matrix::CreateScale(0.8f);
	//		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(36.0f*i + rot));
	//	}
	//	else
	//		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(36.0f*i + rot*-1));

	//	// 回転の合成
	//	Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	//	// 平行移動
	//	Matrix transmat = Matrix::CreateTranslation(20.0f*(i / 10 + 1), 0.0f, 0.0f);

	//	// 合成
	//	m_worldBall[i] = scalemat * transmat * rotmat;
	//}

	cnt++;
	// ティーポットのワールド行列を計算 ====================================================================
	for (size_t i = 0; i < 20; i++)
	{
		// スケーリング
		Matrix scalemat;
		float val = (sinf(XMConvertToRadians(cnt)) + 1.0f)* 5.0f + 1.0f;
		scalemat = Matrix::CreateScale(val);

		// 回転
		Matrix rotmatY;																// ヨー(方位角)
		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(cnt));
		
		// 回転の合成
		Matrix rotmat = rotmatY;

		// 平行移動
		//posRandom[i] = posRandom[i] / (10.0f*60.0f);
		
		Matrix transmat;
		if (cnt <= 600)
		{
			transmat = Matrix::CreateTranslation(
				cosf(XMConvertToRadians(rotRandom[i]))*(1 - cnt / 60.0f / 10.0f)*posRandom[i] + cnt / 60.0f / 10 * 0,
				0,
				sinf(XMConvertToRadians(rotRandom[i]))*(1 - cnt / 60.0f / 10.0f)*posRandom[i] + cnt / 60.0f / 10 * 0);
		}
		m_worldTeapot[i] = transmat;

		// 合成
		m_worldTeapot[i] = rotmat*transmat;
	}

	// スケーリング
	//Matrix scalemat = Matrix::CreateScale(2.0f);

	// 回転
	//Matrix rotmatZ = Matrix::CreateRotationZ(XMConvertToRadians(0.0f));			// ロール
	//Matrix rotmatX = Matrix::CreateRotationX(XMConvertToRadians(0.0f));			// ピッチ(仰角)														
	//Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(-rot));			// ヨー(方位角)
	//
	// 回転の合成
	//Matrix rotmat = rotmatZ * rotmatX * rotmatY;

	// 合成
	//m_worldBall[20] = scalemat*rotmat;

	// キー操作で頭を動かす =====================================================================
	// キーボードの情報を取得
	Keyboard::State kb = m_keyboard->GetState();

	// Wキーが押されたら
	if (kb.W)
	{
		// 移動ベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルの回転
		moveV = Vector3::TransformNormal(moveV, m_worldTank);
		// 自機の座標を移動
		m_tankPos += moveV;
	}
	// Sキーが押されたら
	if (kb.S)
	{
		// 移動ベクトル
		Vector3 moveV(0, 0, 0.1f);
		// 移動ベクトルの回転
		moveV = Vector3::TransformNormal(moveV, m_worldTank);
		// 自機の座標を移動
		m_tankPos += moveV;
	}
	// Aキーが押されたら
	if (kb.A)
	{
		// 回転
		float rot = 1.0f;
		// 自機を回転
		m_tankRot += rot;
	}
	// Dキーが押されたら
	if (kb.D)
	{
		// 回転
		float rot = -1.0f;
		// 自機を回転
		m_tankRot += rot;
	}

	// タンクの旋回
	Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(m_tankRot));
	// 回転の合成
	Matrix rotmat = rotmatY;
	// タンクの平行移動
	Matrix transmat = Matrix::CreateTranslation(m_tankPos);
	// 合成
	m_worldTank = rotmat * transmat;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.

	// 描画処理 =====================================================================

	// 描画設定を作成
	CommonStates states(m_d3dDevice.Get());
	m_d3dContext->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(states.DepthNone(), 0);
	m_d3dContext->RSSetState(states.CullNone());

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);
	m_effect->SetWorld(m_world);

	// 描画設定を反映
	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// 描画処理 =============================================================
	// 地面モデルの描画
	//for (int i = 0; i < 200 * 200; i++)
	{
		m_modelGround->Draw(m_d3dContext.Get(), states, Matrix::Identity, m_view, m_proj);
	}

	// ティーポットの描画
	/*for (int i = 0; i < 20; i++)
	{
		m_modelTeapot->Draw(m_d3dContext.Get(), states,m_worldTeapot[i], m_view, m_proj);
	}*/
	
	/*for (int i = 0; i < 21; i++)
	{
		m_modelSkydome2->Draw(m_d3dContext.Get(), states, m_worldBall[i], m_view, m_proj);
	}*/

	m_modelSkydome->Draw(m_d3dContext.Get(), states, m_world, m_view, m_proj);

	// タンクの描画
	m_modelHead->Draw(m_d3dContext.Get(), states, m_worldTank, m_view, m_proj);
	m_batch->Begin();

	// 線の描画
	/*m_batch->DrawLine
	(
		VertexPositionColor(Vector3(0, 0, 0), Color(1, 1, 1)),
		VertexPositionColor(Vector3(800, 600, 0), Color(1, 1, 1))
	);*/

	//// 三角形の表示
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	/*VertexPositionColor v1(Vector3(0.0f, 500.0f, 0.0f), Colors::Yellow);
	VertexPositionColor v2(Vector3(500.0f, 0.0f, 0.0f), Colors::Yellow);
	VertexPositionColor v3(Vector3(0.0f, 0.0f, 0.0f), Colors::Yellow);*/

	//m_batch->DrawTriangle(v1, v2, v3);

	// 頂点インデックス
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3,
	};

	// 頂点座標
	VertexPositionNormal vertices[] =
	{
					// 座標					// 法線方向ベクトル
		{ Vector3(-1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(-1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
	};

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);


	m_batch->End();
	// ======================================================================

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}