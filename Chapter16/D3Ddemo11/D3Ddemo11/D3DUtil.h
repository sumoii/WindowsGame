//=============================================================================
// Desc: D3DUtil.h头文件，用于公共辅助宏的定义
// 2024年 9月24日  Create by sumoii 
//=============================================================================

#pragma once


#ifndef HR
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }         //自定义一个HR宏，方便执行错误的返回
#endif

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }       //自定义一个SAFE_DELETE()宏,便于指针资源的释放
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //自定义一个SAFE_RELEASE()宏,便于COM资源的释放
#endif