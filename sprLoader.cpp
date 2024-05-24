#include "pch.h"
#include "sprLoader.h"
#ifdef _DX7
void sprData::Init(IDirectDraw7* pDD)
{
    int x = 0, y = 0;

    int drawHres = m_pSpr->clHeader.siHRes;
    int drawVres = m_pSpr->clHeader.siVRes;

    unsigned char* pSrc = m_pSpr->Image;
    unsigned short* pPalette = m_pSpr->pal;

    int _colEnd = m_pSpr->GetXCount();
    int _rowEnd = m_pSpr->GetYCount();
    int _colSize = m_pSpr->GetXSize();
    int _rowSize = m_pSpr->GetYSize();

    DDSURFACEDESC2 ddsd;	   
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
    ddsd.dwWidth = drawHres;
    ddsd.dwHeight = drawVres;
    ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
    ddsd.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
    ddsd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
    ddsd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
    ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;

    IDirectDrawSurface7* pSurface = nullptr;
    HRESULT hr = pDD->CreateSurface(&ddsd, &pSurface, nullptr);
    assert(SUCCEEDED(hr));


    hr = pSurface->Lock(nullptr, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, nullptr);
    assert(SUCCEEDED(hr));

    int pitch = ddsd.lPitch / 4;
    DWORD* pBuffer = (DWORD*)ddsd.lpSurface;

	for (int _row = 0; _row < _rowEnd; ++_row)
	{
		int offsetY = y + _row * _rowSize;
		for (int _col = 0; _col < _colEnd; ++_col)
		{
			int offsetX = x + _col * _colSize;
			for (int i = 0; i < _rowSize; ++i)
			{
				int j = 0;
				while (j < _colSize)
				{
					unsigned short rgb16Color = m_pSpr->pal[*pSrc];

					unsigned char blue = static_cast<unsigned char>((rgb16Color >> 11) & 0x1F) << 3;
					unsigned char green = static_cast<unsigned char>((rgb16Color >> 5) & 0x3F) << 2;
					unsigned char red = static_cast<unsigned char>((rgb16Color) & 0x1F) << 3;
					unsigned char alpha = (*pSrc == COLOR_TRANS) ? 0 : 255; // Set alpha to 0 for transparent pixels

					DWORD color = (alpha << 24) | (blue << 16) | (green << 8) | red;

					if (*pSrc == COLOR_TRANS)
					{
						int k = j;
						unsigned char* pTempSour = pSrc;
						int _end = k + *(++pTempSour);

                        for (int k = j; k < _end; k++)
                        {
                            pBuffer[(offsetY + i) * pitch + (offsetX + k)] = 0x00000000;
                        }
						++pSrc;
						j += *pSrc;
					}
					else
					{
						pBuffer[(offsetY + i) * pitch + (offsetX + j)] = color;
						++j;
					}
					++pSrc;
				}
			}
		} // end column
	} // end row

    hr = pSurface->Unlock(nullptr);
    assert(SUCCEEDED(hr));

    m_pSurface = pSurface;
}


sprData* sprLoader::LoadSprite(const char* filename)
{
	std::map<const char*, sprData*>::iterator it = m_mapSprite.find(filename);
    sprData* pData = nullptr;
	if (it != m_mapSprite.end()) {
        pData = it->second;
	}
	else
	{
        cltTSpr* pSpr = nullptr;
        pSpr = new cltTSpr();
		if (pSpr->LoadSpr(filename)) {

            pData = new sprData(m_pDD, pSpr);

            if (pData != nullptr) {
                m_mapSprite.insert(std::make_pair(filename, pData));
            }
		}
	}
	return pData;
}
void sprLoader::Init(IDirectDraw7* pDD)
{
    m_pDD = pDD;
}
void sprLoader::Release()
{
    std::map<const char*, sprData*>::iterator it;

    for (it = m_mapSprite.begin(); it != m_mapSprite.end();)
    {
        sprData* pData = it->second;
        delete pData;
        it = m_mapSprite.erase(it);
    }
}
#endif