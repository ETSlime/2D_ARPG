//=============================================================================
//
// map���� [map.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct BG
{
	XMFLOAT3	pos;		// �|���S���̍��W
	float		w, h;		// ���ƍ���
	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	float		scrl;		// �X�N���[��
	XMFLOAT3	old_pos;	// �P�t���O�̃|���S���̍��W
	float		scrl2;		// �X�N���[��
};

#define TEXTURE_BG_WIDTH			(3770)//(3015)			// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_BG_HEIGHT			(1504)//(1360)			// (SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(25)				// �e�N�X�`���̐�

#define	MAP_NUM_MAX					(5)
#define MAP_GROUND_MAX				(10)
#define MAP_ENEMY_MAX				(10)
#define MOVE_NUM_MAX				(5)
#define PLAYER_INIT_POS_MAX			(4)
#define MAP01_GROUND_H				(1504 - 1290)

#define PLAYER_INIT_POS_X_MAP01_0	(938.0f)
#define PLAYER_INIT_POS_Y_MAP01_0	(1306.5f)

#define	PLAYER_INIT_POS_X_TUTORIAL01_0 (475.0f)
#define PLAYER_INIT_POS_Y_TUTORIAL01_0	(1356.5f)

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

enum
{
	INITPOS_01,
	INITPOS_02,
	INITPOS_03,
	INITPOS_04,
};

struct EnemyConfig
{
	int enemyType;
	INTERPOLATION_DATA* moveTbl;
	int moveTblSize;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMap(void);
void InitMapCollisionBox(int map);
void InitMoveTbl(int map);
void InitEnemyConfig(int map);
void InitPlayerInitPos(int map);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

BG* GetBG(void);

AABB* GetMap01AABB(void);
EnemyConfig* GetEnemyConfig(int map);
XMFLOAT3 GetPlayerInitPos(int map, int idx);

int GetCurrentMap();
void SetCurrentMap(int map);

void DrawMapWalls(int map);