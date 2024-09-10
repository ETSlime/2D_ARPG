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
#define DIALOGUE_START_TEX_NO		(2)

enum
{
	TUTORIAL_RUN,
	TUTORIAL_ATTACK,
	TUTORIAL_ENEMY,
	TUTORIAL_DASH,
	TUTORIAL_COMBO,
	TUTORIAL_MAGIC,
	TUTORIAL_EXPLORE,
	TUTORIAL_AIRJUMP,
	TUTORIAL_EXPLORE2,
	TUTORIAL_DEFEND,
	TUTORIAL_PARRY,
	TUTORIAL_ENEMY2,
	TUTORIAL_FINAL,
};

enum
{
	DIALOGUE_00 = DIALOGUE_START_TEX_NO,
	DIALOGUE_00_2,
	DIALOGUE_01,
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
	DIALOGUE_17,
	DIALOGUE_18,
	DIALOGUE_19,
	DIALOGUE_20,
	DIALOGUE_21,
	DIALOGUE_22,
	DIALOGUE_23,
	DIALOGUE_24,
	DIALOGUE_25,
	DIALOGUE_26,
	DIALOGUE_27,
	DIALOGUE_28,
	DIALOGUE_29,
	DIALOGUE_30,
};

enum
{
	PAUSE_NONE,
	PAUSE_RUN,
	PAUSE_ATTACK,
	PAUSE_ENEMY,
	PAUSE_DASH,
	PAUSE_COMBO,
	PAUSE_MAGIC,
	PAUSE_EXPLORE,
	PAUSE_AIRJUMP,
	PAUSE_EXPLORE2,
	PAUSE_DEFEND,
	PAUSE_PARRY,
	PAUSE_ENEMY2,
	PAUSE_FINAL,
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
int GetTutorialStage(void);

int GetNextStage(void);

void DisableAllPlayerAction(BOOL disbale);
