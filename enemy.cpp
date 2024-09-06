//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "map.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "score.h"
#include "magic.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
// キャラサイズ
#define TEXTURE_WIDTH_CYCLOPS		(540/2)
#define TEXTURE_HEIGHT_CYCLOPS		(400/2)
#define TEXTURE_WIDTH_GARGOYLE		(280/2)
#define TEXTURE_HEIGHT_GARGOYLE		(210/2)
#define TEXTURE_WIDTH_GNOLL			(465/2)
#define TEXTURE_HEIGHT_GNOLL		(375/2)
#define TEXTURE_WIDTH_GOBLIN		(300/2)
#define TEXTURE_HEIGHT_GOBLIN		(250/2)
#define TEXTURE_WIDTH_GOLEM			(720/2)
#define TEXTURE_HEIGHT_GOLEM		(600/2)
#define TEXTURE_WIDTH_IMP			(300/2)
#define TEXTURE_HEIGHT_IMP			(250/2)
#define TEXTURE_WIDTH_MUMMY			(450/2)
#define TEXTURE_HEIGHT_MUMMY		(375/2)
#define TEXTURE_WIDTH_OGRE			(650/2)
#define TEXTURE_HEIGHT_OGRE			(470/2)
#define TEXTURE_WIDTH_SKELL			(390/2)
#define TEXTURE_HEIGHT_SKELL		(297/2)
#define TEXTURE_MAX					(15)		// テクスチャの数

#define	ENEMY_HP_GAUGE_TEXTURE		(9)
#define ENEMY_SHADOW_TEXTURE		(11)
#define LIGHTPOINT_TEXTURE			(12)

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(5)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_IDLE_PATTERN_NUM		(1)
#define ANIM_WALK_PATTERN_NUM		(4)
#define ANIM_ATTACK_PATTERN_NUM		(3)
#define ANIM_ATTACK_OFFSET			(6)
#define ANIM_DIE_OFFSET				(4)
#define	ANIM_HIT_OFFSET				(9)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

// アニメーションの切り替わるWait値
#define ANIM_WAIT_WALK				(20)
#define ANIM_WAIT_ATTACK			(15)
#define ANIM_WAIT_IDLE				(25)
#define ANIM_WAIT_DIE				(150)
#define IDLE_WAIT					(50)


// lightpoint
#define TEXTURE_WIDTH_LIGHTPOINT	(150.0f)
#define TEXTURE_HEIGHT_LIGHTPOINT	(150.0f)
#define LIGHPOINT_SPEED_MIN			(2)
#define LIGHTPOINT_SPEED_MAX		(5)
#define LIGHTPOINT_FLY_TIME			(25)
#define LIGHTPOINT_STAY_TIME		(45)
#define LIGHTPOINT_REACH_PLAYER_TIME (80.0f)
#define LIGHTPOINT_TARGET_OFFSET_X	(-70.0f)
#define LIGHTPOINT_TARGET_OFFSET_Y	(-70.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void EnemyTakeDamage(ENEMY* enemy, Magic* magic = nullptr);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

#ifdef _DEBUG	
static ID3D11Buffer* g_bodyAABBVertexBuffer = NULL;
static ID3D11Buffer* g_attackAABBVertexBuffer = NULL;
#endif

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/monster/M_cyclops.png",
	"data/TEXTURE/monster/M_gargoyle.png",
	"data/TEXTURE/monster/M_gnoll_sword.png",
	"data/TEXTURE/monster/M_goblin_swordman.png",
	"data/TEXTURE/monster/M_golem.png",
	"data/TEXTURE/monster/M_imp_sword_.png",
	"data/TEXTURE/monster/M_mummy.png",
	"data/TEXTURE/monster/M_ogre.png",
	"data/TEXTURE/monster/M_skell_warrior.png",
	"data/TEXTURE/monster/EnemyHPGauge.png",
	"data/TEXTURE/monster/EnemyHPGauge_bg.png",
	"data/TEXTURE/char/shadow000.jpg",
	"data/TEXTURE/lightpoint.png",
};


static BOOL			g_Load = FALSE;			// 初期化を行ったかのフラグ
static ENEMY		g_Enemy[ENEMY_MAX];		// エネミー構造体
static LightPoint 	g_LightPoint[LIGHTPOINT_MAX];

static int		g_EnemyCount = ENEMY_MAX;
static int		g_LightPointInterval = 10;
static BOOL		g_Update = TRUE;

