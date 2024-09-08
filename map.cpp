//=============================================================================
//
// map処理 [map.cpp]
// Author : 
//
//=============================================================================
#include "map.h"
#include "enemy.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_TELEPORT_WIDTH				(150.0f)
#define TEXTURE_TELEPORT_HEIGHT				(150.0f)
#define TEXTURE_TELEPORT_PATTERN_DIVIDE_X	(5)
#define TEXTURE_TELEPORT_PATTERN_DIVIDE_Y	(2)
#define	ANIM_WAIT_TELEPORT					(5)

#define TUTORIAL_01_GROUND_WIDTH	(TEXTURE_BG_WIDTH + 800.0f)
#define TUTORIAL_01_GROUND_HEIGHT	(250)
#define TUTORIAL_01_GROUND_POS_X	(TEXTURE_BG_WIDTH * 0.5f - 50)
#define TUTORIAL_01_GROUND_POS_Y	(TEXTURE_BG_HEIGHT)

#define TUTORIAL_01_ROCK01_WIDTH	(1600)
#define TUTORIAL_01_ROCK01_HEIGHT	(400)
#define TUTORIAL_01_ROCK01_POS_X	(2550)
#define TUTORIAL_01_ROCK01_POS_Y	(TEXTURE_BG_HEIGHT - 200)

#define TUTORIAL_01_EXIT_POS_X		(3000.0f)
#define TUTORIAL_01_EXIT_POS_Y		(800.0f)

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
	"data/TEXTURE/map/tutorial02.jpg",
	"data/TEXTURE/map/map01.png",
	"data/TEXTURE/map/rock01.png",
	"data/TEXTURE/map/rock02.png",
	"data/TEXTURE/map/teleport.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG	g_BG;
static AABB g_AABB[MAP_GROUND_MAX];
static int	g_MapNo = MAP_01;
static Teleport g_Teleport[MAP_NUM_MAX][TELEPORT_NUM_MAX];

