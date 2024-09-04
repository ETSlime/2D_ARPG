//=============================================================================
//
// UI処理 [UI.cpp]
// Author : 
//
//=============================================================================
#include "UI.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// in game
#define TEXTURE_HP_WIDTH				(450)
#define TEXTURE_MP_WIDTH				(350)
#define TEXTURE_ST_WIDTH				(400)
#define TEXTURE_MAGIC_ICON_SIZE			(75)
#define TEXTURE_MAGIC_ICON_SMALL_SIZE	(45)
#define TEXTURE_JUMP_ICON_WIDTH			(35)
#define TEXTURE_JUMP_ICON_HEIGHT		(35)
#define TEXTURE_GAUGE_HEIGHT			(25)
#define	TEXTURE_FLAMEBLADE_ICON_WIDTH	(235)
#define TEXTURE_FLAMEBLADE_ICON_HEIGHT	(215)
#define TEXTURE_SKILL_ENABLED_SCALE		(3.3)

#define UI_GAUGE_POS_X					(-8.0f)
#define UI_GAUGE_POS_Y					(0.0f)
#define	UI_GAUGE_OFFSET_X				(11.0f)
#define	UI_GAUGE_OFFSET_Y				(7.0f)
#define UI_GAUGE_SCALE_X				(0.95f)
#define UI_GAUGE_SCALE_Y				(0.6f)
#define UI_SKILL_ENABLED_OFFSET_X		(-87.5f)
#define UI_SKILL_ENABLED_OFFSET_Y		(-87.5f)
#define UI_SKILL_ENABLED_SMALL_OFFSET_X	(-51.5f)
#define UI_SKILL_ENABLED_SMALL_OFFSET_Y	(-51.5f)
#define UI_JUMP_ICON_POS_X				(15.0f)
#define UI_MAGIC_ICON_MIDDLE_POS_X		(76.0f)
#define UI_MAGIC_ICON_MIDDLE_POS_Y		(450.0f)
#define UI_MAGIC_ICON_LEFT_POS_X		(16.0f)
#define UI_MAGIC_ICON_LEFT_POS_Y		(480.0f)
#define UI_MAGIC_ICON_RIGHT_POS_X		(166.0f)
#define UI_MAGIC_ICON_RIGHT_POS_Y		(480.0f)
#define UI_FLAMEBLADE_ICON_POS_X		(756.0f)
#define UI_FLAMEBLADE_ICON_POS_Y		(376.0f)
#define	UI_MAX							(20)
#define	GAUGE_MAX						(3)

#define TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_X	(4)
#define TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_Y	(2)
#define TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_X		(9)
#define TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_Y		(4)

#define ANIM_WAIT_FLAMEBLADE_ICON		(5)
#define ANIM_WAIT_SKILL_ENABLED			(8)

// タイトルUIテクスチャサイズ
#define TEXTURE_SYSTEM_MENU_BOX_TITLE_WIDTH			(450)
#define TEXTURE_SYSTEM_MENU_BOX_TITLE_HEIGHT		(250)
#define TEXTURE_BUTTON_TITLE_WIDTH					(260)
#define TEXTURE_BUTTON_TITLE_HEIGHT					(40)
// タイトルUIポジション
#define UI_SYSTEM_MENU_BOX_TITLE_POS_X				(200)
#define UI_SYSTEM_MENU_BOX_TITLE_POS_Y				(200)
#define UI_TITLE_BUTTON_POS_X						(350)
#define	UI_TITLE_BUTTON_POS_Y						(250)
#define UI_TITLE_BUTTON_OFFSET_Y					(70)


// 一時停止UIテクスチャサイズ
#define TEXTURE_SYSTEM_MENU_BOX_PAUSE_WIDTH			(550)
#define TEXTURE_SYSTEM_MENU_BOX_PAUSE_HEIGHT		(150)
#define	TEXTURE_BUTTON_PAUSE_WIDTH					(80)
#define	TEXTURE_BUTTON_PAUSE_HEIGHT					(40)
// 一時停止UIポジション
#define UI_SYSTEM_MENU_BOX_PAUSE_POS_X				(400)
#define UI_SYSTEM_MENU_BOX_PAUSE_POS_Y				(400)
#define UI_PAUSE_BUTTON_POS_X						(350)
#define UI_PAUSE_BUTTON_POS_Y						(350)
#define UI_PAUSE_BUTTON_OFFSET_X					(150)
#define UI_BUTTON_SELECTED_SCALE					(1.2f)