static EnemyAttributes cyclopsAttributes = {
	100.0f,	// float hp
	100.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes gargoyleAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes gnollAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes goblinAttributes = {
	130.0f,	// float hp
	130.0f,	// float maxhp
	5,			// int damage
	5,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	75.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes golemAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes impAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes mummyAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes ogreAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes skellAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes* attributesTbl[] =
{
	&cyclopsAttributes,
	&gargoyleAttributes,
	&gnollAttributes,
	&goblinAttributes,
	&golemAttributes,
	&impAttributes,
	&mummyAttributes,
	&ogreAttributes,
	&skellAttributes
};


static INTERPOLATION_DATA* g_MoveTblAdr[MAP_ENEMY_MAX];

static AttackAABBTBL cyclopsAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(95.0f, 0.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-95.0f, 10.0f, 0.0f), 50.0f, 140.0f},
	{XMFLOAT3(0.0f, -80.0f, 0.0f), 160.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-95.0f, 10.0f, 0.0f), 50.0f, 140.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL gargoyleAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 20.0f, 0.0f), 20.0f, 60.0f},
	{XMFLOAT3(0.0f, 30.0f, 0.0f), 80.0f, 30.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL gnollAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(75.0f, 0.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, -80.0f, 0.0f), 160.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL goblinAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(55.0f, 0.0f, 0.0f), 31.0f, 85.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-55.0f, 10.0f, 0.0f), 31.0f, 85.0f},
	{XMFLOAT3(0.0f, -40.0f, 0.0f), 130.0f, 45.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-55.0f, 10.0f, 0.0f), 31.0f, 85.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL golemAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(105.0f, -100.0f, 0.0f), 80.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 125.0f, 250.0f},
	{XMFLOAT3(0.0f, -120.0f, 0.0f), 160.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 125.0f, 250.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL impAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 10.0f, 0.0f), 30.0f, 90.0f},
	{XMFLOAT3(-20.0f, -40.0f, 0.0f), 60.0f, 30.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL mummyAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(0.0f, -65.0f, 0.0f), 120.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-85.0f, 10.0f, 0.0f), 40.0f, 140.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL ogreAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(115.0f, 0.0f, 0.0f), 80.0f, 180.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-115.0f, 10.0f, 0.0f), 80.0f, 180.0f},
	{XMFLOAT3(-10.0f, -80.0f, 0.0f), 250.0f, 70.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-115.0f, 10.0f, 0.0f), 80.0f, 180.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL skellAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(65.0f, 0.0f, 0.0f), 50.0f, 110.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-65.0f, 10.0f, 0.0f), 50.0f, 110.0f},
	{XMFLOAT3(0.0f, -60.0f, 0.0f), 160.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-65.0f, 10.0f, 0.0f), 50.0f, 110.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
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

	EnemyConfig* enemyConfig = GetEnemyConfig(GetCurrentMap());
	// エネミー構造体の初期化
	g_EnemyCount = 0;
	for (int i = 0; i < MAP_ENEMY_MAX; i++)
	{
		if (enemyConfig[i].moveTbl == nullptr) break;

		g_MoveTblAdr[i] = enemyConfig[i].moveTbl;

		g_Enemy[i].enemyType = enemyConfig[i].enemyType;
		g_Enemy[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = i;		// 再生するアニメデータの先頭アドレスをセット
		g_Enemy[i].tblMax = enemyConfig[i].moveTblSize;	// 再生するアニメデータのレコード数をセット
		g_Enemy[i].pos = enemyConfig[i].moveTbl[0].pos;
		g_Enemy[i].returnPos = g_Enemy[i].pos;

		SetupEnemyAttributes(&g_Enemy[i]);

		g_EnemyCount++;
		g_Enemy[i].use = TRUE;
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].idleCount = 0;
		g_Enemy[i].state = ENEMY_IDLE;
		g_Enemy[i].dir = CHAR_DIR_RIGHT;
		g_Enemy[i].oldDir = CHAR_DIR_RIGHT;
		g_Enemy[i].stepBack = FALSE;
		g_Enemy[i].onAirCnt = 0;
		g_Enemy[i].isFalling = FALSE;

		// battle
		g_Enemy[i].isHit = FALSE;
		g_Enemy[i].hitTimer = 0.0f;
		g_Enemy[i].hitCD = 0.0f;
		g_Enemy[i].diePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].dieInitSpeedX = 0.0f;
		g_Enemy[i].dieInitSpeedY = 0.0f;

		// AABB
		g_Enemy[i].bodyAABB.pos = g_Enemy[i].pos;
		g_Enemy[i].bodyAABB.w = g_Enemy[i].w * 0.5f;
		g_Enemy[i].bodyAABB.h = g_Enemy[i].h * 0.7f;
		g_Enemy[i].bodyAABB.tag = ENEMY_BODY_AABB;

		for (int j = 0; j < MAX_ATTACK_AABB; j++)
		{
			g_Enemy[i].attackAABB[j].pos = g_Enemy[i].pos;
			g_Enemy[i].attackAABB[j].w = 0;
			g_Enemy[i].attackAABB[j].h = 0;
			g_Enemy[i].attackAABB[j].tag = ENEMY_ATTACK_AABB;
		}

	}

	for (int i = 0; i < LIGHTPOINT_MAX; i++)
	{
		g_LightPoint[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_LightPoint[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_LightPoint[i].w = TEXTURE_WIDTH_LIGHTPOINT;
		g_LightPoint[i].h = TEXTURE_HEIGHT_LIGHTPOINT;
		g_LightPoint[i].timeToPlayer = 0.0f;
		g_LightPoint[i].use = FALSE;
		g_LightPoint[i].flyingToPlayer = FALSE;
		g_LightPoint[i].distanceToPlayer = 0.0f;
		g_LightPoint[i].timeToFly = 0.0f;
		g_LightPoint[i].desiredToPlayerTime = LIGHTPOINT_REACH_PLAYER_TIME;
	}


	//// 1番だけ線形補間で動かしてみる
	//g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	//g_Enemy[1].pos = g_MoveTbl1[0].pos;

	//// 2番だけ線形補間で動かしてみる
	//g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

#ifdef _DEBUG	
	{
		int aabbCount = ENEMY_MAX;
		int maxVertices = ENEMY_MAX * 4;
		int maxAttackVertices = maxVertices * MAX_ATTACK_AABB;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * maxVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_bodyAABBVertexBuffer);

		bd.ByteWidth = sizeof(VERTEX_3D) * maxAttackVertices;
		GetDevice()->CreateBuffer(&bd, NULL, &g_attackAABBVertexBuffer);
	}
#endif

	g_Load = TRUE;
	return S_OK;
}

void SetupEnemyAttributes(ENEMY* enemy)
{
	enemy->attributes.hp = attributesTbl[enemy->enemyType]->hp;
	enemy->attributes.maxHp = attributesTbl[enemy->enemyType]->maxHp;
	enemy->attributes.damage = attributesTbl[enemy->enemyType]->damage;
	enemy->attributes.staggerResistance = attributesTbl[enemy->enemyType]->staggerResistance;
	enemy->attributes.staggerRecoveryTime = attributesTbl[enemy->enemyType]->staggerRecoveryTime;
	enemy->attributes.stunTime = attributesTbl[enemy->enemyType]->stunTime;
	enemy->attributes.attackRange = attributesTbl[enemy->enemyType]->attackRange;
	enemy->attributes.attackCooldown = attributesTbl[enemy->enemyType]->attackCooldown;
	enemy->attributes.canFly = attributesTbl[enemy->enemyType]->canFly;
	enemy->attributes.move = attributesTbl[enemy->enemyType]->move;

	enemy->w = GetCurrentTextureSizeW(enemy->enemyType);
	enemy->h = GetCurrentTextureSizeH(enemy->enemyType);
}

