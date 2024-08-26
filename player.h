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
#define MAX_ATTACK_AABB			(3)

#define	PLAYER_OFFSET_CNT		(8)	// 8���g
#define ACTION_QUEUE_SIZE		(4)
#define ACTION_QUEUE_CLEAR_WAIT	(150)
#define DASH_CD_TIME			(40)
#define MAX_DASH_COUNT			(2)
#define ATTACK_PATTERN_MAX		(4)		// �L���[���N���A����܂ł̑ҋ@����

#define PLAYER_INIT_POS_X		(938.0f)
#define PLAYER_INIT_POS_Y		(1306.5f)
#define FALLING_THRESHOLD		(3.0f)
#define HARDLANDING_HEIGHT		(60)
#define	DIVE_ATTACK_SPEED		(10.0f)

#define SET_PLAYER_POS_Y(y_value) \
    do { \
        g_Player->pos.y = (y_value); \
        g_Player->bodyAABB.pos.y = (y_value); \
    } while (0)

#define SET_PLAYER_POS_X(x_value) \
    do { \
        g_Player->pos.x = (x_value); \
        g_Player->bodyAABB.pos.x = (x_value); \
    } while (0)

#define CHANGE_PLAYER_POS_Y(y_value) \
    do { \
        g_Player->pos.y += (y_value); \
        g_Player->bodyAABB.pos.y += (y_value); \
    } while (0)

#define CHANGE_PLAYER_POS_X(x_value) \
    do { \
        g_Player->pos.x += (x_value); \
        g_Player->bodyAABB.pos.x += (x_value); \
    } while (0)

// animation
enum
{
	CHAR_IDLE,
	CHAR_WALK,
	CHAR_RUN,
	CHAR_DASH,
	CHAR_NORMAL_ATTACK1,
	CHAR_NORMAL_ATTACK2,
	CHAR_NORMAL_ATTACK3,
	CHAR_NORMAL_ATTACK4,
	CHAR_DASH_ATTACK,
	CHAR_JUMP,
	CHAR_HARD_LANDING,
	CHAR_HIT,
	CHAR_KNOCKDOWN,
	CHAR_REBOUND,
	CHAR_SHADOW,
};

// dir
enum
{
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
	CHAR_DIR_UP,
	CHAR_DIR_DOWN,
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
	FALL,
	HARD_LANDING,
	HIT,
	KNOCKDOWN,
	REBOUND,
};

// attack pattern
enum
{
	NORMAL_ATTACK1,
	NORMAL_ATTACK2,
	NORMAL_ATTACK3,
	NORMAL_ATTACK4,
	NONE,
	DASH_ATTACK,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PLAYER
{
	// anim/texture/pos
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
	BOOL		invertTex;

	// dash
	int			dashCount;
	int			dashCD;
	int			airDashCount;
	int			maxDashCount;

	// state
	int			dir;			// �����i0:�� 1:�E 2:�� 3:���j
	BOOL		running;
	BOOL		playAnim;
	BOOL		dashOnAir;
	BOOL		jumpOnAir;
	BOOL		jump;			// �W�����v�t���O
	int			jumpCnt;		// �W�����v���̃J�E���g
	float		jumpYMax;		// 
	int			jumpOnAirCnt;
	int			onAirCnt;

	XMFLOAT3	move;			// �ړ����x
	XMFLOAT3	offset[PLAYER_OFFSET_CNT];		// �c���|���S���̍��W

	//	action queue
	int			attackPattern;
	int			actionQueue[ACTION_QUEUE_SIZE];
	int			actionQueueStart;
	int			actionQueueEnd;
	int			actionQueueClearTime;

	// AABB
	AABB		bodyAABB;
	AABB		attackAABB[MAX_ATTACK_AABB];
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void DrawPlayerShadow(void);

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
void PlayFallAnim(void);
void PlayHardLandingAnim(void);
void AdjustAttackTexturePos(float& px, float& py);
void AdjustAttackTextureSize(void);
void AdjustAttackPlayerPos(void);

void HandleActionQueue(void);
void UpdateBackGroundScroll(void);
// �L�[���͂ňړ�
void UpdateKeyboardInput(void);
void HandlePlayerMove(float speed, int direction);
void HandlePlayerDash(void);
void HandlePlayerJump(void);
void HandlePlayerAttack(void);
// �Q�[���p�b�h�łňړ�
void UpdateGamepadInput(void);
// �����蔻��
void UpdateGroundCollision(void);
void UpdateActionQueue(void);
void UpdatePlayerAttackAABB(void);

// �v���C���[�ƒn�ʂ̏Փ˔���֐�
BOOL CheckGroundCollision(PLAYER* g_Player, AABB* ground);
BOOL CheckMoveCollision(float move, int dir, BOOL checkGround = FALSE);
