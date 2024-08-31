//=============================================================================
//
// �G�l�~�[���� [enemy.h]
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
#define ENEMY_MAX							(1)			// �G�l�~�[��Max�l��
#define MAX_ATTACK_AABB						(3)
#define ATTACK_COOLDOWN_TIME_CYCLOPS		(100)
#define ATTACK_COOLDOWN_TIME_GARGOYLE		(100)
#define ATTACK_COOLDOWN_TIME_GNOLL			(100)
#define ATTACK_COOLDOWN_TIME_GOBLIN			(100)
#define ATTACK_COOLDOWN_TIME_GOLEM			(100)
#define ATTACK_COOLDOWN_TIME_IMP			(100)
#define ATTACK_COOLDOWN_TIME_MUMMY			(100)
#define ATTACK_COOLDOWN_TIME_OGRE			(100)
#define ATTACK_COOLDOWN_TIME_SKELL			(100)

#define SET_ENEMY_POS(enemy, value) \
    do { \
        enemy->pos = value; \
        enemy->bodyAABB.pos = value; \
    } while (0)

// �G�̎��
enum
{
	CYCLOPS,
	GARGOYLE,
	GNOLL,
	GOBLIN,
	GOLEM,
	IMP,
	MUMMY,
	OGRE,
	SKELL
};

// states
enum
{
	ENEMY_IDLE,		// �ҋ@���
	ENEMY_WALK,		// �ړ����
	ENEMY_CHASE,	// �v���C���[��ǐՂ�����
	ENEMY_ATTACK,	// �U�����
	ENEMY_COOLDOWN,	// �U����̃N�[���_�E�����
	ENEMY_WALK_BACK,// �߂���
	ENEMY_RETREAT,	// �|�ޏ��
	ENEMY_HIT,		// �d�����
	ENEMY_DIE,		// ���񂾏��
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct EnemyAttributes
{
	float	hp;
	float	maxHp;
	int damage;
	int	staggerResistance;
	int	staggerRecoveryTime;
	int stunTime;
	float attackRange;
	int	attackCooldown;
	BOOL canFly;
	XMFLOAT3 move;			// �ړ����x
};

struct ENEMY
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	XMFLOAT3	scl;			// �|���S���̊g��k��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			enemyType;		// �e�N�X�`���ԍ�
	BOOL		invertTex;

	// state
	int			dir;			// ����
	int			oldDir;
	int			idleCount;
	int			state;
	int			stateOld;
	BOOL		finishAttack;
	XMFLOAT3	returnPos;
	BOOL		stepBack;
	int			onAirCnt;
	BOOL		isFalling;

	// battle
	EnemyAttributes attributes;
	BOOL		isHit;
	float		hitTimer;
	float		hitCD;
	XMFLOAT3	diePos;
	float		dieInitSpeedX;
	float		dieInitSpeedY;

	float		time;			// ���`��ԗp
	int			tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;			// ���̃e�[�u���̃f�[�^��

	// AABB
	AABB		bodyAABB;
	AABB		attackAABB[MAX_ATTACK_AABB];

	//INTERPOLATION_DATA* tbl_adr;			// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	//int					tbl_size;			// �o�^�����e�[�u���̃��R�[�h����
	//float				move_time;			// ���s����
};

struct AttackAABBTBL
{
	XMFLOAT3 posOffset;
	float w;
	float h;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void UpdateEnemyStates(ENEMY* enemy);
void DrawEnemy(void);
void DrawEnemySprite(const ENEMY* enemy, BOOL die = FALSE);
void DrawEnemyHPGauge(const ENEMY* enemy);
void EnemyTakeDamage(ENEMY* enemy);
ENEMY* GetEnemy(void);

int GetEnemyCount(void);

int GetCurrentTextureSizeW(int enemyType);
int GetCurrentTextureSizeH(int enemyType);

// anim
void PlayEnemyWalkAnim(ENEMY* enemy);
void PlayEnemyAttackAnim(ENEMY* enemy);
void PlayEnemyIdleAnim(ENEMY* enemy);
void PlayEnemyHitAnim(ENEMY* enemy);
void PlayEnemyDieAnim(ENEMY* enemy);
void UpdateEnemyAttackAABB(ENEMY* enemy);

BOOL CheckChasingPlayer(const ENEMY* enemy);

void UpdateEnemyGroundCollision(ENEMY* enemy);
BOOL CheckEnemyMoveCollision(ENEMY* enemy, XMFLOAT3 newPos, int dir);

void SetupEnemyAttributes(ENEMY* enemy);

const EnemyAttributes* GetEnemyAttributes(ENEMY* enemy);