const EnemyAttributes* GetEnemyAttributes(ENEMY* enemy)
{
	return attributesTbl[enemy->enemyType];
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
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

	for (int i = 0; i < MAP_ENEMY_MAX; i++)
	{
		ClearEnemy(&g_Enemy[i]);
		g_MoveTblAdr[i] = nullptr;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	if (g_Load == FALSE) return;
	g_EnemyCount = 0;			// 生きてるエネミーの数

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// 生きてるエネミーだけ処理をする
		if (g_Enemy[i].use == TRUE)
		{
			g_EnemyCount++;		// 生きてた敵の数
			
			// 地形との当たり判定用に座標のバックアップを取っておく
			XMFLOAT3 pos_old = g_Enemy[i].pos;

			// アニメーション
			switch (g_Enemy[i].state)
			{
			case ENEMY_IDLE:
			case ENEMY_COOLDOWN:
				PlayEnemyIdleAnim(&g_Enemy[i]);
				break;
			case ENEMY_WALK:
			case ENEMY_CHASE:
			case ENEMY_WALK_BACK:
			case ENEMY_RETREAT:
				PlayEnemyWalkAnim(&g_Enemy[i]);
				break;
			case ENEMY_ATTACK:
				PlayEnemyAttackAnim(&g_Enemy[i]);
				break;
			case ENEMY_HIT:
				PlayEnemyHitAnim(&g_Enemy[i]);
				break;
			case ENEMY_DIE:
				PlayEnemyDieAnim(&g_Enemy[i]);
				break;
			default:
				break;
			}

			UpdateEnemyStates(&g_Enemy[i]);

			BOOL isMoveable = g_Enemy[i].state != ENEMY_HIT && g_Enemy[i].state != ENEMY_DIE && g_Enemy[i].onAirCnt < 5;
			// 移動処理
			if (g_Enemy[i].tblMax > 0 && isMoveable)
			{	
				PLAYER* player = GetPlayer();
				// 現在の座標を保存しておくための変数
				float oldPosX = g_Enemy[i].pos.x;

				// 敵とプレイヤーの距離を計算する
				XMVECTOR enemyPos = XMLoadFloat3(&g_Enemy[i].pos);
				XMVECTOR playerPos = XMLoadFloat3(&player->pos);
				XMVECTOR direction = XMVector3Normalize(playerPos - enemyPos);
				XMVECTOR returnPos = XMLoadFloat3(&g_Enemy[i].returnPos);
				if (g_Enemy[i].attributes.canFly == FALSE)
					direction = XMVectorSetY(direction, 0);  // Y方向の速度を0に設定
				float distanceToPlayer = XMVectorGetX(XMVector3Length(playerPos - enemyPos));
				float distanceToReturnPos = XMVectorGetX(XMVector3Length(returnPos - enemyPos));

				switch (g_Enemy[i].state)
				{
				case ENEMY_CHASE:
				{
					if (distanceToPlayer < g_Enemy[i].attributes.attackRange && player->state != DIE)
					{
						g_Enemy[i].stateOld = g_Enemy[i].state;
						g_Enemy[i].state = ENEMY_ATTACK;
						g_Enemy[i].dir = g_Enemy[i].pos.x - player->pos.x > 0.0f ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
						g_Enemy[i].patternAnim = ANIM_ATTACK_OFFSET;
						g_Enemy[i].countAnim = 0.0f;
						g_Enemy[i].finishAttack = FALSE;
					}
					// 最大追跡距離を超えた場合、RETREAT状態に移行
					else if (distanceToReturnPos > MAX_CHASE_DISTANCE) 
					{
						g_Enemy[i].state = ENEMY_RETREAT;  // 倒退状態に移行
						g_Enemy[i].patternAnim = ANIM_WALK_PATTERN_NUM - 1;
					}
					else if (distanceToPlayer > ENEMY_VISION_RADIUS)
					{
						// プレイヤーが視野範囲外に出た場合、元の位置に戻るためWALK_BACK状態に移行
						g_Enemy[i].state = ENEMY_WALK_BACK;
					}
					else if (player->state != DIE)
					{
						// 追跡を行う
						g_Enemy[i].stepBack = FALSE;
						float speed = g_Enemy[i].attributes.move.x;
						XMVECTOR moveStep = direction * speed;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);
						
					}
					else
						g_Enemy[i].state = ENEMY_WALK_BACK;
					break;
				}

				case ENEMY_RETREAT:
				{
					// プレイヤーが視野範囲外に出た場合、WALK_BACK状態に遷移
					if (distanceToPlayer > ENEMY_VISION_RADIUS) 
					{
						g_Enemy[i].state = ENEMY_WALK_BACK;  // プレイヤーが視野範囲外なら元のパスに戻る
					}
					// プレイヤーが最大追跡距離内に戻った場合、CHASE状態に遷移
					else if (player->state != DIE && CheckChasingPlayer(&g_Enemy[i]))
					{
						g_Enemy[i].state = ENEMY_CHASE;  // 最大追跡距離内にプレイヤーがいる場合、再び追跡する
					}
					else
					{
						// 倒退方向（プレイヤーとは逆）
						XMVECTOR direction = XMVector3Normalize(enemyPos - playerPos);
						if (g_Enemy[i].attributes.canFly == FALSE)
							direction = XMVectorSetY(direction, 0);  // Y方向の速度を0に設定
						XMVECTOR moveStep = direction * g_Enemy[i].attributes.move.x * RETREAT_SPEED_RATE;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);

						// 倒退中でも向きはプレイヤーに向ける
						g_Enemy[i].stepBack = TRUE;

						// MIN_RETURN_DISTANCEまで倒退したら、WALK_BACK状態に戻る
						if (distanceToReturnPos <= MIN_RETURN_DISTANCE)
						{
							g_Enemy[i].state = ENEMY_WALK_BACK;  
						}
					}

					break;
				}
				case ENEMY_COOLDOWN:
				{
					if (distanceToPlayer > ENEMY_VISION_RADIUS) 
					{
						// プレイヤーが視野範囲外に出た場合、元の位置に戻るためWALK_BACK状態に移行
						g_Enemy[i].state = ENEMY_WALK_BACK;
					}
					else if (distanceToReturnPos > MAX_CHASE_DISTANCE)	// 最大追跡距離を超えた場合
					{
						g_Enemy[i].state = ENEMY_RETREAT;  // 倒退状態に移行
						g_Enemy[i].patternAnim = ANIM_WALK_PATTERN_NUM - 1;
					}
					else if (distanceToPlayer > g_Enemy[i].attributes.attackRange 
						&& distanceToReturnPos < MAX_CHASE_DISTANCE
						&& player->state != DIE)
					{
						// 攻撃範囲外なら追跡を続ける
						g_Enemy[i].stepBack = FALSE;
						float speed = g_Enemy[i].attributes.move.x;  // 追跡速度

						XMVECTOR moveStep = direction * speed * 0.5f;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);
					}
					
					// 攻撃範囲内にいる場合は停止し、クールダウンを待つ
					// クールダウンロジック
					g_Enemy[i].attributes.attackCooldown--;
					if (g_Enemy[i].attributes.attackCooldown <= 0)
					{
						// クールダウン終了後、再び敵の行動を決定
						if (distanceToPlayer < g_Enemy[i].attributes.attackRange && player->state != DIE)
						{
							g_Enemy[i].stateOld = g_Enemy[i].state;
							g_Enemy[i].state = ENEMY_ATTACK;  // プレイヤーが攻撃範囲内にいる場合
							g_Enemy[i].dir = g_Enemy[i].pos.x - player->pos.x > 0.0f ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
							g_Enemy[i].patternAnim = ANIM_ATTACK_OFFSET;
							g_Enemy[i].countAnim = 0.0f;
							g_Enemy[i].finishAttack = FALSE;
						}
						else if (CheckChasingPlayer(&g_Enemy[i]) && player->state != DIE)
						{
							g_Enemy[i].state = ENEMY_CHASE;   // プレイヤーが視野内にいる場合
						}
						else
						{
							g_Enemy[i].state = ENEMY_IDLE;    // 視野外にいる場合は待機状態
						}
					}
					break;
				}

				// 線形補間を実行する
				case ENEMY_IDLE:
				case ENEMY_WALK:
				{
					// プレイヤーが視野内にいて、敵がプレイヤーを向いている場合にCHASE状態に遷移
					if (player->state != DIE 
						&& distanceToPlayer < ENEMY_VISION_RADIUS 
						&& CheckChasingPlayer(&g_Enemy[i]))
					{
						g_Enemy[i].state = ENEMY_CHASE;
						g_Enemy[i].returnPos = g_Enemy[i].pos;  // 行動パスから離れる前の位置を記録
					}
					else
					{
						g_Enemy[i].stepBack = FALSE;

						// 線形補間の処理
						int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
						int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
						int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[i];	// 行動テーブルのアドレスを取得
						//if (nowNo != 0)

						if (g_Enemy[i].state == ENEMY_IDLE)
						{
							g_Enemy[i].idleCount++;
							if (g_Enemy[i].idleCount == IDLE_WAIT)
							{
								g_Enemy[i].idleCount = 0;
								g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
								g_Enemy[i].state = ENEMY_WALK;
							}
						}
						else
						{
							
							XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
							XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
							XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

							XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
							XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
							XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

							float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している

							Pos *= nowTime;								// 現在の移動量を計算している
							Rot *= nowTime;								// 現在の回転量を計算している
							Scl *= nowTime;								// 現在の拡大率を計算している


							enemyPos = nowPos + Pos;
							XMFLOAT3 newPos;
							XMStoreFloat3(&newPos, enemyPos);

							// 敵が右に移動しているか、左に移動しているかを判断	
							g_Enemy[i].dir = XMVectorGetX(Pos) < 0 ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;

							if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							{
								// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
								SET_ENEMY_POS((&g_Enemy[i]), newPos);

								// 計算して求めた回転量を現在の移動テーブルに足している
								XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

								// 計算して求めた拡大率を現在の移動テーブルに足している
								XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
								g_Enemy[i].w = GetCurrentTextureSizeW(g_Enemy[i].enemyType) * g_Enemy[i].scl.x;
								g_Enemy[i].h = GetCurrentTextureSizeH(g_Enemy[i].enemyType) * g_Enemy[i].scl.y;

								// frameを使て時間経過処理をする
								g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
								if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
								{
									g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
								}
							}
								

							if (fabs(g_Enemy[i].time - nowNo) < EPSILON && g_Enemy[i].state == ENEMY_WALK)
							{
								g_Enemy[i].state = ENEMY_IDLE;
							}
						}
					}
					break;
				}

				case ENEMY_WALK_BACK:
				{	
					// プレイヤーが視野内にいるかをチェック
					if (player->state != DIE 
						&& distanceToPlayer < ENEMY_VISION_RADIUS 
						&& CheckChasingPlayer(&g_Enemy[i]))
					{
						// プレイヤーが再び視野内に入った場合、CHASE状態に遷移
						g_Enemy[i].state = ENEMY_CHASE;
					}
					else
					{
						// 元の行動パスに戻るために、記録した位置に移動する
						g_Enemy[i].stepBack = FALSE;
						XMVECTOR enemyPos = XMLoadFloat3(&g_Enemy[i].pos);
						XMVECTOR returnPos = XMLoadFloat3(&g_Enemy[i].returnPos);
						XMVECTOR direction = XMVector3Normalize(returnPos - enemyPos);
						if (g_Enemy[i].attributes.canFly == FALSE)
							direction = XMVectorSetY(direction, 0);  // Y方向の速度を0に設定
						float speed = g_Enemy[i].attributes.move.x;  // 歩いて戻る速度
						XMVECTOR moveStep = direction * speed;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);

						XMVECTOR delta = returnPos - enemyPos;
						float deltaX = fabs(XMVectorGetX(delta));
						float deltaY = fabs(XMVectorGetY(delta));

						// 元の位置に近づいたかを確認する
						if (deltaX <= g_Enemy[i].attributes.move.x && deltaY <= g_Enemy[i].attributes.move.y)
						{
							XMFLOAT3 newPos;
							XMStoreFloat3(&newPos, enemyPos);
							if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
								SET_ENEMY_POS((&g_Enemy[i]), newPos);
							// 元の位置に戻ったら、元の行動パスに戻る
							g_Enemy[i].state = ENEMY_WALK;  // 移動パスに戻る
						}
					}
					break;
				}

				default:
					break;
				}

				// 敵が右に移動しているか、左に移動しているかを判断	
				if (oldPosX < g_Enemy[i].pos.x)
					g_Enemy[i].dir = g_Enemy[i].stepBack == TRUE ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
				else if (oldPosX > g_Enemy[i].pos.x)
					g_Enemy[i].dir = g_Enemy[i].stepBack == TRUE ? CHAR_DIR_RIGHT : CHAR_DIR_LEFT;

				g_Enemy[i].invertTex = g_Enemy[i].dir == CHAR_DIR_RIGHT ? FALSE : TRUE;
			}

			UpdateEnemyGroundCollision(&g_Enemy[i]);

			// 移動が終わったらエネミーとの当たり判定
			PLAYER* player = GetPlayer();

			if (g_Enemy[i].hitCD <= 0.0f && g_Enemy[i].state != ENEMY_DIE)
			{
				// 攻撃用の包囲ボックスを取得
				for (int j = 0; j < MAX_ATTACK_AABB; j++)
				{
					AABB attackBox = player->attackAABB[j];

					// 敵のAABB情報を取得
					XMFLOAT3 enemyPos = g_Enemy[i].bodyAABB.pos;
					float enemyW = g_Enemy[i].bodyAABB.w;
					float enemyH = g_Enemy[i].bodyAABB.h;

					// 攻撃のAABB情報を取得
					XMFLOAT3 attackPos = attackBox.pos;
					float attackW = attackBox.w;
					float attackH = attackBox.h;

					// エネミーの包囲ボックスとプレイヤーの攻撃範囲が重なっているかを確認
					BOOL isColliding = CollisionBB(enemyPos, enemyW, enemyH, attackPos, attackW, attackH);

					if (isColliding)
					{
						// 当たり判定があった場合、敵にダメージを与える
						EnemyTakeDamage(&g_Enemy[i]);
						break;
					}
				}

				// 魔法の包囲ボックスを取得
				Magic* magic = GetMagic();
				for (int j = 0; j < MAX_ATTACK_AABB; j++)
				{
					if (magic[j].use == FALSE) continue;

					if (magic[j].magicAABB.tag == PLAYER_MAGIC_AABB)
					{
						AABB attackBox = magic[j].magicAABB;

						// 敵のAABB情報を取得
						XMFLOAT3 enemyPos = g_Enemy[i].bodyAABB.pos;
						float enemyW = g_Enemy[i].bodyAABB.w;
						float enemyH = g_Enemy[i].bodyAABB.h;

						// 魔法のAABB情報を取得
						XMFLOAT3 attackPos = attackBox.pos;
						float attackW = attackBox.w;
						float attackH = attackBox.h;

						// エネミーの包囲ボックスとプレイヤーの攻撃範囲が重なっているかを確認
						BOOL isColliding = CollisionBB(enemyPos, enemyW, enemyH, attackPos, attackW, attackH);

						if (isColliding)
						{
							// 当たり判定があった場合、敵にダメージを与える
							EnemyTakeDamage(&g_Enemy[i], &magic[j]);
							break;
						}
					}
				}
			}
		}
	}

	UpdateLightPoint();

	//// エネミー全滅チェック
	//if (g_EnemyCount <= 0)
	//{
 //		SetFade(FADE_OUT, MODE_RESULT);
	//}

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

