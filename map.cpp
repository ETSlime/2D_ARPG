//=============================================================================
//
// map処理 [map.cpp]
// Author : 
//
//=============================================================================
#include "map.h"
#include "enemy.h"
#include "tutorial.h"
#include "player.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_TELEPORT_WIDTH				(125.0f)
#define TEXTURE_TELEPORT_HEIGHT				(125.0f)
#define TEXTURE_TELEPORT_PATTERN_DIVIDE_X	(5)
#define TEXTURE_TELEPORT_PATTERN_DIVIDE_Y	(2)
#define	ANIM_WAIT_TELEPORT					(20)

#define ROCK_01_TEXTURE_NO					(5)
#define ROCK_02_TEXTURE_NO					(6)
#define TELEPORT_TEXTURE_NO					(7)

#define TUTORIAL_01_GROUND_WIDTH	(TEXTURE_BG_TUTORIAL_01_WIDTH + 800.0f)
#define TUTORIAL_01_GROUND_HEIGHT	(250)
#define TUTORIAL_01_GROUND_POS_X	(TEXTURE_BG_TUTORIAL_01_WIDTH * 0.5f - 50)
#define TUTORIAL_01_GROUND_POS_Y	(TEXTURE_BG_TUTORIAL_01_HEIGHT)

#define TUTORIAL_01_ROCK01_WIDTH	(1600)
#define TUTORIAL_01_ROCK01_HEIGHT	(400)
#define TUTORIAL_01_ROCK01_POS_X	(2550)
#define TUTORIAL_01_ROCK01_POS_Y	(TEXTURE_BG_TUTORIAL_01_HEIGHT - 200)

#define TUTORIAL_01_EXIT_POS_X		(3680.0f)
#define TUTORIAL_01_EXIT_POS_Y		(1350.0f)

#define MAP_01_GROUND_WIDTH			(TEXTURE_BG_MAP_01_WIDTH + 1200.0f)
#define MAP_01_GROUND_HEIGHT		(250)
#define MAP_01_GROUND_POS_X			(TEXTURE_BG_MAP_01_WIDTH * 0.5f)
#define MAP_01_GROUND_POS_Y			(TEXTURE_BG_MAP_01_HEIGHT - 250.0f)

#define MAP_01_ROCK01_WIDTH		(1294.0f)
#define MAP_01_ROCK01_HEIGHT	(189.0f)
#define MAP_01_ROCK01_POS_X		(1434.0f)
#define MAP_01_ROCK01_POS_Y		(2032.0f)

#define MAP_01_ROCK02_WIDTH		(547.0f)
#define MAP_01_ROCK02_HEIGHT	(131.0f)
#define MAP_01_ROCK02_POS_X		(1479.0f)
#define MAP_01_ROCK02_POS_Y		(1793.0f)

#define MAP_01_ROCK03_WIDTH		(526.0f)
#define MAP_01_ROCK03_HEIGHT	(114.0f)
#define MAP_01_ROCK03_POS_X		(200.0f)
#define MAP_01_ROCK03_POS_Y		(1310.0f)

#define MAP_01_ROCK04_WIDTH		(1277.0f)
#define MAP_01_ROCK04_HEIGHT	(272.0f)
#define MAP_01_ROCK04_POS_X		(1270.0f)
#define MAP_01_ROCK04_POS_Y		(1140.0f)

#define MAP_01_ROCK05_WIDTH		(724.0f)
#define MAP_01_ROCK05_HEIGHT	(150.0f)
#define MAP_01_ROCK05_POS_X		(560.0f)
#define MAP_01_ROCK05_POS_Y		(1540.0f)

#define MAP_01_ROCK06_WIDTH		(1747.0f)
#define MAP_01_ROCK06_HEIGHT	(191.0f)
#define MAP_01_ROCK06_POS_X		(2519.0f)
#define MAP_01_ROCK06_POS_Y		(1553.0f)

#define MAP_01_TELEPORT_01_POS_X	(PLAYER_INIT_POS_X_MAP01_02 + 150.0f)
#define MAP_01_TELEPORT_01_POS_Y	(PLAYER_INIT_POS_Y_MAP01_02)

#define MAP_02_ROCK01_WIDTH	(600.0f)
#define MAP_02_ROCK01_HEIGHT	(135.0f)
#define MAP_02_ROCK01_POS_X	(213.0f)
#define MAP_02_ROCK01_POS_Y	(776.0f)

#define MAP_02_ROCK02_WIDTH		(740.0f)
#define MAP_02_ROCK02_HEIGHT	(40.0f)
#define	MAP_02_ROCK02_POS_X		(900.0f)
#define MAP_02_ROCK02_POS_Y		(855.1f)

#define MAP_02_ROCK03_WIDTH		(2600.0f)
#define MAP_02_ROCK03_HEIGHT	(40.0f)
#define	MAP_02_ROCK03_POS_X		(1470.0f)
#define MAP_02_ROCK03_POS_Y		(589.f)


#define MAP_02_TELEPORT_01_POS_X	(PLAYER_INIT_POS_X_MAP02_01 - 100.0f)
#define MAP_02_TELEPORT_01_POS_Y	(PLAYER_INIT_POS_Y_MAP02_01)
#define MAP_02_TELEPORT_02_POS_X	(PLAYER_INIT_POS_X_MAP02_02 + 100.0f)
#define MAP_02_TELEPORT_02_POS_Y	(PLAYER_INIT_POS_Y_MAP02_02)

