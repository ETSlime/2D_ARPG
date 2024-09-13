//=============================================================================
//
// magic���� [magic.h]
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
// �}�N����`
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
	BOOL hitTarget;
	float width, height;
	float damage;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	int magicType;			// ���@�̎�ށi�e�N�X�`���[�j
	XMFLOAT3 move;			// ���@�̈ړ���
	int	countAnim;			// �A�j���[�V�����J�E���g
	int	patternAnim;		// �A�j���[�V�����p�^�[���i���o�[
	float delayMove;
	AABB magicAABB;
};

enum
{
	MAGIC_HEALING,
	MAGIC_FIRE_BALL,
	MAGIC_FLAMEBLADE,
	MAGIC_BOOM,
	MAGIC_STOMP,
	MAGIC_EARTH,
	MAGIC_THUNDER,
	MAGIC_BARRAGE,
	MAGIC_NONE,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMagic(void);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);

void InitializeMagicMove(Magic* magic, const XMFLOAT3& playerPos);
void PlayMagicAnim(Magic* magic);
int GetMagicTexturePatternDivideX(Magic* magic);
int GetMagicTexturePatternDivideY(Magic* magic);
int GetMagicAnimWait(Magic* magic);
void TriggerMagic(int magic, XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f), ENEMY* enemy = nullptr);
void ClearMagic(Magic* magic);
void SetUpMagicCollision(Magic* magic);
void HandleMagicCollision(Magic* magic);

Magic* GetMagic();
float GetMagicDamage(int type);
float GetMagicPoiseDamage(int type);