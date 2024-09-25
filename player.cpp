//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "map.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "file.h"
#include "UI.h"
#include "tutorial.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// キャラサイズ
#define TEXTURE_IDLE_WIDTH					(160/2)
#define TEXTURE_IDLE_HEIGHT					(160/2) 
#define TEXTURE_NORMAL_ATTACK1_WIDTH		(285/2)
#define TEXTURE_NORMAL_ATTACK1_HEIGHT		(240/2)
#define TEXTURE_NORMAL_ATTACK2_WIDTH		(315/2)
#define TEXTURE_NORMAL_ATTACK2_HEIGHT		(260/2)
#define TEXTURE_NORMAL_ATTACK3_WIDTH		(312/2)
#define TEXTURE_NORMAL_ATTACK3_HEIGHT		(247/2)
#define TEXTURE_NORMAL_ATTACK4_WIDTH		(230/2)
#define TEXTURE_NORMAL_ATTACK4_HEIGHT		(245/2)
#define TEXTURE_FLAME_ATTACK1_WIDTH			(296/2)
#define TEXTURE_FLAME_ATTACK1_HEIGHT		(245/2)
#define TEXTURE_FLAME_ATTACK2_WIDTH			(326/2)
#define TEXTURE_FLAME_ATTACK2_HEIGHT		(285/2)
#define TEXTURE_DASH_ATTACK_WIDTH			(315/2)
#define TEXTURE_DASH_ATTACK_HEIGHT			(235/2)
#define TEXTURE_PARRY_WIDTH					(312/2)
#define TEXTURE_PARRY_HEIGHT				(295/2)
#define TEXTURE_RUN_WIDTH					(185/2)
#define TEXTURE_RUN_HEIGHT					(185/2)
#define TEXTURE_JUMP_WIDTH					(210/2)
#define TEXTURE_JUMP_HEIGHT					(210/2)
#define TEXTURE_HARDLAND_WIDTH				(200/2)
#define TEXTURE_HARDLAND_HEIGHT				(160/2)
#define TEXTURE_HIT_WIDTH					(160/2)
#define TEXTURE_HIT_HEIGHT					(160/2)
#define TEXTURE_KNOCKDOWN_WIDTH				(200/2)
#define TEXTURE_KNOCKDOWN_HEIGHT			(160/2)
#define TEXTURE_REBOUND_WIDTH				(180/2)
#define TEXTURE_REBOUND_HEIGHT				(150/2)
#define TEXTURE_DEFEND_WIDTH				(160/2)
#define TEXTURE_DEFEND_HEIGHT				(160/2)
#define TEXTURE_MAX							(25)	// テクスチャの数

// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_IDLE_PATTERN_DIVIDE_X			(4)		
#define TEXTURE_WALK_PATTERN_DIVIDE_X			(4)
#define TEXTURE_RUN_PATTERN_DIVIDE_X			(8)
#define TEXTURE_DASH_PATTERN_DIVIDE_X			(1)
#define TEXTURE_JUMP_PATTERN_DIVIDE_X			(10)
#define TEXTURE_HARD_LANDING_PATTERN_DIVIDE_X	(4)
#define TEXTURE_HIT_PATTERN_DIVIDE_X			(2)
#define TEXTURE_KNOCKDOWN_PATTERN_DIVIDE_X		(3)
#define TEXTURE_REBOUND_PATTERN_DIVIDE_X		(3)
#define TEXTURE_DEFEND_PATTERN_DIVIDE_X			(2)
#define TEXTURE_NORMAL_ATTACK1_PATTERN_DIVIDE_X	(7)
#define TEXTURE_NORMAL_ATTACK2_PATTERN_DIVIDE_X	(9)
#define TEXTURE_NORMAL_ATTACK3_PATTERN_DIVIDE_X	(8)
#define TEXTURE_NORMAL_ATTACK4_PATTERN_DIVIDE_X	(16)
#define TEXTURE_FLAME_ATTACK1_PATTERN_DIVIDE_X	(11)
#define TEXTURE_FLAME_ATTACK2_PATTERN_DIVIDE_X	(11)
#define TEXTURE_DASH_ATTACK_PATTERN_DIVIDE_X	(8)
#define TEXTURE_PARRY_PATTERN_DIVIDE_X			(8)
#define TEXTURE_PATTERN_DIVIDE_Y				(1)

// アニメーションの切り替わるWait値
#define ANIM_WAIT_WALK							(10)
#define ANIM_WAIT_IDLE							(30)
#define ANIM_WAIT_RUN							(5)
#define ANIM_WAIT_DASH							(1)
#define ANIM_WAIT_ATTACK						(4)
#define ANIM_WAIT_DASH_ATTACK					(8)
#define ANIM_WAI_FLAME_ATTACK2					(7)
#define ANIM_WAIT_PARRY							(10)
#define ANIM_WAIT_JUMP							(7)
#define ANIM_WAIT_HARDLAND						(6)
#define ANIM_WAIT_HIT							(6)
#define ANIM_WAIT_KNOCKDOWN						(18)
#define ANIM_WAIT_REBOUND						(6)
#define ANIM_WAIT_DEFEND						(25)
#define ANIM_DASH_FRAME							(11)
#define ANIM_HARDLANDING_FRAME					(4)
#define ANIM_HIT_FRAME							(4)
#define ANIM_KNOCKDOWN_FRAME					(4)
#define ANIM_REBOUND_FRAME						(5)
#define ANIM_NORMAL_ATTACK1_FRAME				(7)
#define ANIM_NORMAL_ATTACK2_FRAME				(9)
#define ANIM_NORMAL_ATTACK3_FRAME				(8)
#define ANIM_NORMAL_ATTACK4_FRAME				(16)
#define ANIM_PARRY_FRAME						(8)
#define	ANIM_DASH_ATTACK_FRAME					(8)
#define ANIM_FLAME_ATTACK1_FRAME				(8)
#define	ANIM_FLAME_ATTACK2_FRAME				(8)
#define ANIM_FADE_OUT_FRAME						(55)


#define	NORMAL_ATTACK4_DROP_FRAME				(9)
#define FLAME_ATTACK2_DROP_FRAME				(4)

#define TEXTURE_NORMAL_ATTACK1_OFFSET			XMFLOAT3(5.0f, -15.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK2_OFFSET			XMFLOAT3(5.0f, -4.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK3_OFFSET			XMFLOAT3(8.0f, -15.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK4_OFFSET			XMFLOAT3(10.0f, -20.0f, 0.0f)
#define TEXTURE_DASH_ATTACK_OFFSET				XMFLOAT3(10.0f, -5.0f, 0.0f)
#define TEXTURE_PARRY_OFFSET					XMFLOAT3(-5.0f, -15.0f, 0.0f)
#define TEXTURE_FLAME_ATTACK1_OFFSET			XMFLOAT3(5.0f, -15.0f, 0.0f)
#define TEXTURE_FLAME_ATTACK2_OFFSET			XMFLOAT3(-5.0f, -15.0f, 0.0f)

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_IDLE_HEIGHT)

// ジャンプ処理
#define	PLAYER_JUMP_CNT_MAX			(60)		// 60フレームで着地する
#define	PLAYER_JUMP_Y_MAX			(7.5f)	// ジャンプの高さ
#define JUMP_EFFECT_TIME			(55)
#define JUMP_EFFECT_TEXTURE			(19)
#define TEXTURE_JUMP_EFFECT_WIDTH	(195.0f)
#define TEXTURE_JUMP_EFFECT_HEIGHT	(70.0f)

// cast effect
#define CAST_HEALING_TEXTURE					(20)
#define TEXTURE_CAST_HEALING_WIDTH				(195.0f)
#define TEXTURE_CAST_HEALING_HEIGHT				(70.0f)
#define ANIM_WAIT_CAST_HEALING					(5)
#define ANIM_HEALING_CAST_FRAME					(12)
#define TEXTURE_CAST_HEALING_PATTERN_DIVIDE_X	(12)
#define TEXTURE_CAST_HEALING_PATTERN_DIVIDE_Y	(1)
#define CAST_FIRE_BALL_TEXTURE					(21)
#define TEXTURE_CAST_FIRE_BALL_WIDTH			(195.0f)
#define TEXTURE_CAST_FIRE_BALL_HEIGHT			(70.0f)
#define ANIM_WAIT_CAST_FIRE_BALL				(5)
#define ANIM_FIRE_BALL_CAST_FRAME				(11)
#define TEXTURE_CAST_FIRE_BALL_PATTERN_DIVIDE_X	(11)
#define TEXTURE_CAST_FIRE_BALL_PATTERN_DIVIDE_Y (1)
#define TEXTURE_CAST_OFFSET_X					(2.1f)
#define TEXTURE_CAST_OFFSET_Y					(-12.5f)

#define ST_COST_RUN					(6.5f)
#define	ST_COST_DEFEND				(5.5f)
#define	ST_COST_DASH				(125.0f)
#define	ST_COST_NORMAL_ATTACK1		(150.0f)
#define	ST_COST_NORMAL_ATTACK2		(135.0f)
#define	ST_COST_NORMAL_ATTACK3		(160.0f)
#define	ST_COST_NORMAL_ATTACK4		(180.0f)
#define ST_COST_DASH_ATTACK			(150.0f)
#define ST_COST_FLAME_ATTACK1		(165.0f)
#define ST_COST_FLAME_ATTACK2		(185.0f)
#define ST_COST_JUMP				(100.0f)
#define ST_RUN_THRESHOLD			(150.0f)
#define	ST_DEFEND_THRESHOLD			(100.0f)
#define ST_RECOVERY_RATE			(4.5f)
#define MP_RECOVERY_RATE			(0.5f)

#define MP_COST_FIRE_BALL			(400.0f)
#define	MP_COST_HEALING				(700.0f)
#define MP_COST_FLAMEBLADE			(2.5f)

// die
#define DIE_FADE_OUT_TEXTURE		(22)
#define DIE_LOGO_TEXTURE			(23)
#define TEXTURE_DIE_LOGO_WIDTH		(420.0f)
#define TEXTURE_DIE_LOGO_HEIGHT		(210.0f)
#define TEXTURE_DIE_LOGO_POS_X		(485.0f)
#define TEXTURE_DIE_LOGO_POS_Y		(155.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawPlayerOffset(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

#ifdef _DEBUG	
static ID3D11Buffer* g_AABBVertexBuffer = NULL;
static ID3D11Buffer* g_attackAABBVertexBuffer = NULL;
#endif

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/char/char_idle_right.png",
	"data/TEXTURE/char/char_walkforward.png",
	"data/TEXTURE/char/char_runforward.png",
	"data/TEXTURE/char/char_dashforward.png",
	"data/TEXTURE/char/char_attack1.png",
	"data/TEXTURE/char/char_attack2.png",
	"data/TEXTURE/char/char_attack3.png",
	"data/TEXTURE/char/char_attack4.png",
	"data/TEXTURE/char/char_dashattack.png",
	"data/TEXTURE/char/char_parry.png",
	"data/TEXTURE/char/char_flamestrike.png",
	"data/TEXTURE/char/char_flamestrike2.png",
	"data/TEXTURE/char/char_jump.png",
	"data/TEXTURE/char/char_hardlanding.png",
	"data/TEXTURE/char/char_hit.png",
	"data/TEXTURE/char/char_knockdown.png",
	"data/TEXTURE/char/char_rebound.png",
	"data/TEXTURE/char/char_defend.png",
	"data/TEXTURE/char/shadow000.jpg",
	"data/TEXTURE/char/jump_effect.png",
	"data/TEXTURE/char/heal_effect.png",
	"data/TEXTURE/char/fire_ball_effect.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/die_logo.png",
};


static BOOL		g_Load = FALSE;				// 初期化を行ったかのフラグ
static PLAYER	g_Player[PLAYER_MAX];		// プレイヤー構造体
static int		g_PlayerCount = PLAYER_MAX;	// 生きてるプレイヤーの数
static XMFLOAT3 g_InitPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

static int      g_jumpCnt = 0;
static int		g_jump[PLAYER_JUMP_CNT_MAX] =
{
	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5,-4,-3,-2,-1,
	  1,   2,   3,   4,   5,   6,  7,  8,  9, 10, 11,12,13,14,15
};

static BOOL		g_Update = TRUE;

static BOOL		noGravity = FALSE;
static BOOL		allowAirJump = FALSE;
static BOOL		g_LimitPlayerMoveLeft = FALSE;
static BOOL		g_LimitPlayerMoveRight = FALSE;
static BOOL		g_DisableMoveLeft = FALSE;
static BOOL		g_DisableMoveRight = FALSE;
static BOOL		g_DisableRun = FALSE;
static BOOL		g_DisableDash = FALSE;
static BOOL		g_DisableJump = FALSE;
static BOOL		g_DisableMagicSwitch = FALSE;
static BOOL		g_DisableHealing = FALSE;
static BOOL		g_DisableFireBall = FALSE;
static BOOL		g_DisableFlameblade = FALSE;
static BOOL		g_DisableAttack = FALSE;
static BOOL		g_DisableDefend = FALSE;
static BOOL		g_DisableDefendCount = FALSE;

static BOOL		g_LimitBGMove = TRUE;

static PlayerData g_PlayerData;

