#pragma once
#ifdef _DX7
#include <ddraw.h>
#include <map>
#include <assert.h>
#include <filesystem>

// Spr 종류 
#define SPRTYPE_SPR             0   // 일반 스프라이트 (0번 압축. ) 
#define SPRTYPE_TLE             1   // 타일용 스프라이트 (0번 압축.) 

// 출력 속성. 
#define SPRATB_DOT              1
#define COLOR_TRANS             254 // RGB(240,0,248)

#define TSPR_VERSION            10
#define MAX_IMG_PER_TSPR        256 // Spr 파일 하나당 이미지의 개수.

class cltTSprImgInfo
{
public:
    int siCollInfo;
    int siStartPos;
    int siLength;

    cltTSprImgInfo()
    {
        siCollInfo = 0;
        siStartPos = 0;
        siLength = 0;
    }
};

class cltTSprHeader
{
public:
    int siVersion;
    int siSprType;        // 어떤 타입의 Spr인가 ?
    int siXsize;
    int siYsize;
    int siHRes;
    int siVRes;
    int siTotalLength;

    int Reserved[8];

    int siFontNum;
    cltTSprImgInfo clImgInfo[MAX_IMG_PER_TSPR];

    cltTSprHeader()
    {
        siVersion = TSPR_VERSION;
        siSprType = SPRTYPE_SPR;
        siXsize = 0;
        siYsize = 0;
        siHRes = 0;
        siVRes = 0;
        siTotalLength = 0;
        siFontNum = 0;
        memset(Reserved, 0, sizeof(Reserved));
    }
};

class cltTSpr
{
public:
    cltTSprHeader clHeader;
    unsigned char* Image;
    unsigned short pal[256];

    cltTSpr() { 
        Image = nullptr;
        memset(pal, 0, sizeof(pal));
    }
    ~cltTSpr()
    {
        if (Image)
        {
            delete[] Image;
            Image = nullptr;
        }
    }

    inline int GetHres() { return clHeader.siHRes; }
    inline int GetVres() { return clHeader.siVRes; }
    inline int GetXSize() { return clHeader.siXsize; }
    inline int GetYSize() { return clHeader.siYsize; }
    inline int GetXCount() { return clHeader.siHRes / clHeader.siXsize; }
    inline int GetYCount() { return clHeader.siVRes / clHeader.siYsize; }
    inline int GetCol(int idx) { return idx == 0 ? 0 : idx / GetXCount(); }
    inline int GetRow(int idx) { return idx == 0 ? 0 : idx % GetYCount(); }

    bool LoadSpr(const char* szfilepath)
    {
        FILE* fp;
        errno_t err = fopen_s(&fp, szfilepath, "rb");
        if (err != 0 || !fp) return false;

        fread(&clHeader, sizeof(cltTSprHeader), 1, fp);

        // 필요한 양의 이미지 버퍼를 만든다.
        if (Image) delete[] Image;
        Image = new unsigned char[clHeader.siTotalLength];

        if (!Image)
        {
            fclose(fp);
            return false;
        }

        // 이미지를 불러온다.
        fread(Image, clHeader.siTotalLength, 1, fp);

        // 팔레트를 불러온다 
        fread(pal, 512, 1, fp);

        fclose(fp);
        return true;
    }

    bool SaveSpr(const char* szfilepath)
    {
        FILE* fp;
        errno_t err = fopen_s(&fp, szfilepath, "wb"); // 쓰기 + 이진 모드
        if (err != 0 || !fp) return false;

        // 헤더 정보 저장
        fwrite(&clHeader, sizeof(cltTSprHeader), 1, fp);

        // 이미지 데이터 저장
        fwrite(Image, clHeader.siTotalLength, 1, fp);

        // 팔레트 데이터 저장
        fwrite(pal, 512, 1, fp);

        fclose(fp);

        return true;
    }
};

class sprData {
public:
    sprData(IDirectDraw7* pDD, cltTSpr* spr)
    {
        m_pSurface = nullptr;
        m_pSpr = spr;
    }

    ~sprData()
    {
        if (m_pSurface != nullptr) {
            m_pSurface->Release();
            m_pSurface = nullptr;
        }
        if (m_pSpr != nullptr) {
            delete m_pSpr;
            m_pSpr = nullptr;
        }
    }
    IDirectDrawSurface7* GetSurface() { return m_pSurface; }
    cltTSpr* GetSpr() { return m_pSpr; }

public:
    void LoadTexture(IDirectDraw7* pDD);
    void Release();

private:
    IDirectDrawSurface7* m_pSurface;
    cltTSpr* m_pSpr;
};

class sprLoader
{
public:
    sprData* LoadSprite(const char* filename);
    std::string GetRelativePath(const char* absolutePath);
    std::string GetExecutablePath();
    void Init(IDirectDraw7* pDD);
    void Release();
private:
    std::map<std::string, sprData*> m_mapSprite;
    IDirectDraw7* m_pDD;
};
#endif // _DX7