static INTERPOLATION_DATA g_MoveTbl0[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_MoveTbl1[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_MoveTbl2[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Goblin_01[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Goblin_02[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_Goblin_03[MOVE_NUM_MAX];
static INTERPOLATION_DATA g_Tutorial_01_MoveTbl_CYCLOPS_01[MOVE_NUM_MAX];

static EnemyConfig	g_EnemyConfig[MAP_NUM_MAX][MAP_ENEMY_MAX];
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
	g_BG.w     = TEXTURE_BG_WIDTH;
	g_BG.h     = TEXTURE_BG_HEIGHT;
	g_BG.pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.texNo = 0;

	g_BG.scroll = FALSE;
	g_BG.scrollSpeedX = 0.0f;
	g_BG.scrollSpeedY = 0.0f;

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

	g_Load = FALSE;
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
		g_AABB[0].pos.x = TEXTURE_BG_WIDTH * 0.5f;
		g_AABB[0].pos.y = 1340.0f + (TEXTURE_BG_HEIGHT - 1290.0f) * 0.5f;
		g_AABB[0].w = TEXTURE_BG_WIDTH + 200.0f;
		g_AABB[0].h = MAP01_GROUND_H * 0.5f;

		g_AABB[1].pos.x = TUTORIAL_01_ROCK01_POS_X * 0.99f;
		g_AABB[1].pos.y = TUTORIAL_01_ROCK01_POS_Y;
		g_AABB[1].w = TUTORIAL_01_ROCK01_WIDTH * 0.83f;
		g_AABB[1].h = TUTORIAL_01_ROCK01_HEIGHT * 0.78f;
		break;
	}
	case MAP_01:
	{
		g_AABB[0].pos.x = TEXTURE_BG_WIDTH * 0.5f;
		g_AABB[0].pos.y = 1290.0f + (TEXTURE_BG_HEIGHT - 1290.0f) * 0.5f;
		g_AABB[0].w = TEXTURE_BG_WIDTH;
		g_AABB[0].h = MAP01_GROUND_H * 0.5f;

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
		g_Tutorial_01_MoveTbl_Goblin_01[1] = { XMFLOAT3(938.0f, PLAYER_INIT_POS_Y_TUTORIAL01_0, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Tutorial_01_MoveTbl_Goblin_02[0] = { XMFLOAT3(1675.0f, 1349.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Tutorial_01_MoveTbl_Goblin_02[1] = { XMFLOAT3(2257.0f, PLAYER_INIT_POS_Y_TUTORIAL01_0, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Tutorial_01_MoveTbl_Goblin_03[0] = { XMFLOAT3(2454.0f, 1349.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };
		g_Tutorial_01_MoveTbl_Goblin_03[1] = { XMFLOAT3(1753, PLAYER_INIT_POS_Y_TUTORIAL01_0, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_Tutorial_01_MoveTbl_CYCLOPS_01[0] = { XMFLOAT3(2805.0f, 1078.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	200 };
		g_Tutorial_01_MoveTbl_CYCLOPS_01[1] = { XMFLOAT3(2105, 1078.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	150 };

		break;
	case MAP_01:
		//座標									回転率							拡大率					時間
		g_MoveTbl0[0] = { XMFLOAT3(1200.0f, 523.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_MoveTbl0[1] = { XMFLOAT3(1850.0f,  523.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_MoveTbl1[0] = { XMFLOAT3(1500.0f, 1284.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };
		g_MoveTbl1[1] = { XMFLOAT3(450.0f,  1284.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 };

		g_MoveTbl2[0] = {XMFLOAT3(3000.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60};
		g_MoveTbl2[1] = { XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 };
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
		g_PlayerInitPos[TUTORIAL_01][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_TUTORIAL01_0, PLAYER_INIT_POS_Y_TUTORIAL01_0, 0.0f);
		break;
	case MAP_01:
		g_PlayerInitPos[MAP_01][INITPOS_01] = XMFLOAT3(PLAYER_INIT_POS_X_MAP01_0, PLAYER_INIT_POS_Y_MAP01_0, 0.0f);
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
		//g_EnemyConfig[TUTORIAL_01][0] = { GOBLIN, g_Tutorial_01_MoveTbl_Goblin_01, 2 };
		//g_EnemyConfig[TUTORIAL_01][1] = { GOBLIN, g_Tutorial_01_MoveTbl_Goblin_02, 2 };
		//g_EnemyConfig[TUTORIAL_01][2] = { GOBLIN, g_Tutorial_01_MoveTbl_Goblin_03, 2 };
		g_EnemyConfig[TUTORIAL_01][TUTORIAL_CYCLOPS_ID] = { CYCLOPS, g_Tutorial_01_MoveTbl_CYCLOPS_01, 2 };
		break;
	case MAP_01:
		g_EnemyConfig[MAP_01][0] = {GOLEM, g_MoveTbl0, 2};
		g_EnemyConfig[MAP_01][1] = {CYCLOPS, g_MoveTbl1, 2};
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


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG.texNo]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			0 - g_BG.pos.x, 0 - g_BG.pos.y, g_BG.w, g_BG.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	DrawMapWalls(g_MapNo);
	DrawTeleport(g_MapNo);


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
				SetSpriteColorRotation(g_AABBVertexBuffer, g_AABB[i].pos.x - g_BG.pos.x, g_AABB[i].pos.y - g_BG.pos.y, g_AABB[i].w, g_AABB[i].h,
					0.0f, 0.0f, 0.0f, 0.0f,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 0.2f),
					0.0f,
					vertexOffset);
			else if (g_AABB[i].tag == GROUND_AABB)
				SetSpriteColorRotation(g_AABBVertexBuffer, g_AABB[i].pos.x - g_BG.pos.x, g_AABB[i].pos.y - g_BG.pos.y, g_AABB[i].w, g_AABB[i].h,
					0.0f, 0.0f, 0.0f, 0.0f,
					XMFLOAT4(1.0f, 1.0f, 0.0f, 0.2f),
					0.0f,
					vertexOffset);

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
		std::cout << g_BG.pos.x << std::endl;

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_01]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			TUTORIAL_01_GROUND_POS_X - g_BG.pos.x, TUTORIAL_01_GROUND_POS_Y - g_BG.pos.y,
			TUTORIAL_01_GROUND_WIDTH, TUTORIAL_01_GROUND_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ROCK_02]);
		SetSpriteColor(g_VertexBuffer,
			TUTORIAL_01_ROCK01_POS_X - g_BG.pos.x, TUTORIAL_01_ROCK01_POS_Y - g_BG.pos.y,
			TUTORIAL_01_ROCK01_WIDTH, TUTORIAL_01_ROCK01_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		break;
	}
	default:
		break;
	}
}

void DrawTeleport(int map)
{
	for (int i = 0; i < TELEPORT_NUM_MAX; i++)
	{
		float px, py, pw, ph, tx, ty, tw, th;
		px = g_Teleport[map][i].pos.x;
		py = g_Teleport[map][i].pos.y;

		pw = TEXTURE_TELEPORT_WIDTH;
		ph = TEXTURE_TELEPORT_HEIGHT;
		tw = 1.0f / TEXTURE_TELEPORT_PATTERN_DIVIDE_X; 
		th = 1.0f / TEXTURE_TELEPORT_PATTERN_DIVIDE_Y;

		//ty = (float)(g_Magic[i].patternAnim / TEXTURE_TELEPORT_PATTERN_DIVIDE_X * th);

		//tx = (float)(g_Magic[i].patternAnim % TEXTURE_TELEPORT_PATTERN_DIVIDE_X * tw):


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[map]);
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


AABB* GetMap01AABB(void)
{
	return g_AABB;
}


EnemyConfig* GetEnemyConfig(int map)
{
	return g_EnemyConfig[map];
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
	g_MapNo = map;
}

void ScrollBG(float x, float y, float time)
{
	g_BG.scroll = TRUE;

	g_BG.scrollTime = time;
	g_BG.scrollSpeedX = x / time;
	g_BG.scrollSpeedY = y / time;
}