static AttackAABBTBL normalAttack1Tbl[MAX_ATTACK_AABB * ANIM_NORMAL_ATTACK1_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-45.0f, 0.0f, 0.0f), 60.0f, 100.0f},
	{XMFLOAT3(-20.0f, -60.0f, 0.0f), 100.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-45.0f, 0.0f, 0.0f), 60.0f, 100.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-45.0f, 0.0f, 0.0f), 60.0f, 100.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL normalAttack2Tbl[MAX_ATTACK_AABB * ANIM_NORMAL_ATTACK2_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 77.0f, 100.0f},
	{XMFLOAT3(-20.0f, -35.0f, 0.0f), 50.0f, 45.0f},
	{XMFLOAT3(0.0f, 50.0f, 0.0f), 90.0f, 35.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 77.0f, 100.0f},
	{XMFLOAT3(-20.0f, -35.0f, 0.0f), 50.0f, 45.0f},
	{XMFLOAT3(0.0f, 50.0f, 0.0f), 90.0f, 35.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 77.0f, 100.0f},
	{XMFLOAT3(-20.0f, -35.0f, 0.0f), 50.0f, 45.0f},
	{XMFLOAT3(0.0f, 50.0f, 0.0f), 90.0f, 35.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL normalAttack3Tbl[MAX_ATTACK_AABB * ANIM_NORMAL_ATTACK3_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 70.0f, 100.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 70.0f, 100.0f},
	{XMFLOAT3(0.0f, 50.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 100.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 50.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 70.0f, 100.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL normalAttack4Tbl[MAX_ATTACK_AABB * ANIM_NORMAL_ATTACK4_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 40.0f, 90.0f},
	{XMFLOAT3(0.0f, -55.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 90.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 40.0f, 90.0f},
	{XMFLOAT3(0.0f, 30.0f, 0.0f), 80.0f, 30.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 90.0f},

	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 90.0f},
	{XMFLOAT3(0.0f, -50.0f, 0.0f), 80.0f, 45.0f},
	{XMFLOAT3(-15.0f, 30.0f, 0.0f), 80.0f, 30.0f},

	{XMFLOAT3(-35.0f, -5.0f, 0.0f), 60.0f, 90.0f},
	{XMFLOAT3(0.0f, -50.0f, 0.0f), 80.0f, 45.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 90.0f},

	{XMFLOAT3(-35.0f, -5.0f, 0.0f), 60.0f, 90.0f},
	{XMFLOAT3(-15.0f, -50.0f, 0.0f), 80.0f, 30.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 90.0f, 90.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 90.0f, 90.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL dashAttackTbl[MAX_ATTACK_AABB * ANIM_DASH_ATTACK_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 40.0f, 90.0f},
	{XMFLOAT3(0.0f, -55.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 90.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 40.0f, 90.0f},
	{XMFLOAT3(0.0f, 30.0f, 0.0f), 80.0f, 30.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 40.0f, 90.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL parryTbl[MAX_ATTACK_AABB * ANIM_PARRY_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 50.0f, 90.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 60.0f, 90.0f},
	{XMFLOAT3(0.0f, -30.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, -30.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL flameAttack1Tbl[MAX_ATTACK_AABB * ANIM_PARRY_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 70.0f, 90.0f},
	{XMFLOAT3(0.0f, -30.0f, 0.0f), 80.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 70.0f, 90.0f},
	{XMFLOAT3(0.0f, -30.0f, 0.0f), 80.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL flameAttack2Tbl[MAX_ATTACK_AABB * ANIM_PARRY_FRAME] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 50.0f, 90.0f},
	{XMFLOAT3(0.0f, -30.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 60.0f, 90.0f},
	{XMFLOAT3(0.0f, 30.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 0.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(35.0f, 0.0f, 0.0f), 80.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_Update = TRUE;
	g_LimitBGMove = TRUE;

	g_PlayerCount = 0;						// 生きてるプレイヤーの数
	g_LimitPlayerMoveLeft = FALSE;
	g_LimitPlayerMoveRight = FALSE;
	g_DisableRun = FALSE;
	g_DisableDash = FALSE;
	g_DisableJump = FALSE;
	g_DisableMagicSwitch = FALSE;
	g_DisableHealing = FALSE;
	g_DisableFireBall = FALSE;
	g_DisableFlameblade = FALSE;
	g_DisableAttack = FALSE;
	g_DisableDefend = FALSE;
	g_DisableDefendCount = FALSE;

	// プレイヤー構造体の初期化
	InitPlayerStatus();

#ifdef _DEBUG	
	{
		int aabbCount = PLAYER_MAX;
		int maxVertices = PLAYER_MAX * 4;
		int maxAttackVertices = maxVertices * MAX_ATTACK_AABB;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * maxVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_AABBVertexBuffer);

		bd.ByteWidth = sizeof(VERTEX_3D) * maxAttackVertices;
		GetDevice()->CreateBuffer(&bd, NULL, &g_attackAABBVertexBuffer);
	}
#endif


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_Load == FALSE) return;
	g_PlayerCount = 0;				// 生きてるプレイヤーの数

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].update == FALSE) continue;

		// 生きてるプレイヤーだけ処理をする
		if (g_Player[i].use == TRUE)
		{
			g_PlayerCount++;		// 生きてるプレイヤーの数

			if (g_Player[i].state == IDLE) g_Player[i].shadowY = g_Player[i].pos.y;

			// 地形との当たり判定用に座標のバックアップを取っておく
			XMFLOAT3 pos_old = g_Player[i].pos;

			// 分身用
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = pos_old;

			UpdateActionQueue();
			
			// 現在のアニメーションが終了したかどうかを確認
			if (g_Player[i].playAnim == FALSE)
			{
				if (g_Player[i].actionQueueStart != g_Player[i].actionQueueEnd)
				{
					HandleActionQueue();
				}
				else
				{
					// アクションキューが空の場合、プレイヤーをアイドル状態に設定
					g_Player[i].state = IDLE;
				}
			}			

			UpdateGroundCollision();
			UpdateKeyboardInput();
			UpdateGamepadInput();
			UpdatePlayerStates();
			UpdateBackGroundScroll();

			// 移動が終わったらエネミーとの当たり判定
			ENEMY* enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				// 生きてるエネミーと当たり判定をする
				if (enemy[j].use == TRUE 
					&& enemy[j].state == ENEMY_ATTACK 
					&& enemy[i].hitPlayer == FALSE
					&& g_Player->isInvincible == FALSE)
				{
					// 攻撃用の包囲ボックスを取得
					for (int k = 0; k < MAX_ATTACK_AABB; k++)
					{
						AABB attackBox = enemy[j].attackAABB[k];

						// プレイヤーのAABB情報を取得
						XMFLOAT3 playerPos = g_Player->bodyAABB.pos;
						float playerW = g_Player->bodyAABB.w;
						float playerH = g_Player->bodyAABB.h;

						// 攻撃のAABB情報を取得
						XMFLOAT3 attackPos = attackBox.pos;
						float attackW = attackBox.w;
						float attackH = attackBox.h;

						// プレイヤーの包囲ボックスとエネミーの攻撃範囲が重なっているかを確認
						BOOL isColliding = CollisionBB(playerPos, playerW, playerH, attackPos, attackW, attackH);

						if (isColliding)
						{
							// 当たり判定があった場合、プレイヤーにダメージを与える
							PlayerTakeDamage(&enemy[j]);
							break;
						}
					}
				}
			}

			// アニメーション  
			switch (g_Player[i].state)
			{
			case IDLE:
				PlayIdleAnim();
				break;
			case WALK:
				PlayWalkAnim();
				break;
			case RUN:
				PlayRunningAnim();
				break;
			case DASH:
				PlayDashAnim();
				break;
			case ATTACK:
				PlayAttackAnim();
				break;
			case JUMP:
				PlayJumpAnim();
				break;
			case FALL:
				PlayFallAnim();
				break;
			case HARD_LANDING:
				PlayHardLandingAnim();
				break;
			case HIT:
				PlayHitAnim();
				break;
			case KNOCKDOWN:
				PlayKnockDownAnim();
				break;
			case REBOUND:
				PlayReboundAnim();
				break;
			case DEFEND:
				PlayDefendAnim();
				break;
			case CAST:
				PlayCastAnim();
				break;
			case DIE:
				PlayDieAnim();
			default:
				break;
			}
		}
	}

}

void HandleActionQueue(void)
{
	// キュー内の次のアクションを実行
	int action = g_Player->actionQueue[g_Player->actionQueueStart];
	if (g_Player->ST < GetActionStaminaCost(action))
		return;
	BOOL canExecuteAction = FALSE;

	switch (action)
	{
	case JUMP:
		// プレイヤーが空中にいる場合
		if (g_Player->onAirCnt > 0)
		{
			// 空中ジャンプの回数制限に達しているか確認
			if (g_Player->jumpOnAirCnt == 1)
				canExecuteAction = FALSE;	// 既に空中ジャンプを行っている場合は実行不可
			else
			{
				// 空中ジャンプが可能であれば実行
				canExecuteAction = TRUE;
				g_Player->jumpEffectCnt = JUMP_EFFECT_TIME;
				g_Player->airJumpPos = g_Player->pos;
				g_Player->jumpOnAirCnt = 1;	 // 空中ジャンプ回数を更新
			}
		}
		else
		{
			canExecuteAction = TRUE;	// 地上にいる場合はジャンプ可能
		}

		break;

	case DASH:
		// クールダウン時間が経過している、またはダッシュ回数が残っている場合に実行可能
		if ((g_Player->dashCD > DASH_CD_TIME || g_Player->dashCount < g_Player->maxDashCount) 
			&& g_Player->dashInterval <= 0)
		{
			g_Player->dashInterval = DASH_INTERVAL;
			g_Player->dashCD = 0;
			g_Player->dashCount++;
			canExecuteAction = TRUE;
		}
		else
			canExecuteAction = FALSE;
		break;
	case ATTACK:
		if (g_Player->attackInterval > ATTACK_COMBO_WINDOW)
			g_Player->attackPattern = g_Player->flameblade == TRUE ? FLAME_ATTACK1 : NORMAL_ATTACK1;
		g_Player->attackInterval = 0;
		canExecuteAction = TRUE;
		break;
	default:
		// その他のアクションは特別な条件なく実行可能
		canExecuteAction = TRUE;
		break;
	}

	if (canExecuteAction)
	{
		// アクションキューのスタート位置を更新（次のアクションに進む）
		g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE;
		g_Player->patternAnim = 0;
		g_Player->state = action;
		g_Player->animFrameCount = 0;
		g_Player->playAnim = TRUE;
		g_Player->ST -= GetActionStaminaCost(action);
	}
}

void HandlePlayerMove(float speed, int direction)
{
	g_Player->dir = direction; // プレイヤーの向きを設定

	// プレイヤーが地面にいる場合の処理
	if (g_Player->move.y == 0)
	{
		g_Player->state = WALK;
		if (g_Player->isDefending)
			g_Player->isWalkingOnDefend = TRUE;
		// 衝突判定を行い、移動可能であればプレイヤーのX座標を更新
		if (CheckMoveCollision(speed, g_Player->dir))
			CHANGE_PLAYER_POS_X(speed);

		// 走る
		if (GetKeyboardTrigger(DIK_LSHIFT) 
			&& g_DisableRun == FALSE
			&& g_Player->ST - ST_COST_RUN >= 0 
			&& g_Player->isDefending == FALSE)
		{
			HandlePlayerRun(speed);
			g_Player->wasRunningExhausted = FALSE;
		}
		else if (GetKeyboardPress(DIK_LSHIFT) 
			&& g_DisableRun == FALSE
			&& g_Player->ST > ST_RUN_THRESHOLD 
			&& g_Player->wasRunningExhausted 
			&& g_Player->isDefending == FALSE)
		{
			HandlePlayerRun(speed);
			g_Player->wasRunningExhausted = FALSE;
		}
		else if (GetKeyboardPress(DIK_LSHIFT) 
			&& g_DisableRun == FALSE
			&& g_Player->wasRunningExhausted == FALSE 
			&& g_Player->isDefending == FALSE)
		{
			HandlePlayerRun(speed);
			if (g_Player->ST <= 0.0f)
				g_Player->wasRunningExhausted = TRUE;
		}
	}
	// プレイヤーが空中にいる場合、移動判定を行い、空中移動を処理
	else if (CheckMoveCollision(speed * 0.8f, g_Player->dir))
		CHANGE_PLAYER_POS_X(speed * 0.8f);

	std::cout << g_Player->pos.x << " ";
	std::cout << g_Player->pos.y << std::endl;

}

void HandlePlayerRun(float speed)
{
	if (GetTutorialPause() == PAUSE_RUN)
		SetTutorialPause(PAUSE_NONE);

	// プレイヤーを「走行」状態にし、状態を「走る」に変更
	g_Player->isRunning = TRUE;
	g_Player->state = RUN;
	g_Player->ST -= ST_COST_RUN;
	// 衝突判定を行い、移動可能であればプレイヤーのX座標を更新（ダッシュ速度）
	if (CheckMoveCollision(speed * 0.5f, g_Player->dir))
		CHANGE_PLAYER_POS_X(speed * 0.5f);
}

void HandlePlayerDash(void)
{
	if (GetTutorialPause() == PAUSE_DASH)
		SetTutorialPause(PAUSE_NONE);

	if (g_Player->ST < ST_COST_DASH)
		return;
	// もしアイドル状態なら、すぐにアクションを実行
	if (g_Player->playAnim == FALSE && g_Player->dashInterval <= 0)
	{
		if (g_Player->onAirCnt == 0 || (g_Player->onAirCnt > 0 && g_Player->dashOnAir == FALSE))
		{
			g_Player->patternAnim = 0;
			g_Player->state = DASH;
			g_Player->animFrameCount = 0;
			g_Player->dashCD = 0;
			g_Player->playAnim = TRUE;
			g_Player->dashCount++;
			g_Player->dashInterval = DASH_INTERVAL;
			g_Player->ST -= ST_COST_DASH;

			if (g_Player->onAirCnt > 0 && g_Player->airDashCount < g_Player->maxDashCount)
			{
				g_Player->dashOnAir = TRUE;
				g_Player->airDashCount++;
			}

			// 無敵状態を開始する
			g_Player->isInvincible = TRUE;
		}
	}
	// 空中の状態
	else if (g_Player->onAirCnt > 0 && g_Player->airDashCount < g_Player->maxDashCount && g_Player->dashInterval <= 0)
	{
		g_Player->patternAnimOld = g_Player->patternAnim;
		g_Player->patternAnim = 0;
		g_Player->state = DASH;
		g_Player->dashOnAir = TRUE;
		g_Player->dashCD = 0;
		g_Player->animFrameCount = 0;
		g_Player->dashCount++;
		g_Player->airDashCount++;
		g_Player->dashInterval = DASH_INTERVAL;
		g_Player->ST -= ST_COST_DASH;

		// 無敵状態を開始する
		g_Player->isInvincible = TRUE;
	}
	// その他の状態の場合、アクションをキューに追加
	else if (!(g_Player->dashOnAir == TRUE && g_Player->onAirCnt > 0))
	{
		g_Player->actionQueue[g_Player->actionQueueEnd] = DASH;
		g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

		// キューのオーバーフローを防止
		if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
		{
			g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // 最も古いアクションを破棄
		}
	}

}

void HandlePlayerJump(void)
{
	if (g_Player->ST < ST_COST_JUMP)
		return;

	// もしアイドル状態なら、すぐにアクションを実行
	if (g_Player->playAnim == FALSE)
	{
		if (g_Player->onAirCnt == 0 || (g_Player->jumpOnAir && g_Player->jumpOnAirCnt == 0))
		{
			g_Player->patternAnim = 0;
			g_Player->state = JUMP;
			g_Player->animFrameCount = 0;
			g_Player->playAnim = TRUE;
			g_Player->ST -= ST_COST_JUMP;
			g_Player->state = JUMP;
			g_Player->jumpCnt = 0;
			if (g_Player->onAirCnt > 0)
			{
				g_Player->jumpEffectCnt = JUMP_EFFECT_TIME;
				g_Player->airJumpPos = g_Player->pos;
				g_Player->jumpOnAirCnt = 1;
			}
		}
	}
	// 空中の状態
	else if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX / 2 && g_Player->jumpOnAir && g_Player->jumpOnAirCnt == 0)
	{

		g_Player->patternAnim = 0;
		g_Player->state = JUMP;
		g_Player->animFrameCount = 0;
		g_Player->playAnim = TRUE;
		g_Player->jumpCnt = 0;
		g_Player->jumpEffectCnt = JUMP_EFFECT_TIME;
		g_Player->airJumpPos = g_Player->pos;
		g_Player->jumpOnAirCnt = 1;
		g_Player->ST -= ST_COST_JUMP;

	}
	// その他の状態の場合、アクションをキューに追加
	else
	{
		g_Player->actionQueue[g_Player->actionQueueEnd] = JUMP;
		g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

		// キューのオーバーフローを防止
		if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
		{
			g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // 最も古いアクションを破棄
		}
	}
}

void HandlePlayerAttack(void)
{
	if (GetActionStaminaCost(ATTACK) > g_Player->ST)
		return;

	switch (g_Player->state)
	{
		// もしアイドル状態なら、すぐにアクションを実行
	case IDLE:
	case WALK:
		if (g_Player->isParrying)
		{
			g_Player->patternAnim = 0;
			g_Player->state = ATTACK;
			g_Player->animFrameCount = 0;
			g_Player->attackPattern = PARRY;
			g_Player->playAnim = TRUE;
			g_Player->isParrying = FALSE;
		}
		else
		{
			if (g_Player->attackInterval > ATTACK_COMBO_WINDOW)
				g_Player->attackPattern = g_Player->flameblade == TRUE ? FLAME_ATTACK1 : NORMAL_ATTACK1;
			g_Player->patternAnim = 0;
			g_Player->state = ATTACK;
			g_Player->animFrameCount = 0;
			g_Player->playAnim = TRUE;
			g_Player->attackInterval = 0;
			g_Player->ST -= GetActionStaminaCost(ATTACK);
		}
		break;

		// 走るの状態
	case RUN:
		g_Player->patternAnim = 0;
		g_Player->animFrameCount = 0;
		g_Player->playAnim = TRUE;
		g_Player->state = ATTACK;
		g_Player->attackPattern = DASH_ATTACK;
		g_Player->ST -= GetActionStaminaCost(ATTACK);
		break;

	case DASH:
	case ATTACK:
	case JUMP:
	case FALL:
		// アニメーション再生していないなら、すぐにアクションを実行
		if (g_Player->playAnim == FALSE)
		{
			if (g_Player->attackInterval > ATTACK_COMBO_WINDOW)
				g_Player->attackPattern = g_Player->flameblade == TRUE ? FLAME_ATTACK1 : NORMAL_ATTACK1;
			g_Player->patternAnim = 0;
			g_Player->animFrameCount = 0;
			g_Player->playAnim = TRUE;
			g_Player->attackInterval = 0;
			g_Player->ST -= GetActionStaminaCost(ATTACK);
			break;
		}
		// 空中の状態
		else if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX / 2 && g_Player->state != ATTACK)
		{
			if (g_Player->attackInterval > ATTACK_COMBO_WINDOW)
				g_Player->attackPattern = g_Player->flameblade == TRUE ? FLAME_ATTACK1 : NORMAL_ATTACK1;
			g_Player->patternAnimOld = g_Player->patternAnim;
			g_Player->patternAnim = 0;
			g_Player->state = ATTACK;
			g_Player->animFrameCount = 0;
			g_Player->attackInterval = 0;
			g_Player->ST -= GetActionStaminaCost(ATTACK);
		}
		// その他の状態の場合、アクションをキューに追加
		else
		{
			g_Player->actionQueue[g_Player->actionQueueEnd] = ATTACK;
			g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

			// キューのオーバーフローを防止
			if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
			{
				g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // 最も古いアクションを破棄
			}
		}
		break;
	default:
		break;
	}

}

void HandlePlayerDefend(void)
{
	if (GetTutorialPause() == PAUSE_DEFEND)
	{
		if (GetKeyboardPress(DIK_LCONTROL) || GetKeyboardTrigger(DIK_LCONTROL))
			SetUpdateEnemy(TRUE);
		if (GetKeyboardRelease(DIK_LCONTROL))
			SetUpdateEnemy(FALSE);
	}

	if (g_Player->state != WALK)
		g_Player->isWalkingOnDefend = FALSE;

	if (GetKeyboardTrigger(DIK_LCONTROL) && g_Player->ST - ST_COST_DEFEND >= 0 && g_Player->isRunning == FALSE)
	{
		// 現在の方向を防御方向として保存
		if (g_Player->isDefending == FALSE)
			g_Player->defendDir = g_Player->dir;

		g_Player->state = DEFEND;
		g_Player->isDefending = TRUE;
		g_Player->ST -= ST_COST_DEFEND;
		// 防御中の移動速度を半減
		g_Player->move.x = PLAYER_WALK_SPEED * 0.5f;
		g_Player->wasDefendingExhausted = FALSE;
	}
	else if (GetKeyboardPress(DIK_LCONTROL) && g_Player->ST > ST_DEFEND_THRESHOLD && g_Player->wasDefendingExhausted && g_Player->isRunning == FALSE)
	{
		// 現在の方向を防御方向として保存
		if (g_Player->isDefending == FALSE)
			g_Player->defendDir = g_Player->dir;

		g_Player->state = DEFEND;
		g_Player->isDefending = TRUE;
		g_Player->ST -= ST_COST_DEFEND;
		// 防御中の移動速度を半減
		g_Player->move.x = PLAYER_WALK_SPEED * 0.5f;
		g_Player->wasDefendingExhausted = FALSE;
	}
	else if (GetKeyboardPress(DIK_LCONTROL) && g_Player->wasDefendingExhausted == FALSE && g_Player->isRunning == FALSE)
	{
		// 現在の方向を防御方向として保存
		if (g_Player->isDefending == FALSE)
			g_Player->defendDir = g_Player->dir;

		g_Player->state = DEFEND;
		g_Player->isDefending = TRUE;
		g_Player->ST -= ST_COST_DEFEND;
		// 防御中の移動速度を半減
		g_Player->move.x = PLAYER_WALK_SPEED * 0.5f;
		if (g_Player->ST <= 0.0f)
		{
			g_Player->wasDefendingExhausted = TRUE;
			g_Player->move.x = PLAYER_WALK_SPEED;	// 移動速度を通常に戻す
		}
	}
}

float GetActionStaminaCost(int action)
{
	switch (action)
	{
	case ATTACK:
	{
		int attackPattern = g_Player->attackPattern;
		if (g_Player->state == RUN)
			attackPattern = DASH_ATTACK;
		else if (g_Player->attackInterval > ATTACK_COMBO_WINDOW)
		{
			if (attackPattern < NORMAL_ATTACK_PATTERN_MAX)
				attackPattern = NORMAL_ATTACK1;
			else if (attackPattern < FLAME_ATTACK_PATTERN_MAX)
				attackPattern = FLAME_ATTACK1;
		}
		switch (attackPattern)
		{
		case NORMAL_ATTACK1:
			return ST_COST_NORMAL_ATTACK1;
		case NORMAL_ATTACK2:
			return ST_COST_NORMAL_ATTACK2;
		case NORMAL_ATTACK3:
			return ST_COST_NORMAL_ATTACK3;
		case NORMAL_ATTACK4:
			return ST_COST_NORMAL_ATTACK4;
		case DASH_ATTACK:
			return ST_COST_DASH_ATTACK;
		case FLAME_ATTACK1:
			return ST_COST_FLAME_ATTACK1;
		case FLAME_ATTACK2:
			return ST_COST_FLAME_ATTACK2;
		default:
			return 0.0f;
		}
	}
	case JUMP:
		return ST_COST_JUMP;
	case DASH:
		return ST_COST_DASH;
	default:
		return 0.0f;
	}
}

void UpdateKeyboardInput(void)
{
	float speed = g_Player->move.x;
	noGravity = FALSE;

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_LALT))
		noGravity = TRUE;

	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_LALT))
	{
		CHANGE_PLAYER_POS_Y(-speed * 5);
		g_Player->dir = CHAR_DIR_RIGHT;
		g_Player->state = WALK;
	}
	else if (GetKeyboardPress(DIK_DOWN) && GetKeyboardPress(DIK_LALT))
	{
		CHANGE_PLAYER_POS_Y(speed * 5);
		g_Player->dir = CHAR_DIR_LEFT;
		g_Player->state = WALK;
	}

	if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_LALT))
	{
		CHANGE_PLAYER_POS_X(-speed * 5);
		g_Player->dir = CHAR_DIR_RIGHT;
		g_Player->state = WALK;
	}
	else if (GetKeyboardPress(DIK_RIGHT) && GetKeyboardPress(DIK_LALT))
	{
		CHANGE_PLAYER_POS_X(speed * 5);
		g_Player->dir = CHAR_DIR_LEFT;
		g_Player->state = WALK;
	}
