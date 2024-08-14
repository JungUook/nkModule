#include "pch.h"
#include "NuklearUI.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define nk_white { 255,255,255,255 };

#ifdef _DX7
void NuklearUI::Initialize(IDirectDraw7* pdd, IDirect3DDevice7* pdevice, int width, int height, int lang, const char* fontPath)
{
	m_iLanguage = lang;
	m_sFontPath = fontPath;

	CHAR systemPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, systemPath))) {
		std::cout << "System font path: " << systemPath << std::endl;
	}

	char path[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL) {
		// 현재 실행 파일의 경로를 얻습니다.
		GetModuleFileNameA(hModule, path, MAX_PATH);
	}
	else {
		std::cerr << "Failed to get module handle." << std::endl;
		return;
	}

	std::string basePath(path);
	basePath = basePath.substr(0, basePath.find_last_of("\\\\"));
	std::string dataPath = "";
	if (fontPath != nullptr) {
		dataPath = basePath + fontPath;
		if (!CreateDirectoryIfNotExists(dataPath)) {
			std::cerr << "Failed to create directory: " << dataPath << std::endl;
			return;
		}
	}
	else {
		dataPath = "None";
	}

	m_ctx = m_dx7.nk_d3d7_init(pdd, pdevice);

	struct nk_font_atlas* atlas;
	if (dataPath != "None") {
		m_dx7.nk_d3d7_font_stash_begin(&atlas, systemPath, lang, dataPath.c_str());
	}
	else {
		m_dx7.nk_d3d7_font_stash_begin(&atlas, systemPath, lang);
	}
	m_font = m_dx7.d3d7.font;

	m_bMouseHovering = false;
	m_bEditActive = false;

	m_luaInterface.Init();

	for (auto it = m_vecModule.begin(); it != m_vecModule.end(); ++it) {
		NKBase* ptr = *it;

		ptr->SetContext(m_ctx);
		ptr->Setfont(m_font);
	}
}
void NuklearUI::Render(IDirect3DDevice7* pdevice)
{
	m_dx7.nk_d3d7_render(NK_ANTI_ALIASING_ON);

#ifdef _NKDEBUG
	EditorRender();
#endif // _NKDEBUG

	for (std::vector<NKBase*>::iterator iter = m_vecModule.begin(); iter != m_vecModule.end(); ++iter)
	{
		(*iter)->SafeRenderEnd(m_ctx);
	}
	ReleaseRenderData();
}
void NuklearUI::Restore()
{

}
int NuklearUI::HandleEvent(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (this == nullptr) {
		return 0;
	}

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (m_dx7.d3d7.device)
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);
			if (width != 0 && height != 0)
			{
				m_dx7.nk_d3d7_resize(width, height);
			}
		}
		break;
	}


	return m_dx7.nk_d3d7_handle_event(wnd, msg, wparam, lparam);
}

bool NuklearUI::LoadSpriteData(IDirectDrawSurface7* sprite, int width, int height, int sliceSizeX, int sliceSizeY, int countX, int countY)
{
	int index = 0;
	if (sliceSizeX && sliceSizeY && countX && countY)
	{
		for (int y = 0; y < countY; ++y)
		{
			for (int x = 0; x < countX; ++x)
			{
				uint16_t region[4] = { 0, };
				region[0] = sliceSizeX * x;
				region[1] = sliceSizeY * y;
				region[2] = sliceSizeX;
				region[3] = sliceSizeY;
				AddImage(index++, sprite, width, height, region);
			}
		}
	}
	else
	{
		AddImage(index, sprite);
	}

	return true;
}
bool NuklearUI::ReadImageFile(const char* filename, IDirectDrawSurface7** pTexture)
{
	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 4); // 4는 RGBA로 로드하라는 의미
	if (!data)
		return false;

	// 텍스처 생성
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
	ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
	ddsd.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
	ddsd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
	ddsd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
	ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;

	HRESULT hr = m_dx7.d3d7.dd->CreateSurface(&ddsd, pTexture, NULL);
	if (FAILED(hr)) {
		stbi_image_free(data);
		return false;
	}

	// 텍스처에 이미지 데이터 복사
	DDSURFACEDESC2 lockedSurface;
	ZeroMemory(&lockedSurface, sizeof(lockedSurface));
	lockedSurface.dwSize = sizeof(lockedSurface);
	if (FAILED((*pTexture)->Lock(NULL, &lockedSurface, 0, NULL))) {
		stbi_image_free(data);
		return false;
	}

	BYTE* dest = (BYTE*)lockedSurface.lpSurface;
	for (int y = 0; y < height; y++) {
		memcpy(dest + y * lockedSurface.lPitch, data + y * width * 4, width * 4);
	}
	(*pTexture)->Unlock(NULL);

	stbi_image_free(data);
	return true;
}

void NuklearUI::AddImage(int SID, IDirectDrawSurface7* texture)
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);
	img.color = nk_white;

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}

void NuklearUI::AddImage(int SID, IDirectDrawSurface7* texture, uint16_t width, uint16_t height, uint16_t region[])
{
	struct nk_image img;
	memset(&img, 0, sizeof(img));
	img.handle = nk_handle_ptr(texture);

	img.w = width;
	img.h = height;

	img.region[0] = region[0];
	img.region[1] = region[1];
	img.region[2] = region[2];
	img.region[3] = region[3];

	img.color = nk_white;

	std::pair<int, struct nk_image> pairData = std::make_pair(SID, img);
	m_mapImage.insert(pairData);
}

void NuklearUI::Register_spr(sprLoader* pSpr)
{
	m_sprLoader = pSpr;
}

