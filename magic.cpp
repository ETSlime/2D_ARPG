//=============================================================================
//
// UI処理 [UI.cpp]
// Author : 
//
//=============================================================================
#include "magic.h"
#include "player.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_FIRE_BALL_WIDTH					(155)
#define TEXTURE_FIRE_BALL_HEIGHT				(95)
#define TEXTURE_BOOM_WIDTH						(250)
#define TEXTURE_BOOM_HEIGHT						(250)
#define TEXTURE_HEAL_WIDTH						(150)
#define TEXTURE_HEAL_HEIGHT						(210)
#define TEXTURE_STOMP_WIDTH						(250)
#define TEXTURE_STOMP_HEIGHT					(110)
#define TEXTURE_EARTH_WIDTH						(360)
#define TEXTURE_EARTH_HEIGHT					(580)
#define TEXTURE_THUNDER_WIDTH					(150)
#define TEXTURE_THUNDER_HEIGHT					(SCREEN_HEIGHT)
#define TEXTURE_BARRAGE_WIDTH					(125)
#define TEXTURE_BARRAGE_HEIGHT					(125)

#define MAGIC_FIRE_BALL_OFFSET					(50.0f)
#define MAGIC_STOMP_OFFSET_X					(55.0f)
#define MAGIC_STOMP_OFFSET_Y					(-50.0f)

#define	ANIM_WAIT_FIRE_BALL						(3)
#define	ANIM_WAIT_BOOM							(6)
#define ANIM_WAIT_HEAL							(4)
#define ANIM_WAIT_STOMP							(8)
#define ANIM_WAIT_EARTH							(24)
#define ANIM_WAIT_THUNDER						(5)
#define ANIM_WAIT_BARRAGE						(8)
#define	TEXTURE_FIRE_BALL_PATTERN_DIVIDE_X		(27)
#define TEXTURE_FIRE_BALL_PATTERN_DIVIDE_Y		(1)
#define	TEXTURE_BOOM_PATTERN_DIVIDE_X			(4)
#define	TEXTURE_BOOM_PATTERN_DIVIDE_Y			(4)
#define	TEXTURE_HEAL_PATTERN_DIVIDE_X			(26)
#define	TEXTURE_HEAL_PATTERN_DIVIDE_Y			(1)
#define TEXTURE_STOMP_PATTERN_DIVIDE_X			(11)
#define TEXTURE_STOMP_PATTERN_DIVIDE_Y			(1)
#define	TEXTURE_EARTH_PATTERN_DIVIDE_X			(8)
#define	TEXTURE_EARTH_PATTERN_DIVIDE_Y			(1)
#define TEXTURE_THUNDER_PATTERN_DIVIDE_X		(8)
#define TEXTURE_THUNDER_PATTERN_DIVIDE_Y		(1)
#define TEXTURE_BARRAGE_PATTERN_DIVIDE_X		(4)
#define TEXTURE_BARRAGE_PATTERN_DIVIDE_Y		(4)


#define MAGIC_FIRE_BALL_SPEED					(6.0f)
#define MAGIC_STOMP_SPEED						(3.5f)
#define MAGIC_BARRAGE_SPEED						(4.0f)

#define TEXTURE_MAX								(20)


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static AABB			g_BoomAABBTbl[TEXTURE_BOOM_PATTERN_DIVIDE_X * TEXTURE_BOOM_PATTERN_DIVIDE_Y] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.1f, 0.1f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.1f, 0.1f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.1f, 0.1f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.3f, 0.3f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.6f, 0.6f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.6f, 0.6f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.6f, 0.6f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.6f, 0.6f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.6f, 0.6f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AABB			g_EarthAABBTbl[TEXTURE_EARTH_PATTERN_DIVIDE_X * TEXTURE_EARTH_PATTERN_DIVIDE_Y] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 50.0f, 0.0f), 0.1f, 0.2f},
	{XMFLOAT3(0.0f, 25.0f, 0.0f), 0.3f, 0.5f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.7f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.7f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, 0.7f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.3f, 0.4f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

#ifdef _DEBUG	
static ID3D11Buffer* g_AABBVertexBuffer = NULL;
#endif

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/magic/heal.png",
	"data/TEXTURE/magic/fireball.png",
	"data/TEXTURE/UI/flameblade_icon.png",
	"data/TEXTURE/magic/boom.png",
	"data/TEXTURE/magic/stomp.png",
	"data/TEXTURE/magic/earth.png",
	"data/TEXTURE/magic/thunder.png",
	"data/TEXTURE/magic/barrage.png",

};

