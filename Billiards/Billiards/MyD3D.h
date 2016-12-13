#include "Header.h"
/****************************************************************/
/*�� MyD3D Object Class Header   2007/10/15  Ver 1.3  �O�c �� ��*/
/****************************************************************/
#ifndef _MyD3D
#define _MyD3D

#define ERMSG(x)                MessageBox(NULL,x,"DirectX9",MB_OK);
#define SAFE_DELETE(p)          { if (p) { delete (p);      (p)=NULL; } }
#define SAFE_RELEASE(p)         { if (p) { (p)->Release();  (p)=NULL; } }
#define SAFE_DELOBJ(p)          { if (p) { DeleteObject(p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)    { if (p) { delete[] (p);    (p)=NULL; } }
#define SAFE_DELDC(p)           { if (p) { DeleteDC (p);   (p)=NULL; } }

//typedef struct _D3DVERTEX
//{   float   x,y,z;
//    float   nx,ny,nz;
//}   D3DVERTEX;

typedef struct _D3DVERTEX
{
	D3DXVECTOR3 position; // The 3D position for the vertex
	D3DXVECTOR3 normal;   // The surface normal for the vertex
}   D3DVERTEX;

// SPI ENTRY
typedef int (PASCAL *GetPicture)(LPSTR, long, UINT, HANDLE*, HANDLE*, FARPROC, long);

class MyD3D
{
protected:
	HWND                    hWnd;
	LPDIRECT3DSURFACE9      pBack;          //Back Buffer Surface
	LPDIRECT3DSURFACE9      pSurface;       //�摜�T�[�t�F�[�X
	LPDIRECT3DTEXTURE9      pTexture;       //�e�N�X�`��
	HBITMAP     			hBmp;           //�摜�� HBITMAP
	long        			XNum;           //�������� SPrite ��
	long        			YNum;           //�c������ SPrite ��
	long        			SWidth;         //Sprite �̕�
	long        			SHeight;        //Sprite �̍���
	long        			xp, yp;
	HINSTANCE   			hSpi;

	void        CreateBuf(long width, long height);
	HPALETTE    DIBtoPAL(HANDLE hBInfo);

public:
	LPDIRECT3D9             pD3D;
	LPDIRECT3DDEVICE9       pDEV;
	D3DPRESENT_PARAMETERS   D3DPPWin;
	D3DPRESENT_PARAMETERS   D3DPPFull;
	D3DLIGHT9               Light;
	D3DMATERIAL9            Material;
	HDC                     hBmpDC;         //�摜�� HDC
	RECT                    ImgRect;        //�摜�̃T�C�Y
	char                    szFile[MAX_PATH];   //�I�[�v������t�@�C�����i�p�X�t���j