#define BG_PAUSE_TEXTURE							(16)

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
	"data/TEXTURE/UI/heal.png",
	"data/TEXTURE/UI/flameblade.png",
	"data/TEXTURE/UI/fire_ball.png",
	"data/TEXTURE/UI/flameblade_icon.png",
	"data/TEXTURE/UI/skillEnabled.png",
	"data/TEXTURE/UI/systemMenuBox.png",
	"data/TEXTURE/UI/button_new_game.png",
	"data/TEXTURE/UI/button_tutorial.png",
	"data/TEXTURE/UI/button_exit_game.png",
	"data/TEXTURE/UI/button_yes.png",
	"data/TEXTURE/UI/button_no.png",
	"data/TEXTURE/fade_black.png",

};

static UISprite					g_UI[UI_MAX];
static int						g_flamebladeIconCountAnim;
static int						g_flamebladePatternAnim;
static int						g_skillEnabledCountAnim;
static int						g_skillEnabledPatternAnim;
static BOOL						g_Load = FALSE;
static int						g_Cursor;
static int						g_Pause;

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
	bd.ByteWidth = sizeof(VERTEX_3D) * 12;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 変数の初期化
	for (int i = 0; i < UI_MAX; i++)
	{
		g_UI[i].use = FALSE;
		g_UI[i].width = 0.0f;
		g_UI[i].height = 0.0f;
		g_UI[i].ratio = 1.0f;
		g_UI[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// ゲージの初期化
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].height = TEXTURE_GAUGE_HEIGHT;
		g_UI[i].ratio = 1.0f;
	}

	g_flamebladeIconCountAnim = 0;
	g_flamebladePatternAnim = 0;
	g_skillEnabledCountAnim = 0;
	g_skillEnabledPatternAnim = 0;

	g_UI[UI_HP].width = TEXTURE_HP_WIDTH;
	g_UI[UI_HP].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y, 0.0f);

	g_UI[UI_MP].width = TEXTURE_MP_WIDTH;
	g_UI[UI_MP].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT, 0.0f);

	g_UI[UI_ST].width = TEXTURE_ST_WIDTH;
	g_UI[UI_ST].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT * 2, 0.0f);

	g_UI[UI_JUMP_ICON].width = TEXTURE_JUMP_ICON_WIDTH;
	g_UI[UI_JUMP_ICON].height = TEXTURE_JUMP_ICON_HEIGHT;
	g_UI[UI_JUMP_ICON].pos = XMFLOAT3(UI_JUMP_ICON_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT * 3, 0.0f);

	g_UI[UI_FLAMEBLADE_ICON].width = TEXTURE_FLAMEBLADE_ICON_WIDTH;
	g_UI[UI_FLAMEBLADE_ICON].height = TEXTURE_FLAMEBLADE_ICON_HEIGHT;
	g_UI[UI_FLAMEBLADE_ICON].pos = XMFLOAT3(UI_FLAMEBLADE_ICON_POS_X, UI_FLAMEBLADE_ICON_POS_Y, 0.0f);

	g_UI[UI_SYSTEM_MENU_BOX].width = TEXTURE_SYSTEM_MENU_BOX_TITLE_WIDTH;
	g_UI[UI_SYSTEM_MENU_BOX].height = TEXTURE_SYSTEM_MENU_BOX_TITLE_HEIGHT;
	g_UI[UI_SYSTEM_MENU_BOX].pos = XMFLOAT3(UI_SYSTEM_MENU_BOX_TITLE_POS_X, UI_SYSTEM_MENU_BOX_TITLE_POS_Y, 0.0f);

	g_UI[UI_BUTTON_NEW_GAME].width = TEXTURE_BUTTON_TITLE_WIDTH;
	g_UI[UI_BUTTON_NEW_GAME].height = TEXTURE_BUTTON_TITLE_HEIGHT;
	g_UI[UI_BUTTON_NEW_GAME].pos = XMFLOAT3(UI_TITLE_BUTTON_POS_X, UI_TITLE_BUTTON_POS_Y, 0.0f);

	g_UI[UI_BUTTON_TUTORIAL].width = TEXTURE_BUTTON_TITLE_WIDTH;
	g_UI[UI_BUTTON_TUTORIAL].height = TEXTURE_BUTTON_TITLE_HEIGHT;
	g_UI[UI_BUTTON_TUTORIAL].pos = XMFLOAT3(UI_TITLE_BUTTON_POS_X, UI_TITLE_BUTTON_POS_Y + UI_TITLE_BUTTON_OFFSET_Y, 0.0f);

	g_UI[UI_BUTTON_EXIT_GAME].width = TEXTURE_BUTTON_TITLE_WIDTH;
	g_UI[UI_BUTTON_EXIT_GAME].height = TEXTURE_BUTTON_TITLE_HEIGHT;
	g_UI[UI_BUTTON_EXIT_GAME].pos = XMFLOAT3(UI_TITLE_BUTTON_POS_X, UI_TITLE_BUTTON_POS_Y + UI_TITLE_BUTTON_OFFSET_Y * 2, 0.0f);

	g_UI[UI_BUTTON_YES].width = TEXTURE_BUTTON_PAUSE_WIDTH;
	g_UI[UI_BUTTON_YES].height = TEXTURE_BUTTON_PAUSE_HEIGHT;
	g_UI[UI_BUTTON_YES].pos = XMFLOAT3(UI_PAUSE_BUTTON_POS_X, UI_PAUSE_BUTTON_POS_Y, 0.0f);

	g_UI[UI_BUTTON_NO].width = TEXTURE_BUTTON_PAUSE_WIDTH;
	g_UI[UI_BUTTON_NO].height = TEXTURE_BUTTON_PAUSE_HEIGHT;
	g_UI[UI_BUTTON_NO].pos = XMFLOAT3(UI_PAUSE_BUTTON_POS_X + UI_PAUSE_BUTTON_OFFSET_X, UI_PAUSE_BUTTON_POS_Y, 0.0f);

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

	for (int i = 0; i < UI_MAX; i++)
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
	int mode = GetMode();
	switch (mode)
	{
	case MODE_TITLE:
	{
		g_Cursor = GetTitleCursor();
		break;
	}
	case MODE_GAME:
	{
		if (GetKeyboardRelease(DIK_P))
		{
			BOOL pause = GetPause() == TRUE ? FALSE : TRUE;
			SetPause(pause);
			g_Cursor = UI_BUTTON_NO;
		}
		g_Pause = GetPause();

		if (g_Pause == FALSE)
		{
			PLAYER* player = GetPlayer();

			g_UI[UI_HP].ratio = player->HP / player->maxHP;
			g_UI[UI_MP].ratio = player->MP / player->maxMP;
			g_UI[UI_ST].ratio = player->ST / player->maxST;

			if (player->flameblade == TRUE)
			{
				g_flamebladeIconCountAnim++;
				// 一定のカウントに達したらアニメーションを更新
				if (g_flamebladeIconCountAnim > ANIM_WAIT_FLAMEBLADE_ICON)
				{
					g_flamebladeIconCountAnim = 0;
					// パターンの切り替え
					g_flamebladePatternAnim = (g_flamebladePatternAnim + 1) %
						(TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_X * TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_Y);
				}

				g_skillEnabledCountAnim++;
				// 一定のカウントに達したらアニメーションを更新
				if (g_skillEnabledCountAnim > ANIM_WAIT_SKILL_ENABLED)
				{
					g_skillEnabledCountAnim = 0;
					// パターンの切り替え
					g_skillEnabledPatternAnim = (g_skillEnabledPatternAnim + 1) %
						(TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_X * TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_Y);
				}
			}
		}
		else
		{
			if (GetKeyboardRelease(DIK_LEFT) || GetKeyboardRelease(DIK_RIGHT))
				g_Cursor = g_Cursor == UI_BUTTON_NO ? UI_BUTTON_YES : UI_BUTTON_NO;

			if (GetKeyboardRelease(DIK_RETURN))
			{
				if (g_Cursor == UI_BUTTON_YES)
					SetFade(FADE_OUT, MODE_TITLE);
				SetPause(FALSE);
			}
		}

		break;
	}

	default:
		break;
	}



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
{
	int mode = GetMode();
	
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	switch (mode)
	{
	case MODE_TITLE:
		DrawTitleUI();
		break;
	case MODE_GAME:
		DrawInGameUI();

		if (GetPause())
			DrawPauseUI();
		break;
	default:
		break;
	}
}

