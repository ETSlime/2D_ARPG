//=============================================================================
//
// BG���� [bg.h]
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


#define TEXTURE_WIDTH				(5026)//(3015)			// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(2006)//(1360)			// (SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(3)				// �e�N�X�`���̐�

#define	SKY_H						(150.0f)	// ��̃|���S���̍���
#define MAP01_GROUND_MAX			(1)
#define GROUND_H					(2006 - 1740 - 100)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

BG* GetBG(void);

AABB* GetMap01AABB(void);

