//=============================================================================
//
// tutorial処理 [tutorial.cpp]
// Author : 
//
//=============================================================================
#include "tutorial.h"
#include "player.h"
#include "UI.h"
#include "enemy.h"
#include "input.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define DIALOGUE_FADE_TIME_RATIO		(0.2)
#define DIALOGUE_DISPLAY_INTERVAL		(40)
#define DIALOGUE_DISPLAY_TIME			(30)

#define DIALOGUE_POS_X					(500.0f)
#define DIALOGUE_POS_Y					(200.0f)
#define TEXTURE_DIALOGUE_WIDTH			(540)
#define TEXTURE_DIALOGUE_HEIGHT			(130)

#define HIGHLIGHT_BOX_GAUGE_NO			(5)
#define HIGHLIGHT_BOX_GAUGE_POS_X		(222.0f)
#define HIGHLIGHT_BOX_GAUGE_POS_Y		(40.0f)
#define HIGHLIGHT_BOX_GAUGE_WIDTH		(500.0f)
#define HIGHLIGHT_BOX_GAUGE_HEIGHT		(100.0f)
#define HIGHLIGHT_BOX_JUMP_ICON_NO		(9)
#define HIGHLIGHT_BOX_JUMP_ICON_POS_X	(50.0f)
#define HIGHLIGHT_BOX_JUMP_ICON_POS_Y	(90.0f)
#define HIGHLIGHT_BOX_JUMP_ICON_WIDTH	(100.0f)
#define HIGHLIGHT_BOX_JUMP_ICON_HEIGHT	(50.0f)
#define HIGHLIGHT_BOX_SCORE_POS_X		(50.0f)
#define HIGHLIGHT_BOX_SCORE_POS_Y		(90.0f)
#define HIGHLIGHT_BOX_SCORE_WIDTH		(100.0f)
#define HIGHLIGHT_BOX_SCORE_HEIGHT		(50.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial/dialogue_bg.png",
	"data/TEXTURE/tutorial/Highlight_Box.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_01.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_02.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_03.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_04.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_05.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_06.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_07.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_08.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_09.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_10.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_11.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_12.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_13.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_14.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_15.png",
};