	MyD3D(HWND hwnd);                       //Constructor
	~MyD3D();                               //Destructor
	void        DMsg(char *chr, int n);
	void        DMsg(char *chr, float f);
	HRESULT     InitD3D(LPDIRECT3DDEVICE9 *ppDEV);
	HRESULT     InitD3DFull(LPDIRECT3DDEVICE9 *ppDEV, UINT Width, UINT Height);
	HRESULT     InitD3DFull(LPDIRECT3DDEVICE9 *ppDEV)
	{
		return InitD3DFull(ppDEV, 800, 600);
	}
	HRESULT     SetD3DPPWin();
	HRESULT     SetD3DPPFull(UINT Width, UINT Height);
	HRESULT     SetD3DPPFull()
	{
		return SetD3DPPFull(800, 600);
	}
	DWORD       LoadXFile(char*, LPD3DXMESH*, D3DMATERIAL9**, LPDIRECT3DTEXTURE9**);
	void        SetLight(float r, float g, float b);
	void        SetLight(D3DCOLOR cor);
	void        SetLight();
	void        SetMaterial(float r, float g, float b);
	void        SetMaterial();
	void        Move_Mesh(LPD3DXMESH pMesh, D3DXVECTOR3 v);
	void        BlendMeshes(LPD3DXMESH pDst, LPD3DXMESH pSrc1, LPD3DXMESH pSrc2, FLOAT fWeight);
	HRESULT     GetVtxBuff(LPD3DXMESH, DWORD*, DWORD*, LPDIRECT3DVERTEXBUFFER9*, LPDIRECT3DINDEXBUFFER9*);
	char        *SetPos(char *buf);
	char        *SetFloat(float *tbl, char *buf, int cnt);
	char        *SetInt(int *val, char *buf, int cnt);
	char        *SetStr(char *val, char *buf);
	DWORD       ReadData(LPSTR fname, void *buf, DWORD len);
	void        SetVect(D3DXVECTOR3 *v3, float fv[3]);
	void        BlendVect(D3DXVECTOR3 *v3, float fv1[3], float fv2[3], float fWeight);
	void        BlendF6(float fv[6], float fv1[6], float fv2[6], float fWeight);
	void        SetView(D3DXVECTOR3 *viewForm, D3DXVECTOR3 *tergForm, float viewt[12]);
	void        BlendFrame(D3DXVECTOR3 *viewForm, D3DXVECTOR3 *tergForm,
		float src1[12], float src2[12], float fWeight);
	HDC         GetBackDC();
	void        ReleaseBackDC(HDC hDC);
	LPDIRECT3DSURFACE9  LoadSurface(LPSTR imgfile);
	LPDIRECT3DTEXTURE9  LoadTexture(LPSTR imgfile);
	LPDIRECT3DTEXTURE9  LoadTexture(LPSTR imgfile, RECT rc);
	LPDIRECT3DTEXTURE9  LoadTextureBlack(LPSTR imgfile);
	LPDIRECT3DSURFACE9  CreateSurface();
	RECT        GetSurfaceRect(LPDIRECT3DSURFACE9);
	RECT        GetTextureRect(LPDIRECT3DTEXTURE9, int);
	RECT        GetTextureRect(LPDIRECT3DTEXTURE9 tex)
	{
		return GetTextureRect(tex, 0);
	}
	RECT        GetFileRect(LPSTR imgfile);
	DWORD       PathCheck(LPSTR path);
	void        Rotate(D3DXVECTOR3 *Vect, D3DXVECTOR3 *Cen, float rt, int xyz);
	HRESULT     BlendPix(LPDIRECT3DSURFACE9 D_Sur, LPDIRECT3DSURFACE9 S_Sur1, LPDIRECT3DSURFACE9 S_Sur2, RECT Rect, float Rate);
	HRESULT     Scroll3D(LPDIRECT3DSURFACE9 D_Sur, LPDIRECT3DSURFACE9 S_Sur, RECT Rect, int Pos, float Rate, float Side);
	//Sprite
	HRESULT     LoadBmp(LPSTR szBitmap, WORD WN, WORD HN);
	HRESULT     LoadBmp(LPSTR szBitmap) { return LoadBmp(szBitmap, 1, 1); }
	HRESULT     LoadImg(LPSTR ImageName);
	HRESULT     LoadJpeg(LPSTR ImageName) { return LoadDLL(ImageName, "ifjpeg.spi"); }
	HRESULT     LoadGif(LPSTR ImageName) { return LoadDLL(ImageName, "ifgif.spi"); }
	HRESULT     LoadDLL(LPSTR ImageName, LPSTR SPI);
	RECT        GetImgRect() { return ImgRect; }
	//BitBlt() �ŕ`�悷��
	HRESULT     Show(HDC hdc, long x, long y, long w, long h, long xd, long yd);
	HRESULT     Show(long x, long y, long w, long h, long xd, long yd);
	HRESULT     Show(HDC hdc)
	{
		return Show(hdc, 0, 0, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	HRESULT     Show(HDC hdc, long x, long y)
	{
		return Show(hdc, x, y, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	HRESULT     Show()
	{
		return Show(0, 0, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	HRESULT     Show(long x, long y)
	{
		return Show(x, y, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	//�����F�� TransparentBlt() �ŕ`�悷��
	HRESULT     ShowBlack(HDC hdc, long x, long y, long w, long h, long xd, long yd);
	HRESULT     ShowBlack(long x, long y, long w, long h, long xd, long yd);
	HRESULT     ShowBlack(HDC hdc)
	{
		return ShowBlack(hdc, 0, 0, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	HRESULT     ShowBlack(HDC hdc, long x, long y)
	{
		return ShowBlack(hdc, x, y, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	HRESULT     ShowBlack()
	{
		return ShowBlack(0, 0, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	HRESULT     ShowBlack(long x, long y)
	{
		return ShowBlack(x, y, ImgRect.right, ImgRect.bottom, 0, 0);
	}
	//Sprite �ԍ����w�肷��Ƃ��́A�����F��ݒ肵�� TransparentBlt() �ŕ`�悷��
	HRESULT     Show(HDC hdc, long n, long x, long y, long w, long h, long xoff, long yoff);
	HRESULT     Show(HDC hdc, long n, long x, long y)
	{
		return Show(hdc, n, x, y, SWidth, SHeight, 0, 0);
	}
	HRESULT     Show(long n, long x, long y, long w, long h, long xoff, long yoff);
	HRESULT     Show(long n, long x, long y)
	{
		return Show(n, x, y, SWidth, SHeight, 0, 0);
	}
	//�g��^�k���� TransparentBlt() �ŕ`�悷��
	HRESULT     ShowSize(HDC hdc, long x, long y, float size);
	HRESULT     ShowSize(long x, long y, float size);
	HRESULT     ShowSize(HDC hdc, long n, long x, long y, float size);
	HRESULT     ShowSize(long n, long x, long y, float size);

	void        Adjust();
	void        Open(LPSTR szDir);
	HRESULT     SetNum(WORD WN, WORD HN);
	HRESULT     SetSize(WORD WSIZ, WORD HSIZ);
	HFONT       SetMyFont(LPCTSTR face, int h, int angle);
};

class MATLIST
{
protected:
	ID3DXMatrixStack    *MatStack;
public:
	D3DXVECTOR3         Mov;        //�e����̑��Έʒu���W
	D3DXVECTOR3         Rot;        //�e����̑��Ή�]���
	D3DXVECTOR3         Up;         //������]���
	MATLIST             *Parent;    //�e�� MATLIST

	MATLIST(MATLIST *par);          //Constructor
	~MATLIST();                     //Destructor
	void                MatOne(MATLIST *model);
	D3DXMATRIX*         MatAll();
};

#endif

