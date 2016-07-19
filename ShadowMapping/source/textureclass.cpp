////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include <vector>

#include <windows.h>
#include <d3d11_1.h>
#include <DXGIType.h>
#include <DXGI.h>
#include <dxgi1_2.h>

//#include <dxgi1_3.h>
#include "textureclass.h"

using namespace std;

TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}


//from: http://stackoverflow.com/questions/5069104/fastest-method-of-screen-capturing
bool TextureClass::LoadDesktopBitmap(ID3D11Device* device)//, ID3D11DeviceContext* deviceContext) 
{
	HRESULT hr;
	//RECT user_window_rectangle;
	ID3D11Texture2D* tex = 0;

	HDC hdcScreen = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcScreen);

	UINT screenshot_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	UINT screenshot_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);

	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, screenshot_width, screenshot_height);

	SelectObject(hdc, hbmp);

	// copy from the desktop device context to the bitmap device context
	BitBlt(hdc, 0, 0, screenshot_width, screenshot_height, hdcScreen, 0, 0, SRCCOPY);

	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;
	bmih.biWidth = screenshot_width;
	bmih.biHeight = 0 - screenshot_height;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;

	int bytes_per_pixel = 32 / 8; // bmih.biBitCount / 8;

	BYTE *pixels = (BYTE*)malloc(bytes_per_pixel * screenshot_width * screenshot_height);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader = bmih;

	int row_count = GetDIBits(hdc, hbmp, 0, screenshot_height, pixels, &bmi, DIB_RGB_COLORS);

	ReleaseDC(NULL, hdc);

	// ..and delete the context you created
	DeleteDC(hdc);


	D3D11_TEXTURE2D_DESC screenshot_desc = CD3D11_TEXTURE2D_DESC(
		DXGI_FORMAT_B8G8R8A8_UNORM,     // format
		screenshot_width,               // width
		screenshot_height,              // height
		1,                              // arraySize
		1,                              // mipLevels
		D3D11_BIND_SHADER_RESOURCE,     // bindFlags
		D3D11_USAGE_DYNAMIC,            // usage
		D3D11_CPU_ACCESS_WRITE,         // cpuaccessFlags
		1,                              // sampleCount
		0,                              // sampleQuality
		0                               // miscFlags
	);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = pixels; // texArray; // &bmp.bmBits; //pixel buffer
	data.SysMemPitch = bytes_per_pixel * screenshot_width;// line size in byte
	data.SysMemSlicePitch = bytes_per_pixel * screenshot_width * screenshot_height;

	hr = device->CreateTexture2D(
		&screenshot_desc, //texture format
		&data,          // pixel buffer use to fill the texture
		&tex  // created texture
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = screenshot_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MostDetailedMip = screenshot_desc.MipLevels;

	device->CreateShaderResourceView(tex, NULL, &m_texture);

	return true;
#pragma region duplicationAPI
//Below is from: https://github.com/roxlu/screen_capture/blob/master/src/test/test_win_api_directx_research.cpp
// However, it works for windows 8 or later with Duplication API
//	IDXGIFactory1* factory = nullptr;
//	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&factory));
//
//	UINT i = 0;
//	IDXGIAdapter1* adapter = nullptr;
//	std::vector<IDXGIAdapter1*> adapters;
//
//	//while (DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter));
//	//{
//	hr = factory->EnumAdapters1(i, &adapter);
//	if (hr != S_OK)
//	{
//		cout << "EnumAdapters1 failed " << endl;
//		return false;
//	}
//		adapters.push_back(adapter);
//		++i;
//	//}
//
//	for (size_t i = 0; i < adapters.size(); i++)
//	{
//		DXGI_ADAPTER_DESC1 desc;
//		adapter = adapters[i];
//		hr = adapter->GetDesc1(&desc);
//
//		if (S_OK != hr) {
//			printf("Error: failed to get a description for the adapter: %lu\n", i);
//			continue;
//		}
//		else
//			break;
//
//		wprintf(L"Adapter: %lu, description: %s\n", i, desc.Description);
//	}
//
//	/* Check what devices/monitors are attached to the adapters. */
//	UINT dx = 0;
//	IDXGIOutput* output = NULL;
//	std::vector<IDXGIOutput*> outputs; /* Needs to be Released(). */
//
//	for (size_t i = 0; i < adapters.size(); ++i) {
//
//		dx = 0;
//		adapter = adapters[i];
//
//		while (DXGI_ERROR_NOT_FOUND != adapter->EnumOutputs(dx, &output)) {
//			printf("Found monitor %d on adapter: %lu\n", dx, i);
//			outputs.push_back(output);
//			++dx;
//		}
//	}
//
//	if (0 >= outputs.size()) {
//		printf("Error: no outputs found (%lu).\n", outputs.size());
//		exit(EXIT_FAILURE);
//	}
//
//	/* Print some info about the monitors. */
//	for (size_t i = 0; i < outputs.size(); ++i) {
//
//		DXGI_OUTPUT_DESC desc;
//		output = outputs[i];
//		hr = output->GetDesc(&desc);
//
//		if (S_OK != hr) {
//			printf("Error: failed to retrieve a DXGI_OUTPUT_DESC for output %lu.\n", i);
//			continue;
//		}
//
//		wprintf(L"Monitor: %s, attached to desktop: %c\n", desc.DeviceName, (desc.AttachedToDesktop) ? 'y' : 'n');
//	}
//
//	IDXGIOutput1* output1 = nullptr;
//	IDXGIOutputDuplication* duplication = nullptr;
//
//
//	{ /* Start IDGIOutputDuplication init. */
//
//		int use_monitor = 0;
//		if (use_monitor >= outputs.size()) {
//			printf("Invalid monitor index: %d, we only have: %lu - 1\n", use_monitor, outputs.size());
//			exit(EXIT_FAILURE);
//		}
//
//		output = outputs[use_monitor];
//		if (NULL == output) {
//			printf("No valid output found. The output is NULL.\n");
//			exit(EXIT_FAILURE);
//		}
//
//		hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
//		if (S_OK != hr) {
//			printf("Error: failed to query the IDXGIOutput1 interface.\n");
//			exit(EXIT_FAILURE);
//		}
//
//		//Sigh: Duplication API only works for windows8 and later.....
//		hr = output1->DuplicateOutput(device, &duplication);
//		if (S_OK != hr) {
//			printf("Error: failed to create the duplication output.\n");
//			//exit(EXIT_FAILURE);
//			return false;
//		}
//		printf("Queried the IDXGIOutput1.\n");
//
//	} /* End IDGIOutputDuplication init. */
//
//	if (NULL == duplication) {
//		printf("Error: okay, we shouldn't arrive here but the duplication var is NULL.\n");
//		return false;
//		//exit(EXIT_FAILURE);
//	}
//
//	/*
//	To download the pixel data from the GPU we need a
//	staging texture. Therefore we need to determine the width and
//	height of the buffers that we receive.
//
//	@TODO - We could also retrieve the width/height from the texture we got
//	from through the acquired frame (see the 'tex' variable below).
//	That may be a safer solution.
//	*/
//	DXGI_OUTPUT_DESC output_desc;
//	{
//		hr = output->GetDesc(&output_desc);
//		if (S_OK != hr) {
//			printf("Error: failed to get the DXGI_OUTPUT_DESC from the output (monitor). We need this to create a staging texture when downloading the pixels from the gpu.\n");
//			exit(EXIT_FAILURE);
//		}
//
//		printf("The monitor has the following dimensions: left: %d, right: %d, top: %d, bottom: %d.\n"
//			, (int)output_desc.DesktopCoordinates.left
//			, (int)output_desc.DesktopCoordinates.right
//			, (int)output_desc.DesktopCoordinates.top
//			, (int)output_desc.DesktopCoordinates.bottom
//		);
//	}
//
//	if (0 == output_desc.DesktopCoordinates.right
//		|| 0 == output_desc.DesktopCoordinates.bottom)
//	{
//		printf("The output desktop coordinates are invalid.\n");
//		exit(EXIT_FAILURE);
//	}
//
//	/* Create the staging texture that we need to download the pixels from gpu. */
//	D3D11_TEXTURE2D_DESC tex_desc;
//	tex_desc.Width = output_desc.DesktopCoordinates.right;
//	tex_desc.Height = output_desc.DesktopCoordinates.bottom;
//	tex_desc.MipLevels = 1;
//	tex_desc.ArraySize = 1; /* When using a texture array. */
//	tex_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; /* This is the default data when using desktop duplication, see https://msdn.microsoft.com/en-us/library/windows/desktop/hh404611(v=vs.85).aspx */
//	tex_desc.SampleDesc.Count = 1; /* MultiSampling, we can use 1 as we're just downloading an existing one. */
//	tex_desc.SampleDesc.Quality = 0; /* "" */
//	tex_desc.Usage = D3D11_USAGE_STAGING;
//	tex_desc.BindFlags = 0;
//	tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//	tex_desc.MiscFlags = 0;
//
//	ID3D11Texture2D* staging_tex = NULL;
//	hr = device->CreateTexture2D(&tex_desc, NULL, &staging_tex);
//	if (E_INVALIDARG == hr) {
//		printf("Error: received E_INVALIDARG when trying to create the texture.\n");
//		exit(EXIT_FAILURE);
//	}
//	else if (S_OK != hr) {
//		printf("Error: failed to create the 2D texture, error: %d.\n", hr);
//		exit(EXIT_FAILURE);
//	}
//
//	/*
//	Get some info about the output duplication.
//	When the DesktopImageInSystemMemory is TRUE you can use
//	the MapDesktopSurface/UnMapDesktopSurface directly to retrieve the
//	pixel data. If not, then you need to use a surface.
//	*/
//	DXGI_OUTDUPL_DESC duplication_desc;
//	duplication->GetDesc(&duplication_desc);
//	printf("duplication desc.DesktopImageInSystemMemory: %c\n", (duplication_desc.DesktopImageInSystemMemory) ? 'y' : 'n');
//
//	/* Access a couple of frames. */
//	DXGI_OUTDUPL_FRAME_INFO frame_info;
//	IDXGIResource* desktop_resource = NULL;
//	ID3D11Texture2D* tex = NULL;
//	DXGI_MAPPED_RECT mapped_rect;
//
//	for (int i = 0; i < 1; ++i) {
//		//  printf("%02d - ", i);
//
//		hr = duplication->AcquireNextFrame(1000, &frame_info, &desktop_resource);
//		if (DXGI_ERROR_ACCESS_LOST == hr) {
//			printf("Received a DXGI_ERROR_ACCESS_LOST.\n");
//		}
//		else if (DXGI_ERROR_WAIT_TIMEOUT == hr) {
//			printf("Received a DXGI_ERROR_WAIT_TIMEOUT.\n");
//		}
//		else if (DXGI_ERROR_INVALID_CALL == hr) {
//			printf("Received a DXGI_ERROR_INVALID_CALL.\n");
//		}
//		else if (S_OK == hr) {
//			//printf("Yay we got a frame.\n");
//
//			/* Print some info. */
//			//printf("frame_info.TotalMetadataBufferSize: %u\n", frame_info.TotalMetadataBufferSize);
//			//printf("frame_info.AccumulatedFrames: %u\n", frame_info.AccumulatedFrames);
//
//			/* Get the texture interface .. */
//#if 1      
//			hr = desktop_resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tex);
//			if (S_OK != hr) {
//				printf("Error: failed to query the ID3D11Texture2D interface on the IDXGIResource we got.\n");
//				exit(EXIT_FAILURE);
//			}
//#endif      
//
//			/* Map the desktop surface */
//			hr = duplication->MapDesktopSurface(&mapped_rect);
//			if (S_OK == hr) {
//				printf("We got acess to the desktop surface\n");
//				hr = duplication->UnMapDesktopSurface();
//				if (S_OK != hr) {
//					printf("Error: failed to unmap the desktop surface after successfully mapping it.\n");
//				}
//			}
//			else if (DXGI_ERROR_UNSUPPORTED == hr) {
//				//printf("MapDesktopSurface returned DXGI_ERROR_UNSUPPORTED.\n");
//				/*
//				According to the docs, when we receive this error we need
//				to transfer the image to a staging surface and then lock the
//				image by calling IDXGISurface::Map().
//				To get the data from GPU to the CPU, we do:
//				- copy the frame into our staging texture
//				- map the texture
//				- ... do something
//				- unmap.
//				@TODO figure out what solution is faster:
//				There are multiple solutions to copy a texture. I have
//				to look into what solution is better.
//				-  d3d_context->CopySubresourceRegion();
//				-  d3d_context->CopyResource(dest, src)
//				@TODO we need to make sure that the width/height are valid.
//
//				*/
//
//				deviceContext->CopyResource(staging_tex, tex);
//
//				D3D11_MAPPED_SUBRESOURCE map;
//				HRESULT map_result = deviceContext->Map(staging_tex,          /* Resource */
//					0,                    /* Subresource */
//					D3D11_MAP_READ,       /* Map type. */
//					0,                    /* Map flags. */
//					&map);
//
//				if (S_OK == map_result) {
//					unsigned char* data = (unsigned char*)map.pData;
//					//printf("Mapped the staging tex; we can access the data now.\n");
//					printf("RowPitch: %u, DepthPitch: %u, %02X, %02X, %02X\n", map.RowPitch, map.DepthPitch, data[0], data[1], data[2]);
//#if 0
//					if (i < 25) {
//						char fname[512];
//						/* We have to make the image opaque. */
//						for (int k = 0; k < tex_desc.Width; ++k) {
//							for (int l = 0; l < tex_desc.Height; ++l) {
//								int dx = l * tex_desc.Width * 4 + k * 4;
//								data[dx + 3] = 0xFF;
//							}
//						}
//						sprintf(fname, "capture_%03d.png", i);
//						save_png(fname,
//							tex_desc.Width, tex_desc.Height, 8, PNG_COLOR_TYPE_RGBA,
//							(unsigned char*)map.pData, map.RowPitch, PNG_TRANSFORM_BGR);
//					}
//#endif
//				}
//				else {
//					printf("Error: failed to map the staging tex. Cannot access the pixels.\n");
//				}
//
//				deviceContext->Unmap(staging_tex, 0);
//			}
//			else if (DXGI_ERROR_INVALID_CALL == hr) {
//				printf("MapDesktopSurface returned DXGI_ERROR_INVALID_CALL.\n");
//			}
//			else if (DXGI_ERROR_ACCESS_LOST == hr) {
//				printf("MapDesktopSurface returned DXGI_ERROR_ACCESS_LOST.\n");
//			}
//			else if (E_INVALIDARG == hr) {
//				printf("MapDesktopSurface returned E_INVALIDARG.\n");
//			}
//			else {
//				printf("MapDesktopSurface returned an unknown error.\n");
//			}
//		}
//
//		/* Clean up */
//		{
//
//			if (NULL != tex) {
//				tex->Release();
//				tex = NULL;
//			}
//
//			if (NULL != desktop_resource) {
//				desktop_resource->Release();
//				desktop_resource = NULL;
//			}
//
//			/* We must release the frame. */
//			hr = duplication->ReleaseFrame();
//			if (S_OK != hr) {
//				printf("Failed to release the duplication frame.\n");
//			}
//		}
//	}
//
//	//printf("Monitors connected to adapter: %lu\n", i);
//
//	/* Cleanup */
//	{
//
//		if (NULL != staging_tex) {
//			staging_tex->Release();
//			staging_tex = NULL;
//		}
//
//		//if (NULL != d3d_device) {
//		//	d3d_device->Release();
//		//	d3d_device = NULL;
//		//}
//
//		//if (NULL != d3d_context) {
//		//	d3d_context->Release();
//		//	d3d_context = NULL;
//		//}
//
//		if (NULL != duplication) {
//			duplication->Release();
//			duplication = NULL;
//		}
//
//		for (size_t i = 0; i < adapters.size(); ++i) {
//			if (NULL != adapters[i]) {
//				adapters[i]->Release();
//				adapters[i] = NULL;
//			}
//		}
//
//		for (size_t i = 0; i < outputs.size(); ++i) {
//			if (NULL != outputs[i]) {
//				outputs[i]->Release();
//				outputs[i] = NULL;
//			}
//		}
//
//		if (NULL != output1) {
//			output1->Release();
//			output1 = NULL;
//		}
//
//		if (NULL != factory) {
//			factory->Release();
//			factory = NULL;
//		}
//	}
//
//
//	return true;
#pragma endregion
}