void UpdateEnemyStates(ENEMY* enemy)
{
	// 敵が攻撃状態ではなく、かつ攻撃が終了していない場合
	if (enemy->state != ENEMY_ATTACK && enemy->finishAttack == FALSE)
	{
		// 攻撃判定のAABBをリセット
		for (int j = 0; j < MAX_ATTACK_AABB; j++)
		{
			enemy->attackAABB[j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			enemy->attackAABB[j].w = 0.0f;
			enemy->attackAABB[j].h = 0.0f;
		}
		enemy->finishAttack = TRUE;
	}

	if (enemy->hitCD > 0.0f) 
	{
		enemy->hitCD--;
	}
	// 敵が被弾した場合
	if (enemy->isHit)
	{
		enemy->hitTimer--;
		if (enemy->hitTimer <= 0.0f)
		{
			enemy->isHit = false;
			enemy->hitTimer = 0.0f;
		}
	}

	enemy->attributes.staggerRecoveryTime--;
	if (enemy->attributes.staggerRecoveryTime <= 0)
		enemy->attributes.staggerResistance = GetEnemyAttributes(enemy)->staggerResistance;

	if (enemy->attributes.attackCooldown > 0 && enemy->state != ENEMY_COOLDOWN && enemy->state != ENEMY_HIT && enemy->state != ENEMY_DIE)
		enemy->state = ENEMY_COOLDOWN;
}

void UpdateEnemyGroundCollision(ENEMY* enemy)
{
	if (enemy->attributes.canFly == TRUE) return;

	AABB* grounds = GetMap01AABB();

	if (enemy->onAirCnt >= ENEMY_FALL_CNT_MAX)
		enemy->attributes.move.y = ENEMY_FALL_SPEED;
	else
	{
		// sin関数を使用して下落速度を計算
		float angle = (XM_PI / ENEMY_FALL_CNT_MAX) * enemy->onAirCnt;
		enemy->attributes.move.y = ENEMY_FALL_SPEED * sinf(angle);
	}
	if (enemy->attributes.move.y < 2.0f)
		enemy->attributes.move.y = 2.0f;
	// 滞空時間を増加させる
	enemy->onAirCnt++;

	XMVECTOR enemyPos = XMLoadFloat3(&enemy->pos);
	XMVECTOR direction = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	enemyPos += direction * enemy->attributes.move.y;

	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, enemyPos);
	if (CheckEnemyMoveCollision(enemy, newPos, CHAR_DIR_DOWN))
	{
		SET_ENEMY_POS(enemy, newPos);
		enemy->isFalling = TRUE;
		enemy->dir = CHAR_DIR_DOWN;
	}
	else
	{
		enemy->isFalling = FALSE;
		enemy->onAirCnt = 0;
	}
		

}

