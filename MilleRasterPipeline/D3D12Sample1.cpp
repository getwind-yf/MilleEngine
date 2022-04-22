#pragma once

#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h> 
#include <tchar.h> 

#include <wrl.h>
#include <strsafe.h> 
#include <dxgi1_6.h>
#include <DirectXMath.h> 
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcompiler.h> 

#if defined(_DEBUG) 
#include <dxgidebug.h>
#endif 

using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace DirectX; 

// Linker 
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib") 

#define GRS_WND_CLASS_NAME _T("GRS Game Window Class") 
#define GRS_WND_TITLE _T("GRS DirectX12 Trigger Sample") 

#define GRS_THROW_IF_FAILED(hr) {HRESULT _hr =(hr); if(FAILED(_hr)) {throw CGRSCOMException(_hr); }}

class CGRSCOMException
{
public:
	CGRSCOMException(HRESULT hr) : m_hrError(hr)
	{
	}
	HRESULT Error() const
	{
		return m_hrError; 
	}
private:
	const HRESULT m_hrError; 
};

struct GRS_VERTEX
{
	XMFLOAT4 m_vtPos; 
	XMFLOAT4 m_vtColor; 
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	const UINT  nFrameBackBufCount = 3u; 
	int iWidth = 1024; 
	int iHeight = 768; 
	UINT nFrameIndex = 0; 

	DXGI_FORMAT emRenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM; 
	const float faClearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f }; 
	UINT nDXGIFactoryFlags = 0U; 
	UINT nRTVDescriptorSize = 0U; 

	HWND hWnd = nullptr; 
	MSG msg = {}; 
	TCHAR pszAppPath[MAX_PATH] = {}; 

	float fTriangleSize = 3.0f; 

	D3D12_VERTEX_BUFFER_VIEW stVertexBufferView = {}; 

}