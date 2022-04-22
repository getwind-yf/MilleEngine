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

	UINT64 n64FenceValue = 0ui64; 
	HANDLE hEventFence = nullptr; 
	
	D3D12_VIEWPORT stViewPort = { 0.0f, 0.0f, static_cast<float>(iWidth),static_cast<float>(iHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH }; 
	D3D12_RECT     stScissorRect = { 0, 0, static_cast<LONG>(iWidth), static_cast<LONG>(iHeight) }; 

	D3D_FEATURE_LEVEL  emFeatureLevel = D3D_FEATURE_LEVEL_12_1; 

	ComPtr<IDXGIFactory5>		pIDXGIFactory5; 
	ComPtr<IDXGIAdapter1>		pIAdapter1; 
	ComPtr<ID3D12Device4>		pID3D12Device4; 
	ComPtr<ID3D12CommandQueue>	pICMDQueue; 
	ComPtr<IDXGISwapChain1>		pISwapChain1; 
	ComPtr<IDXGISwapChain3>		pISwapChain3;
	ComPtr<ID3D12DescriptorHeap> pIRTVHeap; 
	ComPtr<ID3D12Resource>		pIARenderTargets[nFrameBackBufCount]; 
	ComPtr<ID3D12RootSignature>		pIRootSignature; 
	ComPtr<ID3D12PipelineState>		pIPipelineStates; 

	ComPtr<ID3D12CommandAllocator>	pICMDAlloc; 
	ComPtr<ID3D12GraphicsCommandList> pICMDList; 
	ComPtr<ID3D12Resource>		pIVertexBuffer; 
	ComPtr<ID3D12Fence>		pIFence; 

	try
	{
		{
			if (0 == ::GetModuleFileName(nullptr, pszAppPath, MAX_PATH))
			{
				GRS_THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError())); 
			}

			WCHAR* lastSlash = _tcsrchr(pszAppPath, _T('\\')); 
			if (lastSlash)
			{
				*(lastSlash) = _T('\0'); 
			}

			lastSlash = _tcsrchr(pszAppPath, _T('\\')); 
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//删除x64路径
				*(lastSlash) = _T('\0');
			}

			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//删除Debug 或 Release路径
				*(lastSlash + 1) = _T('\0');
			}
		}

		// create window 
		{
			WNDCLASSEX wcex = {}; 
			wcex.cbSize = sizeof(WNDCLASSEX); 
			wcex.style = CS_GLOBALCLASS; 
			wcex.lpfnWndProc = WndProc; 
			wcex.cbClsExtra = 0; 
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); 
			wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); 
			wcex.lpszClassName = GRS_WND_CLASS_NAME; 
			RegisterClassEx(&wcex); 
			
			DWORD dwWndStyle = WS_OVERLAPPED | WS_SYSMENU; 
			RECT rtWnd = { 0, 0, iWidth, iHeight }; 
			AdjustWindowRect(&rtWnd, dwWndStyle, FALSE); 

			// Screen Position 
			INT posX = (GetSystemMetrics(SM_CXSCREEN) - rtWnd.right - rtWnd.left) / 2; 
			INT posY = (GetSystemMetrics(SM_CYSCREEN) - rtWnd.bottom - rtWnd.top) / 2; 

			hWnd = CreateWindowW(GRS_WND_CLASS_NAME
				, GRS_WND_TITLE
				, dwWndStyle
				, posX
				, posY
				, rtWnd.right - rtWnd.left
				, rtWnd.bottom - rtWnd.top
				, nullptr
				, nullptr
				, hInstance
				, nullptr); 
			
			if (!hWnd)
			{
				return FALSE; 
			}

			ShowWindow(hWnd, nCmdShow); 
			UpdateWindow(hWnd); 
		
		}

		// 打开调试
		{
#if defined(_DEBUG)
			ComPtr<ID3D12Debug> debugController; 
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer(); 
				nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}

#endif
		}

		// Create DXGI Factory object 
		{
			GRS_THROW_IF_FAILED(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&pIDXGIFactory5))); 
			GRS_THROW_IF_FAILED(pIDXGIFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
		}

		// Adaptor Object 
		{
			DXGI_ADAPTER_DESC1 stAdapterDesc = {}; 
			for (UINT adapterIndex =0; DXGI_ERROR_NOT_FOUND!= pIDXGIFactory5->EnumAdapters1(adapterIndex,&pIAdapter1); ++adapterIndex)
			{
				pIAdapter1->GetDesc1(&stAdapterDesc);

				if (stAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					continue; 
				}

				if (SUCCEEDED(D3D12CreateDevice(pIAdapter1.Get(), emFeatureLevel, _uuidof(ID3D12Device), nullptr)))
				{
					break; 
				}
			}

			// Create D3D12 Device 
		}


	}
}