#endif

	if (GetKeyboardPress(DIK_RIGHT) 
		&& g_DisableMoveRight == FALSE
		&& g_Player->playAnim == FALSE)
	{
		// 右移動処理
		HandlePlayerMove(speed, CHAR_DIR_RIGHT);
	}
	else if (GetKeyboardPress(DIK_LEFT) 
		&& g_DisableMoveLeft == FALSE
		&& g_Player->playAnim == FALSE)
	{
		// 左移動処理
		HandlePlayerMove(-speed, CHAR_DIR_LEFT);
	}

	if (GetKeyboardPress(DIK_LCONTROL) 
		&& g_DisableDefend == FALSE
		&& g_Player->playAnim == FALSE 
		&& g_Player->onAirCnt == 0)
	{
		// 防御処理
		HandlePlayerDefend();
	}
	else if (GetKeyboardTrigger(DIK_C) 
		&& g_DisableDash == FALSE
		&& g_Player->dashCount < g_Player->maxDashCount)
	{
		// ダッシュ処理
		HandlePlayerDash();
	}
	else if (GetKeyboardTrigger(DIK_SPACE) 
		&& g_DisableJump == FALSE
		&& g_Player->jumpOnAirCnt == 0)
	{
		// ジャンプ処理
		HandlePlayerJump();
	}
	else if (GetKeyboardTrigger(DIK_X) && g_DisableAttack == FALSE)
	{
		// 攻撃処理
		HandlePlayerAttack();
	}

	int magicCount = GetMagicEquippedCount();
	if (GetKeyboardRelease(DIK_Q) && g_DisableMagicSwitch == FALSE && magicCount != 0)
	{
		g_Player->currentMagicIdx = (g_Player->currentMagicIdx + magicCount - 1) % magicCount;
		while (g_Player->magicList[g_Player->currentMagicIdx] == MAGIC_NONE)
			g_Player->currentMagicIdx = (g_Player->currentMagicIdx + magicCount - 1) % magicCount;
	}
	else if (GetKeyboardRelease(DIK_E) && g_DisableMagicSwitch == FALSE && magicCount != 0)
	{
		g_Player->currentMagicIdx = (g_Player->currentMagicIdx + 1) % magicCount;
		while (g_Player->magicList[g_Player->currentMagicIdx] == MAGIC_NONE)
			g_Player->currentMagicIdx = (g_Player->currentMagicIdx + 1) % magicCount;
	}
		

	if (GetKeyboardRelease(DIK_F))
	{
		switch (g_Player->magicList[g_Player->currentMagicIdx])
		{
		case MAGIC_HEALING:
			if (g_Player->healingCD <= 0 
				&& g_DisableHealing == FALSE
				&& g_Player->state == IDLE
				&& g_Player->MP >= MP_COST_HEALING)
			{
				g_Player->state = CAST;
				g_Player->patternAnim = 0;
				g_Player->patternAnimCast = 0;
				g_Player->animFrameCount = 0;
				g_Player->animFrameCountCast = 0;
				g_Player->countAnim = 0;
				g_Player->playAnim = TRUE;
				g_Player->magicCasting = MAGIC_HEALING;
			}

			break;
		case MAGIC_FLAMEBLADE:
			if (g_DisableFlameblade == FALSE)
			g_Player->flameblade = g_Player->flameblade == TRUE ? FALSE : TRUE;
			break;
		case MAGIC_FIRE_BALL:
			if (g_Player->fireBallCD <= 0 
				&& g_DisableFireBall == FALSE
				&& g_Player->state == IDLE
				&& g_Player->MP >= MP_COST_FIRE_BALL)
			{
				g_Player->state = CAST;
				g_Player->patternAnim = 0;
				g_Player->patternAnimCast = 0;
				g_Player->animFrameCount = 0;
				g_Player->animFrameCountCast = 0;
				g_Player->countAnim = 0;
				g_Player->playAnim = TRUE;
				g_Player->magicCasting = MAGIC_FIRE_BALL;
			}

			break;
		default:
			break;
		}
	}
}

void UpdateGamepadInput(void)
{
	//float speed = g_Player->move.x;

	//if (IsButtonPressed(0, BUTTON_RIGHT))
	//{
	//	g_Player->pos.x += speed;
	//	g_Player->dir = CHAR_IDLE_RIGHT;
	//}
	//else if (IsButtonPressed(0, BUTTON_LEFT))
	//{
	//	g_Player->pos.x -= speed;
	//	g_Player->dir = CHAR_IDLE_LEFT;
	//}
}

