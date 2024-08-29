//=============================================================================
//
// エネミー処理 [enemy.h]
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
#define ENEMY_MAX					(1)			// エネミーのMax人数
#define MAX_ATTACK_AABB				(3)


#define SET_ENEMY_POS(enemy, value) \
    do { \
        enemy->pos = value; \
        enemy->bodyAABB.pos = value; \
    } while (0)

// 敵の種類
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
	ENEMY_IDLE,		// 待機状態
	ENEMY_WALK,		// 移動状態
	ENEMY_CHASE,	// プレイヤーを追跡する状態
	ENEMY_ATTACK,	// 攻撃状態
	ENEMY_COOLDOWN,	// 攻撃後のクールダウン状態
	ENEMY_WALK_BACK,// 戻る状態
	ENEMY_RETREAT,	// 倒退状態
	ENEMY_HIT,		// 硬直状態
	ENEMY_DIE,		// 死んだ状態
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct ENEMY
{
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	rot;			// ポリゴンの回転量
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			enemyType;		// テクスチャ番号
	BOOL		invertTex;

	// state
	int			dir;			// 向き
	int			oldDir;
	XMFLOAT3	move;			// 移動速度
	int			idleCount;
	int			state;
	int			stateOld;
	BOOL		finishAttack;
	int			attackCooldown;
	XMFLOAT3	returnPos;
	BOOL		canFly;
	BOOL		stepBack;
	int			onAirCnt;
	BOOL		isFalling;

	// battle
	int			damage;
	BOOL		isHit;
	float		hitTimer;
	float		hitCD;
	float		attackRange;
	int			hp;
	int			maxHp;
	int			staggerResistance;
	int			staggerRecovery;
	XMFLOAT3	diePos;
	float		dieInitSpeedX;
	float		dieInitSpeedY;

	float		time;			// 線形補間用
	int			tblNo;			// 行動データのテーブル番号
	int			tblMax;			// そのテーブルのデータ数

	// AABB
	AABB		bodyAABB;
	AABB		attackAABB[MAX_ATTACK_AABB];

	//INTERPOLATION_DATA* tbl_adr;			// アニメデータのテーブル先頭アドレス
	//int					tbl_size;			// 登録したテーブルのレコード総数
	//float				move_time;			// 実行時間
};

struct AttackAABBTBL
{
	XMFLOAT3 posOffset;
	float w;
	float h;
};

//*****************************************************************************
// プロトタイプ宣言
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

void SetupEnemyStates(ENEMY* enemy);