void DrawTitleUI(void)
{
	// システムメニューボックス
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_SYSTEM_MENU_BOX]);
	SetSpriteColor(g_VertexBuffer, 
		g_UI[UI_SYSTEM_MENU_BOX].pos.x, g_UI[UI_SYSTEM_MENU_BOX].pos.y, 
		g_UI[UI_SYSTEM_MENU_BOX].width, g_UI[UI_SYSTEM_MENU_BOX].height, 
		0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);

	// ボタン
	BOOL selected = g_Cursor == 0;
	DrawButton(UI_BUTTON_NEW_GAME, selected);

	selected = g_Cursor == UI_BUTTON_TUTORIAL - UI_BUTTON_NEW_GAME;
	DrawButton(UI_BUTTON_TUTORIAL, selected);

	selected = g_Cursor == UI_BUTTON_EXIT_GAME - UI_BUTTON_NEW_GAME;
	DrawButton(UI_BUTTON_EXIT_GAME, selected);
}

void DrawInGameUI(void)
{
	DrawPlayerGauge();
	DrawPlayerJumpIcon();
	DrawSkillIcon();
	DrawFlamebladeIcon();
}

void DrawPlayerGauge(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f);
	SetMaterial(material);

	// HPゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_HP]);
	SetSpriteLeftTop(g_VertexBuffer, 
		g_UI[UI_HP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_HP].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_HP].width * UI_GAUGE_SCALE_X * g_UI[UI_HP].ratio, g_UI[UI_HP].height * UI_GAUGE_SCALE_Y, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// MPゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_MP]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_MP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_MP].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_MP].width * UI_GAUGE_SCALE_X * g_UI[UI_MP].ratio, g_UI[UI_MP].height * UI_GAUGE_SCALE_Y, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// STゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_ST]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_ST].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_ST].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_ST].width * UI_GAUGE_SCALE_X * g_UI[UI_ST].ratio, g_UI[UI_ST].height * UI_GAUGE_SCALE_Y, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
	SetMaterial(material);

	// HPゲージカバー
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAUGE_COVER]);
	SetSpriteLeftTop(g_VertexBuffer, 
		g_UI[UI_HP].pos.x, g_UI[UI_HP].pos.y, 
		g_UI[UI_HP].width, g_UI[UI_HP].height, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// MPゲージカバー
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAUGE_COVER]);
	SetSpriteLeftTop(g_VertexBuffer, 
		g_UI[UI_MP].pos.x, g_UI[UI_MP].pos.y, 
		g_UI[UI_MP].width, g_UI[UI_MP].height, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// STゲージカバー
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAUGE_COVER]);
	SetSpriteLeftTop(g_VertexBuffer, 
		g_UI[UI_ST].pos.x, g_UI[UI_ST].pos.y, 
		g_UI[UI_ST].width, g_UI[UI_ST].height, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);
}

