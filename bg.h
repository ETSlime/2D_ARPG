//=============================================================================
//
// BG処理 [bg.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct BG
{
	XMFLOAT3	pos;		// ポリゴンの座標
	float		w, h;		// 幅と高さ
	int			texNo;		// 使用しているテクスチャ番号

	float		scrl;		// スクロール
	XMFLOAT3	old_pos;	// １フレ前のポリゴンの座標
	float		scrl2;		// スクロール
};


#define TEXTURE_WIDTH				(5026)//(3015)			// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(2006)//(1360)			// (SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// テクスチャの数

#define	SKY_H						(150.0f)	// 空のポリゴンの高さ
#define MAP01_GROUND_MAX			(1)
#define GROUND_H					(2006 - 1740 - 100)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

BG* GetBG(void);

AABB* GetMap01AABB(void);