void UpdateEnemyMoveTbl(ENEMY* enemy, float newPosY)
{
	int size = enemy->tblMax;

	INTERPOLATION_DATA* moveTbl = g_MoveTblAdr[enemy->tblNo];

	for (int i = 0; i < size; ++i) 
	{
		moveTbl[i].pos.y = newPosY;
	}
}

void EnemyTakeDamage(ENEMY* enemy, Magic* magic)
{
	PLAYER* player = GetPlayer();
	enemy->dir = enemy->pos.x - player->pos.x > 0 ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
	enemy->isHit = true;
	enemy->hitTimer = ENEMY_HIT_TIMER;
	enemy->hitCD = ENEMY_HIT_CD;
	enemy->attributes.staggerRecoveryTime = GetEnemyAttributes(enemy)->staggerRecoveryTime;

	if (magic == nullptr)
	{
		if (player->attackPattern == PARRY)
			enemy->hitCD = ENEMY_HIT_CD * 0.5f;


		enemy->attributes.staggerResistance -= GetPoiseDamage();
		enemy->attributes.hp -= GetPlayerDamage();
	}
	else
	{
		switch (magic->magicType)
		{
		case MAGIC_FIRE_BALL:
			enemy->attributes.staggerResistance -= 20;
			enemy->attributes.hp -= 15;
			break;
		default:
			return;
		}
	}

	if (enemy->attributes.staggerResistance <= 0)
	{
		if (enemy->state != ENEMY_ATTACK)
			enemy->stateOld = enemy->state;
		enemy->attributes.attackCooldown = GetEnemyAttributes(enemy)->attackCooldown + GetRand(10, 60);  // クールダウンタイムをリセット
		enemy->attributes.staggerResistance = GetEnemyAttributes(enemy)->staggerResistance; // 強靭さをリセット
		enemy->state = ENEMY_HIT;
		enemy->countAnim = 0.0f;
	}

	if (enemy->attributes.hp <= 0)
	{
		enemy->state = ENEMY_DIE;
		enemy->countAnim = 0;
		enemy->diePos = enemy->pos;
		enemy->pos.y -= 70.0f;
		enemy->dieInitSpeedX = 0.2f * (float)GetRand(-10, 10);
		enemy->dieInitSpeedY = 0.1f * (float)GetRand(1, 5);
		EnemyDieOnTrigger(enemy);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
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
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
		{									// Yes

			// 敵の影
			DrawEnemyShadow(&g_Enemy[i]);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].enemyType]);

			DrawEnemySprite(&g_Enemy[i]);
			if (g_Enemy[i].state == ENEMY_DIE)
			{
				g_Enemy[i].patternAnim = ANIM_DIE_OFFSET + 1;
				DrawEnemySprite(&g_Enemy[i], TRUE);
			}

			// 敵のHPゲージ
			DrawEnemyHPGauge(&g_Enemy[i]);

		}
	}

	DrawLightPoint();


#ifdef _DEBUG
	{
		MATERIAL materialAABB;
		ZeroMemory(&materialAABB, sizeof(materialAABB));
		materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materialAABB.noTexSampling = 1;

		SetMaterial(materialAABB);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_bodyAABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < ENEMY_MAX; ++i)
		{
			if (g_Enemy[i].use == FALSE) continue;

			int vertexOffset = i * 4;

			SetSpriteColorRotation(g_bodyAABBVertexBuffer, g_Enemy[i].bodyAABB.pos.x - bg->pos.x,
				g_Enemy[i].bodyAABB.pos.y - bg->pos.y, g_Enemy[i].bodyAABB.w, g_Enemy[i].bodyAABB.h,
				0.0f, 0.0f, 0.0f, 0.0f,
				XMFLOAT4(0.0f, 0.0f, 1.0f, 0.2f),
				0.0f,
				vertexOffset);

			GetDeviceContext()->Draw(4, vertexOffset);
		}

		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_attackAABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < ENEMY_MAX; ++i)
		{
			if (g_Enemy[i].use == FALSE) continue;

			int vertexOffset = i * 4;

			for (int j = 0; j < MAX_ATTACK_AABB; j++)
			{
				SetSpriteColorRotation(g_attackAABBVertexBuffer, g_Enemy[i].attackAABB[j].pos.x - bg->pos.x,
					g_Enemy[i].attackAABB[j].pos.y - bg->pos.y, g_Enemy[i].attackAABB[j].w, g_Enemy[i].attackAABB[j].h,
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

void DrawEnemySprite(const ENEMY* enemy, BOOL die)
{
	BG* bg = GetBG();

	float px, py, pw, ph;
	if (die)
	{
		px = enemy->diePos.x - bg->pos.x;	// エネミーの表示位置X
		py = enemy->diePos.y - bg->pos.y;	// エネミーの表示位置Y
		pw = enemy->w;		// エネミーの表示幅
		ph = enemy->h;		// エネミーの表示高さ
	}
	else
	{
		px = enemy->pos.x - bg->pos.x;	// エネミーの表示位置X
		py = enemy->pos.y - bg->pos.y;	// エネミーの表示位置Y
		pw = enemy->w;		// エネミーの表示幅
		ph = enemy->h;		// エネミーの表示高さ
	}


	// アニメーション用
	float tw, th, tx, ty;
	tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X; // テクスチャの幅
	th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
	ty = (float)(enemy->patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標
	if (enemy->invertTex)
	{
		tw *= -1.0f;
		tx = (float)(enemy->patternAnim % TEXTURE_PATTERN_DIVIDE_X) * (-1 * tw) - tw;	// テクスチャの左上X座標
	}
	else
		tx = (float)(enemy->patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	if (enemy->isHit)
	{
		color = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
		//if (fmod(enemy->hitTimer, 3.0f) < 1.5f) 
		//{
		//	color.w = 0.0f;
		//}
	}
	if (enemy->state == ENEMY_DIE)
	{
		color.w = 1.0f - enemy->countAnim / ANIM_WAIT_DIE;
	}

	float rotation;
	if (die)
		rotation = 0.0f;
	else
		rotation = enemy->rot.z;
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		color,
		rotation);

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

void DrawEnemyHPGauge(const ENEMY* enemy)
{
	BG* bg = GetBG();

	float posX, posY;
	if (enemy->state == ENEMY_DIE)
	{
		posX = enemy->diePos.x;
		posY = enemy->diePos.y;
	}
	else
	{
		posX = enemy->bodyAABB.pos.x;
		posY = enemy->bodyAABB.pos.y;
	}
	// HPバーの位置を計算
	float px = posX - enemy->bodyAABB.w * 0.5f - bg->pos.x;  // HPバーのX座標（敵の中央に表示）
	float py = posY - enemy->bodyAABB.h * 0.7f - bg->pos.y;  // HPバーのY座標
	float pw = enemy->bodyAABB.w;   // HPバーの幅
	float ph = 10.0f;    // HPバーの高さ

	float tw = 1.0f;     // テクスチャの幅
	float th = 1.0f;     // テクスチャの高さ
	float tx = 0.0f;     // テクスチャのX座標
	float ty = 0.0f;     // テクスチャのY座標

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);  // 背景を白色で表示
	if (enemy->state == ENEMY_DIE)
	{
		color.w = 1.0f - enemy->countAnim / ANIM_WAIT_DIE;
	}

	// 1. HPバーの背景を描画（敵の総HPを表示、_bgテクスチャを使用）
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ENEMY_HP_GAUGE_TEXTURE + 1]);
	SetSpriteLTColor(g_VertexBuffer,
		px, py, pw, ph,
		tx, ty, tw, th,
		color);
	GetDeviceContext()->Draw(4, 0);


	// 2. HPバーの前景を描画（敵の現在のHPを表示、ENEMY_HP_GAUGE_TEXTUREテクスチャを使用）
	float hpRatio = (float)enemy->attributes.hp / (float)enemy->attributes.maxHp;  // HP比率を計算
	float hpWidth = pw * hpRatio;  // HP比率に基づいてHPバーの長さを計算

	// 前景の色を設定（通常は緑色、現在のHPを表す）
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ENEMY_HP_GAUGE_TEXTURE]);
	SetSpriteLTColor(g_VertexBuffer,
		px, py, hpWidth, ph,         // 前景の幅をHPに基づいて縮小
		tx, ty, tw * hpRatio, th,    // HP比率に応じてテクスチャの幅を縮小
		color);
	GetDeviceContext()->Draw(4, 0);
}