//from: http://stackoverflow.com/questions/27150901/rendering-windows-screenshot-capture-bitmap-as-directx-texture
bool TextureClass::LoadWindowBitmap(ID3D11Device* device) {
	HRESULT hr;
	RECT user_window_rectangle;
	ID3D11Texture2D* tex = 0;

	HWND user_window = FindWindow(NULL, TEXT("计算器"));    //the window can't be min
	//HWND user_window = FindWindow(NULL, TEXT("无标题 - 画图"));    //the window can't be min
	if (user_window == NULL)
	{
		MessageBoxA(NULL, "Can't find Calculator", "Camvas", MB_OK);
		return false;
	}
	GetClientRect(user_window, &user_window_rectangle);
	//create
	HDC hdcScreen = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	UINT screenshot_width = user_window_rectangle.right - user_window_rectangle.left;
	UINT screenshot_height = user_window_rectangle.bottom - user_window_rectangle.top;

	//UINT screenshot_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	//UINT screenshot_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, screenshot_width, screenshot_height);

	SelectObject(hdc, hbmp);

	//Print to memory hdc
	PrintWindow(user_window, hdc, PW_CLIENTONLY);

	// copy from the desktop device context to the bitmap device context
//	BitBlt(hdc, 0, 0, screenshot_width, screenshot_height, hdcScreen, 0, 0, SRCCOPY);


	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;
	bmih.biWidth = screenshot_width;
	bmih.biHeight = 0 - screenshot_height;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;

	int bytes_per_pixel = bmih.biBitCount / 8;

	BYTE *pixels = (BYTE*)malloc(bytes_per_pixel * screenshot_width * screenshot_height);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader = bmih;

	int row_count = GetDIBits(hdc, hbmp, 0, screenshot_height, pixels, &bmi, DIB_RGB_COLORS);

	ReleaseDC(NULL, hdc);

	// ..and delete the context you created
	DeleteDC(hdc);


	D3D11_TEXTURE2D_DESC screenshot_desc = CD3D11_TEXTURE2D_DESC(
		DXGI_FORMAT_B8G8R8A8_UNORM,     // format
		screenshot_width,               // width
		screenshot_height,              // height
		1,                              // arraySize
		1,                              // mipLevels
		D3D11_BIND_SHADER_RESOURCE,     // bindFlags
		D3D11_USAGE_DYNAMIC,            // usage
		D3D11_CPU_ACCESS_WRITE,         // cpuaccessFlags
		1,                              // sampleCount
		0,                              // sampleQuality
		0                               // miscFlags
	);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = pixels; // texArray; // &bmp.bmBits; //pixel buffer
	data.SysMemPitch = bytes_per_pixel * screenshot_width;// line size in byte
	data.SysMemSlicePitch = bytes_per_pixel * screenshot_width * screenshot_height;

	hr = device->CreateTexture2D(
		&screenshot_desc, //texture format
		&data,          // pixel buffer use to fill the texture
		&tex  // created texture
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = screenshot_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MostDetailedMip = screenshot_desc.MipLevels;

	device->CreateShaderResourceView(tex, NULL, &m_texture);

	return true;

}
	

//from: http://stackoverflow.com/questions/29316524/directx11-cant-set-texture2d-as-shaderresourceview
bool TextureClass::LoadBitMap(ID3D11Device* device) 
{
	cout << __FUNCTION__ << endl;

	HRESULT result;
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(tdesc));
	D3D11_SUBRESOURCE_DATA srInitData;
	ZeroMemory(&srInitData, sizeof(srInitData));
	ID3D11Texture2D* tex = 0;
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	ZeroMemory(&srDesc, sizeof(srDesc));

	int w = 512 / 4;
	int h = 512 / 4;
	int bpp = 4;
	int *buf = new int[w*h];

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if ((i & 32) == (j & 32))
				buf[i*w + j] = 0x0;
			else
				buf[i*w + j] = 0xffffff;
		}
	}

	srInitData.pSysMem = (void *)buf;
	srInitData.SysMemPitch = w*bpp;
	srInitData.SysMemSlicePitch = w*h*bpp; //No need here, it's for 3D texture

	tdesc.Width = w;
	tdesc.Height = h;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	// checking inputs
	if (device->CreateTexture2D(&tdesc, &srInitData, NULL) == S_FALSE)
		std::cout << "Inputs correct" << std::endl;
	else
		std::cout << "wrong inputs" << std::endl;

	// create the texture
	result = device->CreateTexture2D(&tdesc, &srInitData, &tex);
	if (result != S_OK)
	{
		std::cout << "Failed" << std::endl;
		return(0);
	}
	else
		std::cout << "Success" << std::endl;


	// setup the Shader Resource Desc.
	srDesc.Format = tdesc.Format;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	// ------------------ [ Here it always Fails ...]
	result = device->CreateShaderResourceView(tex, &srDesc, &m_texture);
	if (result != S_OK)
	{
		std::cout << "Failed" << std::endl;
		return(0);
	}

	delete[] buf;

	return true;
}

//HPQ
bool TextureClass::Initialize(ID3D11Device* device)//, ID3D11DeviceContext* deviceContext)
{
	return LoadBitMap(device);
	//return LoadWindowBitmap(device);
	//return LoadDesktopBitmap(device); // , deviceContext);
}
//eHPQ

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}