void UpdatePlayerStates(void)
{
	// スタミナが負の値にならないように0に設定
	if (g_Player->ST < 0.0f)
		g_Player->ST = 0.0f;
	// スタミナが最大値未満の場合、スタミナを回復
	else if(g_Player->ST < g_Player->maxST && g_Player->playAnim == FALSE)
		g_Player->ST += ST_RECOVERY_RATE;

	if (g_Player->MP < 0.0f)
		g_Player->MP = 0.0f;
	else if (g_Player->MP < g_Player->maxMP)
		g_Player->MP += MP_RECOVERY_RATE;

	// ダッシュクールダウンタイムを更新
	g_Player->dashCD++;
	g_Player->dashInterval--;

	// クールダウンタイムが指定された時間を超えた場合、ダッシュ回数をリセット
	if (g_Player->dashCD >= DASH_CD_TIME)
	{
		g_Player->dashCount = 0;
	}

	// プレイヤーがジャンプの半分以上落下し、かつ状態がIDLEの場合、落下状態に移行
	if (g_Player->move.y >= g_Player->jumpYMax * 0.5f && g_Player->state == IDLE)
	{
		g_Player->state = FALL;
	}

	// 攻撃インターバルを更新
	g_Player->attackInterval++;

	// プレイヤーが攻撃状態でない場合、攻撃の当たり判定AABBをリセット
	if (g_Player->state != ATTACK)
	{
		for (int i = 0; i < MAX_ATTACK_AABB; i++)
			g_Player->attackAABB[i] = {XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f};
	}

	// 防御状態の場合
	if (g_Player->state == DEFEND)
	{
		g_Player->isDefending = TRUE;	// 防御中フラグを有効にする
		g_Player->isRunning = FALSE;	// 走行中フラグを無効にする
		if (g_DisableDefendCount == FALSE)
			g_Player->defendCnt++;	// 防御カウントを増加
		if (g_Player->isParrying == TRUE && g_Player->defendCnt >= PARRY_WINDOW * 2.5)
		{
			g_Player->isParrying = FALSE;
			g_Player->defendCnt = 0; // 防御カウントをリセット
			g_Player->isInvincible = FALSE; // 無敵状態を終了する
		}
	}
	// 走行状態の場合
	else if (g_Player->state == RUN)
	{
		g_Player->isRunning = TRUE;	// 走行中フラグを有効にする
		g_Player->isDefending = FALSE;	// 防御中フラグを無効にする
		g_Player->isParrying = FALSE; // パリィ状態フラグを無効にする
		g_Player->isInvincible = FALSE; // 無敵状態を終了する

	}
	// パリィ状態の場合
	else if (g_Player->isParrying == TRUE)
	{
		if (g_DisableDefendCount == FALSE)
			g_Player->defendCnt++;
		// 時間が一程度過ごしたら、パリィ状態フラグを無効にする
		if (g_Player->defendCnt >= PARRY_WINDOW * 2.5)
		{
			g_Player->isParrying = FALSE;
			g_Player->defendCnt = 0; // 防御カウントをリセット
			g_Player->isInvincible = FALSE; // 無敵状態を終了する
		}
	}
	else
	{
		g_Player->isRunning = FALSE;
		g_Player->isDefending = FALSE;
	}

	// 防御姿勢を解除した際に、防御解除前の向きを維持する
	if (GetKeyboardRelease(DIK_LCONTROL) && g_Player->playAnim == FALSE && g_Player->onAirCnt == 0)
	{
		g_Player->dir = g_Player->defendDir;
		g_Player->move.x = PLAYER_WALK_SPEED;	// 移動速度を通常に戻す
		if (g_Player->isParrying == FALSE)
			g_Player->defendCnt = 0;
	}

	if (g_Player->fireBallCD > 0)
		g_Player->fireBallCD--;
	if (g_Player->healingCD > 0)
		g_Player->healingCD--;

	if (g_Player->flameblade == TRUE)
	{
		g_Player->MP -= MP_COST_FLAMEBLADE;
		if (g_Player->MP < MP_COST_FLAMEBLADE)
			g_Player->flameblade = FALSE;
	}

	if (g_Player->jumpEffectCnt > 0)
		g_Player->jumpEffectCnt--;

	if (g_Player->recoveryWindow > 0)
		g_Player->recoveryWindow--;
	if (g_Player->greyHP >= g_Player->HP && g_Player->recoveryWindow > 0)
	{
		g_Player->greyHP -= GREY_HP_DECAY_RATE;
		if (g_Player->greyHP < g_Player->HP)
			g_Player->greyHP = g_Player->HP;
	}
}

void UpdateBackGroundScroll(void)
{
	// MAP外チェック
	BG* bg = GetBG();

	if (g_Player->bodyAABB.pos.x < 0.0f)
	{
		SET_PLAYER_POS_X(0.0f);
	}

	if (g_Player->bodyAABB.pos.x > bg->w)
	{
		SET_PLAYER_POS_X(bg->w);
	}

	if (g_Player->bodyAABB.pos.y < 0.0f)
	{
		SET_PLAYER_POS_Y(0.0f);
	}

	if (g_Player->bodyAABB.pos.y > bg->h)
	{
		SET_PLAYER_POS_Y(bg->h);
	}

	// 左右の制限距離（画面の三分の一）
	float leftLimitX = SCREEN_WIDTH / 3;
	float rightLimitX = SCREEN_WIDTH * 2 / 3;

	if (g_LimitPlayerMoveLeft == TRUE && g_Player->pos.x < bg->pos.x + leftLimitX)
	{
		SET_PLAYER_POS_X(bg->pos.x + leftLimitX);
	}
	else if (g_LimitPlayerMoveRight == TRUE && g_Player->pos.x > bg->pos.x + rightLimitX)
	{
		SET_PLAYER_POS_X(bg->pos.x + rightLimitX);
	}

	if (g_LimitBGMove == TRUE)
	{
		// プレイヤーが画面内の移動範囲内かどうかを確認
		if (g_Player->pos.x < bg->pos.x + leftLimitX)
		{
			// プレイヤーが左の制限範囲を越えた場合は、背景を左にスクロール
			bg->pos.x = g_Player->pos.x - leftLimitX;
		}
		else if (g_Player->pos.x > bg->pos.x + rightLimitX)
		{
			// プレイヤーが右の制限範囲を越えた場合は、背景を右にスクロール
			bg->pos.x = g_Player->pos.x - rightLimitX;
		}
	}


	// 背景のX座標を画面端で制限
	if (bg->pos.x < 0) bg->pos.x = 0;
	if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

	// 上下の制限距離（画面の三分の一）
	float topLimitY = SCREEN_HEIGHT / 3;
	float bottomLimitY = SCREEN_HEIGHT * 2 / 3;

	if (g_LimitBGMove == TRUE)
	{
		// プレイヤーが画面内の移動範囲内かどうかを確認
		if (g_Player->pos.y < bg->pos.y + topLimitY)
		{
			// プレイヤーが上の制限範囲を越えた場合は、背景を上にスクロール
			bg->pos.y = g_Player->pos.y - topLimitY;
		}
		else if (g_Player->pos.y > bg->pos.y + bottomLimitY)
		{
			// プレイヤーが下の制限範囲を越えた場合は、背景を下にスクロール
			bg->pos.y = g_Player->pos.y - bottomLimitY;
		}
	}


	// 背景のY座標を画面端で制限
	if (bg->pos.y < 0) bg->pos.y = 0;
	if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;
}

void UpdateGroundCollision(void)
{
	// プレイヤーの位置を更新する前に接地を確認
	MapWall* grounds = GetMapWall();
	BOOL onGround = false;

	if (g_Player->jumpCnt == 0)
	{
		if (g_Player->onAirCnt < PLAYER_JUMP_CNT_MAX * 0.5f && g_Player->onAirCnt != 0)
		{
			// sin関数を使用して下落速度を計算
			float angle = (XM_PI / PLAYER_JUMP_CNT_MAX) * g_Player->onAirCnt;

			g_Player->move.y = g_Player->jumpYMax * sinf(angle);
		}
		else if (g_Player->onAirCnt >= PLAYER_JUMP_CNT_MAX * 0.5f)
		{
			// 滞空時間が最大に達したら、下落速度を最大値に保つ
			g_Player->move.y = g_Player->jumpYMax;
		}
		// 滞空時間を増加させる
		g_Player->onAirCnt++;

		CHANGE_PLAYER_POS_Y(g_Player->move.y);
		for (int j = 0; j < MAP_WALL_MAX; j++)
		{
			if (grounds[j].use == FALSE) continue;
			if (CheckGroundCollision(g_Player, &grounds[j].wallAABB))
				onGround = TRUE;
		}

		// 地面に接触した場合の処理
		if (onGround)
		{
			// 滞空時間が一定以上であればハードランディング状態に移行
			if (g_Player->onAirCnt > HARDLANDING_HEIGHT && g_Player->state != ATTACK && g_Player->state != KNOCKDOWN)
			{
				// ハードランディングの状態に遷移し、アニメーションを初期化
				g_Player->state = HARD_LANDING;
				g_Player->playAnim = TRUE;
				g_Player->animFrameCount = 0;
				g_Player->patternAnim = 0;
			}

			// プレイヤーの滞空時間をリセットする
			g_Player->onAirCnt = 0;
		}

		// プレイヤーがアニメーション再生中または地面にいる場合、移動をキャンセル
		if ((g_Player->playAnim == TRUE && g_Player->state != KNOCKDOWN)|| onGround)
		{
			// Y座標の移動を元に戻す
			CHANGE_PLAYER_POS_Y(-g_Player->move.y);
		}

	}
	else
	{
		for (int j = 0; j < MAP_WALL_MAX; j++)
		{
			if (grounds[j].use == FALSE) continue;
			CheckGroundCollision(g_Player, &grounds[j].wallAABB);
		}
	}
	//if (GetKeyboardPress(DIK_Z))
	//{

	//}
}

void UpdatePlayerAttackAABB()
{
	AttackAABBTBL* attackTable = nullptr;

	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		attackTable = normalAttack1Tbl;
		break;
	case NORMAL_ATTACK2:
		attackTable = normalAttack2Tbl;
		break;
	case NORMAL_ATTACK3:
		attackTable = normalAttack3Tbl;
		break;
	case NORMAL_ATTACK4:
		attackTable = normalAttack4Tbl;
		break;
	case DASH_ATTACK:
		attackTable = dashAttackTbl;
		break;
	case PARRY:
		attackTable = parryTbl;
		break;
	case FLAME_ATTACK1:
		attackTable = flameAttack1Tbl;
		break;
	case FLAME_ATTACK2:
		attackTable = flameAttack2Tbl;
		break;
	default:
		break;
	}


	if (attackTable != nullptr)
	{
		for (int i = 0; i < MAX_ATTACK_AABB; i++)
		{
			int tableIdx = g_Player->patternAnim * MAX_ATTACK_AABB + i;

			// 敵の向きを判定する: 左向きの場合はdirが1、右向きの場合はdirが-1
			int dir = g_Player->dir == CHAR_DIR_LEFT ? 1 : -1;

			// 向きに応じてX方向のオフセットを調整
			XMFLOAT3 offset = attackTable[tableIdx].posOffset;
			offset.x *= dir;

			// attackAABBの位置を更新
			g_Player->attackAABB[i].pos = XMFLOAT3_ADD(g_Player->bodyAABB.pos, offset);
			g_Player->attackAABB[i].h = attackTable[tableIdx].h;
			g_Player->attackAABB[i].w = attackTable[tableIdx].w;
		}
	}
}

void UpdateActionQueue(void)
{
	g_Player->actionQueueClearTime++;
	if (g_Player->actionQueueClearTime == ACTION_QUEUE_CLEAR_WAIT)
	{
		g_Player->actionQueueStart = 0;
		g_Player->actionQueueEnd = 0;
		g_Player->actionQueueClearTime = 0;
	}
}

void PlayerTakeDamage(ENEMY* enemy, Magic* magic)
{
	g_Player->greyHP = g_Player->HP;
	g_Player->recoveryWindow = HP_RECOVERY_WINDOW;
	int dir = 0;
	if (enemy)
	{
		dir = g_Player->pos.x - enemy->pos.x >= 0 ? 1 : -1;
		enemy->hitPlayer = TRUE;
	}
	else if (magic)
	{
		dir = g_Player->pos.x - magic->pos.x >= 0 ? 1 : -1;
	}
	BOOL validDefend = g_Player->defendDir == CHAR_DIR_RIGHT && dir == -1 || g_Player->defendDir == CHAR_DIR_LEFT && dir == 1;
	if (g_Player->state == DEFEND && validDefend)
	{		
		if (enemy && g_Player->defendCnt <= PARRY_WINDOW && GetEnemyAttributes(enemy))
		{
			enemy->attributes.attackCooldown = GetEnemyAttributes(enemy)->attackCooldown + GetRand(10, 60);  // クールダウンタイムをリセット
			enemy->attributes.staggerResistance = GetEnemyAttributes(enemy)->staggerResistance; // 強靭さをリセット
			enemy->state = ENEMY_HIT;
			enemy->countAnim = 0.0f;
			g_Player->isParrying = TRUE;
			g_Player->defendCnt = 0;
			// 無敵状態を開始する
			g_Player->isInvincible = TRUE;
			PlaySound(SOUND_LABEL_SE_PARRY);
			if (GetTutorialPause() == PAUSE_DEFEND)
			{
				SetTutorialPause(NONE);
			}

		}
		else if (enemy)
		{
			g_Player->HP -= enemy->attributes.damage * 0.5f;
			int moveDir = g_Player->defendDir == CHAR_DIR_RIGHT ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
			if (CheckMoveCollision(1.0f * dir, moveDir))
				CHANGE_PLAYER_POS_X(1.0f * dir);
		}

	}
	else
	{
		if (enemy)
		{
			g_Player->move.x = (float)dir * enemy->attributes.damage * 0.7f;
			g_Player->HP -= enemy->attributes.damage * 15;
		}
		else if (magic)
		{
			g_Player->move.x = dir * 5.0f;
			g_Player->HP -= magic->damage;
			magic->isCollision = FALSE;
		}

		if (g_Player->HP <= 0)
		{
			g_Player->jumpCnt = 0;
			g_Player->onAirCnt = 0;
			g_Player->state = KNOCKDOWN;
			g_Player->die = TRUE;

		}
		else if (g_Player->onAirCnt > 0 || g_Player->state == JUMP || fabs(g_Player->move.x) > KNOCKDOWN_THRESHOLD)
		{
			g_Player->jumpCnt = 0;
			g_Player->onAirCnt = 0;
			g_Player->state = KNOCKDOWN;
		}
		else
		{
			g_Player->state = HIT;
		}


		g_Player->animFrameCount = 0;
		g_Player->patternAnim = 0;
		g_Player->playAnim = TRUE;
		g_Player->countAnim = 0;

		// 無敵状態を開始する
		g_Player->isInvincible = TRUE;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes

			DrawPlayerShadow();

			if (g_Player->jumpEffectCnt > 0)
				DrawJumpEffect();



			// プレイヤーの分身を描画
			if (g_Player[i].state == DASH)
			{	// ダッシュ中だけ分身処理
				DrawPlayerOffset(i);
			}
			else if (g_Player->state == CAST)
				DrawCastEffect();
			else if (g_Player->state == DIE)
				DrawDieFadeOut();

			DrawPlayerSprite();


#ifdef _DEBUG
			{
				MATERIAL materialAABB;
				ZeroMemory(&materialAABB, sizeof(materialAABB));
				materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				materialAABB.noTexSampling = 1;

				SetMaterial(materialAABB);
				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);
				for (int i = 0; i < PLAYER_MAX; ++i)
				{
					SetSpriteColorRotation(g_AABBVertexBuffer, g_Player[i].bodyAABB.pos.x - bg->pos.x, 
						g_Player[i].bodyAABB.pos.y - bg->pos.y, g_Player[i].bodyAABB.w, g_Player[i].bodyAABB.h,
						0.0f, 0.0f, 0.0f, 0.0f,
						XMFLOAT4(0.0f, 1.0f, 0.0f, 0.2f),
						0.0f);

					GetDeviceContext()->Draw(4, i * 4);
				}

				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_attackAABBVertexBuffer, &stride, &offset);
				for (int i = 0; i < PLAYER_MAX; ++i)
				{
					int vertexOffset = i * 4;

					for (int j = 0; j < MAX_ATTACK_AABB; j++)
					{
						SetSpriteColorRotation(g_attackAABBVertexBuffer, g_Player[i].attackAABB[j].pos.x - bg->pos.x,
							g_Player[i].attackAABB[j].pos.y - bg->pos.y, g_Player[i].attackAABB[j].w, g_Player[i].attackAABB[j].h,
							0.0f, 0.0f, 0.0f, 0.0f,
							XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f),
							0.0f,
							vertexOffset);

						GetDeviceContext()->Draw(4, vertexOffset);
					}

				}
			}
