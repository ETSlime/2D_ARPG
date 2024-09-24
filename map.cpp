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
#include "input.h"
#include "UI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#ifdef _DEBUG
#define TEXTURE_WALL_NUM_WIDTH				(24)
#define TEXTURE_WALL_NUM_HEIGHT				(40)
#endif // _DEBUG

#define TUTORIAL_01_GROUND_WIDTH	(TEXTURE_BG_TUTORIAL_01_WIDTH + 800.0f)
#define TUTORIAL_01_GROUND_HEIGHT	(250)
#define TUTORIAL_01_GROUND_POS_X	(TEXTURE_BG_TUTORIAL_01_WIDTH * 0.5f - 50)
#define TUTORIAL_01_GROUND_POS_Y	(TEXTURE_BG_TUTORIAL_01_HEIGHT)

#define TUTORIAL_01_ROCK01_WIDTH	(1600)
#define TUTORIAL_01_ROCK01_HEIGHT	(400)
#define TUTORIAL_01_ROCK01_POS_X	(2550)
#define TUTORIAL_01_ROCK01_POS_Y	(TEXTURE_BG_TUTORIAL_01_HEIGHT - 200)

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



// teleport
#define TEXTURE_TELEPORT_WIDTH				(125.0f)
#define TEXTURE_TELEPORT_HEIGHT				(125.0f)
#define TEXTURE_TELEPORT_PATTERN_DIVIDE_X	(5)
#define TEXTURE_TELEPORT_PATTERN_DIVIDE_Y	(2)
#define	ANIM_WAIT_TELEPORT					(20)

#define TUTORIAL_01_EXIT_POS_X		(3680.0f)
#define TUTORIAL_01_EXIT_POS_Y		(1350.0f)

#define MAP_01_TELEPORT_01_POS_X	(PLAYER_INIT_POS_X_MAP01_02 + 150.0f)
#define MAP_01_TELEPORT_01_POS_Y	(PLAYER_INIT_POS_Y_MAP01_02)

#define MAP_02_TELEPORT_01_POS_X	(PLAYER_INIT_POS_X_MAP02_01 - 100.0f)
#define MAP_02_TELEPORT_01_POS_Y	(PLAYER_INIT_POS_Y_MAP02_01)
#define MAP_02_TELEPORT_02_POS_X	(PLAYER_INIT_POS_X_MAP02_02 + 100.0f)
#define MAP_02_TELEPORT_02_POS_Y	(PLAYER_INIT_POS_Y_MAP02_02)

#define MAP_BOSS_TELEPORT_01_POS_X	(PLAYER_INIT_POS_X_MAP_BOSS_01 - 120.0f)
#define MAP_BOSS_TELEPORT_01_POS_Y	(PLAYER_INIT_POS_Y_MAP_BOSS_01)

// sculpture
#define TEXTURE_SCULPTURE_WIDTH				(156.0f)
#define TEXTURE_SCULPTURE_HEIGHT			(255.0f)
#define TEXTURE_SCULPTURE_MSG_WIDTH			(136.0f)
#define TEXTURE_SCULPTURE_MSG_HEIGHT		(45.0f)

#define MAP_01_SCULPTURE_POS_X				(566.0f)
#define MAP_01_SCULPTURE_POS_Y				(1971.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#ifdef _DEBUG
void DrawWallNum(MapWall* wall, int idx);
#endif


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
	"data/TEXTURE/number.png",
	"data/TEXTURE/sculpture.png",
	"data/TEXTURE/UI/action_UI.png",
};