void DrawEnemyShadow(const ENEMY* enemy)
{
	BG* bg = GetBG();

	// 影表示
	SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ENEMY_SHADOW_TEXTURE]);

	float px = enemy->pos.x - bg->pos.x;	// プレイヤーの表示位置X
	float py = enemy->pos.y - bg->pos.y;	// プレイヤーの表示位置Y
	float pw = enemy->bodyAABB.w;		// プレイヤーの表示幅
	float ph = enemy->bodyAABB.w / 3;		// プレイヤーの表示高さ

	// 地面との衝突判定に基づく影の位置
	// プレイヤーの位置に対応する地面の高さを計算
	float shadowPosY = py;  // 影の最終的なY座標
	float highestGroundY = FLT_MAX;  // 最も高い地面のY座標を保持

	// 地面のAABB情報を取得
	AABB* ground = GetMap01AABB();
	BOOL isOnGround = false;

	// プレイヤーが地面にいるかどうかを確認
	for (int j = 0; j < MAP_GROUND_MAX; j++)
	{
		float groundX = ground[j].pos.x;
		float groundY = ground[j].pos.y;
		float groundW = ground[j].w;
		float groundH = ground[j].h;

		if (groundY < enemy->pos.y) continue;

		// プレイヤーのx座標が地面の幅の範囲に入っているかどうかを確認
		if (enemy->pos.x >= groundX - groundW / 2 && enemy->pos.x <= groundX + groundW / 2)
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

void DrawLightPoint(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < LIGHTPOINT_MAX; i++)
	{
		if (g_LightPoint[i].use == TRUE)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[LIGHTPOINT_TEXTURE]);

			float px = g_LightPoint[i].pos.x - bg->pos.x;
			float py = g_LightPoint[i].pos.y - bg->pos.y;
			float pw = g_LightPoint[i].w;
			float ph = g_LightPoint[i].h;

			float tw = 1.0f;     // テクスチャの幅
			float th = 1.0f;     // テクスチャの高さ
			float tx = 0.0f;     // テクスチャのX座標
			float ty = 0.0f;     // テクスチャのY座標

			XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				color);
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