#endif
		}
	}
}

void PlayRunningAnim(void)
{
	g_Player->w = TEXTURE_RUN_WIDTH;
	g_Player->h = TEXTURE_RUN_HEIGHT;

	g_Player->texNo = CHAR_RUN;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_RUN)
	{
		g_Player->countAnim = 0.0f;
		// パターンの切り替え
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayIdleAnim(void)
{
	g_Player->w = TEXTURE_IDLE_WIDTH;
	g_Player->h = TEXTURE_IDLE_HEIGHT;

	g_Player->countAnim += 1.0f;
	g_Player->texNo = CHAR_IDLE;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_IDLE)
	{
		g_Player->countAnim = 0.0f;
		// パターンの切り替え
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayDashAnim(void)
{
	g_Player->w = TEXTURE_RUN_WIDTH;
	g_Player->h = TEXTURE_RUN_HEIGHT;

	float speed = g_Player->move.x;

	g_Player->texNo = CHAR_DASH;

	// キャラクターの向きに応じてテクスチャの反転設定
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	if (g_Player->countAnim == 0)
		PlaySound(SOUND_LABEL_SE_DASH);

	// 移動衝突判定を行いながら位置を変更
	switch (g_Player->dir)
	{
	case CHAR_DIR_LEFT:
		if (CheckMoveCollision(-speed * 2.5f, g_Player->dir))
			CHANGE_PLAYER_POS_X(-speed * 2.5f);

		break;
	case CHAR_DIR_RIGHT:
		if (CheckMoveCollision(speed * 2.5f, g_Player->dir))
			CHANGE_PLAYER_POS_X(speed * 2.5f);
		break;
	default:
		break;
	}

	g_Player->countAnim += 1.0f;
	// 一定のカウントに達したらアニメーションを更新
	if (g_Player->countAnim > ANIM_WAIT_DASH)
	{
		g_Player->countAnim = 0.0f;
		// パターンの切り替え
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
		g_Player->animFrameCount++;
	}

	// アニメーションフレームがダッシュフレーム数に達した場合
	if (g_Player->animFrameCount == ANIM_DASH_FRAME)
	{
		g_Player->animFrameCount = 0;

		// ジャンプカウントが一定以上であればジャンプ状態に移行
		if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX * 0.5f)
			g_Player->state = JUMP;
		else
		{
			// そうでなければアイドル状態に移行し、アニメーションを停止
			g_Player->state = IDLE;
			g_Player->playAnim = FALSE;
			g_Player->jumpCnt = 0;
		}

		g_Player->isInvincible = FALSE; // 無敵状態を終了する

	}
}

void PlayAttackAnim(void)
{
	// アタックパターンが最大または未設定の場合、最初の攻撃パターンを設定
	if (g_Player->attackPattern == NORMAL_ATTACK_PATTERN_MAX ||
		g_Player->attackPattern == NONE)
		g_Player->attackPattern = NORMAL_ATTACK1;

	if (g_Player->flameblade == TRUE)
	{
		if (g_Player->attackPattern == FLAME_ATTACK_PATTERN_MAX || g_Player->attackPattern == NONE)
			g_Player->attackPattern = FLAME_ATTACK1;
	}

	if (g_Player->patternAnim == 2 && g_Player->countAnim == 0)
		PlaySound(SOUND_LABEL_SE_ATTACK);

	if (g_Player->attackPattern == NORMAL_ATTACK4 && GetTutorialPause() == PAUSE_ATTACK)
	{
		SetTutorialPause(NONE);
	}

	else if (g_Player->attackPattern == PARRY && GetTutorialPause() == PAUSE_PARRY)
	{
		SetUpdateEnemy(TRUE);
		SetTutorialPause(NONE);
	}
		

	// 攻撃時のテクスチャサイズを調整
	AdjustAttackTextureSize();

	int attackFrame = 0;
	int animWait = ANIM_WAIT_ATTACK;
	float speed = g_Player->move.x;
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		g_Player->texNo = CHAR_NORMAL_ATTACK1;
		attackFrame = ANIM_NORMAL_ATTACK1_FRAME;
		break;
	case NORMAL_ATTACK2:
		g_Player->texNo = CHAR_NORMAL_ATTACK2;
		attackFrame = ANIM_NORMAL_ATTACK2_FRAME;
		break;
	case NORMAL_ATTACK3:
		g_Player->texNo = CHAR_NORMAL_ATTACK3;
		attackFrame = ANIM_NORMAL_ATTACK3_FRAME;
		break;
	case NORMAL_ATTACK4:
		g_Player->texNo = CHAR_NORMAL_ATTACK4;
		attackFrame = ANIM_NORMAL_ATTACK4_FRAME;
		break;
	case DASH_ATTACK:
		g_Player->texNo = CHAR_DASH_ATTACK;
		attackFrame = ANIM_DASH_ATTACK_FRAME;
		animWait = ANIM_WAIT_DASH_ATTACK;
		break;
	case PARRY:
		g_Player->texNo = CHAR_PARRY;
		attackFrame = ANIM_PARRY_FRAME;
		animWait = ANIM_WAIT_PARRY;
		break;
	case FLAME_ATTACK1:
		g_Player->texNo = CHAR_FLAME_ATTACK1;
		attackFrame = ANIM_FLAME_ATTACK1_FRAME;
		break;
	case FLAME_ATTACK2:
		g_Player->texNo = CHAR_FLAME_ATTACK2;
		attackFrame = ANIM_FLAME_ATTACK2_FRAME;
		animWait = ANIM_WAI_FLAME_ATTACK2;
		break;
	default:
		break;
	}


	BOOL attackTouchGround = TRUE;
	if ((g_Player->attackPattern == NORMAL_ATTACK4 && 
		(g_Player->patternAnim == NORMAL_ATTACK4_DROP_FRAME || g_Player->patternAnim == NORMAL_ATTACK4_DROP_FRAME + 1))
		|| (g_Player->attackPattern == FLAME_ATTACK2 && g_Player->patternAnim == FLAME_ATTACK2_DROP_FRAME))
	{
		if (CheckMoveCollision(DIVE_ATTACK_SPEED, CHAR_DIR_DOWN, TRUE))
		{
			CHANGE_PLAYER_POS_Y(DIVE_ATTACK_SPEED);
			attackTouchGround = FALSE;
		}
		else
			attackTouchGround = TRUE;
	}


	// アニメーションを更新
	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > animWait)
	{
		g_Player->countAnim = 0.0f;
		if (attackTouchGround == FALSE)
		{
			if (g_Player->attackPattern == FLAME_ATTACK2)
				g_Player->patternAnim = FLAME_ATTACK2_DROP_FRAME;
			else
				g_Player->patternAnim = g_Player->patternAnim == NORMAL_ATTACK4_DROP_FRAME ? NORMAL_ATTACK4_DROP_FRAME + 1 : NORMAL_ATTACK4_DROP_FRAME;
		}

		else
		{
			g_Player->patternAnim++;
			// アニメーションパターンの上限に達した場合、上限値に固定
			if (g_Player->patternAnim > GetTexturePatternDivideX() - 2)
				g_Player->patternAnim = GetTexturePatternDivideX() - 2;
			g_Player->animFrameCount++;
		}

	}

	// ATTACKフレームによりプレイヤーの位置を調整
	AdjustAttackPlayerPos();

	// 攻撃判定の更新
	UpdatePlayerAttackAABB();

	if (g_Player->animFrameCount >= attackFrame)
	{
		if (GetTutorialPause() == PAUSE_COMBO)
		{
			if (g_Player->attackPattern != NORMAL_ATTACK4)
			{
				SetUpdateEnemy(FALSE);
				g_Update = FALSE;
			}
			else
			{
				SetTutorialPause(NONE);
			}

		}
			

		g_Player->animFrameCount = 0;

		if (g_Player->attackPattern == PARRY)
			g_Player->isInvincible = FALSE; // 無敵状態を終了する

		// ジャンプ中かどうかを判定し、ジャンプ状態に移行
		if (g_Player->jumpCnt > 0)
			g_Player->state = JUMP;
		else
		{
			// ジャンプしていない場合はアイドル状態に戻る
			g_Player->state = IDLE;
			g_Player->playAnim = FALSE;
		}

		if (g_Player->attackInterval > ATTACK_COMBO_WINDOW)
			g_Player->attackPattern = g_Player->flameblade == TRUE ? FLAME_ATTACK1 : NORMAL_ATTACK1;
		else
			g_Player->attackPattern++;
	}
}

void PlayWalkAnim(void)
{
	g_Player->w = TEXTURE_IDLE_WIDTH;
	g_Player->h = TEXTURE_IDLE_HEIGHT;

	g_Player->texNo = CHAR_WALK;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	g_Player->countAnim++;
	if (g_Player->countAnim > ANIM_WAIT_WALK)
	{
		g_Player->countAnim = 0.0f;
		// パターンの切り替え
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayJumpAnim()
{
	if (GetKeyboardPress(DIK_RIGHT))
	{
		// プレイヤーの向きを右に設定し、テクスチャの反転を解除
		g_Player->dir = CHAR_DIR_RIGHT;
		g_Player->invertTex = FALSE;

		// 右方向への移動が可能か判定し、移動可能であれば位置を変更
		if (CheckMoveCollision(g_Player->move.x * 0.8f, CHAR_DIR_RIGHT))
			CHANGE_PLAYER_POS_X(g_Player->move.x * 0.8f);
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		// プレイヤーの向きを左に設定し、テクスチャを反転
		g_Player->dir = CHAR_DIR_LEFT;
		g_Player->invertTex = TRUE;

		// 左方向への移動が可能か判定し、移動可能であれば位置を変更
		if (CheckMoveCollision(-g_Player->move.x * 0.8f, CHAR_DIR_LEFT))
			CHANGE_PLAYER_POS_X(-g_Player->move.x * 0.8f);
	}


	g_Player->w = TEXTURE_JUMP_WIDTH;
	g_Player->h = TEXTURE_JUMP_HEIGHT;

	g_Player->texNo = CHAR_JUMP;

	float angle = (XM_PI / PLAYER_JUMP_CNT_MAX) * g_Player->jumpCnt;
	g_Player->move.y = -1.0f * g_Player->jumpYMax * cosf(angle);
	if (g_Player->move.y > 0)
		allowAirJump = TRUE;
	else
		allowAirJump = FALSE;
	// 上方向か下方向かを判断して一度に処理
	int direction = (g_Player->move.y < 0) ? CHAR_DIR_UP : CHAR_DIR_DOWN;
	if (CheckMoveCollision(g_Player->move.y, direction))
	{
		// 移動可能であればY軸方向の位置を変更
		CHANGE_PLAYER_POS_Y(g_Player->move.y);
	}
	else if (direction == CHAR_DIR_DOWN)
	{
		// 下方向に移動できない場合、ジャンプカウントを最大に設定
		g_Player->jumpCnt = PLAYER_JUMP_CNT_MAX;
	}


	g_Player->countAnim += 1.0f;
	// 一定のカウントに達したらアニメーションを更新
	if (g_Player->countAnim > ANIM_WAIT_JUMP)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		// パターンの範囲を超えた場合、上限の範囲内に調整
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 3)
			g_Player->patternAnim = GetTexturePatternDivideX() - 4;
		g_Player->animFrameCount++;
	}

	g_Player->jumpCnt++;
	// 空中にいるカウントを設定
	g_Player->onAirCnt = g_Player->jumpCnt * 0.5f;

	// ジャンプカウントがジャンプの最高点に達したかチェック
	bool isAtJumpPeak = (g_Player->jumpCnt == PLAYER_JUMP_CNT_MAX * 0.5f);

	// アクションキューが空ではなく、次のアクションがジャンプかチェック
	bool hasJumpInQueue = (g_Player->actionQueueStart != g_Player->actionQueueEnd &&
		g_Player->actionQueue[g_Player->actionQueueStart] == JUMP);

	// 上記の条件が両方とも満たされた場合、空中ジャンプを実行
	if (isAtJumpPeak && hasJumpInQueue && g_Player->jumpOnAirCnt == 0 && g_Player->ST - ST_COST_JUMP >= 0.0f)
	{
		// アクションキューのスタートを次に進める
		g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE;

		// アニメーションパターンをリセット
		g_Player->patternAnim = 0;
		g_Player->animFrameCount = 0;

		// ジャンプカウントと空中ジャンプ回数をリセット
		g_Player->jumpCnt = 0;
		g_Player->jumpEffectCnt = JUMP_EFFECT_TIME;
		g_Player->ST -= ST_COST_JUMP;
		g_Player->airJumpPos = g_Player->pos;
		g_Player->jumpOnAirCnt = 1;
	}
	// ジャンプカウントが最大に達した場合の処理
	if (g_Player->jumpCnt >= PLAYER_JUMP_CNT_MAX)
	{
		// アニメーションを停止し、プレイヤーの状態をアイドルに変更
		g_Player->playAnim = FALSE;
		g_Player->state = IDLE;
		g_Player->jumpCnt = 0;
	}
}

