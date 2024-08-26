﻿//=============================================================================
//
// BG処理 [bg.cpp]
// Author : 
//
//=============================================================================
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_WIDTH_LOGO			(480)			// ロゴサイズ
#define TEXTURE_HEIGHT_LOGO			(80)			// 

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
	"data/TEXTURE/map/map01.png",
};


static BOOL	g_Load = FALSE;		// 初期化を行ったかのフラグ
static BG	g_BG;
static AABB g_AABB[MAP01_GROUND_MAX];



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(void)
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
	g_BG.w     = TEXTURE_WIDTH;
	g_BG.h     = TEXTURE_HEIGHT;
	g_BG.pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.texNo = 0;

	g_BG.scrl  = 0.0f;		// TEXスクロール
	g_BG.scrl2 = 0.0f;		// TEXスクロール

	// AABB
	for (int i = 0; i < MAP01_GROUND_MAX; i++)
	{
		g_AABB[i].pos.x = 0;
		g_AABB[i].pos.y = 0;
		g_AABB[i].w = 0;
		g_AABB[i].h = 0;
		g_AABB[i].tag = WALL_AABB;
	}

	g_AABB[0].pos.x = TEXTURE_WIDTH * 0.5f;
	g_AABB[0].pos.y = 1290.0f + (TEXTURE_HEIGHT - 1290.0f) * 0.5f;
	g_AABB[0].w = TEXTURE_WIDTH;
	g_AABB[0].h = GROUND_H * 0.5f;

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


#ifdef _DEBUG	
	// debug
	{
		int aabbCount = MAP01_GROUND_MAX;
		const int maxVertices = MAP01_GROUND_MAX * 4;

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
void UninitBG(void)
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
void UpdateBG(void)
{
	g_BG.old_pos = g_BG.pos;	// １フレ前の情報を保存


	//g_BG.scrl -= 0.0f;		// 0.005f;		// スクロール


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
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
		for (int i = 0; i < MAP01_GROUND_MAX; ++i)
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


	// なんちゃって多重スクロール
	//{
	//	// テクスチャ設定
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	//	// １枚のポリゴンの頂点とテクスチャ座標を設定
	//	float	tx = (g_BG.pos.x - g_BG.old_pos.x) * ((float)SCREEN_WIDTH / TEXTURE_WIDTH);
	//	g_BG.scrl2 += tx * 0.01f;
	//	//g_BG.scrl2 += 0.003f;

	//	SetSpriteLTColor(g_VertexBuffer,
	//		0.0f, SKY_H/2, SCREEN_WIDTH, SKY_H,
	//		g_BG.scrl2, 0.0f, 1.0f, 1.0f,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//	// ポリゴン描画
	//	GetDeviceContext()->Draw(4, 0);
	//}


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




