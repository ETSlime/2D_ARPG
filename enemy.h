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
#define ENEMY_MAX			(1)			// �G�l�~�[��Max�l��
#define MAX_ATTACK_AABB		(3)
#define ENEMY_VISION_RADIUS (500.0f)
#define ENEMY_ATTACK_RADIUS (100.0f)
#define MAX_CHASE_DISTANCE	(800.0f)	// �ő�ǐՋ���
#define MIN_RETURN_DISTANCE	(600.0f)	// �ŏ��̖߂苗���i�o�b�t�@�]�[���j
#define RETREAT_SPEED_RATE	(0.5f)		// �|�ގ��̑��x


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
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

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
	XMFLOAT3	move;			// �ړ����x
	int			idleCount;
	int			state;
	BOOL		finishAttack;
	int			attackCooldown;
	XMFLOAT3	returnPos;
	BOOL		canFly;
	BOOL		stepBack;

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
void DrawEnemy(void);

ENEMY* GetEnemy(void);

int GetEnemyCount(void);

int GetCurrentTextureSizeW(int enemyType);
int GetCurrentTextureSizeH(int enemyType);
void PlayEnemyWalkAnim(ENEMY* enemy);
void PlayEnemyAttackAnim(ENEMY* enemy);
void PlayEnemyIdleAnim(ENEMY* enemy);

void UpdateAttackAABB(ENEMY* enemy);

BOOL CheckChasingPlayer(const ENEMY* enemy);
void ChasePlayer(ENEMY* enemy);