BOOL CheckEnemyMoveCollision(ENEMY* enemy, XMFLOAT3 newPos, int dir)
{
	// 壁のAABB情報を取得
	AABB* walls = GetMap01AABB();
	for (int i = 0; i < MAP_GROUND_MAX; i++)
	{
		XMFLOAT3 wallPos = walls[i].pos;
		float wallW = walls[i].w;
		float wallH = walls[i].h;

		// 衝突確認
		switch (dir)
		{
		case CHAR_DIR_LEFT:
		{

			if (CollisionBB(newPos, enemy->bodyAABB.w, enemy->bodyAABB.h, wallPos, wallW, wallH) &&
				enemy->bodyAABB.pos.x > wallPos.x)
			{
				// 左に移動していて、壁が左側にある場合
				//XMFLOAT3 enemyPos = enemy->pos;
				//enemyPos.x = wallPos.x + wallW / 2 + enemy->bodyAABB.w / 2 + 0.01f;
				//SET_ENEMY_POS(enemy, enemyPos); // 左への進行を停止
				return false;
			}
			break;
		}

		case CHAR_DIR_RIGHT:
		{
			if (CollisionBB(newPos, enemy->bodyAABB.w, enemy->bodyAABB.h, wallPos, wallW, wallH) &&
				enemy->bodyAABB.pos.x < wallPos.x)
			{
				// 右に移動していて、壁が右側にある場合
				//XMFLOAT3 enemyPos = enemy->pos;
				//enemyPos.x = wallPos.x - wallW / 2 - enemy->bodyAABB.w / 2 - 0.01f;
				//SET_ENEMY_POS(enemy, enemyPos); // 右への進行を停止
				return false;
			}
			break;
		}
		case CHAR_DIR_DOWN:
		{
			if (CollisionBB(newPos, enemy->bodyAABB.w, enemy->bodyAABB.h, wallPos, wallW, wallH))
			{
				// エネミーが上に移動していて、壁が下側にある場合
				XMFLOAT3 enemyPos = enemy->pos;
				enemyPos.y = wallPos.y - wallH / 2 - enemy->bodyAABB.h / 2 - 0.01f;
				SET_ENEMY_POS(enemy, enemyPos); // 下への進行を停止
				UpdateEnemyMoveTbl(enemy, enemyPos.y);
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

//=============================================================================
// Enemy構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}


// 生きてるエネミーの数
int GetEnemyCount(void)
{
	return g_EnemyCount;
}


void PlayEnemyWalkAnim(ENEMY* enemy)
{
	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	int animWait = g_Enemy->stepBack == TRUE ? ANIM_WAIT_WALK * 2 : ANIM_WAIT_WALK;
	enemy->countAnim++;
	if (enemy->countAnim > animWait)
	{
		enemy->countAnim = 0.0f;
		// パターンの切り替え
		if (g_Enemy->state == ENEMY_RETREAT)
		{
			// 後退状態ではアニメーションを逆順に再生する
			if (enemy->patternAnim == 0) 
			{
				enemy->patternAnim = ANIM_WALK_PATTERN_NUM - 1;  // 最初のフレームであれば最後のフレームに戻る
			}
			else 
			{
				enemy->patternAnim--;  // フレームを逆方向に進める
			}
		}
		else
		{
			enemy->patternAnim = (enemy->patternAnim + 1) % ANIM_WALK_PATTERN_NUM;
		}

	}
}

void PlayEnemyAttackAnim(ENEMY* enemy)
{
	if (enemy->patternAnim == ANIM_ATTACK_OFFSET && GetTutorialPause() == PAUSE_ENEMY)
	{
		g_Update = FALSE;
	}

	PLAYER* player = GetPlayer();

	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	enemy->countAnim++;
	int attackInterval = enemy->patternAnim == ANIM_ATTACK_OFFSET ? ANIM_WAIT_ATTACK * 2 : ANIM_WAIT_ATTACK;
	if (enemy->countAnim > attackInterval)
	{
		enemy->countAnim = 0.0f;
		// パターンの切り替え
		enemy->patternAnim = (enemy->patternAnim + 1) % ANIM_ATTACK_PATTERN_NUM + ANIM_ATTACK_OFFSET;
	}

	// 攻撃判定の更新
	UpdateEnemyAttackAABB(enemy);

	// アニメーションが終了したかをチェック
	if (enemy->patternAnim == ANIM_ATTACK_PATTERN_NUM + ANIM_ATTACK_OFFSET - 1 && enemy->countAnim == ANIM_WAIT_ATTACK)
	{
		// アニメーション終了後に攻撃を実行
		enemy->attributes.attackCooldown = GetEnemyAttributes(enemy)->attackCooldown + GetRand(10, 60);  // クールダウンタイムをリセット
		enemy->state = ENEMY_COOLDOWN;  // クールダウン状態に移行

		for (int i = 0; i < MAX_ATTACK_AABB; i++)
		{
			enemy->attackAABB[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			enemy->attackAABB[i].h = 0;
			enemy->attackAABB[i].w = 0;
		}
	}


	HandleEnemyMagic(enemy);
}

void PlayEnemyHitAnim(ENEMY* enemy)
{
	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;
	int dir = enemy->dir == CHAR_DIR_RIGHT ? -1 : 1;
	enemy->patternAnim = ANIM_HIT_OFFSET;
	float rotationAngle = dir * 0.25f * enemy->countAnim / GetEnemyAttributes(enemy)->stunTime;
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, rotationAngle);
	XMStoreFloat3(&enemy->rot, XMLoadFloat3(&rotation));


	enemy->countAnim++;
	if (enemy->countAnim > GetEnemyAttributes(enemy)->stunTime)
	{
		enemy->countAnim = 0.0f;
		XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMStoreFloat3(&enemy->rot, XMLoadFloat3(&rotation));
		enemy->patternAnim = 0;
		enemy->state = enemy->stateOld;
	}
}

void PlayEnemyIdleAnim(ENEMY* enemy)
{
	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	enemy->countAnim++;
	if (enemy->countAnim > ANIM_WAIT_IDLE)
	{
		enemy->countAnim = 0.0f;
		// パターンの切り替え
		enemy->patternAnim = (enemy->patternAnim == 0) ? 2 : 0;
	}
}

void PlayEnemyDieAnim(ENEMY* enemy)
{
	enemy->patternAnim = ANIM_DIE_OFFSET;

	float gravity = 0.1f;
	float t = (float)enemy->countAnim * 0.05f;
	float airResistance = 0.98f;
	float currentSpeedX = enemy->dieInitSpeedX * pow(airResistance, t);

	enemy->pos.x += currentSpeedX;
	enemy->pos.y += enemy->dieInitSpeedY * t + 0.5f * gravity * t * t;

	float relativeX = enemy->pos.x - enemy->diePos.x;
	float rotationAngle = relativeX * 0.05f;
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, rotationAngle);
	XMStoreFloat3(&enemy->rot, XMLoadFloat3(&rotation));

	enemy->countAnim++;
	if (enemy->countAnim > ANIM_WAIT_DIE)
	{
		enemy->use = FALSE;
	}
}

float GetCurrentTextureSizeW(int enemyType)
{
	switch (enemyType)
	{
	case CYCLOPS:
		return TEXTURE_WIDTH_CYCLOPS;
	case GARGOYLE:
		return TEXTURE_WIDTH_GARGOYLE;
	case GNOLL:
		return TEXTURE_WIDTH_GNOLL;
	case GOBLIN:
		return TEXTURE_WIDTH_GOBLIN;
	case GOLEM:
		return TEXTURE_WIDTH_GOLEM;
	case IMP:
		return TEXTURE_WIDTH_IMP;
	case MUMMY:
		return TEXTURE_WIDTH_MUMMY;
	case OGRE:
		return TEXTURE_WIDTH_OGRE;
	case SKELL:
		return TEXTURE_WIDTH_SKELL;
	default:
		return -1;
	}
}
float GetCurrentTextureSizeH(int enemyType)
{
	switch (enemyType)
	{
	case CYCLOPS:
		return TEXTURE_HEIGHT_CYCLOPS;
	case GARGOYLE:
		return TEXTURE_HEIGHT_GARGOYLE;
	case GNOLL:
		return TEXTURE_HEIGHT_GNOLL;
	case GOBLIN:
		return TEXTURE_HEIGHT_GOBLIN;
	case GOLEM:
		return TEXTURE_HEIGHT_GOLEM;
	case IMP:
		return TEXTURE_HEIGHT_IMP;
	case MUMMY:
		return TEXTURE_HEIGHT_MUMMY;
	case OGRE:
		return TEXTURE_HEIGHT_OGRE;
	case SKELL:
		return TEXTURE_HEIGHT_SKELL;
	default:
		return -1;
	}
}

void UpdateEnemyAttackAABB(ENEMY* enemy)
{
	AttackAABBTBL* attackTable = nullptr;

	switch (enemy->enemyType)
	{
	case CYCLOPS:
		attackTable = cyclopsAttackTbl;
		break;
	case GARGOYLE:
		attackTable = gargoyleAttackTbl;
		break;
	case GNOLL:
		attackTable = gnollAttackTbl;
		break;
	case GOBLIN:
		attackTable = goblinAttackTbl;
		break;
	case GOLEM:
		attackTable = golemAttackTbl;
		break;
	case IMP:
		attackTable = impAttackTbl;
		break;
	case MUMMY:
		attackTable = mummyAttackTbl;
		break;
	case OGRE:
		attackTable = ogreAttackTbl;
		break;
	case SKELL:
		attackTable = skellAttackTbl;
		break;
	default:
		return;
	}

	for (int i = 0; i < MAX_ATTACK_AABB; i++)
	{
		int tableIdx = (enemy->patternAnim - ANIM_ATTACK_OFFSET) * MAX_ATTACK_AABB + i;

		// 敵の向きを判定する: 左向きの場合はdirが1、右向きの場合はdirが-1
		int dir = enemy->dir == CHAR_DIR_LEFT ? 1 : -1;

		// 向きに応じてX方向のオフセットを調整
		XMFLOAT3 offset = attackTable[tableIdx].posOffset;
		offset.x *= dir;

		// attackAABBの位置を更新
		enemy->attackAABB[i].pos = XMFLOAT3_ADD(enemy->bodyAABB.pos, offset);
		enemy->attackAABB[i].h = attackTable[tableIdx].h;
		enemy->attackAABB[i].w = attackTable[tableIdx].w;
	}
}

BOOL CheckChasingPlayer(const ENEMY* enemy)
{
	PLAYER* player = GetPlayer();

	if (enemy->state == ENEMY_RETREAT || enemy->state == ENEMY_CHASE || enemy->state == ENEMY_WALK_BACK)
	{
		XMVECTOR returnPos = XMLoadFloat3(&enemy->returnPos);
		XMVECTOR playerPos = XMLoadFloat3(&player->pos);
		float distancePlayerToReturnPos = XMVectorGetX(XMVector3Length(returnPos - playerPos));
		if (distancePlayerToReturnPos > MAX_CHASE_DISTANCE)
			return FALSE;
	}

	if (player->state == RUN || player->state == DASH)
		return TRUE;

	BOOL isChasingPlayer = FALSE;
	if (enemy->dir == CHAR_DIR_RIGHT && player->pos.x > enemy->pos.x)
	{
		// 敵が右を向いていて、プレイヤーが右側にいる場合
		isChasingPlayer = true;
	}
	else if (enemy->dir == CHAR_DIR_LEFT && player->pos.x < enemy->pos.x)
	{
		// 敵が左を向いていて、プレイヤーが左側にいる場合
		isChasingPlayer = true;
	}
	return isChasingPlayer;
}

void EnemyDieOnTrigger(ENEMY* enemy)
{
	// 敵のサイズに基づいて光点の数を決定する
	int numLightPoints = (int)enemy->w * enemy->h * 0.00025f;

	// 光点の数が最大値を超えないように制限する
	if (numLightPoints > LIGHTPOINT_MAX)
		numLightPoints = LIGHTPOINT_MAX;

	for (int i = 0; i < LIGHTPOINT_MAX; ++i)
	{
		if (g_LightPoint[i].use == TRUE) continue;

		numLightPoints--;

		// 光点が発散する角度を調整し、上方向により多くの光点が向かうようにする
		int directionBias = GetRand(0, 100);  // 方向のバイアスを決定するランダム値
		float angle; 
		if (directionBias < 70) 
		{  // 70%の確率で上方向に発散
			angle = GetRand(180, 360) * PI / 180.0f;
		}
		else 
		{  // 30%の確率で下方向に発散
			angle = GetRand(0, 180) * PI / 180.0f;
		}
		float speed = GetRand(LIGHPOINT_SPEED_MIN, LIGHTPOINT_SPEED_MAX);
		XMFLOAT3 velocity(cosf(angle) * speed, sinf(angle) * speed, 0.0f);

		g_LightPoint[i].timeToFly = LIGHTPOINT_FLY_TIME;
		g_LightPoint[i].use = TRUE;
		g_LightPoint[i].timeToPlayer = LIGHTPOINT_STAY_TIME;
		g_LightPoint[i].move = velocity;
		g_LightPoint[i].pos = enemy->bodyAABB.pos;
		if (numLightPoints == 0)
			break;
	}
}

void UpdateLightPoint(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < LIGHTPOINT_MAX; i++)
	{
		if (g_LightPoint[i].use == FALSE) continue;

		// 光点がプレイヤーに向かって飛んでいる場合
		if (g_LightPoint[i].flyingToPlayer == TRUE)
		{
			XMVECTOR posVec = XMLoadFloat3(&g_LightPoint[i].pos);
			XMFLOAT3 targetPos = player->bodyAABB.pos;
			targetPos.x += LIGHTPOINT_TARGET_OFFSET_X;
			targetPos.y += LIGHTPOINT_TARGET_OFFSET_Y;
			XMVECTOR playerVec = XMLoadFloat3(&targetPos);
			XMVECTOR direction = XMVector3Normalize(XMVectorSubtract(playerVec, posVec));
			XMVECTOR distanceVec = XMVectorSubtract(playerVec, posVec);
			float distance = XMVectorGetX(XMVector3Length(distanceVec));

			// プレイヤーに到達するまでの時間を計算し、適切な速度を設定
			float desiredTime = g_LightPoint[i].desiredToPlayerTime;
			if (g_LightPoint[i].desiredToPlayerTime > 1.0f)
				g_LightPoint[i].desiredToPlayerTime--;
			float adjustedSpeed = distance / desiredTime;

			XMVECTOR velocityVec = XMVectorScale(direction, adjustedSpeed);
			XMStoreFloat3(&g_LightPoint[i].move, velocityVec);

			posVec = XMVectorAdd(posVec, XMVectorScale(velocityVec, 1.0f));
			XMStoreFloat3(&g_LightPoint[i].pos, posVec);
		}
		
		else if (g_LightPoint[i].timeToFly > 0.0f)
		{
			// 光点を四方に発散させる
			XMVECTOR posVec = XMLoadFloat3(&g_LightPoint[i].pos);
			XMVECTOR velocityVec = XMLoadFloat3(&g_LightPoint[i].move);

			posVec = XMVectorAdd(posVec, velocityVec);
			XMStoreFloat3(&g_LightPoint[i].pos, posVec);

			g_LightPoint[i].timeToFly--;

			XMVECTOR playerVec = XMLoadFloat3(&player->bodyAABB.pos);
			XMVECTOR distanceVec = XMVectorSubtract(posVec, playerVec);
			g_LightPoint[i].distanceToPlayer = XMVectorGetX(XMVector3Length(distanceVec));
		}
		// 光点がまだ静止している場合
		else if (g_LightPoint[i].timeToFly <= 0.0f)
		{
			g_LightPoint[i].timeToPlayer--;
			// プレイヤーに向かって飛ぶ
			if (g_LightPoint[i].timeToPlayer <= 0.0f)
				g_LightPoint[i].flyingToPlayer = TRUE;
		}

		// 光点がプレイヤーに到達したかどうかをチェック
		XMVECTOR posVec = XMLoadFloat3(&g_LightPoint[i].pos);
		XMFLOAT3 targetPos = player->bodyAABB.pos;
		targetPos.x += LIGHTPOINT_TARGET_OFFSET_X;
		targetPos.y += LIGHTPOINT_TARGET_OFFSET_Y;
		XMVECTOR playerVec = XMLoadFloat3(&targetPos);
		XMVECTOR distanceVec = XMVectorSubtract(posVec, playerVec);
		float distance = XMVectorGetX(XMVector3Length(distanceVec));
		if (distance < player->w * 0.3f)
		{
			g_LightPoint[i].use = FALSE;
			int score = GetRand(888, 999);
			AddScore(score);
		}
	}
}

void HandleEnemyMagic(ENEMY* enemy)
{
	switch (enemy->enemyType)
	{
	case GOLEM:
		if (enemy->patternAnim == ANIM_ATTACK_OFFSET + 1 && enemy->countAnim == 0)
			TriggerMagic(MAGIC_STOMP, enemy->pos, enemy);
		break;
	default:
		break;
	}
}

void ClearEnemy(ENEMY* enemy)
{
	enemy->enemyType = 0;
	enemy->time = 0.0f;		// 線形補間用のタイマーをクリア
	enemy->tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	enemy->tblMax = 0;
	enemy->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemy->returnPos = enemy->pos;

	enemy->attributes.hp = 0.0f;
	enemy->attributes.maxHp = 0.0f;
	enemy->attributes.damage = 0;
	enemy->attributes.staggerResistance = 0;
	enemy->attributes.staggerRecoveryTime = 0;
	enemy->attributes.stunTime = 0;
	enemy->attributes.attackRange = 0.0f;
	enemy->attributes.attackCooldown = 0;
	enemy->attributes.canFly = FALSE;
	enemy->attributes.move = XMFLOAT3(0.0f, 0.0f, 0.0f);

	enemy->w = 0.0f;
	enemy->h = 0.0f;

	enemy->use = FALSE;
	enemy->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemy->scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	enemy->countAnim = 0;
	enemy->patternAnim = 0;

	enemy->idleCount = 0;
	enemy->state = ENEMY_IDLE;
	enemy->dir = CHAR_DIR_RIGHT;
	enemy->oldDir = CHAR_DIR_RIGHT;
	enemy->stepBack = FALSE;
	enemy->onAirCnt = 0;
	enemy->isFalling = FALSE;

	// battle
	enemy->isHit = FALSE;
	enemy->hitTimer = 0.0f;
	enemy->hitCD = 0.0f;
	enemy->diePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemy->dieInitSpeedX = 0.0f;
	enemy->dieInitSpeedY = 0.0f;

	// AABB
	enemy->bodyAABB.pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
	enemy->bodyAABB.w = 0.0f;
	enemy->bodyAABB.h = 0.0f;
	enemy->bodyAABB.tag = ENEMY_BODY_AABB;

	for (int j = 0; j < MAX_ATTACK_AABB; j++)
	{
		enemy->attackAABB[j].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		enemy->attackAABB[j].w = 0.0f;
		enemy->attackAABB[j].h = 0.0f;
		enemy->attackAABB[j].tag = ENEMY_ATTACK_AABB;
	}
}

BOOL GetUpdateEnemy(void)
{
	return g_Update;

}
void SetUpdateEnemy(BOOL update)
{
	g_Update = update;
}