void DrawPlayerJumpIcon(void)
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
		SetSpriteLeftTop(g_VertexBuffer, 
			g_UI[UI_JUMP_ICON].pos.x + TEXTURE_JUMP_ICON_WIDTH, g_UI[UI_JUMP_ICON].pos.y,
			g_UI[UI_JUMP_ICON].width, g_UI[UI_JUMP_ICON].height, 
			0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}
	if (player->dashCount < 2)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_JUMP_ICON]);
		SetSpriteLeftTop(g_VertexBuffer, 
			g_UI[UI_JUMP_ICON].pos.x, g_UI[UI_JUMP_ICON].pos.y,
			g_UI[UI_JUMP_ICON].width, g_UI[UI_JUMP_ICON].height, 
			0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}

}

void DrawSkillIcon(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	PLAYER* player = GetPlayer();
	int currentMagic = UI_NONE, leftMagic = UI_NONE, rightMagic = UI_NONE;
	
	switch (player->magic)
	{
	case MAGIC_HEALING:
		currentMagic = UI_MAGIC_HEAL;
		leftMagic = UI_MAGIC_FLAMEBLADE;
		rightMagic = UI_MAGIC_FIRE_BALL;
		break;
	case MAGIC_FLAMEBLADE:
		currentMagic = UI_MAGIC_FLAMEBLADE;
		leftMagic = UI_MAGIC_FIRE_BALL;
		rightMagic = UI_MAGIC_HEAL;
		break;
	case MAGIC_FIRE_BALL:
		currentMagic = UI_MAGIC_FIRE_BALL;
		leftMagic = UI_MAGIC_HEAL;
		rightMagic = UI_MAGIC_FLAMEBLADE;
		break;
	default:
		return;
	}

	float healingCDProgress = 1.0f - player->healingCD / HEALING_CD_TIME;
	float fireBallCDProgress = 1.0f - player->fireBallCD / FIRE_BALL_CD_TIME;
	float leftMagicCD = (leftMagic == UI_MAGIC_HEAL) ? healingCDProgress : (leftMagic == UI_MAGIC_FLAMEBLADE ? 1.0f : fireBallCDProgress);
	float currentMagicCD = (currentMagic == UI_MAGIC_HEAL) ? healingCDProgress : (currentMagic == UI_MAGIC_FLAMEBLADE ? 1.0f : fireBallCDProgress);
	float rightMagicCD = (rightMagic == UI_MAGIC_HEAL) ? healingCDProgress : (rightMagic == UI_MAGIC_FLAMEBLADE ? 1.0f : fireBallCDProgress);

	// 中央のマジックアイコン
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[currentMagic]);
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_MIDDLE_POS_X, UI_MAGIC_ICON_MIDDLE_POS_Y,
		TEXTURE_MAGIC_ICON_SIZE, TEXTURE_MAGIC_ICON_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		color, currentMagicCD);
	GetDeviceContext()->Draw(4, 0);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_MIDDLE_POS_X, UI_MAGIC_ICON_MIDDLE_POS_Y,
		TEXTURE_MAGIC_ICON_SIZE, TEXTURE_MAGIC_ICON_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f), 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// 左側のマジックアイコン
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[leftMagic]);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_LEFT_POS_X, UI_MAGIC_ICON_LEFT_POS_Y,
		TEXTURE_MAGIC_ICON_SMALL_SIZE, TEXTURE_MAGIC_ICON_SMALL_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		color, leftMagicCD);
	GetDeviceContext()->Draw(4, 0);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_LEFT_POS_X, UI_MAGIC_ICON_LEFT_POS_Y,
		TEXTURE_MAGIC_ICON_SMALL_SIZE, TEXTURE_MAGIC_ICON_SMALL_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f), 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// 右側のマジックアイコン
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[rightMagic]);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_RIGHT_POS_X, UI_MAGIC_ICON_RIGHT_POS_Y,
		TEXTURE_MAGIC_ICON_SMALL_SIZE, TEXTURE_MAGIC_ICON_SMALL_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		color, rightMagicCD);
	GetDeviceContext()->Draw(4, 0);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_RIGHT_POS_X, UI_MAGIC_ICON_RIGHT_POS_Y,
		TEXTURE_MAGIC_ICON_SMALL_SIZE, TEXTURE_MAGIC_ICON_SMALL_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f), 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// フレームブレードアイコン
	if (player->flameblade == TRUE)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_SKILL_ENABLED]);

		// アニメーション用
		float tw = 1.0f / TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(g_flamebladePatternAnim % TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_Y) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_flamebladePatternAnim / TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_Y) * th;	// テクスチャの左上Y座標

		if (leftMagic == UI_MAGIC_FLAMEBLADE)
		{
			SetSpriteLeftTop(g_VertexBuffer, 
				UI_MAGIC_ICON_LEFT_POS_X + UI_SKILL_ENABLED_SMALL_OFFSET_X,
				UI_MAGIC_ICON_LEFT_POS_Y + UI_SKILL_ENABLED_SMALL_OFFSET_Y,
				TEXTURE_MAGIC_ICON_SMALL_SIZE * TEXTURE_SKILL_ENABLED_SCALE, 
				TEXTURE_MAGIC_ICON_SMALL_SIZE * TEXTURE_SKILL_ENABLED_SCALE, 
				tx, ty, tw, th);
		}
		else if (currentMagic == UI_MAGIC_FLAMEBLADE)
		{
			SetSpriteLeftTop(g_VertexBuffer, 
				UI_MAGIC_ICON_MIDDLE_POS_X + UI_SKILL_ENABLED_OFFSET_X, 
				UI_MAGIC_ICON_MIDDLE_POS_Y + UI_SKILL_ENABLED_OFFSET_Y,
				TEXTURE_MAGIC_ICON_SIZE * TEXTURE_SKILL_ENABLED_SCALE, 
				TEXTURE_MAGIC_ICON_SIZE * TEXTURE_SKILL_ENABLED_SCALE, 
				tx, ty, tw, th);
		}
		else if (rightMagic == UI_MAGIC_FLAMEBLADE)
		{
			SetSpriteLeftTop(g_VertexBuffer, 
				UI_MAGIC_ICON_RIGHT_POS_X + UI_SKILL_ENABLED_SMALL_OFFSET_X,
				UI_MAGIC_ICON_RIGHT_POS_Y + UI_SKILL_ENABLED_SMALL_OFFSET_Y,
				TEXTURE_MAGIC_ICON_SMALL_SIZE * TEXTURE_SKILL_ENABLED_SCALE, 
				TEXTURE_MAGIC_ICON_SMALL_SIZE * TEXTURE_SKILL_ENABLED_SCALE, 
				tx, ty, tw, th);
		}
		GetDeviceContext()->Draw(4, 0);
	}

}

