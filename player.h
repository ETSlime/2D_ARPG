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
#include "enemy.h"
#include "collision.h"
#include "magic.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX				(1)		// �v���C���[��Max�l��
#define MAX_ATTACK_AABB			(3)

#define PLAYER_MAX_HP			(2000.0f)
#define PLAYER_MAX_MP			(1800.0f)
#define PLAYER_MAX_ST			(1000.0f)

#define	PLAYER_OFFSET_CNT		(12)	// 12���g
#define ACTION_QUEUE_SIZE		(4)
#define ACTION_QUEUE_CLEAR_WAIT	(120)
#define ATTACK_COMBO_WINDOW		(70)
#define	PARRY_WINDOW			(15)
#define DASH_CD_TIME			(60)
#define MAX_DASH_COUNT			(2)
#define ATTACK_PATTERN_MAX		(5)

#define PLAYER_WALK_SPEED		(3.75f)
#define FALLING_THRESHOLD		(3.0f)
#define	KNOCKDOWN_THRESHOLD		(15)
#define HARDLANDING_HEIGHT		(60)
#define	DIVE_ATTACK_SPEED		(10.0f)
#define DASH_INTERVAL			(25)
#define MAGIC_NUM_MAX			(3)
#define	HEALING_CD_TIME			(100.0f)
#define	FIRE_BALL_CD_TIME		(200.0f)

#define NORMAL_ATTACK1_DAMAGE_RATE		(1.0f)
#define NORMAL_ATTACK2_DAMAGE_RATE		(1.2f)
#define NORMAL_ATTACK3_DAMAGE_RATE		(1.3f)
#define NORMAL_ATTACK4_DAMAGE_RATE		(1.5f)
#define DASH_ATTACK_DAMAGE_RATE			(1.4f)
#define PARRY_DAMAGE_RATE				(1.7f)
#define FLAME_ATTACK1_DAMAGE_RATE		(1.6f)
#define FLAME_ATTACK2_DAMAGE_RATE		(2.0f)

#define NORMAL_ATTACK1_POISE_RATE		(1.0f)
#define NORMAL_ATTACK2_POISE_RATE		(1.1f)
#define NORMAL_ATTACK3_POISE_RATE		(1.1f)
#define NORMAL_ATTACK4_POISE_RATE		(1.8f)
#define DASH_ATTACK_POISE_RATE			(1.8f)
#define PARRY_POISE_RATE				(1.5f)
#define FLAME_ATTACK1_POISE_RATE		(1.6f)
#define FLAME_ATTACK2_POISE_RATE		(2.0f)

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
	CHAR_PARRY,
	CHAR_FLAME_ATTACK1,
	CHAR_FLAME_ATTACK2,
	CHAR_JUMP,
	CHAR_HARD_LANDING,
	CHAR_HIT,
	CHAR_KNOCKDOWN,
	CHAR_REBOUND,
	CHAR_DEFEND,
	CHAR_SHADOW,
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
	DEFEND,
	CAST,
	DIE,
};

// attack pattern
enum
{
	NONE,
	NORMAL_ATTACK1,
	NORMAL_ATTACK2,
	NORMAL_ATTACK3,
	NORMAL_ATTACK4,
	NORMAL_ATTACK_PATTERN_MAX,
	FLAME_ATTACK1,
	FLAME_ATTACK2,
	FLAME_ATTACK_PATTERN_MAX,
	DASH_ATTACK,
	PARRY,
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
	int			patternAnimCast;
	int			patternAnimOld;
	int			texNo;			// �e�N�X�`���ԍ�
	int			animFrameCount;
	int			animFrameCountCast;
	BOOL		invertTex;

	// dash
	int			dashCount;
	int			dashCD;
	int			airDashCount;
	int			maxDashCount;
	int			dashInterval;

	// state
	BOOL		die;
	BOOL		update;
	BOOL		playAnim;
	BOOL		dashOnAir;
	BOOL		jumpOnAir;
	BOOL		jump;			// �W�����v�t���O
	BOOL		knockDownFall;
	BOOL		isDefending;
	BOOL		isRunning;
	BOOL		isParrying;
	BOOL		isWalkingOnDefend;
	BOOL		wasRunningExhausted;
	BOOL		wasDefendingExhausted;
	int			state;
	int			dir;			// �����i0:�� 1:�E 2:�� 3:���j
	int			defendDir;
	int			jumpCnt;		// �W�����v���̃J�E���g
	float		jumpYMax;		// 
	int			jumpOnAirCnt;
	int			onAirCnt;
	int			defendCnt;
	int			jumpEffectCnt;
	int			magicCasting;
	XMFLOAT3	airJumpPos;

	// battle
	float		HP;
	float		maxHP;
	float		MP;
	float		maxMP;
	float		ST;
	float		maxST;
	float		ATK;
	float		DEF;
	int			attackInterval;
	int			magic;
	float		healingCD;
	float		fireBallCD;
	BOOL		isInvincible;
	BOOL		flameblade;

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
void InitPlayerStatus(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void DrawPlayerSprite(void);
void DrawPlayerShadow(void);
void DrawJumpEffect(void);
void DrawCastEffect(void);
void DrawDieFadeOut(void);

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
void PlayHitAnim(void);
void PlayKnockDownAnim(void);
void PlayReboundAnim(void);
void PlayHardLandingAnim(void);
void PlayDefendAnim(void);
void PlayCastAnim(void);
void PlayDieAnim(void);
void AdjustAttackTexturePos(float& px, float& py);
void AdjustAttackTextureSize(void);
void AdjustAttackPlayerPos(void);

void HandleActionQueue(void);
void UpdateBackGroundScroll(void);
// �L�[���͂ňړ�
void UpdateKeyboardInput(void);
void HandlePlayerMove(float speed, int direction);
void HandlePlayerRun(float speed);
void HandlePlayerDash(void);
void HandlePlayerJump(void);
void HandlePlayerAttack(void);
void HandlePlayerDefend(void);

float GetPlayerDamage(void);
float GetPoiseDamage(void);

// �Q�[���p�b�h�łňړ�
void UpdateGamepadInput(void);
// �����蔻��
void UpdateGroundCollision(void);
void UpdateActionQueue(void);
void UpdatePlayerAttackAABB(void);
void UpdatePlayerStates(void);
void PlayerTakeDamage(ENEMY* enemy = nullptr, Magic* magic = nullptr);

// �v���C���[�ƒn�ʂ̏Փ˔���֐�
BOOL CheckGroundCollision(PLAYER* g_Player, AABB* ground);
BOOL CheckMoveCollision(float move, int dir, BOOL checkGround = FALSE);

float GetActionStaminaCost(int action);

void SetPlayerInitPos(int map, int idx);
void SetPlayerHP(int HP);
void SetPlayerMP(int MP);
void SetPlayerST(int ST);
void SetPlayerDir(int dir);
void SetPlayerPosX(float posX);
void SetPlayerPosY(float posY);
void SetPlayerInvincible(BOOL invincible);
void SetLimitPlayerMove(BOOL limit);
void PlayerRespawn(void);

BOOL GetUpdatePlayer(void);
void SetUpdatePlayer(BOOL update);

void DisablePlayerAttack(BOOL disable);
void DisablePlayerJump(BOOL disable);
void DisablePlayerMagic(BOOL disable);
void DisablePlayerDash(BOOL disable);
void DisablePlayerDefend(BOOL disable);
void DisablePlayerRun(BOOL disable);
void DisablePlayerLeftMove(BOOL disable);
void DisablePlayerRightMove(BOOL disable);