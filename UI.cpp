//=============================================================================
//
// UI処理 [UI.cpp]
// Author : 
//
//=============================================================================
#include "UI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_HP_WIDTH				(450)
#define TEXTURE_MP_WIDTH				(350)
#define TEXTURE_ST_WIDTH				(400)
#define TEXTURE_JUMP_ICON_WIDTH			(35)
#define TEXTURE_JUMP_ICON_HEIGHT		(35)
#define TEXTURE_GAUGE_HEIGHT			(25)

#define UI_GAUGE_POS_X					(-8.0f)
#define UI_GAUGE_POS_Y					(0.0f)
#define	UI_GAUGE_OFFSET_X				(11.0f)
#define	UI_GAUGE_OFFSET_Y				(7.0f)
#define UI_GAUGE_SCALE_X				(0.95f)
#define UI_GAUGE_SCALE_Y				(0.6f)
#define UI_JUMP_ICON_POS_X				(15.0f)
#define	UI_MAX							(15)
#define	GAUGE_MAX						(3)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[UI_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[UI_MAX] = {
	"data/TEXTURE/UI/赤グラデ.png",
	"data/TEXTURE/UI/青.png",
	"data/TEXTURE/UI/緑グラデ.png",
	"data/TEXTURE/UI/ゲージカバー2.png",
	"data/TEXTURE/UI/jump_icon.png",

};

static UISprite					g_UI[UI_MAX];

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < UI_MAX; i++)
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
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].height = TEXTURE_GAUGE_HEIGHT;
		g_UI[i].ratio = 1.0f;
	}

	g_UI[UI_HP].width = TEXTURE_HP_WIDTH;
	g_UI[UI_HP].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y, 0.0f);

	g_UI[UI_MP].width = TEXTURE_MP_WIDTH;
	g_UI[UI_MP].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT, 0.0f);

	g_UI[UI_ST].width = TEXTURE_ST_WIDTH;
	g_UI[UI_ST].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT * 2, 0.0f);

	g_UI[UI_JUMP_ICON].width = TEXTURE_JUMP_ICON_WIDTH;
	g_UI[UI_JUMP_ICON].height = TEXTURE_JUMP_ICON_HEIGHT;
	g_UI[UI_JUMP_ICON].pos = XMFLOAT3(UI_JUMP_ICON_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT * 3, 0.0f);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI(void)
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
void UpdateUI(void)
{
	PLAYER* player = GetPlayer();

	g_UI[UI_HP].ratio = player->HP / player->maxHP;
	g_UI[UI_MP].ratio = player->MP / player->maxMP;
	g_UI[UI_ST].ratio = player->ST / player->maxST;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	DrawPlayerGauge();

	DrawPlayerJumpIcon();
}

void DrawPlayerGauge()
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f);
	SetMaterial(material);

	// HPゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_HP]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_HP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_HP].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_HP].width * UI_GAUGE_SCALE_X * g_UI[UI_HP].ratio, g_UI[UI_HP].height * UI_GAUGE_SCALE_Y, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// MPゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_MP]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_MP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_MP].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_MP].width * UI_GAUGE_SCALE_X * g_UI[UI_MP].ratio, g_UI[UI_MP].height * UI_GAUGE_SCALE_Y, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// STゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_ST]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_ST].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_ST].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_ST].width * UI_GAUGE_SCALE_X * g_UI[UI_ST].ratio, g_UI[UI_ST].height * UI_GAUGE_SCALE_Y, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
	SetMaterial(material);

	// HPゲージカバー
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAUGE_COVER]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_HP].pos.x, g_UI[UI_HP].pos.y, g_UI[UI_HP].width, g_UI[UI_HP].height, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// MPゲージカバー
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAUGE_COVER]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_MP].pos.x, g_UI[UI_MP].pos.y, g_UI[UI_MP].width, g_UI[UI_MP].height, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// STゲージカバー
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAUGE_COVER]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_ST].pos.x, g_UI[UI_ST].pos.y, g_UI[UI_ST].width, g_UI[UI_ST].height, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);
}

void DrawPlayerJumpIcon()
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	PLAYER* player = GetPlayer();
	if (player->dashCount < 1)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_JUMP_ICON]);
		SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_JUMP_ICON].pos.x + TEXTURE_JUMP_ICON_WIDTH, g_UI[UI_JUMP_ICON].pos.y,
			g_UI[UI_JUMP_ICON].width, g_UI[UI_JUMP_ICON].height, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}
	if (player->dashCount < 2)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_JUMP_ICON]);
		SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_JUMP_ICON].pos.x, g_UI[UI_JUMP_ICON].pos.y,
			g_UI[UI_JUMP_ICON].width, g_UI[UI_JUMP_ICON].height, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}

}