void PlayFallAnim()
{
	g_Player->w = TEXTURE_JUMP_WIDTH;
	g_Player->h = TEXTURE_JUMP_HEIGHT;

	g_Player->texNo = CHAR_JUMP;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_JUMP)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		// パターンの範囲を超えた場合、上限の範囲内に調整
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 3)
			g_Player->patternAnim = GetTexturePatternDivideX() - 4;
		g_Player->animFrameCount++;
	}
}

void PlayHardLandingAnim(void)
{
	g_Player->w = TEXTURE_HARDLAND_WIDTH;
	g_Player->h = TEXTURE_HARDLAND_HEIGHT;

	g_Player->texNo = CHAR_HARD_LANDING;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_HARDLAND)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		// パターンの範囲を超えた場合、上限の範囲内に調整
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 2)
			g_Player->patternAnim = GetTexturePatternDivideX() - 2;
		g_Player->animFrameCount++;
	}

	if (g_Player->animFrameCount >= ANIM_HARDLANDING_FRAME)
	{
		// アニメーションを停止し、プレイヤーの状態をアイドルに変更
		g_Player->playAnim = FALSE;
		g_Player->state = IDLE;
	}
}

void PlayHitAnim(void)
{
	g_Player->w = TEXTURE_HIT_WIDTH;
	g_Player->h = TEXTURE_HIT_HEIGHT;

	g_Player->texNo = CHAR_HIT;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_HIT)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		g_Player->patternAnim = g_Player->patternAnim == 0 ? 1 : 0;
		g_Player->animFrameCount++;
	}

	// ヒット後の移動速度を計算
	float speed = (ANIM_HIT_FRAME - g_Player->animFrameCount) * g_Player->move.x * 0.2f;
	int dir = speed > 0 ? CHAR_DIR_RIGHT : CHAR_DIR_LEFT;
	if (CheckMoveCollision(speed, dir))
	{
		CHANGE_PLAYER_POS_X(speed);
	}

	if (g_Player->animFrameCount >= ANIM_HIT_FRAME)
	{
		// アニメーションを停止し、プレイヤーの状態をアイドルに変更
		g_Player->playAnim = FALSE;
		g_Player->state = IDLE;
		g_Player->move.x = PLAYER_WALK_SPEED; // 移動速度を通常に戻す
		g_Player->isInvincible = FALSE; // 無敵状態を終了する
	}
}

void PlayKnockDownAnim(void)
{
	g_Player->w = TEXTURE_KNOCKDOWN_WIDTH;
	g_Player->h = TEXTURE_KNOCKDOWN_HEIGHT;

	g_Player->texNo = CHAR_KNOCKDOWN;

	if (g_Player->knockDownFall && g_Player->onAirCnt == 0)
	{
		g_Player->knockDownFall = FALSE;
		g_Player->animFrameCount++;
		g_Player->patternAnim++;
		g_Player->countAnim = 0.0f;
	}

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_KNOCKDOWN)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		// アニメーションパターンが最大を超えた場合、最大に固定
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 1)
			g_Player->patternAnim = GetTexturePatternDivideX() - 1;

		if (g_Player->onAirCnt > 0 && g_Player->patternAnim > 1)
		{
			g_Player->patternAnim = 1;
		}
		g_Player->animFrameCount++;
	}


	// ノックダウン時の移動速度を計算
	float speed = (GetTexturePatternDivideX() - 1 - g_Player->patternAnim) * g_Player->move.x * 0.35f;
	int dir = g_Player->move.x > 0 ? CHAR_DIR_RIGHT : CHAR_DIR_LEFT;

	if (g_Player->onAirCnt > ANIM_WAIT_KNOCKDOWN * 2)
	{
		g_Player->knockDownFall = TRUE;
		speed = speed > 0 ? 1.0f : -1.0f;
		g_Player->animFrameCount = g_Player->patternAnim;
	}

	if (CheckMoveCollision(speed, dir))
		CHANGE_PLAYER_POS_X(speed);
	//if (g_Player->onAirCnt > HARDLANDING_HEIGHT)
	//	g_Player->hardlanding = TRUE;
	//
	//if (g_Player->hardlanding == TRUE && g_Player->onAirCnt == 0)
	//{
	//	g_Player->hardlanding = FALSE;
	//}
	if (g_Player->animFrameCount >= ANIM_KNOCKDOWN_FRAME)
	{
		g_Player->animFrameCount = 0;
		if (g_Player->die == TRUE)
		{
			g_Player->state = DIE;
		}
		else
		{
			g_Player->patternAnim = 0;
			// 反動状態に変更
			g_Player->state = REBOUND;
			g_Player->move.x = PLAYER_WALK_SPEED; // 移動速度を通常に戻す
		}

	}
}

void PlayReboundAnim(void)
{
	g_Player->w = TEXTURE_REBOUND_WIDTH;
	g_Player->h = TEXTURE_REBOUND_HEIGHT;

	g_Player->texNo = CHAR_REBOUND;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_REBOUND)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		// アニメーションパターンが最大を超えた場合、最大に固定
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 1)
			g_Player->patternAnim = GetTexturePatternDivideX() - 1;
		g_Player->animFrameCount++;
	}

	if (g_Player->animFrameCount >= ANIM_REBOUND_FRAME)
	{
		// プレイヤーの状態をアイドルに戻し、アニメーションを停止
		g_Player->state = IDLE;
		g_Player->playAnim = FALSE;
		g_Player->animFrameCount = 0;
		g_Player->patternAnim = 0;
		g_Player->isInvincible = FALSE; // 無敵状態を終了する
	}
}

void PlayDefendAnim(void)
{
	g_Player->w = TEXTURE_DEFEND_WIDTH;
	g_Player->h = TEXTURE_DEFEND_HEIGHT;

	g_Player->texNo = CHAR_DEFEND;

	if (g_Player->isWalkingOnDefend == TRUE)
	{
		g_Player->countAnim += 1.0f;
		if (g_Player->countAnim > ANIM_WAIT_DEFEND)
		{
			g_Player->countAnim = 0.0f;
			g_Player->patternAnim = g_Player->patternAnim == 0 ? 1 : 0;
		}
	}
	else
		g_Player->patternAnim = 0;

}

void PlayCastAnim(void)
{
	int castFrame, castAnimWait;
	switch (g_Player->magicCasting)
	{
	case MAGIC_HEALING:
		castFrame = ANIM_HEALING_CAST_FRAME;
		castAnimWait = ANIM_WAIT_CAST_HEALING;
		break;
	case MAGIC_FIRE_BALL:
		castFrame = ANIM_FIRE_BALL_CAST_FRAME;
		castAnimWait = ANIM_WAIT_CAST_FIRE_BALL;
		break;
	default:
		castFrame = 0;
		castAnimWait = 0;
		break;
	}

	if (g_Player->countAnim == 0 && g_Player->patternAnim == 0)
		PlaySound(SOUND_LABEL_SE_CAST);

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > castAnimWait)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		g_Player->patternAnimCast++;
		g_Player->animFrameCount++;
		g_Player->animFrameCountCast++;
	}

	if (g_Player->animFrameCount >= castFrame)
	{
		g_Player->state = IDLE;
		g_Player->animFrameCount = 0;
		g_Player->patternAnim = 0;
		g_Player->patternAnimCast = 0;
		g_Player->playAnim = FALSE;
		TriggerMagic(g_Player->magicCasting);
		switch (g_Player->magicCasting)
		{
		case MAGIC_HEALING:
			PlaySound(SOUND_LABEL_SE_HEAL);
			g_Player->healingCD = HEALING_CD_TIME;
			g_Player->MP -= MP_COST_HEALING;
			g_Player->HP += g_Player->maxHP * 0.7f;
			if (g_Player->HP > g_Player->maxHP)
				g_Player->HP = g_Player->maxHP;
			break;
		case MAGIC_FIRE_BALL:
			PlaySound(SOUND_LABEL_SE_FIRE);
			g_Player->fireBallCD = FIRE_BALL_CD_TIME;
			g_Player->MP -= MP_COST_FIRE_BALL;
			break;
		default:
			break;
		}

		if (g_Player->magicCasting == MAGIC_HEALING && GetTutorialPause() == PAUSE_MAGIC)
		{
			SetTutorialPause(NONE);
		}
	}

}

void PlayDieAnim(void)
{
	g_Player->countAnim += 1.0f;

	if (g_Player->countAnim == ANIM_FADE_OUT_FRAME)
	{
		g_Player->update = FALSE;
		SetRespawnMessageBox(TRUE);
	}
}

//=============================================================================
// Player構造体の先頭アドレスを取得
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}

PlayerData* GetPlayerData(void)
{
	return &g_PlayerData;
}

// 生きてるエネミーの数
int GetPlayerCount(void)
{
	return g_PlayerCount;
}

void DrawPlayerShadow(void)
{
	BG* bg = GetBG();

	// 影表示
	SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[CHAR_SHADOW]);

	float px = g_Player->pos.x - bg->pos.x;	// プレイヤーの表示位置X
	float py = g_Player->shadowY - bg->pos.y;	// プレイヤーの表示位置Y
	float pw = TEXTURE_IDLE_WIDTH;		// プレイヤーの表示幅
	float ph = TEXTURE_IDLE_WIDTH / 4;		// プレイヤーの表示高さ

	// 地面との衝突判定に基づく影の位置
	// プレイヤーの位置に対応する地面の高さを計算
	float shadowPosY = py;  // 影の最終的なY座標
	float highestGroundY = FLT_MAX;  // 最も高い地面のY座標を保持

	// 地面のAABB情報を取得
	MapWall* ground = GetMapWall();
	BOOL isOnGround = false;

	// プレイヤーが地面にいるかどうかを確認
	for (int j = 0; j < MAP_WALL_MAX; j++)
	{
		if (ground[j].use == FALSE) continue;

		float groundX = ground[j].wallAABB.pos.x;
		float groundY = ground[j].wallAABB.pos.y;
		float groundW = ground[j].wallAABB.w;
		float groundH = ground[j].wallAABB.h;

		if (groundY < g_Player->pos.y) continue;

		// プレイヤーのx座標が地面の幅の範囲に入っているかどうかを確認
		if (g_Player->pos.x >= groundX - groundW / 2 && g_Player->pos.x <= groundX + groundW / 2)
		{
			// もっとも高い地面を探す
			if (groundY - groundH / 2 < highestGroundY)
			{
				highestGroundY = groundY - groundH / 2;  // 現在の地面が最も高いので更新
				shadowPosY = groundY - groundH / 2 - bg->pos.y - 50.0f; // 地面の上に影を表示
				isOnGround = true;  // プレイヤーが地面にいるとフラグを設定
			}
		}
	}
	py = shadowPosY + 50.0f;

	float tw = 1.0f;	// テクスチャの幅
	float th = 1.0f;	// テクスチャの高さ
	float tx = 0.0f;	// テクスチャの左上X座標
	float ty = 0.0f;	// テクスチャの左上Y座標



	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
}

void DrawPlayerSprite(void)
{
	BG* bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player->texNo]);

	//プレイヤーの位置やテクスチャー座標を反映
	float px = g_Player->pos.x - bg->pos.x;	// プレイヤーの表示位置X
	float py = g_Player->pos.y - bg->pos.y;	// プレイヤーの表示位置Y
	float pw = g_Player->w;		// プレイヤーの表示幅
	float ph = g_Player->h;		// プレイヤーの表示高さ

	if (g_Player->state == ATTACK)
		AdjustAttackTexturePos(px, py);

	// アニメーション用
	float tw, th, tx, ty;
	tw = 1.0f / GetTexturePatternDivideX(); // テクスチャの幅
	th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
	ty = (float)(g_Player->patternAnim / GetTexturePatternDivideX()) * th;	// テクスチャの左上Y座標
	if (g_Player->invertTex)
	{
		tw *= -1.0f;
		tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * (-1 * tw) - tw;	// テクスチャの左上X座標
	}
	else
		tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * tw;

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		g_Player->rot.z);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void DrawJumpEffect(void)
{
	BG* bg = GetBG();

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[JUMP_EFFECT_TEXTURE]);

	float px = g_Player->airJumpPos.x - bg->pos.x;
	float py = g_Player->airJumpPos.y - bg->pos.y;
	float pw = TEXTURE_JUMP_EFFECT_WIDTH;
	float ph = TEXTURE_JUMP_EFFECT_HEIGHT;

	py += 50.0f;

	float tw = 1.0f;	// テクスチャの幅
	float th = 1.0f;	// テクスチャの高さ
	float tx = 0.0f;	// テクスチャの左上X座標
	float ty = 0.0f;	// テクスチャの左上Y座標


	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	color.w = (float)g_Player->jumpEffectCnt / (float)JUMP_EFFECT_TIME;
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, 
		px, py, pw, ph, 
		tx, ty, tw, th,
		color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void DrawCastEffect(void)
{
	BG* bg = GetBG();
	int castingTexNo = 0;
	float px, py, pw, ph;
	int patternDividX, patternDividY;
	switch (g_Player->magicCasting)
	{
	case MAGIC_HEALING:
		castingTexNo = CAST_HEALING_TEXTURE;
		pw = TEXTURE_CAST_HEALING_WIDTH;
		ph = TEXTURE_CAST_HEALING_HEIGHT;
		patternDividX = TEXTURE_CAST_HEALING_PATTERN_DIVIDE_X;
		patternDividY = TEXTURE_CAST_HEALING_PATTERN_DIVIDE_Y;
		break;
	case MAGIC_FIRE_BALL:
		castingTexNo = CAST_FIRE_BALL_TEXTURE;
		pw = TEXTURE_CAST_FIRE_BALL_WIDTH;
		ph = TEXTURE_CAST_FIRE_BALL_HEIGHT;
		patternDividX = TEXTURE_CAST_FIRE_BALL_PATTERN_DIVIDE_X;
		patternDividY = TEXTURE_CAST_FIRE_BALL_PATTERN_DIVIDE_Y;
		break;
	default:
		return;
	}

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[castingTexNo]);

	px = g_Player->pos.x - bg->pos.x + TEXTURE_CAST_OFFSET_X;
	py = g_Player->pos.y - bg->pos.y + TEXTURE_CAST_OFFSET_Y;

	py += 50.0f;

	float tw = 1.0f / patternDividX;	// テクスチャの幅
	float th = 1.0f / patternDividY;
	float tx = g_Player->animFrameCountCast % patternDividX * tw;
	float ty = g_Player->animFrameCountCast / patternDividY * th;

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer,
		px, py, pw, ph,
		tx, ty, tw, th,
		color);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void DrawDieFadeOut(void)
{
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	color.w = g_Player->countAnim / ANIM_FADE_OUT_FRAME * 0.8f;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[DIE_FADE_OUT_TEXTURE]);
	SetSpriteColor(g_VertexBuffer,
		SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f,
		color);
	GetDeviceContext()->Draw(4, 0);

	color.w = g_Player->countAnim / ANIM_FADE_OUT_FRAME;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[DIE_LOGO_TEXTURE]);
	SetSpriteColor(g_VertexBuffer,
		TEXTURE_DIE_LOGO_POS_X, TEXTURE_DIE_LOGO_POS_Y,
		TEXTURE_DIE_LOGO_WIDTH, TEXTURE_DIE_LOGO_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f,
		color);
	GetDeviceContext()->Draw(4, 0);
}