static Magic					g_Magic[MAGIC_MAX];	// 魔法の構造体
static BOOL						g_Load = FALSE;	

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMagic(void)
{
	ID3D11Device* pDevice = GetDevice();

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
	for (int i = 0; i < MAGIC_MAX; i++)
	{
		ClearMagic(&g_Magic[i]);
	}

#ifdef _DEBUG	
	{
		int aabbCount = MAGIC_MAX;
		int maxVertices = MAGIC_MAX * 4;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * maxVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_AABBVertexBuffer);
	}
#endif

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagic(void)
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

void UpdateMagic(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < MAGIC_MAX; i++)
	{
		if (g_Magic[i].use == TRUE)
		{
			PlayMagicAnim(&g_Magic[i]);

			// 画面外まで進んだ？
			BG* bg = GetBG();
			if (g_Magic[i].pos.x < (-g_Magic[i].width / 2))		// 自分の大きさを考慮して画面外か判定している
			{
				ClearMagic(&g_Magic[i]);
			}
			if (g_Magic[i].pos.x > (bg->w + g_Magic[i].width / 2))	// 自分の大きさを考慮して画面外か判定している
			{
				ClearMagic(&g_Magic[i]);
			}

			// 当たり判定処理
			if (g_Magic[i].isCollision == TRUE)
			{
				SetUpMagicCollision(&g_Magic[i]);

				HandleMagicCollision(&g_Magic[i]);

			}

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagic(void)
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

	for (int i = 0; i < MAGIC_MAX; i++)
	{
		if (g_Magic[i].use == TRUE)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Magic[i].magicType]);

			//魔法の位置やテクスチャー座標を反映
			float px = g_Magic[i].pos.x - bg->pos.x;	// 魔法の表示位置X
			float py = g_Magic[i].pos.y - bg->pos.y;	// 魔法の表示位置Y
			float pw = g_Magic[i].width;		// 魔法の表示幅
			float ph = g_Magic[i].height;		// 魔法の表示高さ

			float tw = 1.0f / GetMagicTexturePatternDivideX(&g_Magic[i]);	// テクスチャの幅
			float th = 1.0f / GetMagicTexturePatternDivideY(&g_Magic[i]);	// テクスチャの高さ
			// テクスチャの左上X座標
			float tx;
			// テクスチャの左上Y座標
			float ty = (float)(g_Magic[i].patternAnim / GetMagicTexturePatternDivideX(&g_Magic[i])) * th;
			if (g_Magic[i].invertTex)
			{
				tw *= -1.0f;
				tx = (float)(g_Magic[i].patternAnim % GetMagicTexturePatternDivideX(&g_Magic[i])) * (-1 * tw) - tw;
			}
			else
				tx = (float)(g_Magic[i].patternAnim % GetMagicTexturePatternDivideX(&g_Magic[i])) * tw;
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Magic[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

#ifdef _DEBUG
	{
		MATERIAL materialAABB;
		ZeroMemory(&materialAABB, sizeof(materialAABB));
		materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materialAABB.noTexSampling = 1;

		SetMaterial(materialAABB);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < MAGIC_MAX; ++i)
		{
			if (g_Magic[i].use == FALSE) continue;

			int vertexOffset = i * 4;

			SetSpriteColorRotation(g_AABBVertexBuffer, g_Magic[i].magicAABB.pos.x - bg->pos.x,
				g_Magic[i].magicAABB.pos.y - bg->pos.y, g_Magic[i].magicAABB.w, g_Magic[i].magicAABB.h,
				0.0f, 0.0f, 0.0f, 0.0f,
				XMFLOAT4(0.0f, 1.0f, 1.0f, 0.2f),
				0.0f,
				vertexOffset);

			GetDeviceContext()->Draw(4, vertexOffset);
		}
	}
#endif

}

