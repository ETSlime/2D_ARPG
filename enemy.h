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
#define ENEMY_MAX							(11)			// �G�l�~�[��Max�l��
#define LIGHTPOINT_MAX						(80)

// battle
#define ENEMY_VISION_RADIUS			(500.0f)
#define CYCLOPS_ATTACK_RADIUS		(100.0f)
#define GARGOYLE_ATTACK_RADIUS		(100.0f)
#define GNOLL_ATTACK_RADIUS			(100.0f)
#define GOBLIN_ATTACK_RADIUS		(100.0f)
#define GOLEM_ATTACK_RADIUS			(100.0f)
#define IMP_ATTACK_RADIUS			(100.0f)
#define MUMMY_ATTACK_RADIUS			(100.0f)
#define OGRE_ATTACK_RADIUS			(100.0f)
#define SKELL_ATTACK_RADIUS			(100.0f)
#define	ENEMY_HIT_TIMER				(8.0f)
#define	ENEMY_HIT_CD				(32.0f)
#define ENEMY_STAGGER_RECOVERY_TIME (80)
#define MAX_CHASE_DISTANCE			(800.0f)	// �ő�ǐՋ���
#define MIN_RETURN_DISTANCE			(600.0f)	// �ŏ��̖߂苗���i�o�b�t�@�]�[���j
#define RETREAT_SPEED_RATE			(0.5f)		// �|�ގ��̑��x
#define ENEMY_STUN_TIME				(35)
#define ENEMY_FALL_SPEED			(6.5f)
#define ENEMY_FALL_CNT_MAX			(30)

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
	SKELL,
	BOSS,
};

// states
enum
{
	ENEMY_INIT,
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

enum
{
	BOSS_FIGHT_STAGE_01,
	BOSS_FIGHT_STAGE_02,
	BOSS_FIGHT_STAGE_03,
	BOSS_FIGHT_STAGE_04,
	BOSS_FIGHT_STAGE_05,
	BOSS_FIGHT_STAGE_06,
	BOSS_FIGHT_STAGE_07
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

struct LightPoint
{
	BOOL use;
	BOOL flyingToPlayer;
	XMFLOAT3 pos;
	XMFLOAT3 move;
	float w, h;
	float timeToPlayer;
	float timeToFly;
	float distanceToPlayer;
	float desiredToPlayerTime;
};

struct BossMagic
{
	BOOL		active;
	float		timeInterval;
	float		currentTime;
	int			maxCount;
	int			currentCount;
	XMFLOAT3	initPos;
	int			magicType;
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
	BOOL		update;
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
	BOOL		disableMoveTbl;

	// battle
	EnemyAttributes attributes;
	BOOL		isHit;
	float		hitTimer;
	float		hitCD;
	XMFLOAT3	diePos;
	float		dieInitSpeedX;
	float		dieInitSpeedY;
	BOOL		hitPlayer;

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
void UpdateEnemyMoveTbl(ENEMY* enemy, float newPosY);
void UpdateBossMove(ENEMY* enmey);
void UpdateBossFight(ENEMY* enemy);
void DrawEnemy(void);
void DrawEnemySprite(const ENEMY* enemy, BOOL die = FALSE);
void DrawBossSprite(const ENEMY* enemy);
void DrawEnemyHPGauge(const ENEMY* enemy);
void DrawBossHPGauge(const ENEMY* enemy);
void DrawEnemyShadow(const ENEMY*);
void DrawLightPoint(void);

void HandleEnemyMagic(ENEMY* enemy);
void HandleBossAction(ENEMY* enemy, int stage);

// anim
void PlayEnemyWalkAnim(ENEMY* enemy);
void PlayEnemyAttackAnim(ENEMY* enemy);
void PlayEnemyIdleAnim(ENEMY* enemy);
void PlayEnemyHitAnim(ENEMY* enemy);
void PlayEnemyDieAnim(ENEMY* enemy);
void PlayBossAnim(ENEMY* enemy);
void UpdateEnemyAttackAABB(ENEMY* enemy);
void UpdateLightPoint(void);
BOOL CheckChasingPlayer(const ENEMY* enemy);
void EnemyDieOnTrigger(ENEMY* enemy);
void UpdateEnemyGroundCollision(ENEMY* enemy);
BOOL CheckEnemyMoveCollision(ENEMY* enemy, XMFLOAT3 newPos, int dir);


const EnemyAttributes* GetEnemyAttributes(ENEMY* enemy);
void SetupEnemyAttributes(ENEMY* enemy);
ENEMY* GetEnemy(void);
int GetEnemyCount(void);
float GetCurrentTextureSizeW(int enemyType);
float GetCurrentTextureSizeH(int enemyType);

void ClearEnemy(ENEMY* enemy);

BOOL GetUpdateEnemy(void);
void SetUpdateEnemy(BOOL update);

void SetUpdateEnemyByIdx(int index, BOOL update);