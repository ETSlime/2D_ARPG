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

#define TEXTURE_WIDTH				(3770)//(3015)			// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(1504)//(1360)			// (SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(12)				// �e�N�X�`���̐�

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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMap(void);
void InitMapCollisionBox(int map);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

BG* GetBG(void);

AABB* GetMap01AABB(void);
EnemyConfig* GetEnemyConfig(int map);