void TriggerMagic(int magicType, XMFLOAT3 pos, ENEMY* enemy)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < MAGIC_MAX; i++)
	{
		if (g_Magic[i].use == FALSE)		// 未使用状態のバレットを見つける
		{
			g_Magic[i].use = TRUE;			// 使用状態へ変更する
			g_Magic[i].magicType = magicType;
			g_Magic[i].countAnim = 0;
			g_Magic[i].patternAnim = 0;
			switch (magicType)
			{
			case MAGIC_FIRE_BALL:
			{
				g_Magic[i].pos = player->pos;	// 座標をセット
				g_Magic[i].pos.x += player->dir == CHAR_DIR_RIGHT ? MAGIC_FIRE_BALL_OFFSET : -MAGIC_FIRE_BALL_OFFSET;
				g_Magic[i].width = TEXTURE_FIRE_BALL_WIDTH;
				g_Magic[i].height = TEXTURE_FIRE_BALL_HEIGHT;
				float speed = player->dir == CHAR_DIR_RIGHT ? MAGIC_FIRE_BALL_SPEED : -MAGIC_FIRE_BALL_SPEED;
				g_Magic[i].move = XMFLOAT3(speed, 0.0f, 0.0f);
				g_Magic[i].invertTex = player->invertTex;
				g_Magic[i].isAnimRepeat = TRUE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				break;
			}
			case MAGIC_HEALING:
			{
				g_Magic[i].pos = player->pos;	// 座標をセット
				g_Magic[i].width = TEXTURE_HEAL_WIDTH;
				g_Magic[i].height = TEXTURE_HEAL_HEIGHT;
				g_Magic[i].invertTex = player->invertTex;
				g_Magic[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = TRUE;
				g_Magic[i].isCollision = FALSE;
				break;
			}
			case MAGIC_BOOM:
			{
				g_Magic[i].pos = pos;	// 座標をセット
				g_Magic[i].width = TEXTURE_BOOM_WIDTH;
				g_Magic[i].height = TEXTURE_BOOM_HEIGHT;
				g_Magic[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				break;
			}
			case MAGIC_STOMP:
			{
				g_Magic[i].pos = enemy->pos;	// 座標をセット
				g_Magic[i].pos.x += enemy->dir == CHAR_DIR_RIGHT ? MAGIC_STOMP_OFFSET_X : -MAGIC_STOMP_OFFSET_X;
				g_Magic[i].pos.y += enemy->bodyAABB.h * 0.5f + MAGIC_STOMP_OFFSET_Y;
				g_Magic[i].width = TEXTURE_STOMP_WIDTH;
				g_Magic[i].height = TEXTURE_STOMP_HEIGHT;
				float speed = enemy->dir == CHAR_DIR_RIGHT ? MAGIC_STOMP_SPEED : -MAGIC_STOMP_SPEED;
				g_Magic[i].move = XMFLOAT3(speed, 0.0f, 0.0f);
				g_Magic[i].invertTex = enemy->invertTex;
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				g_Magic[i].damage = GetMagicDamage(MAGIC_STOMP);
				break;
			}
			case MAGIC_THUNDER:
			{
				g_Magic[i].pos = pos;
				g_Magic[i].width = TEXTURE_THUNDER_WIDTH;
				g_Magic[i].height = TEXTURE_THUNDER_HEIGHT;
				g_Magic[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_Magic[i].invertTex = FALSE;
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				g_Magic[i].damage = GetMagicDamage(MAGIC_THUNDER);
				break;
			}
			case MAGIC_BARRAGE:
			{
				g_Magic[i].pos = pos;
				g_Magic[i].width = TEXTURE_BARRAGE_WIDTH;
				g_Magic[i].height = TEXTURE_BARRAGE_HEIGHT;
				g_Magic[i].invertTex = FALSE;
				g_Magic[i].isAnimRepeat = TRUE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				g_Magic[i].delayMove = 100.0f;
				g_Magic[i].move = XMFLOAT3(3.0f, 3.0f, 0.0f);
				g_Magic[i].damage = GetMagicDamage(MAGIC_BARRAGE);
				break;
			}
			case MAGIC_EARTH:
			{
				g_Magic[i].pos = pos;
				g_Magic[i].width = TEXTURE_BARRAGE_WIDTH;
				g_Magic[i].height = TEXTURE_BARRAGE_HEIGHT;
				g_Magic[i].invertTex = FALSE;
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				g_Magic[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_Magic[i].damage = GetMagicDamage(MAGIC_EARTH);
				break;
			}
			default:
				break;
			}
			return;							// 1発セットしたので終了する
		}
	}
}

int GetMagicTexturePatternDivideX(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		return TEXTURE_FIRE_BALL_PATTERN_DIVIDE_X;
	case MAGIC_BOOM:
		return TEXTURE_BOOM_PATTERN_DIVIDE_X;
	case MAGIC_HEALING:
		return TEXTURE_HEAL_PATTERN_DIVIDE_X;
	case MAGIC_STOMP:
		return TEXTURE_STOMP_PATTERN_DIVIDE_X;
	case MAGIC_EARTH:
		return TEXTURE_EARTH_PATTERN_DIVIDE_X;
	case MAGIC_THUNDER:
		return TEXTURE_THUNDER_PATTERN_DIVIDE_X;
	case MAGIC_BARRAGE:
		return TEXTURE_BARRAGE_PATTERN_DIVIDE_X;
	default:
		return -1;
	}
}

int GetMagicTexturePatternDivideY(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		return TEXTURE_FIRE_BALL_PATTERN_DIVIDE_Y;
	case MAGIC_BOOM:
		return TEXTURE_BOOM_PATTERN_DIVIDE_Y;
	case MAGIC_HEALING:
		return TEXTURE_HEAL_PATTERN_DIVIDE_Y;
	case MAGIC_STOMP:
		return TEXTURE_STOMP_PATTERN_DIVIDE_Y;
	case MAGIC_EARTH:
		return TEXTURE_EARTH_PATTERN_DIVIDE_Y;
	case MAGIC_THUNDER:
		return TEXTURE_THUNDER_PATTERN_DIVIDE_Y;
	case MAGIC_BARRAGE:
		return TEXTURE_BARRAGE_PATTERN_DIVIDE_Y;
	default:
		return -1;
	}
}

int GetMagicAnimWait(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		return ANIM_WAIT_FIRE_BALL;
	case MAGIC_BOOM:
		return ANIM_WAIT_BOOM;
	case MAGIC_HEALING:
		return ANIM_WAIT_HEAL;
	case MAGIC_STOMP:
		return ANIM_WAIT_STOMP;
	case MAGIC_EARTH:
		return ANIM_WAIT_EARTH;
	case MAGIC_THUNDER:
		return ANIM_WAIT_THUNDER;
	case MAGIC_BARRAGE:
		return ANIM_WAIT_BARRAGE;
	default:
		return -1;
	}
}

