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
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAGIC_MAX								(15)

struct Magic
{
	BOOL use;
	BOOL invertTex;
	BOOL isAnimRepeat;
	BOOL followPlayer;
	BOOL isCollision;
	BOOL destroyAnim;
	BOOL reverseAnim;
	float width, height;
	float damage;
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
	MAGIC_EARTH,
	MAGIC_STOMP,
	MAGIC_NONE,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);

void PlayMagicAnim(Magic* magic);
int GetMagicTexturePatternDivideX(Magic* magic);
int GetMagicTexturePatternDivideY(Magic* magic);
int GetMagicAnimWait(Magic* magic);
void TriggerMagic(int magic, XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f), ENEMY* enemy = nullptr);
void ClearMagic(Magic* magic);
void SetUpMagicCollision(Magic* magic);
void HandleMagicCollision(Magic* magic);

Magic* GetMagic();