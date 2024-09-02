//=============================================================================
//
// magic処理 [magic.h]
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
// マクロ定義
//*****************************************************************************
#define FIRE_BALL_SPEED							(6.0f)
#define MAGIC_MAX								(5)

struct Magic
{
	BOOL use;
	BOOL invertTex;
	BOOL isAnimRepeat;
	BOOL followPlayer;
	BOOL isCollision;
	float width, height;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	int magicType;			// 魔法の種類（テクスチャー）
	XMFLOAT3 move;			// 魔法の移動量
	int	countAnim;			// アニメーションカウント
	int	patternAnim;		// アニメーションパターンナンバー
	AABB magicAABB;
};

enum
{
	MAGIC_HEALING,
	MAGIC_FIRE_BALL,
	MAGIC_FLAMEBLADE,
	MAGIC_BOOM,
	MAGIC_NONE,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);

int GetMagicTexturePatternDivideX(Magic* magic);
int GetMagicTexturePatternDivideY(Magic* magic);
int GetMagicAnimWait(Magic* magic);
void TriggerMagic(int magic, XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
void ClearMagic(Magic* magic);
void SetUpMagicCollision(Magic* magic);
void HandleMagicCollision(Magic* magic);