void ClearMagic(Magic* magic)
{
	magic->use = FALSE;
	magic->invertTex = FALSE;
	magic->isAnimRepeat = FALSE;
	magic->followPlayer = FALSE;
	magic->isCollision = FALSE;
	magic->destroyAnim = FALSE;
	magic->reverseAnim = FALSE;
	magic->hitTarget = FALSE;
	magic->damage = 0.0f;
	magic->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	magic->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	magic->width = 0.0f;
	magic->height = 0.0f;
	magic->magicType = MAGIC_NONE;

	magic->delayMove = 0;

	magic->countAnim = 0;
	magic->patternAnim = 0;

	magic->move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 移動量を初期化
	magic->magicAABB.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	magic->magicAABB.w = 0.0f;
	magic->magicAABB.h = 0.0f;
}

void SetUpMagicCollision(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.tag = PLAYER_MAGIC_AABB;
		magic->magicAABB.w = magic->width * 0.1f;
		magic->magicAABB.h = magic->height * 0.5f;
		break;
	case MAGIC_BOOM:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.tag = PLAYER_MAGIC_AABB;
		magic->magicAABB.w = magic->width * g_BoomAABBTbl[magic->patternAnim].w;
		magic->magicAABB.h = magic->height * g_BoomAABBTbl[magic->patternAnim].h;
		break;
	case MAGIC_EARTH:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.pos.y += g_EarthAABBTbl[magic->patternAnim].pos.y;
		magic->magicAABB.tag = PLAYER_MAGIC_AABB;
		magic->magicAABB.w = magic->width * g_EarthAABBTbl[magic->patternAnim].w;
		magic->magicAABB.h = magic->height * g_EarthAABBTbl[magic->patternAnim].h;
		break;
	case MAGIC_STOMP:
		magic->magicAABB.pos = magic->pos;
		if (magic->invertTex == TRUE)
			magic->magicAABB.pos.x -= 30.0f;
		else
			magic->magicAABB.pos.x += 30.0f;
		magic->magicAABB.tag = ENEMY_MAGIC_AABB;
		magic->magicAABB.w = magic->width * 0.3f;
		magic->magicAABB.h = magic->height * 0.5f;
		break;
	case MAGIC_THUNDER:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.tag = ENEMY_MAGIC_AABB;
		magic->magicAABB.w = magic->width * 0.5f;
		magic->magicAABB.h = magic->height;
		break;
	case MAGIC_BARRAGE:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.tag = ENEMY_MAGIC_AABB;
		magic->magicAABB.w = magic->width * 0.35f;
		magic->magicAABB.h = magic->height * 0.35f;
		break;
	default:
		break;
	}
}

