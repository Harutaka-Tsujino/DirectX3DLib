#ifndef DX9_X64_LIB_H
#define DX9_X64_LIB_H

#include<windows.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

//ƒ|ƒŠƒSƒ“‚Ì’¸“_î•ñ‚Ì\‘¢‘Ì
struct CustomVertex
{
	FLOAT m_x, m_y, m_z;
	DWORD m_aRGB;
	FLOAT m_tu, m_tv;
};

#endif//DX9_X64_LIB_H