static int				g_tutorialStage;
static BOOL				g_Load = FALSE;		// 初期化を行ったかのフラグ
static DialogueSprite	g_Dialogue;
static DialogueSprite	g_HighlightBox;
static int				g_DialogueTexNo;
static int				g_Wait;
static BOOL				g_DisplayHighlightBox;
static int				g_TutorialPauseType = PAUSE_NONE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(void)
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

	int map = GetCurrentMap();

	g_DialogueTexNo = DIALOGUE_03;
	g_tutorialStage = TUTORIAL_RUN;
	g_Dialogue.fadeCount = 0;
	g_Dialogue.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Dialogue.w = 0.0f;
	g_Dialogue.h = 0.0f;
	g_DisplayHighlightBox = FALSE;
	g_TutorialPauseType = PAUSE_NONE;

	g_HighlightBox.fadeCount = 0;
	g_HighlightBox.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_HighlightBox.w = 0.0f;
	g_HighlightBox.h = 0.0f;

	SetLimitPlayerMove(TRUE);
	SetRenderGauge(FALSE);
	SetRenderSkillIcon(FALSE);
	SetRenderJumpIcon(FALSE);
	SetRenderBladeIcon(FALSE);
	SetPlayerHP(PLAYER_MAX_HP * 0.3f);
	SetPlayerMP(PLAYER_MAX_MP * 0.2f);
	SetRenderScore(FALSE);
	DisablePlayerAttack(TRUE);
	DisablePlayerJump(TRUE);
	DisablePlayerMagic(TRUE);
	DisablePlayerDash(TRUE);
	DisablePlayerDefend(TRUE);
	DisablePlayerRun(TRUE);
	SetUpdateEnemy(FALSE);
	SetTutorialWait(100);
	

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
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
void UpdateTutorial(void)
{
	if (g_Wait > 0)
		g_Wait--;
	else if (g_TutorialPauseType == NONE)
	{
		g_Dialogue.fadeCount++;
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME + DIALOGUE_DISPLAY_INTERVAL)
		{
			g_Dialogue.fadeCount = 0;
			g_DialogueTexNo++;
			g_DisplayHighlightBox = FALSE;
		}
	}
	switch (g_DialogueTexNo)
	{
	case HIGHLIGHT_BOX_GAUGE_NO:
	{
		g_DisplayHighlightBox = TRUE;
		SetRenderGauge(TRUE);
		g_HighlightBox.w = HIGHLIGHT_BOX_GAUGE_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_GAUGE_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_GAUGE_POS_X, HIGHLIGHT_BOX_GAUGE_POS_Y, 0.0f);
		break;
	}
	case HIGHLIGHT_BOX_JUMP_ICON_NO:
	{
		g_DisplayHighlightBox = TRUE;
		SetRenderJumpIcon(TRUE);
		g_HighlightBox.w = HIGHLIGHT_BOX_JUMP_ICON_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_JUMP_ICON_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_JUMP_ICON_POS_X, HIGHLIGHT_BOX_JUMP_ICON_POS_Y, 0.0f);
		break;
	}
	case DIALOGUE_03:
		DisablePlayerRun(FALSE);
		break;

	case DIALOGUE_05:
	{
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * 0.75 && g_tutorialStage == TUTORIAL_RUN)
		{
			SetTutorialPause(PAUSE_RUN);
		}
		break;
	}
	case DIALOGUE_07:
		DisablePlayerJump(FALSE);
		DisablePlayerDash(FALSE);
		break;
	case DIALOGUE_09:
		DisablePlayerAttack(FALSE);
		break;
	case DIALOGUE_11:
	{
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * 0.75 && g_tutorialStage == TUTORIAL_ATTACK)
		{
			SetTutorialPause(PAUSE_ATTACK);
		}
		break;
	}
	case DIALOGUE_13:
	{
		if (g_Dialogue.fadeCount == 0)
		{
			SetUpdateEnemy(TRUE);
			DisablePlayerAttack(TRUE);
			DisablePlayerJump(TRUE);
			DisablePlayerMagic(TRUE);
			DisablePlayerDash(TRUE);
			DisablePlayerDefend(TRUE);
			DisablePlayerRun(TRUE);
			DisablePlayerRightMove(TRUE);
			DisablePlayerLeftMove(TRUE);
			SetPlayerDir(CHAR_DIR_LEFT);
		}

		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * 0.75 && g_tutorialStage == TUTORIAL_ENEMY)
		{
			SetTutorialPause(PAUSE_ENEMY);
		}
		if (GetUpdateEnemy() == FALSE && g_tutorialStage == TUTORIAL_ENEMY)
		{
			SetTutorialPause(NONE);
		}
		break;
	}
	case DIALOGUE_14:
	{
		DisablePlayerDash(FALSE);
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * 0.75 && g_tutorialStage == TUTORIAL_DASH)
		{
			SetTutorialPause(PAUSE_DASH);
			SetUpdateEnemy(FALSE);
		}
		break;
	}
	case DIALOGUE_15:
	{
		PLAYER* player = GetPlayer();
		ENEMY* enemy = GetEnemy();
		if (player->pos.x > enemy->pos.x - 90.0f)
			SetPlayerPosX(enemy->pos.x - 90.0f);

		if (GetKeyboardTrigger(DIK_X))
		{
			SetUpdatePlayer(TRUE);
			SetUpdateEnemy(TRUE);
		}
		SetPlayerInvincible(TRUE);
		DisablePlayerRightMove(FALSE);
		DisablePlayerAttack(FALSE);
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * 0.75 && g_tutorialStage == TUTORIAL_COMBO)
		{
			SetTutorialPause(TUTORIAL_COMBO);
		}
		break;
	}
	case DIALOGUE_16:
	{
		SetPlayerInvincible(FALSE);
		DisablePlayerJump(FALSE);
		DisablePlayerDash(FALSE);
		DisablePlayerDefend(FALSE);
		DisablePlayerRun(FALSE);
		DisablePlayerLeftMove(FALSE);
		SetRenderScore(TRUE);

		g_DisplayHighlightBox = TRUE;
		g_HighlightBox.w = HIGHLIGHT_BOX_JUMP_ICON_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_JUMP_ICON_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_JUMP_ICON_POS_X, HIGHLIGHT_BOX_JUMP_ICON_POS_Y, 0.0f);
		break;
	}
	default:
		break;
	}
}

