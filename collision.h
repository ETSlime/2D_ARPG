//=============================================================================
//
// 当たり判定処理 [collision.h]
// Author : 
//
//=============================================================================
#pragma once

#include "map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EPSILON		(1e-2f)
#define XMFLOAT3_ADD(a, b) XMFLOAT3((a).x + (b).x, (a).y + (b).y, (a).z + (b).z)

// dir
enum
{
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
	CHAR_DIR_UP,
	CHAR_DIR_DOWN,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL CollisionBB(XMFLOAT3 mpos, float mw, float mh, XMFLOAT3 ypos, float yw, float yh);
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2);