void NuklearUI::OpenFileDialog()
{
	wchar_t originalDir[MAX_PATH] = { 0, };
	GetCurrentDirectoryW(MAX_PATH, originalDir);

	OPENFILENAMEW ofn;
	const size_t buffer_size = 65536; // 충분히 큰 버퍼 크기
	wchar_t* szFile = new wchar_t[buffer_size];
	ZeroMemory(szFile, buffer_size * sizeof(wchar_t));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = buffer_size;
	ofn.lpstrFilter = L"All Files\0*.*\0SPR Files\0*.spr;*.Spr;*.SPR\0";
	ofn.nFilterIndex = 2; // 기본 선택을 SPR Files로 설정
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (GetOpenFileNameW(&ofn) == TRUE) {
		wchar_t* p = szFile;
		std::wstring directory = p;
		p += directory.length() + 1;

		while (*p) {
			std::wstring filePath = directory + L"\\" + p;
			std::filesystem::path path(filePath);
			std::wstring extension = path.extension().wstring();
			std::transform(extension.begin(), extension.end(), extension.begin(), towlower);
			if (extension == L".spr") {
				int size_needed = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, NULL, 0, NULL, NULL);
				char* result = new char[size_needed];
				WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, result, size_needed, NULL, NULL);
				LoadSprFile(result);
				delete[] result;
			}
			p += wcslen(p) + 1;
		}

		// If only one file is selected, GetOpenFileNameW does not add the directory separately
		if (directory.length() > 0 && *p == '\0') {
			std::filesystem::path path(directory);
			std::wstring extension = path.extension().wstring();
			std::transform(extension.begin(), extension.end(), extension.begin(), towlower);
			if (extension == L".spr") {
				int size_needed = WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, NULL, 0, NULL, NULL);
				char* result = new char[size_needed];
				WideCharToMultiByte(CP_UTF8, 0, directory.c_str(), -1, result, size_needed, NULL, NULL);
				LoadSprFile(result);
				delete[] result;
			}
		}
	}

	delete[] szFile; // 동적으로 할당한 메모리 해제

	SetCurrentDirectoryW(originalDir);
}

void NuklearUI::LoadSprFile(const char* filename)
{
	sprData* pData = m_sprLoader->LoadSprite(filename);

	if (pData != nullptr) {
		std::string relativePath = m_sprLoader->GetRelativePath(filename);

		std::transform(relativePath.begin(), relativePath.end(), relativePath.begin(),
			[](unsigned char c) { return std::tolower(c); });

		m_mapSpr.insert(std::make_pair(relativePath, pData));
	}
	else {
		char errorLog[256] = { 0, };
		sprintf_s(errorLog, "File not found: %s", filename);
	}
}

bool NuklearUI::GetSprite(const char* filename, int index, struct nk_image& outimg, bool bImmortal)
{
	std::string str = filename;
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });

	auto it = m_mapSpr.find(str);
	if (it != m_mapSpr.end()) {
		sprData* pSpr = (*it).second;
		bool bSuccess = RegisterRenderData(pSpr, bImmortal);
		if (!bSuccess) {
			return false;
		}
		int totalSprites = pSpr->GetSpr()->GetXCount() * pSpr->GetSpr()->GetYCount();

		if (index >= 0 && index < totalSprites) {

			int x = index % pSpr->GetSpr()->GetXCount();
			int y = index / pSpr->GetSpr()->GetXCount();

			struct nk_image img;
			memset(&img, 0, sizeof(img));
			img.handle = nk_handle_ptr(pSpr->GetSurface());

			img.w = pSpr->GetSpr()->GetHres();
			img.h = pSpr->GetSpr()->GetVres();

			img.region[0] = pSpr->GetSpr()->GetXSize() * x;
			img.region[1] = pSpr->GetSpr()->GetYSize() * y;
			img.region[2] = pSpr->GetSpr()->GetXSize();
			img.region[3] = pSpr->GetSpr()->GetYSize();
			outimg = img;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
bool NuklearUI::GetImage(const char* filename, struct nk_image& outimg, bool bImmortal)
{
	auto it = m_mapSpr.find(filename);
	if (it != m_mapSpr.end()) {
		sprData* pSpr = (*it).second;
		bool bSuccess = RegisterRenderData(pSpr, bImmortal);
		if (!bSuccess) {
			throw;
		}
		struct nk_image img;
		memset(&img, 0, sizeof(img));
		img.handle = nk_handle_ptr(pSpr->GetSurface());
		outimg = img;

		return true;
	}
	else {
		return false;
	}
}

std::map<std::string, sprData*>* NuklearUI::GetSprMap()
{
	return &m_mapSpr;
}

bool NuklearUI::RegisterRenderData(sprData* pData, bool bImmortal)
{
	if (pData->GetSurface() == nullptr) {
		pData->LoadTexture(m_dx7.d3d7.dd);
		if (bImmortal) {
			m_vecImmortalRenderData.push_back(pData);
		}
		else {
			m_vecRenderData.push_back(pData);
		}
	}

	if (pData->GetSurface()) {
		return true;
	}
	else {
		pData->LoadTexture(m_dx7.d3d7.dd);

		if (pData->GetSurface()) {
			return true;
		}

		return false;
	}
}
void NuklearUI::ReleaseRenderData()
{
	for (auto it = m_vecRenderData.begin(); it != m_vecRenderData.end();) {
		sprData* pData = *it;
		pData->Release();
		it = m_vecRenderData.erase(it);
	}
}
void NuklearUI::SetDirectX7(IDirectDraw7* pdd, IDirect3DDevice7* pdevice, int width, int height)
{
	m_dx7.d3d7.dd = pdd;
	m_dx7.d3d7.device = pdevice;

	m_dx7.nk_d3d7_shutdown();
	Initialize(pdd, pdevice, width, height, m_iLanguage, m_sFontPath.c_str());
}
#endif