static BOOL			g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG			g_BG;
static int			g_MapNo = MAP_01;
static int			g_MapDrawNo = MAP_01;
static Teleport		g_Teleport[TELEPORT_NUM_MAX];
static Sculpture	g_Sculpture[SCULPTURE_NUM_MAX];

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
static XMFLOAT3 g_TeleportInitPos[MAP_NUM_MAX][PLAYER_INIT_POS_MAX];
static MapWall	g_MapWall[MAP_WALL_MAX];

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

	for (int i = 0; i < MAP_WALL_MAX; i++)
	{
		g_MapWall[i].use = FALSE;
		g_MapWall[i].w = 0.0f;
		g_MapWall[i].h = 0.0f;
		g_MapWall[i].texNo = TEXTURE_NONE;
		g_MapWall[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_MapWall[i].wallAABB.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_MapWall[i].wallAABB.w = 0.0f;
		g_MapWall[i].wallAABB.h = 0.0f;
		g_MapWall[i].wallAABB.tag = WALL_AABB;
		
	}

	// 変数の初期化
	InitMapBG(g_MapNo);
	InitMoveTbl(g_MapNo);
	InitEnemyConfig(g_MapNo);
	InitTeleport(g_MapNo);
	InitSculpture();

#ifdef _DEBUG	
	// debug
	{
		int aabbCount = MAP_WALL_MAX;
		const int maxVertices = MAP_WALL_MAX * 4;

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

	for (int i = 0; i < MAP_ENEMY_MAX; i++)
	{
		g_EnemyConfig[i] = {};
	}

	for (int i = 0; i < MAP_NUM_MAX; i++)
	{
		for (int j = 0; j < TELEPORT_NUM_MAX; j++)
		{
			g_TeleportInitPos[i][j] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int i = 0; i < TELEPORT_NUM_MAX; i++)
	{
		g_Teleport[i].use = FALSE;
		g_Teleport[i].teleportAABB.h = 0.0f;
		g_Teleport[i].teleportAABB.w = 0.0f;
		g_Teleport[i].teleportAABB.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < MAP_WALL_MAX; i++)
	{
		g_MapWall[i].use = FALSE;
		g_MapWall[i].w = 0.0f;
		g_MapWall[i].h = 0.0f;
		g_MapWall[i].texNo = 0;
		g_MapWall[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_MapWall[i].wallAABB.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_MapWall[i].wallAABB.w = 0.0f;
		g_MapWall[i].wallAABB.h = 0.0f;
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
		g_Map_01_MoveTbl_Goblin_01[0] = { XMFLOAT3(1350.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Map_01_MoveTbl_Goblin_01[1] = { XMFLOAT3(1938.0f, PLAYER_INIT_POS_Y_MAP01_01, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

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

void InitTeleportInitPos(int map)
{
	switch (map)
	{
	case TUTORIAL_01:
		g_TeleportInitPos[TUTORIAL_01][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_TUTORIAL01_01, PLAYER_INIT_POS_Y_TUTORIAL01_01, 0.0f);
		break;
	case MAP_01:
		g_TeleportInitPos[MAP_01][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP01_01, PLAYER_INIT_POS_Y_MAP01_01, 0.0f);
		g_TeleportInitPos[MAP_01][INITPOS_02] = XMFLOAT3(PLAYER_INIT_POS_X_MAP01_02, PLAYER_INIT_POS_Y_MAP01_02, 0.0f);
		break;
	case MAP_02:
		g_TeleportInitPos[MAP_02][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP02_01, PLAYER_INIT_POS_Y_MAP02_01, 0.0f);
		g_TeleportInitPos[MAP_02][INITPOS_02] = XMFLOAT3(PLAYER_INIT_POS_X_MAP02_02, PLAYER_INIT_POS_Y_MAP02_02, 0.0f);
		break;
	case MAP_BOSS:
		g_TeleportInitPos[MAP_BOSS][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP_BOSS_01, PLAYER_INIT_POS_Y_MAP_BOSS_01, 0.0f);
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
		SetTeleport(&g_Teleport[0], TUTORIAL_01_EXIT_POS_X, TUTORIAL_01_EXIT_POS_Y, MAP_01, INITPOS_01);
		break;
	case MAP_01:
		SetTeleport(&g_Teleport[0], MAP_01_TELEPORT_01_POS_X, MAP_01_TELEPORT_01_POS_Y, MAP_02, INITPOS_01);
		break;
	case MAP_02:
		SetTeleport(&g_Teleport[0], MAP_02_TELEPORT_01_POS_X, MAP_02_TELEPORT_01_POS_Y, MAP_01, INITPOS_02);
		SetTeleport(&g_Teleport[1], MAP_02_TELEPORT_02_POS_X, MAP_02_TELEPORT_02_POS_Y, MAP_BOSS, INITPOS_01);
		break;
	case MAP_BOSS:
		SetTeleport(&g_Teleport[0], MAP_BOSS_TELEPORT_01_POS_X, MAP_BOSS_TELEPORT_01_POS_Y, MAP_02, INITPOS_02);
		break;
	default:
		break;
	}
}

void InitSculpture(void)
{
	for (int i = 0; i < SCULPTURE_NUM_MAX; i++)
	{
		g_Sculpture[i].sculptureAABB.tag = SCULPTURE_AABB;
		g_Sculpture[i].w = TEXTURE_SCULPTURE_WIDTH;
		g_Sculpture[i].h = TEXTURE_SCULPTURE_HEIGHT;
		g_Sculpture[i].sculptureAABB.w = TEXTURE_SCULPTURE_WIDTH;
		g_Sculpture[i].sculptureAABB.h = TEXTURE_SCULPTURE_HEIGHT;
		g_Sculpture[i].drawMsg = FALSE;
	}
	g_Sculpture[0].id = 0;
	g_Sculpture[0].location = MAP_01;
	g_Sculpture[0].pos = XMFLOAT3(MAP_01_SCULPTURE_POS_X, MAP_01_SCULPTURE_POS_Y, 0.0f);
	g_Sculpture[0].sculptureAABB.pos = g_Sculpture[0].pos;

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
	UpdateSculpture();

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

void UpdateSculpture(void)
{
	for (int i = 0; i < SCULPTURE_NUM_MAX; i++)
	{
		if (g_Sculpture[i].location != g_MapNo) continue;

		AABB sculptureBox = g_Sculpture[i].sculptureAABB;
		PLAYER* player = GetPlayer();

		// プレイヤーのAABB情報を取得
		XMFLOAT3 playerPos = player->bodyAABB.pos;
		float playerW = player->bodyAABB.w;
		float playerH = player->bodyAABB.h;

		// テレポートのAABB情報を取得
		XMFLOAT3 sculpturePos = sculptureBox.pos;
		float sculptureW = sculptureBox.w;
		float sculptureH = sculptureBox.h;

		// プレイヤーの包囲ボックスとテレポートの包囲ボックスが重なっているかを確認
		BOOL isColliding = CollisionBB(playerPos, playerW, playerH, sculpturePos, sculptureW, sculptureH);

		if (isColliding)
		{
			g_Sculpture[i].drawMsg = TRUE;
			if (GetKeyboardRelease(DIK_S))
			{
				SetFade(FADE_OUT, MODE_MENU);
				SetPlayerInitPos(g_Sculpture[i].pos.x, player->pos.y);
				InitSystemMenu();
				ResetCursor();
			}
		}
		else
		{
			g_Sculpture[i].drawMsg = FALSE;
		}
	}
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


void UpdateMapWall(int mapNo)
{
	switch (mapNo)
	{
	case TUTORIAL_01:
		g_MapWall[0].use = TRUE;
		g_MapWall[0].texNo = TEXTURE_ROCK_01;
		g_MapWall[0].w = TUTORIAL_01_GROUND_WIDTH;
		g_MapWall[0].h = TUTORIAL_01_GROUND_HEIGHT;
		g_MapWall[0].pos = XMFLOAT3(TUTORIAL_01_GROUND_POS_X, TUTORIAL_01_GROUND_POS_Y, 0.0f);

		g_MapWall[0].wallAABB.pos.x = TEXTURE_BG_TUTORIAL_01_WIDTH * 0.5f;
		g_MapWall[0].wallAABB.pos.y = 1340.0f + (TEXTURE_BG_TUTORIAL_01_HEIGHT - 1290.0f) * 0.5f;
		g_MapWall[0].wallAABB.w = TEXTURE_BG_TUTORIAL_01_WIDTH + 200.0f;
		g_MapWall[0].wallAABB.h = TUTORIAL01_GROUND_H * 0.5f;

		g_MapWall[1].use = TRUE;
		g_MapWall[1].texNo = TEXTURE_ROCK_02;
		g_MapWall[1].w = TUTORIAL_01_ROCK01_WIDTH;
		g_MapWall[1].h = TUTORIAL_01_ROCK01_HEIGHT;
		g_MapWall[1].pos = XMFLOAT3(TUTORIAL_01_ROCK01_POS_X, TUTORIAL_01_ROCK01_POS_Y, 0.0f);

		g_MapWall[1].wallAABB.pos.x = TUTORIAL_01_ROCK01_POS_X * 0.99f;
		g_MapWall[1].wallAABB.pos.y = TUTORIAL_01_ROCK01_POS_Y;
		g_MapWall[1].wallAABB.w = TUTORIAL_01_ROCK01_WIDTH * 0.83f;
		g_MapWall[1].wallAABB.h = TUTORIAL_01_ROCK01_HEIGHT * 0.78f;

		break;

	case MAP_01:
		g_MapWall[0].use = TRUE;
		g_MapWall[0].texNo = TEXTURE_ROCK_02;
		g_MapWall[0].w = MAP_01_GROUND_WIDTH;
		g_MapWall[0].h = MAP_01_GROUND_HEIGHT;
		g_MapWall[0].pos = XMFLOAT3(MAP_01_GROUND_POS_X, MAP_01_GROUND_POS_Y, 0.0f);

		g_MapWall[0].wallAABB.pos.x = MAP_01_GROUND_POS_X;
		g_MapWall[0].wallAABB.pos.y = MAP_01_GROUND_POS_Y + 5.0f;
		g_MapWall[0].wallAABB.w = TEXTURE_BG_MAP_01_WIDTH;
		g_MapWall[0].wallAABB.h = MAP01_GROUND_H;

		g_MapWall[1].use = TRUE;
		g_MapWall[1].texNo = TEXTURE_ROCK_01;
		g_MapWall[1].w = MAP_01_ROCK01_WIDTH;
		g_MapWall[1].h = MAP_01_ROCK01_HEIGHT;
		g_MapWall[1].pos = XMFLOAT3(MAP_01_ROCK01_POS_X, MAP_01_ROCK01_POS_Y, 0.0f);

		g_MapWall[1].wallAABB.pos.x = MAP_01_ROCK01_POS_X - 25.0f;
		g_MapWall[1].wallAABB.pos.y = MAP_01_ROCK01_POS_Y + 12.0f;
		g_MapWall[1].wallAABB.w = MAP_01_ROCK01_WIDTH * 0.83f;
		g_MapWall[1].wallAABB.h = MAP_01_ROCK01_HEIGHT * 0.73f;

		g_MapWall[2].use = TRUE;
		g_MapWall[2].texNo = TEXTURE_ROCK_02;
		g_MapWall[2].w = MAP_01_ROCK02_WIDTH;
		g_MapWall[2].h = MAP_01_ROCK02_HEIGHT;
		g_MapWall[2].pos = XMFLOAT3(MAP_01_ROCK02_POS_X, MAP_01_ROCK02_POS_Y, 0.0f);

		g_MapWall[2].wallAABB.pos.x = MAP_01_ROCK02_POS_X - 8.0f;
		g_MapWall[2].wallAABB.pos.y = MAP_01_ROCK02_POS_Y;
		g_MapWall[2].wallAABB.w = MAP_01_ROCK02_WIDTH * 0.81f;
		g_MapWall[2].wallAABB.h = MAP_01_ROCK02_HEIGHT * 0.75f;

		g_MapWall[3].use = TRUE;
		g_MapWall[3].texNo = TEXTURE_ROCK_01;
		g_MapWall[3].w = MAP_01_ROCK03_WIDTH;
		g_MapWall[3].h = MAP_01_ROCK03_HEIGHT;
		g_MapWall[3].pos = XMFLOAT3(MAP_01_ROCK03_POS_X, MAP_01_ROCK03_POS_Y, 0.0f);

		g_MapWall[3].wallAABB.pos.x = MAP_01_ROCK03_POS_X;
		g_MapWall[3].wallAABB.pos.y = MAP_01_ROCK03_POS_Y;
		g_MapWall[3].wallAABB.w = MAP_01_ROCK03_WIDTH * 0.8f;
		g_MapWall[3].wallAABB.h = MAP_01_ROCK03_HEIGHT * 0.8f;

		g_MapWall[4].use = TRUE;
		g_MapWall[4].texNo = TEXTURE_ROCK_02;
		g_MapWall[4].w = MAP_01_ROCK04_WIDTH;
		g_MapWall[4].h = MAP_01_ROCK04_HEIGHT;
		g_MapWall[4].pos = XMFLOAT3(MAP_01_ROCK04_POS_X, MAP_01_ROCK04_POS_Y, 0.0f);

		g_MapWall[4].wallAABB.pos.x = MAP_01_ROCK04_POS_X - 25.0f;
		g_MapWall[4].wallAABB.pos.y = MAP_01_ROCK04_POS_Y;
		g_MapWall[4].wallAABB.w = MAP_01_ROCK04_WIDTH * 0.84f;
		g_MapWall[4].wallAABB.h = MAP_01_ROCK04_HEIGHT * 0.8f;

		g_MapWall[5].use = TRUE;
		g_MapWall[5].texNo = TEXTURE_ROCK_02;
		g_MapWall[5].w = MAP_01_ROCK05_WIDTH;
		g_MapWall[5].h = MAP_01_ROCK05_HEIGHT;
		g_MapWall[5].pos = XMFLOAT3(MAP_01_ROCK05_POS_X, MAP_01_ROCK05_POS_Y, 0.0f);

		g_MapWall[5].wallAABB.pos.x = MAP_01_ROCK05_POS_X;
		g_MapWall[5].wallAABB.pos.y = MAP_01_ROCK05_POS_Y;
		g_MapWall[5].wallAABB.w = MAP_01_ROCK05_WIDTH * 0.8f;
		g_MapWall[5].wallAABB.h = MAP_01_ROCK05_HEIGHT * 0.8f;

		g_MapWall[6].use = TRUE;
		g_MapWall[6].texNo = TEXTURE_ROCK_02;
		g_MapWall[6].w = MAP_01_ROCK06_WIDTH;
		g_MapWall[6].h = MAP_01_ROCK06_HEIGHT;
		g_MapWall[6].pos = XMFLOAT3(MAP_01_ROCK06_POS_X, MAP_01_ROCK06_POS_Y, 0.0f);

		g_MapWall[6].wallAABB.pos.x = MAP_01_ROCK06_POS_X - 25.0f;
		g_MapWall[6].wallAABB.pos.y = MAP_01_ROCK06_POS_Y;
		g_MapWall[6].wallAABB.w = MAP_01_ROCK06_WIDTH * 0.83f;
		g_MapWall[6].wallAABB.h = MAP_01_ROCK06_HEIGHT * 0.75f;

		break;
	case MAP_02:
	{
		g_MapWall[0].use = TRUE;
		g_MapWall[0].texNo = TEXTURE_ROCK_02;
		g_MapWall[0].w = MAP_02_ROCK01_WIDTH;
		g_MapWall[0].h = MAP_02_ROCK01_HEIGHT;
		g_MapWall[0].pos = XMFLOAT3(MAP_02_ROCK01_POS_X, MAP_02_ROCK01_POS_Y, 0.0f);

		g_MapWall[0].wallAABB.pos.x = TEXTURE_BG_MAP_02_WIDTH * 0.5f;
		g_MapWall[0].wallAABB.pos.y = 1290.0f + (TEXTURE_BG_MAP_02_HEIGHT - 1290.0f) * 0.5f;
		g_MapWall[0].wallAABB.w = TEXTURE_BG_MAP_02_WIDTH;
		g_MapWall[0].wallAABB.h = MAP02_GROUND_H * 0.5f;

		g_MapWall[1].use = TRUE;
		g_MapWall[1].texNo = TEXTURE_ROCK_02;
		g_MapWall[1].w = MAP_02_ROCK02_WIDTH;
		g_MapWall[1].h = MAP_02_ROCK02_HEIGHT;
		g_MapWall[1].pos = XMFLOAT3(MAP_02_ROCK02_POS_X, MAP_02_ROCK02_POS_Y, 0.0f);

		g_MapWall[1].wallAABB.pos.x = MAP_02_ROCK01_POS_X;
		g_MapWall[1].wallAABB.pos.y = MAP_02_ROCK01_POS_Y;
		g_MapWall[1].wallAABB.w = MAP_02_ROCK01_WIDTH * 0.83f;
		g_MapWall[1].wallAABB.h = MAP_02_ROCK01_HEIGHT * 0.83f;

		g_MapWall[2].use = TRUE;
		g_MapWall[2].texNo = TEXTURE_ROCK_02;
		g_MapWall[2].w = MAP_02_ROCK03_WIDTH;
		g_MapWall[2].h = MAP_02_ROCK03_HEIGHT;
		g_MapWall[2].pos = XMFLOAT3(MAP_02_ROCK03_POS_X, MAP_02_ROCK03_POS_Y, 0.0f);

		g_MapWall[2].wallAABB.pos.x = 1450;
		g_MapWall[2].wallAABB.pos.y = 594;
		g_MapWall[2].wallAABB.w = 2200;
		g_MapWall[2].wallAABB.h = 40;


		g_MapWall[3].use = TRUE;
		g_MapWall[3].texNo = TEXTURE_NONE;
		g_MapWall[3].wallAABB.pos.x = 900;
		g_MapWall[3].wallAABB.pos.y = 857;
		g_MapWall[3].wallAABB.w = 640;
		g_MapWall[3].wallAABB.h = 40;

		g_MapWall[4].use = TRUE;
		g_MapWall[4].texNo = TEXTURE_NONE;
		g_MapWall[4].wallAABB.pos.x = 685;
		g_MapWall[4].wallAABB.pos.y = 1226;
		g_MapWall[4].wallAABB.w = 103;
		g_MapWall[4].wallAABB.h = 224;

		break;
	}
	case MAP_BOSS:
	{
		g_MapWall[0].use = TRUE;
		g_MapWall[0].texNo = TEXTURE_ROCK_02;
		g_MapWall[0].w = MAP_BOSS_GROUND_WIDTH;
		g_MapWall[0].h = MAP_BOSS_GROUND_HEIGHT;
		g_MapWall[0].pos = XMFLOAT3(MAP_BOSS_GROUND_POS_X, MAP_BOSS_GROUND_POS_Y, 0.0f);

		g_MapWall[0].wallAABB.pos.x = MAP_BOSS_GROUND_POS_X;
		g_MapWall[0].wallAABB.pos.y = MAP_BOSS_GROUND_POS_Y + 5.0f;
		g_MapWall[0].wallAABB.w = TEXTURE_BG_MAP_BOSS_WIDTH;
		g_MapWall[0].wallAABB.h = MAP_BOSS_GROUND_HEIGHT * 0.84f;

		g_MapWall[1].use = TRUE;
		g_MapWall[1].texNo = TEXTURE_ROCK_01;
		g_MapWall[1].w = MAP_BOSS_ROCK01_WIDTH;
		g_MapWall[1].h = MAP_BOSS_ROCK01_HEIGHT;
		g_MapWall[1].pos = XMFLOAT3(MAP_BOSS_ROCK01_POS_X, MAP_BOSS_ROCK01_POS_Y, 0.0f);

		g_MapWall[1].wallAABB.pos.x = MAP_BOSS_ROCK01_POS_X - 11.1f;
		g_MapWall[1].wallAABB.pos.y = MAP_BOSS_ROCK01_POS_Y + 12.3f;
		g_MapWall[1].wallAABB.w = MAP_BOSS_ROCK01_WIDTH * 0.83f;
		g_MapWall[1].wallAABB.h = MAP_BOSS_ROCK01_HEIGHT * 0.83f;
		
		g_MapWall[2].use = TRUE;
		g_MapWall[2].texNo = TEXTURE_ROCK_02;
		g_MapWall[2].w = MAP_BOSS_ROCK02_WIDTH;
		g_MapWall[2].h = MAP_BOSS_ROCK02_HEIGHT;
		g_MapWall[2].pos = XMFLOAT3(MAP_BOSS_ROCK02_POS_X, MAP_BOSS_ROCK02_POS_Y, 0.0f);

		g_MapWall[2].wallAABB.pos.x = MAP_BOSS_ROCK02_POS_X;
		g_MapWall[2].wallAABB.pos.y = MAP_BOSS_ROCK02_POS_Y;
		g_MapWall[2].wallAABB.w = MAP_BOSS_ROCK02_WIDTH * 0.83f;
		g_MapWall[2].wallAABB.h = MAP_BOSS_ROCK02_HEIGHT * 0.83f;

		g_MapWall[3].use = TRUE;
		g_MapWall[3].texNo = TEXTURE_ROCK_01;
		g_MapWall[3].w = MAP_BOSS_ROCK03_WIDTH;
		g_MapWall[3].h = MAP_BOSS_ROCK03_HEIGHT;
		g_MapWall[3].pos = XMFLOAT3(MAP_BOSS_ROCK03_POS_X, MAP_BOSS_ROCK03_POS_Y, 0.0f);

		g_MapWall[3].wallAABB.pos.x = MAP_BOSS_ROCK03_POS_X;
		g_MapWall[3].wallAABB.pos.y = MAP_BOSS_ROCK03_POS_Y;
		g_MapWall[3].wallAABB.w = MAP_BOSS_ROCK03_WIDTH * 0.83f;
		g_MapWall[3].wallAABB.h = MAP_BOSS_ROCK03_HEIGHT * 0.83f;

		g_MapWall[4].use = TRUE;
		g_MapWall[4].texNo = TEXTURE_ROCK_02;
		g_MapWall[4].w = MAP_BOSS_ROCK04_WIDTH;
		g_MapWall[4].h = MAP_BOSS_ROCK04_HEIGHT;
		g_MapWall[4].pos = XMFLOAT3(MAP_BOSS_ROCK04_POS_X, MAP_BOSS_ROCK04_POS_Y, 0.0f);

		g_MapWall[4].wallAABB.pos.x = MAP_BOSS_ROCK04_POS_X - 21.1f;
		g_MapWall[4].wallAABB.pos.y = MAP_BOSS_ROCK04_POS_Y;
		g_MapWall[4].wallAABB.w = MAP_BOSS_ROCK04_WIDTH * 0.85f;
		g_MapWall[4].wallAABB.h = MAP_BOSS_ROCK04_HEIGHT * 0.83f;
		break;
	}
	default:
		break;
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_MapDrawNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	DrawMapWalls(g_MapNo);
	DrawSculpture(g_MapNo);
	DrawTeleport();

#ifdef _DEBUG
		MATERIAL materialAABB;
		ZeroMemory(&materialAABB, sizeof(materialAABB));
		materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materialAABB.noTexSampling = 1;


		SetMaterial(materialAABB);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < MAP_WALL_MAX; ++i)
		{
			if (g_MapWall[i].use == FALSE) continue;

			int vertexOffset = i * 4;

			if (g_MapWall[i].wallAABB.tag == WALL_AABB)
				SetSpriteColorRotation(g_AABBVertexBuffer, 
					g_MapWall[i].wallAABB.pos.x - g_BG.pos.x, g_MapWall[i].wallAABB.pos.y - g_BG.pos.y,
					g_MapWall[i].wallAABB.w, g_MapWall[i].wallAABB.h,
					0.0f, 0.0f, 0.0f, 0.0f,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 0.2f),
					0.0f,
					vertexOffset);
			else if (g_MapWall[i].wallAABB.tag == GROUND_AABB)
				SetSpriteColorRotation(g_AABBVertexBuffer, 
					g_MapWall[i].wallAABB.pos.x - g_BG.pos.x, g_MapWall[i].wallAABB.pos.y - g_BG.pos.y,
					g_MapWall[i].wallAABB.w, g_MapWall[i].wallAABB.h,
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
	for (int i = 0; i < MAP_WALL_MAX; i++)
	{
		if (g_MapWall[i].texNo == TEXTURE_NONE || g_MapWall[i].use == FALSE) continue;

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_MapWall[i].texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_MapWall[i].pos.x - g_BG.pos.x, g_MapWall[i].pos.y - g_BG.pos.y,
			g_MapWall[i].w, g_MapWall[i].h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

#ifdef _DEBUG
		DrawWallNum(&g_MapWall[i], i);
#endif // _DEBUG

	}
}

void DrawSculpture(int map)
{
	for (int i = 0; i < SCULPTURE_NUM_MAX; i++)
	{
		if (g_Sculpture[i].location != map) continue;

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEXTURE_SCULPTURE]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			g_Sculpture[i].pos.x - g_BG.pos.x, g_Sculpture[i].pos.y - g_BG.pos.y,
			g_Sculpture[i].w, g_Sculpture[i].h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		if (g_Sculpture[i].drawMsg == TRUE && GetMode() != MODE_MENU)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEXTURE_SCULPTURE_MSG]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer,
				g_Sculpture[i].pos.x - g_BG.pos.x, g_Sculpture[i].pos.y - g_BG.pos.y + 155.0f,
				TEXTURE_SCULPTURE_MSG_WIDTH, TEXTURE_SCULPTURE_MSG_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

#ifdef _DEBUG
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;

		MATERIAL materialAABB;
		ZeroMemory(&materialAABB, sizeof(materialAABB));
		materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materialAABB.noTexSampling = 1;


		SetMaterial(materialAABB);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);

		int vertexOffset = i * 4;

		SetSpriteColorRotation(g_AABBVertexBuffer,
			g_Sculpture[i].sculptureAABB.pos.x - g_BG.pos.x, g_Sculpture[i].sculptureAABB.pos.y - g_BG.pos.y,
			g_Sculpture[i].sculptureAABB.w, g_Sculpture[i].sculptureAABB.h,
			0.0f, 0.0f, 0.0f, 0.0f,
			XMFLOAT4(0.5f, 0.5f, 0.5f, 0.2f),
			0.0f);

		GetDeviceContext()->Draw(4, vertexOffset);
#endif // _DEBUG

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


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEXTURE_TELEPORT]);
		SetSpriteColor(g_VertexBuffer,
			px - g_BG.pos.x, py - g_BG.pos.y,
			pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

	}
}


void DrawWallNum(MapWall* wall, int idx)
{
#ifdef _DEBUG
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEXTURE_NUMBER]);
	// 桁数分処理する
	int number = idx;
	for (int i = 0; i < 3; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(number % 10);


		float px = wall->pos.x - TEXTURE_WALL_NUM_WIDTH * i - g_BG.pos.x;
		float py = wall->pos.y - g_BG.pos.y;
		float pw = TEXTURE_WALL_NUM_WIDTH;	
		float ph = TEXTURE_WALL_NUM_HEIGHT;

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		number /= 10;
	}
#endif // _DEBUG	
}

//=============================================================================
// BG構造体の先頭アドレスを取得
//=============================================================================
BG* GetBG(void)
{
	return &g_BG;
}


MapWall* GetMapWall(void)
{
	return g_MapWall;
}


EnemyConfig* GetEnemyConfig(void)
{
	return g_EnemyConfig;
}

XMFLOAT3 GetTeleportInitPos(int map, int idx)
{
	return g_TeleportInitPos[map][idx];
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
	g_MapDrawNo = g_MapNo;
	UpdateMapWall(g_MapDrawNo);
}

void SetTeleport(Teleport* teleport, float initPosX, float initPosY, int nextMapNo, int nextPosIdx)
{
	teleport->pos.x = initPosX;
	teleport->pos.y = initPosY;
	teleport->nextMapNo = nextMapNo;
	teleport->nextInitPos = nextPosIdx;
	teleport->use = TRUE;
	teleport->teleportAABB.pos = teleport->pos;
	teleport->teleportAABB.w = TEXTURE_TELEPORT_WIDTH * 0.5f;
	teleport->teleportAABB.h = TEXTURE_TELEPORT_HEIGHT * 0.5f;
	teleport->countAnim = 0.0f;
	teleport->patternAnim = 0;
}