void HandleMagicCollision(Magic* magic)
{
	// 地面のAABB情報を取得
	MapWall* grounds = GetMapWall();
	for (int j = 0; j < MAP_WALL_MAX; j++)
	{
		if (grounds->use == FALSE) continue;
		if (grounds[j].wallAABB.tag == GROUND_AABB && magic->magicType != MAGIC_BARRAGE) continue;

		if (CollisionBB(magic->magicAABB.pos, magic->magicAABB.w, magic->magicAABB.h,
			grounds[j].wallAABB.pos, grounds[j].wallAABB.w, grounds[j].wallAABB.h))
		{
			switch (magic->magicType)
			{
			case MAGIC_FIRE_BALL:
				TriggerMagic(MAGIC_BOOM, magic->pos);
				PlaySound(SOUND_LABEL_SE_BOOM);
				magic->use = FALSE;
				break;
			case MAGIC_STOMP:
				magic->destroyAnim = TRUE;
				magic->use = FALSE;
				break;
			case MAGIC_BARRAGE:
				magic->use = FALSE;
			default:
				break;
			}

			return;
		}
	}

	if (magic->magicAABB.tag == PLAYER_MAGIC_AABB)
	{
		ENEMY* enemy = GetEnemy();
		// エネミーの数分当たり判定を行う
		for (int j = 0; j < ENEMY_MAX; j++)
		{
			// 生きてるエネミーと当たり判定をする
			if (enemy[j].use == TRUE && CollisionBB(magic->magicAABB.pos, magic->magicAABB.w, magic->magicAABB.h,
				enemy[j].bodyAABB.pos, enemy[j].bodyAABB.w, enemy[j].bodyAABB.h))
			{
				switch (magic->magicType)
				{
				case MAGIC_FIRE_BALL:
					TriggerMagic(MAGIC_BOOM, magic->pos);
					magic->use = FALSE;
					break;
				default:
					break;
				}

				return;
			}
		}
	}

	if (magic->magicAABB.tag == ENEMY_MAGIC_AABB)
	{
		PLAYER* player = GetPlayer();
		// プレイヤーとの当たり判定
		if (player->isInvincible == FALSE 
			&&CollisionBB(magic->magicAABB.pos, magic->magicAABB.w, magic->magicAABB.h,
			player->bodyAABB.pos, player->bodyAABB.w, player->bodyAABB.h))
		{
			switch (magic->magicType)
			{
			case MAGIC_BARRAGE:
				PlayerTakeDamage(nullptr, magic);;
				ClearMagic(magic);
				break;
			case MAGIC_STOMP:
			case MAGIC_THUNDER:
			case MAGIC_EARTH:
				PlayerTakeDamage(nullptr, magic);
				magic->hitTarget = TRUE;
				break;
			default:
				break;
			}

			return;
		}
	}

}