//=============================================================================
// プレイヤーの分身を描画
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG* bg = GetBG();
	float alpha = 0.0f;

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].texNo]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Player[no].offset[j].x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Player[no].offset[j].y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Player[no].w;		// プレイヤーの表示幅
		float ph = g_Player[no].h;		// プレイヤーの表示高さ

		// アニメーション用
		float tw = 1.0f / GetTexturePatternDivideX();	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float ty = (float)(g_Player[no].patternAnim / GetTexturePatternDivideX()) * th;	// テクスチャの左上Y座標
		float tx;
		if (g_Player->invertTex)
		{
			tw *= -1.0f;
			tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * (-1 * tw) - tw;	// テクスチャの左上X座標
		}
		else
			tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * tw;

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}


int GetTexturePatternDivideX()
{
	switch (g_Player->texNo)
	{
	case CHAR_IDLE:
		return TEXTURE_IDLE_PATTERN_DIVIDE_X;
	case CHAR_WALK:
		return TEXTURE_WALK_PATTERN_DIVIDE_X;
	case CHAR_RUN:
		return TEXTURE_RUN_PATTERN_DIVIDE_X;
	case CHAR_DASH:
		return TEXTURE_DASH_PATTERN_DIVIDE_X;
	case CHAR_JUMP:
		return TEXTURE_JUMP_PATTERN_DIVIDE_X;
	case CHAR_HARD_LANDING:
		return TEXTURE_HARD_LANDING_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK1:
		return TEXTURE_NORMAL_ATTACK1_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK2:
		return TEXTURE_NORMAL_ATTACK2_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK3:
		return TEXTURE_NORMAL_ATTACK3_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK4:
		return TEXTURE_NORMAL_ATTACK4_PATTERN_DIVIDE_X;
	case CHAR_DASH_ATTACK:
		return TEXTURE_DASH_ATTACK_PATTERN_DIVIDE_X;
	case CHAR_HIT:
		return TEXTURE_HIT_PATTERN_DIVIDE_X;
	case CHAR_KNOCKDOWN:
		return TEXTURE_KNOCKDOWN_PATTERN_DIVIDE_X;
	case CHAR_REBOUND:
		return TEXTURE_REBOUND_PATTERN_DIVIDE_X;
	case CHAR_DEFEND:
		return TEXTURE_DEFEND_PATTERN_DIVIDE_X;
	case CHAR_PARRY:
		return TEXTURE_PARRY_PATTERN_DIVIDE_X;
	case CHAR_FLAME_ATTACK1:
		return TEXTURE_FLAME_ATTACK1_PATTERN_DIVIDE_X;
	case CHAR_FLAME_ATTACK2:
		return TEXTURE_FLAME_ATTACK2_PATTERN_DIVIDE_X;
	default:
		return -1;
	}
}

BOOL CheckMoveCollision(float move, int dir, BOOL checkGround)
{
	// 壁のAABB情報を取得
	MapWall* walls = GetMapWall();
	for (int i = 0; i < MAP_WALL_MAX; i++)
	{
		if (walls[i].use == FALSE) continue;
		if (walls[i].wallAABB.tag == GROUND_AABB && checkGround == FALSE) continue;

		XMFLOAT3 wallPos = walls[i].wallAABB.pos;
		float wallW = walls[i].wallAABB.w;
		float wallH = walls[i].wallAABB.h;

		
		// 衝突確認
		switch (dir)
		{
		case CHAR_DIR_LEFT:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x + move, g_Player->pos.y, g_Player->pos.z);

			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH) &&
				g_Player->bodyAABB.pos.x > wallPos.x)
			{
				// プレイヤーが左に移動していて、壁が左側にある場合
				SET_PLAYER_POS_X(wallPos.x + wallW / 2 + g_Player->bodyAABB.w / 2 + 0.01f); // 左への進行を停止
				return false;
			}
			break;
		}

		case CHAR_DIR_RIGHT:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x + move, g_Player->pos.y, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH) &&
				g_Player->bodyAABB.pos.x < wallPos.x)
			{
				// プレイヤーが右に移動していて、壁が右側にある場合
				SET_PLAYER_POS_X(wallPos.x - wallW / 2 - g_Player->bodyAABB.w / 2 - 0.01f); // 右への進行を停止
				return false;
			}
			break;
		}

		case CHAR_DIR_UP:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x, g_Player->pos.y + move, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH))
			{
				// プレイヤーが上に移動していて、壁が上側にある場合
				SET_PLAYER_POS_Y(wallPos.y + wallH / 2 + g_Player->bodyAABB.h / 2); // 上への進行を停止
				return false;
			}
			break;
		}
		case CHAR_DIR_DOWN:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x, g_Player->pos.y + move, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH))
			{
				// プレイヤーが上に移動していて、壁が下側にある場合
				SET_PLAYER_POS_Y(wallPos.y - wallH / 2 - g_Player->bodyAABB.h / 2); // 下への進行を停止
				return false;
			}
			break;
		}
		default:
			break;
		}
	}

	return true;
}

BOOL CheckGroundCollision(PLAYER* g_Player, AABB* ground)
{
	if (noGravity)
		return true;

	// プレイヤーのAABB情報を取得
	XMFLOAT3 playerPos = g_Player->bodyAABB.pos;
	float playerW = g_Player->bodyAABB.w;
	float playerH = g_Player->bodyAABB.h;

	// 地面のAABB情報を取得
	XMFLOAT3 groundPos = ground->pos;
	float groundW = ground->w;
	float groundH = ground->h;

	if (groundPos.y < playerPos.y) return false;

	// 衝突確認
	BOOL isColliding = CollisionBB(playerPos, playerW, playerH, groundPos, groundW, groundH);
	// プレイヤーが地面の上でどれくらい重なっているかを計算
	float overlapLeft = max(playerPos.x - playerW / 2, groundPos.x - groundW / 2);
	float overlapRight = min(playerPos.x + playerW / 2, groundPos.x + groundW / 2);
	float overlapWidth = overlapRight - overlapLeft;

	// 落ちる閾値
	float threshold = playerW / FALLING_THRESHOLD;

	// プレイヤーのy方向の速度が0より小さい（プレイヤーが落下中）場合、かつ衝突が発生し、重なりが閾値を超えている場合
	if (isColliding && g_Player->move.y >= 0 && overlapWidth >= threshold) 
	{
		// プレイヤーが落下中でかつ地面に十分接触している、落下を止め
		g_Player->move.y = 0.0f;

		// ジャンプの状態からidle状態に戻る
		if (g_Player->jumpCnt > 0)
		{
			g_Player->playAnim = FALSE;
			g_Player->state = IDLE;
			g_Player->jumpCnt = 0;
		}
		// 空中ダッシュ状態をリセット
		g_Player->dashOnAir = FALSE;
		// 空中ダッシュ回数をリセット
		g_Player->airDashCount = 0;
		g_Player->jumpOnAirCnt = 0;

		
		// プレイヤーの位置を地面の上に調整し、地面を通り抜けないようにする
		SET_PLAYER_POS_Y(groundPos.y - groundH * 0.5f - playerH * 0.5f + 0.01f); // 0.01fのマージンを追加

		ground->tag = GROUND_AABB;
		return true;
	}
	else if (isColliding && overlapWidth < threshold) 
	{
		// プレイヤーが地面の上にあまり重なっていないので落下を続行
		// プレイヤーを少し押し出して、地面と衝突しないようにする
		if (playerPos.x < groundPos.x) 
		{
			// プレイヤーが左側にある場合
			CHANGE_PLAYER_POS_X(-1.0f);// 少し左に押し出す
		}
		else 
		{
			// プレイヤーが右側にある場合
			CHANGE_PLAYER_POS_X(1.0f);// 少し右に押し出す
		}
	}
	ground->tag = WALL_AABB;

	return false;
}

void AdjustAttackTextureSize()
{
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		g_Player->w = TEXTURE_NORMAL_ATTACK1_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK1_HEIGHT;
		break;
	case NORMAL_ATTACK2:
		g_Player->w = TEXTURE_NORMAL_ATTACK2_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK2_HEIGHT;
		break;
	case NORMAL_ATTACK3:
		g_Player->w = TEXTURE_NORMAL_ATTACK3_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK3_HEIGHT;
		break;
	case NORMAL_ATTACK4:
		g_Player->w = TEXTURE_NORMAL_ATTACK4_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK4_HEIGHT;
		break;
	case DASH_ATTACK:
		g_Player->w = TEXTURE_DASH_ATTACK_WIDTH;
		g_Player->h = TEXTURE_DASH_ATTACK_HEIGHT;
		break;
	case PARRY:
		g_Player->w = TEXTURE_PARRY_WIDTH;
		g_Player->h = TEXTURE_PARRY_HEIGHT;
		break;
	case FLAME_ATTACK1:
		g_Player->w = TEXTURE_FLAME_ATTACK1_WIDTH;
		g_Player->h = TEXTURE_FLAME_ATTACK1_HEIGHT;
		break;
	case FLAME_ATTACK2:
		g_Player->w = TEXTURE_FLAME_ATTACK2_WIDTH;
		g_Player->h = TEXTURE_FLAME_ATTACK2_HEIGHT;
		break;
	default:
		break;
	}
}

void AdjustAttackTexturePos(float& px, float& py)
{
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_NORMAL_ATTACK1_OFFSET.x;
		else
			px -= TEXTURE_NORMAL_ATTACK1_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK1_OFFSET.y;
		break;
	case NORMAL_ATTACK2:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_NORMAL_ATTACK2_OFFSET.x;
		else
			px -= TEXTURE_NORMAL_ATTACK2_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK2_OFFSET.y;
		break;
	case NORMAL_ATTACK3:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_NORMAL_ATTACK3_OFFSET.x;
		else
			px -= TEXTURE_NORMAL_ATTACK3_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK3_OFFSET.y;
		break;
	case NORMAL_ATTACK4:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_NORMAL_ATTACK4_OFFSET.x;
		else
			px -= TEXTURE_NORMAL_ATTACK4_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK4_OFFSET.y;
		break;
	case DASH_ATTACK:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_DASH_ATTACK_OFFSET.x;
		else
			px -= TEXTURE_DASH_ATTACK_OFFSET.x;
		py += TEXTURE_DASH_ATTACK_OFFSET.y;
		break;
	case PARRY:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_PARRY_OFFSET.x;
		else
			px -= TEXTURE_PARRY_OFFSET.x;
		py += TEXTURE_PARRY_OFFSET.y;
		break;
	case FLAME_ATTACK1:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_FLAME_ATTACK1_OFFSET.x;
		else
			px -= TEXTURE_FLAME_ATTACK1_OFFSET.x;
		py += TEXTURE_FLAME_ATTACK1_OFFSET.y;
		break;
	case FLAME_ATTACK2:
		if (g_Player->dir == CHAR_DIR_RIGHT)
			px += TEXTURE_FLAME_ATTACK2_OFFSET.x;
		else
			px -= TEXTURE_FLAME_ATTACK2_OFFSET.x;
		py += TEXTURE_FLAME_ATTACK2_OFFSET.y;
		break;
	default:
		break;
	}
}

