//=============================================================================
//
// プレイヤー処理 [player.h]
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
#define PLAYER_MAX				(1)		// プレイヤーのMax人数

#define	PLAYER_OFFSET_CNT		(8)	// 8分身
#define ACTION_QUEUE_SIZE		(4)
#define ACTION_QUEUE_CLEAR_WAIT	(150)
#define DASH_CD_TIME			(40)
#define MAX_DASH_COUNT			(2)
#define ATTACK_PATTERN_MAX		(4)		// キューをクリアするまでの待機時間

#define PLAYER_INIT_POS_X		(1250.0f)
#define PLAYER_INIT_POS_Y		(1742.0f)
#define FALLING_THRESHOLD		(3.0f)

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
// 構造体定義
//*****************************************************************************

struct PLAYER
{
	// anim/texture/pos
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	rot;			// ポリゴンの回転量
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	float		shadowY;		// 影の高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			patternAnimOld;
	int			texNo;			// テクスチャ番号
	int			state;
	int			animFrameCount;

	// dash
	int			dashCount;
	int			dashCD;
	int			airDashCount;
	int			maxDashCount;

	// state
	int			dir;			// 向き（0:上 1:右 2:下 3:左）
	BOOL		running;
	BOOL		playAnim;
	BOOL		dashOnAir;
	BOOL		jumpOnAir;
	BOOL		jump;			// ジャンプフラグ
	int			jumpCnt;		// ジャンプ中のカウント
	float		jumpYMax;		// 
	int			jumpOnAirCnt;
	int			onAirCnt;

	XMFLOAT3	move;			// 移動速度
	XMFLOAT3	offset[PLAYER_OFFSET_CNT];		// 残像ポリゴンの座標

	//	action queue
	int			attackPattern;
	int			actionQueue[ACTION_QUEUE_SIZE];
	int			actionQueueStart;
	int			actionQueueEnd;
	int			actionQueueClearTime;

	// AABB
	AABB		bodyAABB;
	AABB		attackAABB;
};

//*****************************************************************************
// プロトタイプ宣言
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
void AdjustAttackTexturePosition(float& px, float& py);
void AdjustAttackTextureSize(void);

// キー入力で移動
void UpdateKeyboardInput(void);
// ゲームパッドでで移動
void UpdateGamepadInput(void);
// 当たり判定
void UpdateGroundCollision(void);
void UpdateActionQueue(void);

// プレイヤーと地面の衝突判定関数
BOOL CheckGroundCollision(PLAYER* g_Player, AABB* ground);
BOOL CheckMoveCollision(float move, int dir);