void PlayMagicAnim(Magic* magic)
{
	PLAYER* player = GetPlayer();

	// アニメーション  
	magic->countAnim++;

	switch (magic->magicType)
	{
	case MAGIC_STOMP:
		if ((magic->countAnim % GetMagicAnimWait(magic)) == 0)
		{
			// パターンの切り替え
			if (magic->patternAnim == 10)
			{
				magic->reverseAnim = TRUE;
			}
			else if (magic->patternAnim == 7)
			{
				magic->reverseAnim = FALSE;
			}

			magic->patternAnim += magic->reverseAnim == TRUE ? -1 : 1;

			if (magic->destroyAnim == TRUE)
			{
				ClearMagic(magic);
			}
		}

		// 移動処理
		XMVECTOR pos = XMLoadFloat3(&magic->pos);
		XMVECTOR move = XMLoadFloat3(&magic->move);
		pos += move;
		XMStoreFloat3(&magic->pos, pos);

		break;
	default:
		if ((magic->countAnim % GetMagicAnimWait(magic)) == 0)
		{
			// パターンの切り替え
			magic->patternAnim = (magic->patternAnim + 1) %
				(GetMagicTexturePatternDivideX(magic) * GetMagicTexturePatternDivideY(magic));

			if (magic->isAnimRepeat == FALSE &&
				magic->patternAnim == GetMagicTexturePatternDivideX(magic) * GetMagicTexturePatternDivideY(magic) - 1)
			{
				ClearMagic(magic);
			}
		}

		// 移動処理
		if (magic->followPlayer == TRUE)
		{
			magic->pos = player->pos;
		}
		else
		{
			if (magic->delayMove > 0.0f)
			{
				magic->delayMove--;
				if (magic->delayMove <= 0.0f)
					InitializeMagicMove(magic, player->pos);
			}
			else
			{
				if (magic->magicType == MAGIC_BARRAGE)
				{
					PLAYER* player = GetPlayer();

					// 1. 現在の魔法の位置と移動方向を読み込む
					XMVECTOR magicPos = XMLoadFloat3(&magic->pos);
					XMVECTOR magicMove = XMLoadFloat3(&magic->move);

					// 2. プレイヤーの方向へのベクトルを計算する
					XMVECTOR playerPosVec = XMLoadFloat3(&player->pos);
					XMVECTOR toPlayer = playerPosVec - magicPos; // 魔法からプレイヤーへのベクトル

					// 3. toPlayerベクトルを正規化する
					toPlayer = XMVector3Normalize(toPlayer);

					// 4. 現在の移動方向とプレイヤー方向を部分的に補間し、XとYの速度分量に基づいて移動
					XMFLOAT3 toPlayerFloat3;
					XMStoreFloat3(&toPlayerFloat3, toPlayer);

					float followStrength = 0.06f;
					// 5. 線形補間して部分追従させる
					XMVECTOR interpolatedMove = XMVectorLerp(magicMove, XMVectorSet(
						toPlayerFloat3.x * MAGIC_BARRAGE_SPEED,
						toPlayerFloat3.y * MAGIC_BARRAGE_SPEED,
						0.0f,
						0.0f
					), followStrength);

					// 6. 補間後の方向を正規化して、速度分量を維持する
					XMFLOAT3 interpolatedMoveFloat3;
					XMStoreFloat3(&interpolatedMoveFloat3, interpolatedMove);

					XMVECTOR newDirection = XMVectorSet(
						XMVector3Normalize(interpolatedMove).m128_f32[0] * MAGIC_BARRAGE_SPEED,
						XMVector3Normalize(interpolatedMove).m128_f32[1] * MAGIC_BARRAGE_SPEED,
						0.0f,
						0.0f
					);

					// 7. 魔法の位置を更新する
					magicPos += newDirection;

					// 8. 新しい位置と方向を保存する
					XMStoreFloat3(&magic->pos, magicPos);
					XMStoreFloat3(&magic->move, newDirection); // 移動方向を更新
				}
				else
				{
					XMVECTOR pos = XMLoadFloat3(&magic->pos);
					XMVECTOR move = XMLoadFloat3(&magic->move);
					pos += move;
					XMStoreFloat3(&magic->pos, pos);
				}

			}

		}
		break;
	}

}

void InitializeMagicMove(Magic* magic, const XMFLOAT3& playerPos)
{
	// 1. 魔法の初期位置を読み込む
	XMVECTOR magicPos = XMLoadFloat3(&magic->pos);

	// 2. プレイヤーの位置を読み込む
	XMVECTOR playerPosVec = XMLoadFloat3(&playerPos);

	// 3. プレイヤーの方向へのベクトルを計算する（方向ベクトル）
	XMVECTOR toPlayer = playerPosVec - magicPos;

	// 4. toPlayerベクトルを正規化して、方向を得る
	toPlayer = XMVector3Normalize(toPlayer);

	// 5. XとYの速度分量に基づいて、方向ベクトルに速度を掛ける
	XMFLOAT3 toPlayerFloat3;
	XMStoreFloat3(&toPlayerFloat3, toPlayer);

	// X方向とY方向の速度を分別して掛ける
	XMVECTOR moveVector = XMVectorSet(
		toPlayerFloat3.x * magic->move.x, // X方向の速度
		toPlayerFloat3.y * magic->move.y, // Y方向の速度
		0.0f,                            // Z方向は使わない
		0.0f
	);

	// 6. moveVectorを魔法の初期移動ベクトルとして保存
	XMStoreFloat3(&magic->move, moveVector);
}

Magic* GetMagic(void)
{
	return &g_Magic[0];
}

float GetMagicDamage(int type)
{
	switch (type)
	{
	case MAGIC_BOOM:
		return 25.0f;
	case MAGIC_EARTH:
		return 150.0f;
	case MAGIC_STOMP:
		return 15.0f;
	case MAGIC_THUNDER:
		return 200.0f;
	case MAGIC_BARRAGE:
		return 50.0f;
	default:
		return 0.0f;
	}
}

float GetMagicPoiseDamage(int type)
{
	switch (type)
	{
	case MAGIC_BOOM:
		return 5.0f;
	default:
		return 0.0f;
	}
}