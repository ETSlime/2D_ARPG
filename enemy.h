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
#define ENEMY_MAX							(11)			// エネミーのMax人数
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
#define MAX_CHASE_DISTANCE			(800.0f)	// 最大追跡距離
#define MIN_RETURN_DISTANCE			(600.0f)	// 最小の戻り距離（バッファゾーン）
#define RETREAT_SPEED_RATE			(0.5f)		// 倒退時の速度
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
	SKELL,
	BOSS,
};

// states
enum
{
	ENEMY_INIT,
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
// 構造体定義
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
	XMFLOAT3 move;			// 移動速度
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
	BOOL		update;
	int			dir;			// 向き
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