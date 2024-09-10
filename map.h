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

	BOOL		scroll;
	float		scrollSpeedX;
	float		scrollSpeedY;
	float		scrollTime;

	XMFLOAT3	old_pos;	// １フレ前のポリゴンの座標
};

struct Teleport
{
	XMFLOAT3	pos;
	int			mapNo;
	int			nextInitPos;
	int			nextMapNo;
	BOOL		use;
	AABB		teleportAABB;

	float		countAnim;
	int			patternAnim;
};

struct EnemyConfig
{
	int enemyType;
	INTERPOLATION_DATA* moveTbl;
	int moveTblSize;
};

// 背景サイズ
#define TEXTURE_BG_TUTORIAL_01_WIDTH		(3770.0f)			
#define TEXTURE_BG_TUTORIAL_01_HEIGHT		(1504.0f)
#define TEXTURE_BG_MAP_01_WIDTH				(4368.0f)			
#define TEXTURE_BG_MAP_01_HEIGHT			(2448.0f)
#define TEXTURE_BG_MAP_02_WIDTH				(3770.0f)			
#define TEXTURE_BG_MAP_02_HEIGHT			(1504.0f)
#define TEXTURE_BG_MAP_03_WIDTH				(3770.0f)			
#define TEXTURE_BG_MAP_03_HEIGHT			(1504.0f)
#define TEXTURE_BG_MAP_BOSS_WIDTH			(2000.0f)			
#define TEXTURE_BG_MAP_BOSS_HEIGHT			(2000.0f)
#define TEXTURE_MAX							(35)				// テクスチャの数

#define	MAP_NUM_MAX					(5)
#define MAP_GROUND_MAX				(10)
#define MAP_ENEMY_MAX				(10)
#define MOVE_NUM_MAX				(5)
#define TELEPORT_NUM_MAX			(4)
#define PLAYER_INIT_POS_MAX			(4)

#define TUTORIAL01_GROUND_H			(214.0f)
#define MAP01_GROUND_H				(214.0f)
#define MAP02_GROUND_H				(214.0f)
#define MAP03_GROUND_H				(214.0f)

#define PLAYER_INIT_POS_X_MAP01_01	(566.0f)
#define PLAYER_INIT_POS_Y_MAP01_01	(2051.0f)
#define PLAYER_INIT_POS_X_MAP01_02	(4154.0f)
#define PLAYER_INIT_POS_Y_MAP01_02	(2051.0f)
#define PLAYER_INIT_POS_X_MAP02_01	(150.0f)
#define PLAYER_INIT_POS_Y_MAP02_01	(1303.5f)
#define PLAYER_INIT_POS_X_MAP02_02	(3603.0f)
#define PLAYER_INIT_POS_Y_MAP02_02	(1303.5f)

#define PLAYER_INIT_POS_X_MAP_BOSS_01	(182.0f)
#define PLAYER_INIT_POS_Y_MAP_BOSS_01	(1810.5f)

#define	PLAYER_INIT_POS_X_TUTORIAL01_01 (475.0f)
#define PLAYER_INIT_POS_Y_TUTORIAL01_01	(1356.5f)

#define	TUTORIAL_CYCLOPS_ID			(3)

enum
{
	TUTORIAL_01,
	MAP_01,
	MAP_02,
	MAP_03,
	MAP_BOSS,
};

enum
{
	INITPOS_01,
	INITPOS_02,
	INITPOS_03,
	INITPOS_04,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMap(void);
void InitMapBG(int map);
void InitMapCollisionBox(int map);
void InitMoveTbl(int map);
void InitEnemyConfig(int map);
void InitPlayerInitPos(int map);
void InitTeleport(int map);
void UninitMap(void);
void UpdateMap(void);
void UpdateMapDraw(void);
void UpdateTeleport(void);
void DrawMap(void);

void DisableTeleport(int teleportNo, BOOL disable);

BG* GetBG(void);
void ScrollBG(float x, float y, float time);

AABB* GetMapAABB(void);
EnemyConfig* GetEnemyConfig(void);
XMFLOAT3 GetPlayerInitPos(int map, int idx);

int GetCurrentMap();
void SetCurrentMap(int map);

void DrawMapWalls(int map);
void DrawTeleport(void);