void DrawTutorial(void)
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

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	float fadeTime = DIALOGUE_DISPLAY_TIME * DIALOGUE_FADE_TIME_RATIO;
	float alpha = 1.0f;

	if (g_Dialogue.fadeCount <= fadeTime)
		alpha = g_Dialogue.fadeCount / fadeTime;
	else if (g_Dialogue.fadeCount > fadeTime && g_Dialogue.fadeCount <= DIALOGUE_DISPLAY_TIME - fadeTime)
		alpha = 1.0f;
	else if (g_Dialogue.fadeCount <= DIALOGUE_DISPLAY_TIME)
		alpha =  1.0f - (g_Dialogue.fadeCount - DIALOGUE_DISPLAY_TIME * (1.0f - DIALOGUE_FADE_TIME_RATIO)) / fadeTime;
	else
		alpha = 0.0f;

	float px = DIALOGUE_POS_X;
	float py = DIALOGUE_POS_Y;
	float pw = TEXTURE_DIALOGUE_WIDTH;
	float ph = TEXTURE_DIALOGUE_HEIGHT;
	AdjustDialogueWH(&pw, &ph);

	color.w = alpha;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[DIALOGUE_BG_TEX_NO]);
	SetSpriteColor(g_VertexBuffer,
		px, py,
		pw, ph,
		0.0f, 0.0f, 1.0f, 1.0f,
		color);
	GetDeviceContext()->Draw(4, 0);

	color.w = alpha;
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_DialogueTexNo]);
	SetSpriteColor(g_VertexBuffer,
		px, py,
		pw, ph,
		0.0f, 0.0f, 1.0f, 1.0f,
		color);
	GetDeviceContext()->Draw(4, 0);

	if (g_DisplayHighlightBox)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[HIGHLIGHT_BOX_TEX_NO]);
		SetSpriteColor(g_VertexBuffer,
			g_HighlightBox.pos.x, g_HighlightBox.pos.y,
			g_HighlightBox.w, g_HighlightBox.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			color);
		GetDeviceContext()->Draw(4, 0);
	}


}

void SetTutorialWait(int wait)
{
	g_Wait = wait;
}

void AdjustDialogueWH(float* w, float* h)
{
	switch (g_DialogueTexNo)
	{
	case DIALOGUE_03:
		*w *= 1.4f;
		*h *= 1.0f;
		break;
	case DIALOGUE_04:
		*w *= 1.2f;
		*h *= 1.0f;
		break;
	case DIALOGUE_06:
		*w *= 1.2f;
		*h *= 1.0f;
		break;
	case DIALOGUE_07:
		*w *= 1.4f;
		*h *= 1.0f;
		break;
	case DIALOGUE_08:
		*w *= 1.3f;
		*h *= 1.0f;
		break;
	case DIALOGUE_10:
		*w *= 1.3f;
		*h *= 1.0f;
		break;
	case DIALOGUE_11:
		*w *= 1.1f;
		*h *= 0.8f;
		break;
	case DIALOGUE_12:
		*w *= 1.3f;
		*h *= 1.0f;
		break;
	case DIALOGUE_13:
		*w *= 1.3f;
		*h *= 1.0f;
		break;
	default:
		break;
	}
}

void SetTutorialPause(int pauseType)
{
	g_TutorialPauseType = pauseType;

	if (pauseType == PAUSE_NONE)
		g_tutorialStage = GetNextStage();
}
int GetTutorialPause(void)
{
	return g_TutorialPauseType;
}

int GetNextStage(void)
{
	switch (g_tutorialStage)
	{
	case TUTORIAL_RUN:
		return TUTORIAL_ATTACK;
	case TUTORIAL_ATTACK:
		return TUTORIAL_ENEMY;
	case TUTORIAL_ENEMY:
		return TUTORIAL_DASH;
	case TUTORIAL_DASH:
		return TUTORIAL_COMBO;
	default:
		return -1;
	}
}