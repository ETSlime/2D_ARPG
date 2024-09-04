//=============================================================================
//
// map処理 [map.h]
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

#define TEXTURE_WIDTH				(3770)//(3015)			// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(1504)//(1360)			// (SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(12)				// テクスチャの数

#define	MAP_NUM_MAX					(5)
#define MAP_GROUND_MAX				(10)
#define MAP_ENEMY_MAX				(10)
#define MAP01_GROUND_H				(1504 - 1290)

enum
{
	TUTORIAL_01,
	TUTORIAL_02,
	MAP_01,
	ROCK_01,
	ROCK_02,
	ROCK_03,
	ROCK_04,
};

struct EnemyConfig
{
	int enemyType;
	INTERPOLATION_DATA* moveTbl;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMap(void);
void InitMapCollisionBox(int map);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

BG* GetBG(void);

AABB* GetMap01AABB(void);
EnemyConfig* GetEnemyConfig(int map);