void AdjustAttackPlayerPos(void)
{
	float speedX = g_Player->move.x;

	speedX *= g_Player->dir == CHAR_DIR_RIGHT ? 1.0f : -1.0f;

	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
	{
		BOOL isMoveFrame = g_Player->patternAnim >= 2 && g_Player->patternAnim <= 4;
		if (isMoveFrame && CheckMoveCollision(speedX * 0.2f, g_Player->dir))
			CHANGE_PLAYER_POS_X(speedX * 0.2f);
		break;
	}
	case NORMAL_ATTACK2:
	{
		BOOL isMoveFrame = g_Player->patternAnim >= 1 && g_Player->patternAnim <= 6;
		int dir = g_Player->dir == CHAR_DIR_RIGHT ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
		if (isMoveFrame && CheckMoveCollision(-speedX * 0.2f, dir))
			CHANGE_PLAYER_POS_X(-speedX * 0.2f);
		break;
	}	
	case NORMAL_ATTACK3:
	{
		BOOL isMoveFrame = g_Player->patternAnim >= 1 && g_Player->patternAnim <= 5;
		if (isMoveFrame && CheckMoveCollision(speedX * 0.15f, g_Player->dir))
			CHANGE_PLAYER_POS_X(speedX * 0.15f);
		break;
	}
	case NORMAL_ATTACK4:
	{
		float speedY = -40.0f;
		BOOL isMoveFrame = g_Player->patternAnim >= 1 && g_Player->patternAnim <= 3 && g_Player->onAirCnt == 0;
		if (isMoveFrame && CheckMoveCollision(speedY, g_Player->dir))
			CHANGE_PLAYER_POS_Y(speedY);
			
		break;
	}
	case DASH_ATTACK:
	{
		BOOL isMoveFrame = g_Player->patternAnim >= 0 && g_Player->patternAnim <= 5;
		float speedAdjust = 1.5f - g_Player->patternAnim * 0.2f;
		if (isMoveFrame && CheckMoveCollision(speedX * speedAdjust, g_Player->dir))
			CHANGE_PLAYER_POS_X(speedX * speedAdjust);
		break;
	}
	case FLAME_ATTACK2:
	{
		float speedY = -40.0f;
		BOOL isMoveFrame = g_Player->patternAnim >= 1 && g_Player->patternAnim <= 3 && g_Player->onAirCnt == 0;
		if (isMoveFrame && CheckMoveCollision(speedY, g_Player->dir))
			CHANGE_PLAYER_POS_Y(speedY);

		break;
	}
	default:
		break;
	}
}

float GetPlayerDamage(void)
{
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		return g_Player->ATK * NORMAL_ATTACK1_DAMAGE_RATE;
	case NORMAL_ATTACK2:
		return g_Player->ATK * NORMAL_ATTACK2_DAMAGE_RATE;
	case NORMAL_ATTACK3:
		return g_Player->ATK * NORMAL_ATTACK3_DAMAGE_RATE;
	case NORMAL_ATTACK4:
		return g_Player->ATK * NORMAL_ATTACK4_DAMAGE_RATE;
	case DASH_ATTACK:
		return g_Player->ATK * DASH_ATTACK_DAMAGE_RATE;
	case PARRY:
		return g_Player->ATK * PARRY_DAMAGE_RATE;
	case FLAME_ATTACK1:
		return g_Player->ATK * FLAME_ATTACK1_DAMAGE_RATE;
	case FLAME_ATTACK2:
		return g_Player->ATK * FLAME_ATTACK2_DAMAGE_RATE;
	default:
		return 0.0f;
	}
}
float GetPoiseDamage(void)
{
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		return g_Player->ATK * NORMAL_ATTACK1_POISE_RATE;
	case NORMAL_ATTACK2:
		return g_Player->ATK * NORMAL_ATTACK2_POISE_RATE;
	case NORMAL_ATTACK3:
		return g_Player->ATK * NORMAL_ATTACK3_POISE_RATE;
	case NORMAL_ATTACK4:
		return g_Player->ATK * NORMAL_ATTACK4_POISE_RATE;
	case DASH_ATTACK:
		return g_Player->ATK * DASH_ATTACK_POISE_RATE;
	case PARRY:
		return g_Player->ATK * PARRY_POISE_RATE;
	case FLAME_ATTACK1:
		return g_Player->ATK * FLAME_ATTACK1_POISE_RATE;
	case FLAME_ATTACK2:
		return g_Player->ATK * FLAME_ATTACK2_POISE_RATE;
	default:
		return 0.0f;
	}
}

int GetMagicLearnedCount(void)
{
	int count = 0;
	for (int i = 0; i < MAGIC_NUM_MAX; i++)
	{
		if (g_PlayerData.magicLearned[i] != MAGIC_NONE)
			count++;
	}

	return count;
}

int GetMagicEquippedCount(void)
{
	int count = 0;
	for (int i = 0; i < MAGIC_SLOT_MAX; i++)
	{
		if (g_PlayerData.magicList[i] != MAGIC_NONE)
			count++;
	}

	return count;
}

void SetPlayerInitPosByMap(int map, int idx)
{
	XMFLOAT3 initPos = GetTeleportInitPos(map, idx);
	g_InitPos.x = initPos.x;
	g_InitPos.y = initPos.y;
}

void SetPlayerInitPos(float posX, float posY)
{
	g_InitPos.x = posX;
	g_InitPos.y = posY;
	g_PlayerData.initPos = XMFLOAT3(posX, posY, 0.0f);
}

void ResetPlayerPos(void)
{
	SET_PLAYER_POS_X(g_InitPos.x);
	SET_PLAYER_POS_Y(g_InitPos.y);
}

int CalculateExpForLevel(int level)
{
	// 初期の経験値
	int baseExp = INIT_REQUIRED_EXP;

	// 経験値の成長率
	float growthRate = 1.1f; // レベルごとに10%成長

	// 指数関数を使用して必要な経験値を計算
	return (int)(baseExp * pow(growthRate, level - 1));
}

void InitPlayerData(void)
{
	g_PlayerData.maxHP = PLAYER_INIT_MAX_HP;
	g_PlayerData.maxMP = PLAYER_INIT_MAX_MP;
	g_PlayerData.maxST = PLAYER_INIT_MAX_ST;
	g_PlayerData.ATK = PLAYER_INIT_ATK;
	g_PlayerData.DEF = PLAYER_INIT_DEF;
	g_PlayerData.MAT = PLAYER_INIT_MAT;
	g_PlayerData.MDF = PLAYER_INIT_MDF;
	g_PlayerData.level = 1;
	g_PlayerData.skillPointLeft = 5;
	g_PlayerData.spHP = 8;
	g_PlayerData.spMP = 8;
	g_PlayerData.spST = 5;
	g_PlayerData.spATK = 4;
	g_PlayerData.spDEF = 4;
	g_PlayerData.spMAT = 3;
	g_PlayerData.spMDF = 2;
	for (int i = 0; i < MAGIC_NUM_MAX; i++)
	{
		g_PlayerData.magicLearned[i] = MAGIC_NONE;
	}
	for (int i = 0; i < MAGIC_SLOT_MAX; i++)
	{
		g_PlayerData.magicList[i] = MAGIC_NONE;
	}
	g_PlayerData.magicList[0] = MAGIC_FLAMEBLADE;
	g_PlayerData.magicLearned[0] = MAGIC_FLAMEBLADE;
	g_PlayerData.magicList[1] = MAGIC_HEALING;
	g_PlayerData.magicLearned[1] = MAGIC_HEALING;
	g_PlayerData.magicList[2] = MAGIC_FIRE_BALL;
	g_PlayerData.magicLearned[2] = MAGIC_FIRE_BALL;
	if (GetMode() == MODE_TUTORIAL)
	{
		g_PlayerData.magicList[3] = MAGIC_NONE;
		g_PlayerData.magicList[4] = MAGIC_NONE;
	}
	else
	{
		g_PlayerData.magicLearned[3] = MAGIC_SHIELD;
		g_PlayerData.magicLearned[4] = MAGIC_HIDDEN;
	}
}

void InitPlayerStatus(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_PlayerCount++;
		g_Player[i].use = TRUE;
		g_Player[i].update = TRUE;
		g_Player[i].die = FALSE;
		g_Player[i].pos = g_InitPos;
		g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player[i].w = TEXTURE_IDLE_WIDTH;
		g_Player[i].h = TEXTURE_IDLE_HEIGHT;
		g_Player[i].shadowY = g_Player[i].pos.y;
		g_Player[i].texNo = 0;
		g_Player[i].state = IDLE;
		g_Player[i].invertTex = FALSE;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;
		g_Player[i].patternAnimOld = 0;
		g_Player[i].animFrameCount = 0;
		g_Player[i].animFrameCountCast = 0;
		g_Player[i].dashCount = 0;
		g_Player[i].dashCD = 0;
		g_Player[i].airDashCount = 0;
		g_Player[i].maxDashCount = MAX_DASH_COUNT;
		g_Player[i].dashInterval = 0;

		g_Player[i].move = XMFLOAT3(PLAYER_WALK_SPEED, 0.0f, 0.0f);		// 移動量

		g_Player[i].dir = CHAR_DIR_RIGHT;					// 右向きにしとくか
		g_Player[i].defendDir = CHAR_DIR_RIGHT;
		g_Player[i].playAnim = FALSE;
		g_Player[i].dashOnAir = FALSE;
		g_Player[i].knockDownFall = FALSE;
		g_Player[i].jumpOnAir = TRUE;
		g_Player[i].isRunning = FALSE;
		g_Player[i].isDefending = FALSE;
		g_Player[i].isParrying = FALSE;
		g_Player[i].wasRunningExhausted = FALSE;
		g_Player[i].wasDefendingExhausted = FALSE;
		g_Player[i].jumpOnAirCnt = 0;
		g_Player[i].defendCnt = 0;
		g_Player[i].jumpEffectCnt = 0;
		g_Player[i].patternAnimCast = 0;
		g_Player[i].magicCasting = MAGIC_NONE;
		g_Player[i].patternAnim = CHAR_IDLE;
		g_Player[i].airJumpPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

		g_Player[i].maxHP = g_PlayerData.maxHP;
		g_Player[i].HP = g_Player[i].maxHP;
		g_Player[i].maxMP = g_PlayerData.maxMP;
		g_Player[i].MP = g_Player[i].maxMP;
		g_Player[i].maxST = g_PlayerData.maxST;
		g_Player[i].ST = g_Player[i].maxST;
		g_Player[i].ATK = g_PlayerData.ATK;
		g_Player[i].DEF = g_PlayerData.DEF;
		g_Player[i].MAT = g_PlayerData.MAT;
		g_Player[i].MDF = g_PlayerData.MDF;
		g_Player[i].greyHP = g_Player[i].maxHP;
		g_Player[i].recoveryWindow = 0;
		g_Player[i].isInvincible = FALSE;
		g_Player[i].attackPattern = NONE;
		g_Player[i].currentMagicIdx = 0;
		g_Player[i].healingCD = 0;
		g_Player[i].fireBallCD = 0;
		g_Player[i].flameblade = FALSE;

		g_Player[i].actionQueueStart = 0;
		g_Player[i].actionQueueEnd = 0;
		g_Player[i].actionQueueClearTime = 0;

		// ジャンプの初期化
		g_Player[i].jump = FALSE;
		g_Player[i].jumpCnt = 0;
		g_Player[i].jumpYMax = PLAYER_JUMP_Y_MAX;

		g_Player[i].onAirCnt = 0;
		g_Player[i].attackInterval = 0;

		for (int j = 0; j < ACTION_QUEUE_SIZE; j++)
		{
			g_Player[i].actionQueue[j] = 0;
		}

		// 分身用
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}

		// AABB
		g_Player[i].bodyAABB.pos = g_Player[i].pos;
		g_Player[i].bodyAABB.w = g_Player[i].w * 0.7f;
		g_Player[i].bodyAABB.h = g_Player[i].h;
		g_Player[i].bodyAABB.tag = PLAYER_BODY_AABB;

		// magic
		for (int i = 0; i < MAGIC_SLOT_MAX; i++)
		{
			g_Player->magicList[i] = g_PlayerData.magicList[i];
		}

	}
}

void PlayerRespawn(void)
{
	SetRespawnMessageBox(FALSE);
	InitPlayerStatus();
}

void PlayerRespawnDirectly(void)
{
	XMFLOAT3 pos = g_Player->pos;
	InitPlayerStatus();
	g_Player->state = REBOUND;
	g_Player->playAnim = TRUE;
	SET_PLAYER_POS_X(pos.x);
	SET_PLAYER_POS_Y(pos.y);

}

void SetLimitPlayerMoveLeft(BOOL limit)
{
	g_LimitPlayerMoveLeft = limit;
}

void SetLimitPlayerMoveRight(BOOL limit)
{
	g_LimitPlayerMoveRight = limit;
}

void SetLimitBGMove(BOOL limit)
{
	g_LimitBGMove = limit;
}

void SetPlayerHP(int HP)
{
	g_Player->HP = HP;
}

void SetPlayerMP(int MP)
{
	g_Player->MP = MP;
}

void SetPlayerST(int ST)
{
	g_Player->ST = ST;
}

void SetPlayerDir(int dir)
{
	g_Player->dir = dir;
}

void SetPlayerPosX(float posX)
{
	SET_PLAYER_POS_X(posX);
}

void SetPlayerPosY(float posY)
{
	SET_PLAYER_POS_Y(posY);
}

void SetPlayerInvincible(BOOL invincible)
{
	g_Player->isInvincible = invincible;
}

void SetPlayerCurrentMagic(int magic)
{
	g_Player->magicCasting = magic;
}

BOOL GetUpdatePlayer(void)
{
	return g_Update;
}

BOOL GetMagicActive(int magic)
{
	switch (magic)
	{
	case MAGIC_HEALING:
		return g_DisableHealing == TRUE ? FALSE : TRUE;
	case MAGIC_FIRE_BALL:
		return g_DisableFireBall == TRUE ? FALSE : TRUE;
	case MAGIC_FLAMEBLADE:
		return g_DisableFlameblade == TRUE ? FALSE : TRUE;
	default:
		return FALSE;
	}
}

void SetUpdatePlayer(BOOL update)
{
	g_Update = update;
}

void DisablePlayerAttack(BOOL disable)
{
	g_DisableAttack = disable;
}

void DisablePlayerJump(BOOL disable)
{
	g_DisableJump = disable;
}

void DisablePlayerMagicSwitch(BOOL disable)
{
	g_DisableMagicSwitch = disable;
}

void DisablePlayerHealing(BOOL disable)
{
	g_DisableHealing = disable;
}

void DisablePlayerFireBall(BOOL disable)
{
	g_DisableFireBall = disable;
}

void DisablePlayerFlameblade(BOOL disable)
{
	g_DisableFlameblade = disable;
}

void DisablePlayerDash(BOOL disable)
{
	g_DisableDash = disable;
}

void DisablePlayerDefend(BOOL disable)
{
	g_DisableDefend = disable;
}

void DisablePlayerRun(BOOL disable)
{
	g_DisableRun = disable;
}

void DisablePlayerLeftMove(BOOL disable)
{
	g_DisableMoveLeft = disable;
}

void DisablePlayerRightMove(BOOL disable)
{
	g_DisableMoveRight = disable;
}

void DisablePlayerDefendCount(BOOL disable)
{
	g_DisableDefendCount = disable;
}