void DrawFlamebladeIcon(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_FLAMEBLADE_ICON]);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// アニメーション用
	float tw = 1.0f / TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_X;	// テクスチャの幅
	float th = 1.0f / TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_Y;	// テクスチャの高さ
	float tx = (float)(g_flamebladePatternAnim % TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
	float ty = (float)(g_flamebladePatternAnim / TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_Y) * th;	// テクスチャの左上Y座標

	PLAYER* player = GetPlayer();
	if (player->flameblade == FALSE)
	{
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
		SetMaterial(material);
	}
	SetSpriteLeftTop(g_VertexBuffer, 
		g_UI[UI_FLAMEBLADE_ICON].pos.x, g_UI[UI_FLAMEBLADE_ICON].pos.y,
		g_UI[UI_FLAMEBLADE_ICON].width, g_UI[UI_FLAMEBLADE_ICON].height, 
		tx, ty, tw, th);
	GetDeviceContext()->Draw(4, 0);
}

void DrawPauseUI(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG_PAUSE_TEXTURE]);
	SetSpriteColor(g_VertexBuffer,
		SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
	GetDeviceContext()->Draw(4, 0);

	// システムメニューボックス
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_SYSTEM_MENU_BOX]);
	SetSpriteColor(g_VertexBuffer,
		UI_SYSTEM_MENU_BOX_PAUSE_POS_X, UI_SYSTEM_MENU_BOX_PAUSE_POS_Y,
		TEXTURE_SYSTEM_MENU_BOX_PAUSE_WIDTH, TEXTURE_SYSTEM_MENU_BOX_PAUSE_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);

	// ボタン
	BOOL selected = g_Cursor == UI_BUTTON_YES;
	DrawButton(UI_BUTTON_YES, selected);

	selected = g_Cursor == UI_BUTTON_NO;
	DrawButton(UI_BUTTON_NO, selected);
}

void DrawButton(int button, BOOL selected)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[button]);
	float buttonWidth, buttonHeight;

	if (selected == TRUE)
	{
		buttonWidth = g_UI[button].width * UI_BUTTON_SELECTED_SCALE;
		buttonHeight = g_UI[button].height * UI_BUTTON_SELECTED_SCALE;
	}
	else
	{
		buttonWidth = g_UI[button].width;
		buttonHeight = g_UI[button].height;
	}
	SetSpriteColor(g_VertexBuffer,
		g_UI[button].pos.x, g_UI[button].pos.y,
		buttonWidth, buttonHeight,
		0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
}