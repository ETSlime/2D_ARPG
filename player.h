//=============================================================================
//
// �v���C���[���� [player.h]
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
#define PLAYER_MAX				(1)		// �v���C���[��Max�l��

#define	PLAYER_OFFSET_CNT		(8)	// 8���g
#define ACTION_QUEUE_SIZE		(4)
#define ACTION_QUEUE_CLEAR_WAIT	(150)
#define DASH_CD_TIME			(40)
#define ATTACK_PATTERN_MAX		(4)		// �L���[���N���A����܂ł̑ҋ@����

// animation
enum
{
	CHAR_IDLE_RIGHT,
	CHAR_IDLE_LEFT,
	CHAR_WALK_RIGHT,
	CHAR_WALK_LEFT,
	CHAR_RUN_RIGHT,
	CHAR_RUN_LEFT,
	CHAR_DASH_RIGHT,
	CHAR_DASH_LEFT,
	CHAR_NORMAL_ATTACK1,
	CHAR_NORMAL_ATTACK2,
	CHAR_NORMAL_ATTACK3,
	CHAR_NORMAL_ATTACK4,
	CHAR_JUMP,
	CHAR_SHADOW,
};

// dir
enum
{
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
};

// states
enum
{
	IDLE,
	WALK,
	RUN,
	DASH,
	ATTACK,
	JUMP,
};

// attack pattern
enum
{
	NORMAL_ATTACK1,
	NORMAL_ATTACK2,
	NORMAL_ATTACK3,
	NORMAL_ATTACK4,
	NONE,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PLAYER
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	float		shadowY;		// �e�̍���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			patternAnimOld;
	int			texNo;			// �e�N�X�`���ԍ�
	int			state;
	int			animFrameCount;
	int			dashCount;

	int			dir;			// �����i0:�� 1:�E 2:�� 3:���j
	BOOL		running;
	BOOL		playAnim;
	BOOL		dashOnAir;
	XMFLOAT3	move;			// �ړ����x
	XMFLOAT3	offset[PLAYER_OFFSET_CNT];		// �c���|���S���̍��W

	int			attackPattern;
	int			actionQueue[ACTION_QUEUE_SIZE];
	int			actionQueueStart;
	int			actionQueueEnd;
	int			actionQueueClearTime;

	BOOL		jump;			// �W�����v�t���O
	int			jumpCnt;		// �W�����v���̃J�E���g
	float		jumpYMax;		// 
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);

int GetPlayerCount(void);

int GetTexturePatternDivideX(void);


// anim
void PlayIdleAnim(void);
void PlayWalkAnim(void);
void PlayRunningAnim(void);
void PlayDashAnim(void);
void PlayAttackAnim(void);
void PlayJumpAnim(void);
void AdjustAttackTexturePosition(float& px, float& py);
void AdjustAttackTextureSize(void);

void UpdateActionQueue(void);