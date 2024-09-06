//=============================================================================
//
// tutorial処理 [tutorial.h]
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

#define TEXTURE_MAX					(12)				// テクスチャの数

#define DIALOGUE_BG_TEX_NO			(0)
#define HIGHLIGHT_BOX_TEX_NO		(1)
#define DIALOGUE_START_TEX_NO		(4)

enum
{
	TUTORIAL_RUN,
	TUTORIAL_ATTACK,
	TUTORIAL_ENEMY,
	TUTORIAL_DASH,
	TUTORIAL_COMBO,
};

enum
{
	DIALOGUE_01 = 2,
	DIALOGUE_02,
	DIALOGUE_03,
	DIALOGUE_04,
	DIALOGUE_05,
	DIALOGUE_06,
	DIALOGUE_07,
	DIALOGUE_08,
	DIALOGUE_09,
	DIALOGUE_10,
	DIALOGUE_11,
	DIALOGUE_12,
	DIALOGUE_13,
	DIALOGUE_14,
	DIALOGUE_15,
	DIALOGUE_16,

};

enum
{
	PAUSE_NONE,
	PAUSE_RUN,
	PAUSE_ATTACK,
	PAUSE_ENEMY,
	PAUSE_DASH,
	PAUSH_COMBO,
};

struct DialogueSprite
{
	XMFLOAT3 pos;
	float w, h;
	float fadeCount;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

void SetTutorialWait(int wait);
void AdjustDialogueWH(float* w, float* h);

void SetTutorialPause(int pauseType);
int GetTutorialPause(void);
int GetNextStage(void);