#define MAP_BOSS_ROCK01_WIDTH	(594.0f)
#define MAP_BOSS_ROCK01_HEIGHT	(189.0f)
#define MAP_BOSS_ROCK01_POS_X	(634.0f)
#define MAP_BOSS_ROCK01_POS_Y	(1432.0f)

#define MAP_BOSS_ROCK02_WIDTH	(547.0f)
#define MAP_BOSS_ROCK02_HEIGHT	(131.0f)
#define MAP_BOSS_ROCK02_POS_X	(1479.0f)
#define MAP_BOSS_ROCK02_POS_Y	(1643.0f)

#define MAP_BOSS_ROCK03_WIDTH	(526.0f)
#define MAP_BOSS_ROCK03_HEIGHT	(114.0f)
#define MAP_BOSS_ROCK03_POS_X	(200.0f)
#define MAP_BOSS_ROCK03_POS_Y	(1170.0f)

#define MAP_BOSS_ROCK04_WIDTH	(1277.0f)
#define MAP_BOSS_ROCK04_HEIGHT	(150.0f)
#define MAP_BOSS_ROCK04_POS_X	(1300.0f)
#define MAP_BOSS_ROCK04_POS_Y	(1040.0f)

#define MAP_BOSS_ROCK05_WIDTH	(700.0f)
#define MAP_BOSS_ROCK05_HEIGHT	(150.0f)
#define MAP_BOSS_ROCK05_POS_X	(700.0f)
#define MAP_BOSS_ROCK05_POS_Y	(400.0f)

#define MAP_BOSS_GROUND_WIDTH		(TEXTURE_BG_MAP_BOSS_WIDTH + 600.0f)
#define MAP_BOSS_GROUND_HEIGHT		(372.0f)
#define MAP_BOSS_GROUND_POS_X		(TEXTURE_BG_MAP_BOSS_WIDTH * 0.5f)
#define MAP_BOSS_GROUND_POS_Y		(TEXTURE_BG_MAP_BOSS_HEIGHT)

#define MAP_BOSS_TELEPORT_01_POS_X	(PLAYER_INIT_POS_X_MAP_BOSS_01 - 120.0f)
#define MAP_BOSS_TELEPORT_01_POS_Y	(PLAYER_INIT_POS_Y_MAP_BOSS_01)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

#ifdef _DEBUG	
static ID3D11Buffer* g_AABBVertexBuffer = NULL;
#endif


static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/map/tutorial01.jpg",
	"data/TEXTURE/map/map01.jpg",
	"data/TEXTURE/map/map02.png",
	"data/TEXTURE/map/map03.jpg",
	"data/TEXTURE/map/map_boss.jpg",
	"data/TEXTURE/map/rock01.png",
	"data/TEXTURE/map/rock02.png",
	"data/TEXTURE/map/teleport.png",
};


static BOOL			g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG			g_BG;
static AABB			g_AABB[MAP_GROUND_MAX];
static int			g_MapNo = MAP_01;
static int			g_MapDraw = MAP_01;
static Teleport		g_Teleport[TELEPORT_NUM_MAX];

