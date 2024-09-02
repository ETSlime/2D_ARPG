//=============================================================================
//
// UI���� [UI.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

struct UISprite
{
	BOOL use;
	float width, height;
	float ratio;
	XMFLOAT3 pos;
};

// texture num
enum
{
	UI_HP,
	UI_MP,
	UI_ST,
	UI_GAUGE_COVER,
	UI_JUMP_ICON,
	UI_MAGIC_HEAL,
	UI_MAGIC_FLAMEBLADE,
	UI_MAGIC_FIRE_BALL,
	UI_FLAMEBLADE_ICON,
	UI_SKILL_ENABLED,
	UI_NONE,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
void DrawPlayerGauge(void);
void DrawPlayerJumpIcon(void);
void DrawSkillIcon(void);
void DrawFlamebladeIcon(void);