static INTERPOLATION_DATA g_MoveTbl0[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_MoveTbl1[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_MoveTbl2[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Goblin_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Goblin_02[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Goblin_03[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Cyclops_01[MOVE_NUM_MAX];

static INTERPOLATION_DATA g_Map_01_MoveTbl_Goblin_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Goblin_02[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Goblin_03[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Cyclops_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Orge_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Imp_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Imp_02[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Gnoll_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Gnoll_02[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Gargoyle_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Gargoyle_02[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Map_01_MoveTbl_Gargoyle_03[MOVE_NUM_MAX];

static INTERPOLATION_DATA g_Map_BOSS_MoveTbl_BOSS[MOVE_NUM_MAX];

static EnemyConfig	g_EnemyConfig[MAP_ENEMY_MAX];
static XMFLOAT3 g_PlayerInitPos[MAP_NUM_MAX][PLAYER_INIT_POS_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMap(void)
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

	// 変数の初期化
	InitMapBG(g_MapNo);
	InitMapCollisionBox(g_MapNo);
	InitMoveTbl(g_MapNo);
	InitEnemyConfig(g_MapNo);
	InitTeleport(g_MapNo);

#ifdef _DEBUG	
	// debug
	{
		int aabbCount = MAP_GROUND_MAX;
		const int maxVertices = MAP_GROUND_MAX * 4;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * maxVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// 创建顶点缓冲区
		GetDevice()->CreateBuffer(&bd, NULL, &g_AABBVertexBuffer);
	}
#endif


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMap(void)
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

	for (int i = 0; i < MAP_GROUND_MAX; i++)
	{
		g_AABB[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_AABB[i].w = 0.0f;
		g_AABB[i].h = 0.0f;
	}

	for (int i = 0; i < MAP_ENEMY_MAX; i++)
	{
		g_EnemyConfig[i] = {};
	}

	for (int i = 0; i < MAP_NUM_MAX; i++)
	{
		for (int j = 0; j < TELEPORT_NUM_MAX; j++)
		{
			g_PlayerInitPos[i][j] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int i = 0; i < TELEPORT_NUM_MAX; i++)
	{
		g_Teleport[i].use = FALSE;
		g_Teleport[i].teleportAABB.h = 0.0f;
		g_Teleport[i].teleportAABB.w = 0.0f;
		g_Teleport[i].teleportAABB.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}


	g_Load = FALSE;
}


void InitMapBG(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
		g_BG.w = TEXTURE_BG_TUTORIAL_01_WIDTH;
		g_BG.h = TEXTURE_BG_TUTORIAL_01_HEIGHT;
		break;
	case MAP_01:
		g_BG.w = TEXTURE_BG_MAP_01_WIDTH;
		g_BG.h = TEXTURE_BG_MAP_01_HEIGHT;
		break;
	case MAP_02:
		g_BG.w = TEXTURE_BG_MAP_02_WIDTH;
		g_BG.h = TEXTURE_BG_MAP_02_HEIGHT;
		break;
	case MAP_03:
		g_BG.w = TEXTURE_BG_MAP_03_WIDTH;
		g_BG.h = TEXTURE_BG_MAP_03_HEIGHT;
		break;
	case MAP_BOSS:
		g_BG.w = TEXTURE_BG_MAP_BOSS_WIDTH;
		g_BG.h = TEXTURE_BG_MAP_BOSS_HEIGHT;
		break;
	default:
		break;
	}

	g_BG.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.texNo = map;

	g_BG.scroll = FALSE;
	g_BG.scrollSpeedX = 0.0f;
	g_BG.scrollSpeedY = 0.0f;
}
void InitMapCollisionBox(int map)
{
	// AABB
	for (int i = 0; i < MAP_GROUND_MAX; i++)
	{
		g_AABB[i].pos.x = 0;
		g_AABB[i].pos.y = 0;
		g_AABB[i].w = 0;
		g_AABB[i].h = 0;
		g_AABB[i].tag = WALL_AABB;
	}

	switch (map)
	{
	case TUTORIAL_01:
	{
		g_AABB[0].pos.x = TEXTURE_BG_TUTORIAL_01_WIDTH * 0.5f;
		g_AABB[0].pos.y = 1340.0f + (TEXTURE_BG_TUTORIAL_01_HEIGHT - 1290.0f) * 0.5f;
		g_AABB[0].w = TEXTURE_BG_TUTORIAL_01_WIDTH + 200.0f;
		g_AABB[0].h = TUTORIAL01_GROUND_H * 0.5f;

		g_AABB[1].pos.x = TUTORIAL_01_ROCK01_POS_X * 0.99f;
		g_AABB[1].pos.y = TUTORIAL_01_ROCK01_POS_Y;
		g_AABB[1].w = TUTORIAL_01_ROCK01_WIDTH * 0.83f;
		g_AABB[1].h = TUTORIAL_01_ROCK01_HEIGHT * 0.78f;
		break;
	}
	case MAP_01:
	{
		g_AABB[0].pos.x = MAP_01_GROUND_POS_X;
		g_AABB[0].pos.y = MAP_01_GROUND_POS_Y + 5.0f;
		g_AABB[0].w = TEXTURE_BG_MAP_01_WIDTH;
		g_AABB[0].h = MAP01_GROUND_H;

		g_AABB[1].pos.x = MAP_01_ROCK01_POS_X - 25.0f;
		g_AABB[1].pos.y = MAP_01_ROCK01_POS_Y + 12.0f;
		g_AABB[1].w = MAP_01_ROCK01_WIDTH * 0.83f;
		g_AABB[1].h = MAP_01_ROCK01_HEIGHT * 0.78f;

		g_AABB[2].pos.x = MAP_01_ROCK02_POS_X - 5.0f;
		g_AABB[2].pos.y = MAP_01_ROCK02_POS_Y;
		g_AABB[2].w = MAP_01_ROCK02_WIDTH * 0.87f;
		g_AABB[2].h = MAP_01_ROCK02_HEIGHT * 1.0f;

		g_AABB[3].pos.x = MAP_01_ROCK03_POS_X;
		g_AABB[3].pos.y = MAP_01_ROCK03_POS_Y;
		g_AABB[3].w = MAP_01_ROCK03_WIDTH * 0.8f;
		g_AABB[3].h = MAP_01_ROCK03_HEIGHT * 0.8f;

		g_AABB[4].pos.x = MAP_01_ROCK04_POS_X - 25.0f;
		g_AABB[4].pos.y = MAP_01_ROCK04_POS_Y;
		g_AABB[4].w = MAP_01_ROCK04_WIDTH * 0.84f;
		g_AABB[4].h = MAP_01_ROCK04_HEIGHT * 0.8f;

		g_AABB[5].pos.x = MAP_01_ROCK05_POS_X;
		g_AABB[5].pos.y = MAP_01_ROCK05_POS_Y;
		g_AABB[5].w = MAP_01_ROCK05_WIDTH * 0.8f;
		g_AABB[5].h = MAP_01_ROCK05_HEIGHT * 0.8f;

		g_AABB[6].pos.x = MAP_01_ROCK06_POS_X - 25.0f;
		g_AABB[6].pos.y = MAP_01_ROCK06_POS_Y;
		g_AABB[6].w = MAP_01_ROCK06_WIDTH * 0.85f;
		g_AABB[6].h = MAP_01_ROCK06_HEIGHT * 0.8f;

		break;
	}
	case MAP_02:
	{
		g_AABB[0].pos.x = TEXTURE_BG_MAP_02_WIDTH * 0.5f;
		g_AABB[0].pos.y = 1290.0f + (TEXTURE_BG_MAP_02_HEIGHT - 1290.0f) * 0.5f;
		g_AABB[0].w = TEXTURE_BG_MAP_02_WIDTH;
		g_AABB[0].h = MAP02_GROUND_H * 0.5f;

		g_AABB[1].pos.x = 685;
		g_AABB[1].pos.y = 1226;
		g_AABB[1].w = 103;
		g_AABB[1].h = 224;

		g_AABB[2].pos.x = 900;
		g_AABB[2].pos.y = 857;
		g_AABB[2].w = 640;
		g_AABB[2].h = 40;

		g_AABB[3].pos.x = 1450;
		g_AABB[3].pos.y = 594;
		g_AABB[3].w = 2200;
		g_AABB[3].h = 40;

		g_AABB[4].pos.x = MAP_02_ROCK01_POS_X;
		g_AABB[4].pos.y = MAP_02_ROCK01_POS_Y;
		g_AABB[4].w = MAP_02_ROCK01_WIDTH * 0.83f;
		g_AABB[4].h = MAP_02_ROCK01_HEIGHT * 0.83f;

		break;
	}
	case MAP_BOSS:
	{
		g_AABB[0].pos.x = MAP_BOSS_GROUND_POS_X;
		g_AABB[0].pos.y = MAP_BOSS_GROUND_POS_Y + 5.0f;
		g_AABB[0].w = TEXTURE_BG_MAP_BOSS_WIDTH;
		g_AABB[0].h = MAP_BOSS_GROUND_HEIGHT * 0.84f;

		g_AABB[1].pos.x = MAP_BOSS_ROCK01_POS_X - 11.1f;
		g_AABB[1].pos.y = MAP_BOSS_ROCK01_POS_Y + 12.3f;
		g_AABB[1].w = MAP_BOSS_ROCK01_WIDTH * 0.83f;
		g_AABB[1].h = MAP_BOSS_ROCK01_HEIGHT * 0.83f;

		g_AABB[2].pos.x = MAP_BOSS_ROCK02_POS_X;
		g_AABB[2].pos.y = MAP_BOSS_ROCK02_POS_Y;
		g_AABB[2].w = MAP_BOSS_ROCK02_WIDTH * 0.83f;
		g_AABB[2].h = MAP_BOSS_ROCK02_HEIGHT * 0.83f;

		g_AABB[3].pos.x = MAP_BOSS_ROCK03_POS_X;
		g_AABB[3].pos.y = MAP_BOSS_ROCK03_POS_Y;
		g_AABB[3].w = MAP_BOSS_ROCK03_WIDTH * 0.83f;
		g_AABB[3].h = MAP_BOSS_ROCK03_HEIGHT * 0.83f;

		g_AABB[4].pos.x = MAP_BOSS_ROCK04_POS_X - 21.1f;
		g_AABB[4].pos.y = MAP_BOSS_ROCK04_POS_Y;
		g_AABB[4].w = MAP_BOSS_ROCK04_WIDTH * 0.85f;
		g_AABB[4].h = MAP_BOSS_ROCK04_HEIGHT * 0.83f;

		//g_AABB[5].pos.x = MAP_BOSS_ROCK05_POS_X;
		//g_AABB[5].pos.y = MAP_BOSS_ROCK05_POS_Y;
		//g_AABB[5].w = MAP_BOSS_ROCK05_WIDTH;
		//g_AABB[5].h = MAP_BOSS_ROCK05_HEIGHT;

		break;
	}
	default:
		break;
	}
}

void InitMoveTbl(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
		g_Tutorial_01_MoveTbl_Goblin_01[0] = { XMFLOAT3(-50.0f, 1349.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Tutorial_01_MoveTbl_Goblin_01[1] = { XMFLOAT3(938.0f, PLAYER_INIT_POS_Y_TUTORIAL01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Tutorial_01_MoveTbl_Goblin_02[0] = { XMFLOAT3(1675.0f, 1349.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Tutorial_01_MoveTbl_Goblin_02[1] = { XMFLOAT3(2257.0f, PLAYER_INIT_POS_Y_TUTORIAL01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Tutorial_01_MoveTbl_Goblin_03[0] = { XMFLOAT3(1854.0f, 1349.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Tutorial_01_MoveTbl_Goblin_03[1] = { XMFLOAT3(1253, PLAYER_INIT_POS_Y_TUTORIAL01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Tutorial_01_MoveTbl_Cyclops_01[0] = { XMFLOAT3(2805.0f, 1078.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Tutorial_01_MoveTbl_Cyclops_01[1] = { XMFLOAT3(2105, 1078.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		break;
	case MAP_01:
	{
		g_Map_01_MoveTbl_Goblin_01[0] = { XMFLOAT3(350.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Map_01_MoveTbl_Goblin_01[1] = { XMFLOAT3(938.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Goblin_02[0] = { XMFLOAT3(1675.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Map_01_MoveTbl_Goblin_02[1] = { XMFLOAT3(2257.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Goblin_03[0] = { XMFLOAT3(2454.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Map_01_MoveTbl_Goblin_03[1] = { XMFLOAT3(1753, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Cyclops_01[0] = { XMFLOAT3(2805.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Cyclops_01[1] = { XMFLOAT3(2105, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Orge_01[0] = { XMFLOAT3(3386.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Orge_01[1] = { XMFLOAT3(2951.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Imp_01[0] = { XMFLOAT3(2358.0f, 1436.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Imp_01[1] = { XMFLOAT3(2891.0f, 1436.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Imp_02[0] = { XMFLOAT3(2741, 1436.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Imp_02[1] = { XMFLOAT3(2134.0f, 1436.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Gnoll_01[0] = { XMFLOAT3(1364.0f, 991.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Gnoll_01[1] = { XMFLOAT3(1646, 991.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Gargoyle_01[0] = { XMFLOAT3(927.0f, 941.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Gargoyle_01[1] = { XMFLOAT3(1477.0f, 941.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Map_01_MoveTbl_Gargoyle_02[0] = { XMFLOAT3(561.0f, 921.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_01_MoveTbl_Gargoyle_02[1] = { XMFLOAT3(1274.0f, 921.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		break;
	}
	case MAP_02:
		//座標									回転率							拡大率					時間
		g_MoveTbl0[0] = { XMFLOAT3(1200.0f, 523.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_MoveTbl0[1] = { XMFLOAT3(1850.0f,  523.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_MoveTbl1[0] = { XMFLOAT3(1500.0f, 1284.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_MoveTbl1[1] = { XMFLOAT3(450.0f,  1284.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_MoveTbl2[0] = {XMFLOAT3(3000.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60};
		g_MoveTbl2[1] = { XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 };
		break;
	case MAP_BOSS:
		g_Map_BOSS_MoveTbl_BOSS[0] = { XMFLOAT3(1000.0f, 310.f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_BOSS_MoveTbl_BOSS[1] = { XMFLOAT3(1000.0f, 1710.f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_Map_BOSS_MoveTbl_BOSS[2] = { XMFLOAT3(1251.0f, 875.f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		break;
	default:
		break;
	}
}

void InitPlayerInitPos(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
		g_PlayerInitPos[TUTORIAL_01][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_TUTORIAL01_01, PLAYER_INIT_POS_Y_TUTORIAL01_01, 0.0f);
		break;
	case MAP_01:
		g_PlayerInitPos[MAP_01][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP01_01, PLAYER_INIT_POS_Y_MAP01_01, 0.0f);
		g_PlayerInitPos[MAP_01][INITPOS_02] = XMFLOAT3(PLAYER_INIT_POS_X_MAP01_02, PLAYER_INIT_POS_Y_MAP01_02, 0.0f);
		break;
	case MAP_02:
		g_PlayerInitPos[MAP_02][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP02_01, PLAYER_INIT_POS_Y_MAP02_01, 0.0f);
		g_PlayerInitPos[MAP_02][INITPOS_02] = XMFLOAT3(PLAYER_INIT_POS_X_MAP02_02, PLAYER_INIT_POS_Y_MAP02_02, 0.0f);
		break;
	case MAP_BOSS:
		g_PlayerInitPos[MAP_BOSS][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP_BOSS_01, PLAYER_INIT_POS_Y_MAP_BOSS_01, 0.0f);
		break;
	default:
		break;
	}
}

void InitEnemyConfig(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
		g_EnemyConfig[0] = { GOBLIN, g_Tutorial_01_MoveTbl_Goblin_01, 2 };
		g_EnemyConfig[1] = { GOBLIN, g_Tutorial_01_MoveTbl_Goblin_02, 2 };
		g_EnemyConfig[2] = { GOBLIN, g_Tutorial_01_MoveTbl_Goblin_03, 2 };
		g_EnemyConfig[TUTORIAL_CYCLOPS_ID] = { CYCLOPS, g_Tutorial_01_MoveTbl_Cyclops_01, 2 };
		break;
	case MAP_01:
		g_EnemyConfig[0] = { GOBLIN, g_Map_01_MoveTbl_Goblin_01, 2 };
		g_EnemyConfig[1] = { GOBLIN, g_Map_01_MoveTbl_Goblin_02, 2 };
		g_EnemyConfig[2] = { GOBLIN, g_Map_01_MoveTbl_Goblin_03, 2 };
		g_EnemyConfig[3] = { CYCLOPS, g_Map_01_MoveTbl_Cyclops_01, 2 };
		g_EnemyConfig[4] = { OGRE, g_Map_01_MoveTbl_Orge_01, 2 };
		g_EnemyConfig[5] = { IMP, g_Map_01_MoveTbl_Imp_01, 2 };
		g_EnemyConfig[6] = { IMP, g_Map_01_MoveTbl_Imp_02, 2 };
		g_EnemyConfig[7] = { GNOLL, g_Map_01_MoveTbl_Gnoll_01, 2 };
		g_EnemyConfig[8] = { GARGOYLE, g_Map_01_MoveTbl_Gargoyle_01, 2 };
		g_EnemyConfig[9] = { GARGOYLE, g_Map_01_MoveTbl_Gargoyle_02, 2 };
		break;
	case MAP_02:
		g_EnemyConfig[0] = {GOLEM, g_MoveTbl0, 2};
		g_EnemyConfig[1] = {CYCLOPS, g_MoveTbl1, 2};
		break;
	case MAP_03:
		break;
	case MAP_BOSS:
		g_EnemyConfig[0] = { BOSS, g_Map_BOSS_MoveTbl_BOSS, 3};
		break;
	default:
		break;
	}
}

void InitTeleport(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
		g_Teleport[0].pos.x = TUTORIAL_01_EXIT_POS_X;
		g_Teleport[0].pos.y = TUTORIAL_01_EXIT_POS_Y;
		g_Teleport[0].nextMapNo = MAP_01;
		g_Teleport[0].nextInitPos = INITPOS_01;
		g_Teleport[0].use = TRUE;
		g_Teleport[0].teleportAABB.pos = g_Teleport[0].pos;
		g_Teleport[0].teleportAABB.w = TEXTURE_TELEPORT_WIDTH * 0.5f;
		g_Teleport[0].teleportAABB.h = TEXTURE_TELEPORT_HEIGHT * 0.5f;
		g_Teleport[0].countAnim = 0.0f;
		g_Teleport[0].patternAnim = 0;
		break;
	case MAP_01:
		g_Teleport[0].pos.x = MAP_01_TELEPORT_01_POS_X;
		g_Teleport[0].pos.y = MAP_01_TELEPORT_01_POS_Y;
		g_Teleport[0].nextMapNo = MAP_02;
		g_Teleport[0].nextInitPos = INITPOS_01;
		g_Teleport[0].use = TRUE;
		g_Teleport[0].teleportAABB.pos = g_Teleport[0].pos;
		g_Teleport[0].teleportAABB.w = TEXTURE_TELEPORT_WIDTH * 0.5f;
		g_Teleport[0].teleportAABB.h = TEXTURE_TELEPORT_HEIGHT * 0.5f;
		g_Teleport[0].countAnim = 0.0f;
		g_Teleport[0].patternAnim = 0;
		break;
	case MAP_02:
		g_Teleport[0].pos.x = MAP_02_TELEPORT_01_POS_X;
		g_Teleport[0].pos.y = MAP_02_TELEPORT_01_POS_Y;
		g_Teleport[0].nextMapNo = MAP_01;
		g_Teleport[0].nextInitPos = INITPOS_02;
		g_Teleport[0].use = TRUE;
		g_Teleport[0].teleportAABB.pos = g_Teleport[0].pos;
		g_Teleport[0].teleportAABB.w = TEXTURE_TELEPORT_WIDTH * 0.5f;
		g_Teleport[0].teleportAABB.h = TEXTURE_TELEPORT_HEIGHT * 0.5f;
		g_Teleport[0].countAnim = 0.0f;
		g_Teleport[0].patternAnim = 0;

		g_Teleport[1].pos.x = MAP_02_TELEPORT_02_POS_X;
		g_Teleport[1].pos.y = MAP_02_TELEPORT_02_POS_Y;
		g_Teleport[1].nextMapNo = MAP_BOSS;
		g_Teleport[1].nextInitPos = INITPOS_01;
		g_Teleport[1].use = TRUE;
		g_Teleport[1].teleportAABB.pos = g_Teleport[1].pos;
		g_Teleport[1].teleportAABB.w = TEXTURE_TELEPORT_WIDTH * 0.5f;
		g_Teleport[1].teleportAABB.h = TEXTURE_TELEPORT_HEIGHT * 0.5f;
		g_Teleport[1].countAnim = 0.0f;
		g_Teleport[1].patternAnim = 0;
		break;
	case MAP_BOSS:
		g_Teleport[0].pos.x = MAP_BOSS_TELEPORT_01_POS_X;
		g_Teleport[0].pos.y = MAP_BOSS_TELEPORT_01_POS_Y;
		g_Teleport[0].nextMapNo = MAP_02;
		g_Teleport[0].nextInitPos = INITPOS_02;
		g_Teleport[0].use = TRUE;
		g_Teleport[0].teleportAABB.pos = g_Teleport[0].pos;
		g_Teleport[0].teleportAABB.w = TEXTURE_TELEPORT_WIDTH * 0.5f;
		g_Teleport[0].teleportAABB.h = TEXTURE_TELEPORT_HEIGHT * 0.5f;
		g_Teleport[0].countAnim = 0.0f;
		g_Teleport[0].patternAnim = 0;
		break;
	default:
		break;
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap(void)
{
	g_BG.old_pos = g_BG.pos;	// １フレ前の情報を保存

	g_BG.texNo = GetCurrentMap();

	if (g_BG.scroll == TRUE)
	{
		g_BG.pos.x += g_BG.scrollSpeedX;
		g_BG.pos.y += g_BG.scrollSpeedY;
		g_BG.scrollTime--;

		if (g_BG.scrollTime <= 0.0f)
			g_BG.scroll = FALSE;
	}

	UpdateTeleport();

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

void UpdateTeleport(void)
{
	for (int i = 0; i < TELEPORT_NUM_MAX; i++)
	{
		if (g_Teleport[i].use == FALSE) continue;

		g_Teleport[i].countAnim++;
		if (g_Teleport[i].countAnim > ANIM_WAIT_TELEPORT)
		{
			g_Teleport[i].countAnim = 0.0f;

			// パターンの切り替え
			g_Teleport[i].patternAnim = (g_Teleport[i].patternAnim + 1) %
				(TEXTURE_TELEPORT_PATTERN_DIVIDE_X * TEXTURE_TELEPORT_PATTERN_DIVIDE_Y);
		}

		AABB teleportBox = g_Teleport[i].teleportAABB;
		PLAYER* player = GetPlayer();

		// プレイヤーのAABB情報を取得
		XMFLOAT3 playerPos = player->bodyAABB.pos;
		float playerW = player->bodyAABB.w;
		float playerH = player->bodyAABB.h;

		// テレポートのAABB情報を取得
		XMFLOAT3 teleportPos = teleportBox.pos;
		float teleportW = teleportBox.w;
		float teleportH = teleportBox.h;

		// プレイヤーの包囲ボックスとテレポートの包囲ボックスが重なっているかを確認
		BOOL isColliding = CollisionBB(playerPos, playerW, playerH, teleportPos, teleportW, teleportH);

		if (isColliding)
		{
			// 当たり判定があった場合、プレイヤーをテレポートする
			int nextMap = g_Teleport[i].nextMapNo;
			int nextInitPos = g_Teleport[i].nextInitPos;
			ChangeMapFade(nextMap, nextInitPos);
			PlaySound(SOUND_LABEL_SE_TELEPORT);
			break;
		}

	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawMap(void)
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


	// 地面を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_MapDraw]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	DrawMapWalls(g_MapNo);
	DrawTeleport();


	// 空を描画
	//{
	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

	//	// １枚のポリゴンの頂点とテクスチャ座標を設定
	//	//float	tx = (g_BG.pos.x - g_BG.old_pos.x) * ((float)SCREEN_WIDTH / TEXTURE_WIDTH);
	//	//g_BG.scrl += tx * 0.001f;
	//	g_BG.scrl += 0.001f;

	//	SetSpriteLTColor(g_VertexBuffer,
	//		0.0f, 0.0f, SCREEN_WIDTH, SKY_H,
	//		g_BG.scrl, 0.0f, 1.0f, 1.0f,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//	// ポリゴン描画
	//	GetDeviceContext()->Draw(4, 0);
	//}

#ifdef _DEBUG
		MATERIAL materialAABB;
		ZeroMemory(&materialAABB, sizeof(materialAABB));
		materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materialAABB.noTexSampling = 1;


		SetMaterial(materialAABB);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < MAP_GROUND_MAX; ++i)
		{
			int vertexOffset = i * 4;

			if (g_AABB[i].tag == WALL_AABB)
				SetSpriteColorRotation(g_AABBVertexBuffer, 
					g_AABB[i].pos.x - g_BG.pos.x, g_AABB[i].pos.y - g_BG.pos.y, 
					g_AABB[i].w, g_AABB[i].h,
					0.0f, 0.0f, 0.0f, 0.0f,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 0.2f),
					0.0f,
					vertexOffset);
			else if (g_AABB[i].tag == GROUND_AABB)
				SetSpriteColorRotation(g_AABBVertexBuffer, 
					g_AABB[i].pos.x - g_BG.pos.x, g_AABB[i].pos.y - g_BG.pos.y, 
					g_AABB[i].w, g_AABB[i].h,
					0.0f, 0.0f, 0.0f, 0.0f,
					XMFLOAT4(1.0f, 1.0f, 0.0f, 0.2f),
					0.0f,
					vertexOffset);

			GetDeviceContext()->Draw(4, vertexOffset);
		}

		for (int i = 0; i < TELEPORT_NUM_MAX; i++)
		{
			if (g_Teleport[i].use == FALSE) continue;

			int vertexOffset = i * 4;

			SetSpriteColorRotation(g_AABBVertexBuffer, 
				g_Teleport[i].teleportAABB.pos.x - g_BG.pos.x, g_Teleport[i].teleportAABB.pos.y - g_BG.pos.y, 
				g_Teleport[i].teleportAABB.w, g_Teleport[i].teleportAABB.h,
				0.0f, 0.0f, 0.0f, 0.0f,
				XMFLOAT4(1.0f, 0.0f, 1.0f, 0.2f),
				0.0f);

			GetDeviceContext()->Draw(4, vertexOffset);
		}
#endif

}

void DrawMapWalls(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			TUTORIAL_01_GROUND_POS_X - g_BG.pos.x, TUTORIAL_01_GROUND_POS_Y - g_BG.pos.y,
			TUTORIAL_01_GROUND_WIDTH, TUTORIAL_01_GROUND_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			TUTORIAL_01_ROCK01_POS_X - g_BG.pos.x, TUTORIAL_01_ROCK01_POS_Y - g_BG.pos.y,
			TUTORIAL_01_ROCK01_WIDTH, TUTORIAL_01_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		break;
	}
	case MAP_01:
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_GROUND_POS_X - g_BG.pos.x, MAP_01_GROUND_POS_Y - g_BG.pos.y,
			MAP_01_GROUND_WIDTH, MAP_01_GROUND_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
		

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_ROCK01_POS_X - g_BG.pos.x, MAP_01_ROCK01_POS_Y - g_BG.pos.y,
			MAP_01_ROCK01_WIDTH, MAP_01_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_ROCK02_POS_X - g_BG.pos.x, MAP_01_ROCK02_POS_Y - g_BG.pos.y,
			MAP_01_ROCK02_WIDTH, MAP_01_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_ROCK03_POS_X - g_BG.pos.x, MAP_01_ROCK03_POS_Y - g_BG.pos.y,
			MAP_01_ROCK03_WIDTH, MAP_01_ROCK03_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_ROCK04_POS_X - g_BG.pos.x, MAP_01_ROCK04_POS_Y - g_BG.pos.y,
			MAP_01_ROCK04_WIDTH, MAP_01_ROCK04_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_ROCK05_POS_X - g_BG.pos.x, MAP_01_ROCK05_POS_Y - g_BG.pos.y,
			MAP_01_ROCK05_WIDTH, MAP_01_ROCK05_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_01_ROCK06_POS_X - g_BG.pos.x, MAP_01_ROCK06_POS_Y - g_BG.pos.y,
			MAP_01_ROCK06_WIDTH, MAP_01_ROCK06_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		break;
	}
	case MAP_02:
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_02_ROCK01_POS_X - g_BG.pos.x, MAP_02_ROCK01_POS_Y - g_BG.pos.y,
			MAP_02_ROCK01_WIDTH, MAP_02_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_02_ROCK02_POS_X - g_BG.pos.x, MAP_02_ROCK02_POS_Y - g_BG.pos.y,
			MAP_02_ROCK02_WIDTH, MAP_02_ROCK02_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_02_ROCK03_POS_X - g_BG.pos.x, MAP_02_ROCK03_POS_Y - g_BG.pos.y,
			MAP_02_ROCK03_WIDTH, MAP_02_ROCK03_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		break;
	}
	case MAP_BOSS:
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_BOSS_GROUND_POS_X - g_BG.pos.x, MAP_BOSS_GROUND_POS_Y - g_BG.pos.y,
			MAP_BOSS_GROUND_WIDTH, MAP_BOSS_GROUND_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_BOSS_ROCK01_POS_X - g_BG.pos.x, MAP_BOSS_ROCK01_POS_Y - g_BG.pos.y,
			MAP_BOSS_ROCK01_WIDTH, MAP_BOSS_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_BOSS_ROCK02_POS_X - g_BG.pos.x, MAP_BOSS_ROCK02_POS_Y - g_BG.pos.y,
			MAP_BOSS_ROCK02_WIDTH, MAP_BOSS_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_BOSS_ROCK03_POS_X - g_BG.pos.x, MAP_BOSS_ROCK03_POS_Y - g_BG.pos.y,
			MAP_BOSS_ROCK03_WIDTH, MAP_BOSS_ROCK03_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			MAP_BOSS_ROCK04_POS_X - g_BG.pos.x, MAP_BOSS_ROCK04_POS_Y - g_BG.pos.y,
			MAP_BOSS_ROCK04_WIDTH, MAP_BOSS_ROCK04_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02_TEXTURE_NO]);
		//SetSpriteColor(g_VertexBuffer,
		//	MAP_BOSS_ROCK05_POS_X - g_BG.pos.x, MAP_BOSS_ROCK05_POS_Y - g_BG.pos.y,
		//	MAP_BOSS_ROCK05_WIDTH, MAP_BOSS_ROCK05_HEIGHT,
		//	0.0f, 0.0f, 1.0f, 1.0f,
		//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		//GetDeviceContext()->Draw(4, 0);
		break;
	}
	default:
		break;
	}
}

void DrawTeleport(void)
{
	for (int i = 0; i < TELEPORT_NUM_MAX; i++)
	{
		if (g_Teleport->use == FALSE) continue;

		float px, py, pw, ph, tx, ty, tw, th;
		px = g_Teleport[i].pos.x;
		py = g_Teleport[i].pos.y;

		pw = TEXTURE_TELEPORT_WIDTH;
		ph = TEXTURE_TELEPORT_HEIGHT;

		tw = 1.0f / TEXTURE_TELEPORT_PATTERN_DIVIDE_X; 
		th = 1.0f / TEXTURE_TELEPORT_PATTERN_DIVIDE_Y;

		tx = (float)(g_Teleport[i].patternAnim % TEXTURE_TELEPORT_PATTERN_DIVIDE_X * tw);
		ty = (float)(g_Teleport[i].patternAnim / TEXTURE_TELEPORT_PATTERN_DIVIDE_X * th);


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TELEPORT_TEXTURE_NO]);
		SetSpriteColor(g_VertexBuffer,
			px - g_BG.pos.x, py - g_BG.pos.y,
			pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

	}
}

//=============================================================================
// BG構造体の先頭アドレスを取得
//=============================================================================
BG* GetBG(void)
{
	return &g_BG;
}


AABB* GetMapAABB(void)
{
	return g_AABB;
}


EnemyConfig* GetEnemyConfig(void)
{
	return g_EnemyConfig;
}

XMFLOAT3 GetPlayerInitPos(int map, int idx)
{
	return g_PlayerInitPos[map][idx];
}

int GetCurrentMap()
{
	return g_MapNo;
}
void SetCurrentMap(int map)
{
	if (map >= MAP_NUM_MAX) return;

	g_MapNo = map;
}

void DisableTeleport(int teleportNo, BOOL disable)
{
	if (teleportNo >= TELEPORT_NUM_MAX) return;

	g_Teleport[teleportNo].use = disable == FALSE ? TRUE : FALSE;
}

void ScrollBG(float x, float y, float time)
{
	g_BG.scroll = TRUE;

	g_BG.scrollTime = time;
	g_BG.scrollSpeedX = x / time;
	g_BG.scrollSpeedY = y / time;
}

void UpdateMapDraw(void)
{
	g_MapDraw = g_MapNo;
}