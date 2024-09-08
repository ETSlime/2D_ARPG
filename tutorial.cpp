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

#define DIALOGUE_FADE_TIME_RATIO		(0.12)
#define DIALOGUE_DISPLAY_INTERVAL		(40)
#define DIALOGUE_DISPLAY_TIME			(150)

#define DIALOGUE_POS_X					(500.0f)
#define DIALOGUE_POS_Y					(170.0f)
#define TEXTURE_DIALOGUE_WIDTH			(570)
#define TEXTURE_DIALOGUE_HEIGHT			(120)

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
#define HIGHLIGHT_BOX_SCORE_POS_X		(865.0f)
#define HIGHLIGHT_BOX_SCORE_POS_Y		(20.0f)
#define HIGHLIGHT_BOX_SCORE_WIDTH		(215.0f)
#define HIGHLIGHT_BOX_SCORE_HEIGHT		(50.0f)
#define HIGHLIGHT_BOX_MAGIC_POS_X		(113.0f)
#define HIGHLIGHT_BOX_MAGIC_POS_Y		(490.0f)
#define HIGHLIGHT_BOX_MAGIC_WIDTH		(225.0f)
#define HIGHLIGHT_BOX_MAGIC_HEIGHT		(100.0f)

#define DISPLAY_DIALOGUE_RATE			(0.25f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

static ID3D11Buffer*				g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView*	g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static float						g_DialogueDisplaySpeedRate = 1.0f;
static int							g_NextDialogue = -1;

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial/dialogue_bg.png",
	"data/TEXTURE/tutorial/Highlight_Box.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_00.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_00_2.png",
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
	"data/TEXTURE/tutorial/tutorial_dialogue_16.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_17.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_18.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_19.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_20.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_21.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_22.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_23.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_24.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_25.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_26.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_27.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_28.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_29.png",
	"data/TEXTURE/tutorial/tutorial_dialogue_30.png",
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
	g_DialogueDisplaySpeedRate = 1.0f;

	int map = GetCurrentMap();

	g_DialogueTexNo = DIALOGUE_21;
	g_tutorialStage = TUTORIAL_EXPLORE;

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

	g_NextDialogue = -1;

	SetPlayerHP(PLAYER_MAX_HP * 0.3f);
	SetPlayerMP(PLAYER_MAX_MP * 0.2f);

	SetLimitPlayerMoveLeft(TRUE);
	SetLimitPlayerMoveRight(TRUE);

	SetRenderGauge(FALSE);
	SetRenderJumpIcon(FALSE);
	SetRenderSkillIcon(FALSE);
	SetRenderBladeIcon(FALSE);
	SetRenderScore(FALSE);

	DisableAllPlayerAction(TRUE);
	DisablePlayerLeftMove(FALSE);
	DisablePlayerRightMove(FALSE);

	SetUpdateEnemyByIdx(TUTORIAL_CYCLOPS_ID, FALSE);

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
	if (GetKeyboardTrigger(DIK_RCONTROL))
	{
		g_DialogueDisplaySpeedRate = DISPLAY_DIALOGUE_RATE;
		g_Dialogue.fadeCount *= g_DialogueDisplaySpeedRate;
	}
		
	if (GetKeyboardPress(DIK_RCONTROL))
	{
		g_DialogueDisplaySpeedRate = DISPLAY_DIALOGUE_RATE;
	}
		
	if (GetKeyboardRelease(DIK_RCONTROL))
	{
		g_Dialogue.fadeCount /= g_DialogueDisplaySpeedRate;
		g_DialogueDisplaySpeedRate = 1.0f;
	}
		
	
	if (g_Wait > 0)
	{
		g_Wait--;
	}
	else if (g_TutorialPauseType == NONE)
	{
		g_Dialogue.fadeCount++;
		if (g_Dialogue.fadeCount > (DIALOGUE_DISPLAY_TIME + DIALOGUE_DISPLAY_INTERVAL) * g_DialogueDisplaySpeedRate)
		{
			g_Dialogue.fadeCount = 0;

			if (g_NextDialogue != -1)
			{
				g_DialogueTexNo = g_NextDialogue;
				g_NextDialogue = -1;
			}
			else
			{
				g_DialogueTexNo++;
			}

			g_DisplayHighlightBox = FALSE;
		}
	}

	PLAYER* player = GetPlayer();
	switch (g_DialogueTexNo)
	{

	case DIALOGUE_03:
		DisablePlayerRun(FALSE);
		break;
	case DIALOGUE_04:
	{
		g_DisplayHighlightBox = TRUE;
		SetRenderGauge(TRUE);
		g_HighlightBox.w = HIGHLIGHT_BOX_GAUGE_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_GAUGE_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_GAUGE_POS_X, HIGHLIGHT_BOX_GAUGE_POS_Y, 0.0f);
		break;
	}
	case DIALOGUE_05:
	{
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f
			&& g_tutorialStage == TUTORIAL_RUN)
		{
			SetTutorialPause(PAUSE_RUN);
		}
		break;
	}
	case DIALOGUE_07:
		DisablePlayerJump(FALSE);
		DisablePlayerDash(FALSE);
		break;
	case DIALOGUE_08:
	{
		g_DisplayHighlightBox = TRUE;
		SetRenderJumpIcon(TRUE);
		g_HighlightBox.w = HIGHLIGHT_BOX_JUMP_ICON_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_JUMP_ICON_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_JUMP_ICON_POS_X, HIGHLIGHT_BOX_JUMP_ICON_POS_Y, 0.0f);
		break;
	}
	case DIALOGUE_09:
		DisablePlayerAttack(FALSE);
		break;
	case DIALOGUE_11:
	{
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f 
			&& g_tutorialStage == TUTORIAL_ATTACK)
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
			DisableAllPlayerAction(TRUE);
			SetPlayerDir(CHAR_DIR_LEFT);
		}

		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f 
			&& g_tutorialStage == TUTORIAL_ENEMY)
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
		SetLimitPlayerMoveLeft(TRUE);
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f 
			&& g_tutorialStage == TUTORIAL_DASH)
		{
			DisablePlayerDash(FALSE);
			SetTutorialPause(PAUSE_DASH);
			SetUpdateEnemy(FALSE);
		}
		break;
	}
	case DIALOGUE_15:
	{
		DisablePlayerDash(TRUE);

		ENEMY* enemy = GetEnemy();
		if (player->pos.x > enemy->pos.x - 90.0f)
			SetPlayerPosX(enemy->pos.x - 90.0f);

		if (player->dir == CHAR_DIR_RIGHT)
			DisablePlayerAttack(FALSE);

		if (GetKeyboardTrigger(DIK_X) && player->dir == CHAR_DIR_RIGHT)
		{
			SetUpdatePlayer(TRUE);
			SetUpdateEnemy(TRUE);
		}

		SetPlayerInvincible(TRUE);

		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f 
			&& g_tutorialStage == TUTORIAL_COMBO)
		{
			DisablePlayerRightMove(FALSE);
			SetTutorialPause(PAUSE_COMBO);
		}
		break;
	}
	case DIALOGUE_16:
	{
		SetPlayerInvincible(FALSE);
		DisablePlayerDash(FALSE);
		DisablePlayerJump(FALSE);
		DisablePlayerDash(FALSE);
		DisablePlayerRun(FALSE);
		DisablePlayerLeftMove(FALSE);
		SetRenderScore(TRUE);

		g_DisplayHighlightBox = TRUE;
		g_HighlightBox.w = HIGHLIGHT_BOX_SCORE_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_SCORE_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_SCORE_POS_X, HIGHLIGHT_BOX_SCORE_POS_Y, 0.0f);
		break;
	}
	case DIALOGUE_17:
	{
		SetUpdateEnemy(FALSE);
		SetRenderSkillIcon(TRUE);
		SetRenderBladeIcon(TRUE);

		g_DisplayHighlightBox = TRUE;
		g_HighlightBox.w = HIGHLIGHT_BOX_MAGIC_WIDTH;
		g_HighlightBox.h = HIGHLIGHT_BOX_MAGIC_HEIGHT;
		g_HighlightBox.pos = XMFLOAT3(HIGHLIGHT_BOX_MAGIC_POS_X, HIGHLIGHT_BOX_MAGIC_POS_Y, 0.0f);
		break;
	}
	case DIALOGUE_18:
	{
		DisablePlayerMagicSwitch(FALSE);
		DisablePlayerHealing(FALSE);
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f
			&& g_tutorialStage == TUTORIAL_MAGIC)
		{
			SetTutorialPause(PAUSE_MAGIC);
		}
		break;
	}
	case DIALOGUE_21:
	{
		SetUpdateEnemy(TRUE);

		SetRenderGauge(TRUE);
		SetRenderJumpIcon(TRUE);
		SetRenderSkillIcon(TRUE);
		SetRenderBladeIcon(TRUE);
		SetRenderScore(TRUE);

		DisableAllPlayerAction(FALSE);
		DisablePlayerDefend(TRUE);
		SetLimitPlayerMoveLeft(FALSE);
		SetLimitPlayerMoveRight(FALSE);

		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate
			&& g_tutorialStage == TUTORIAL_EXPLORE)
		{
			SetTutorialPause(PAUSE_EXPLORE);
		}

		if (player->pos.x >= 1551.0f && g_tutorialStage == TUTORIAL_EXPLORE)
		{
			SetTutorialPause(NONE);
		}

		break;
	}
	case DIALOGUE_22:
	{
		if (player->pos.x >= 2340.0f)
		{
			SetTutorialPause(NONE);
			g_tutorialStage = TUTORIAL_DEFEND;
			ScrollBG(300.0f, -300.0f, 200.0f);
			DisableAllPlayerAction(TRUE);
			g_NextDialogue = DIALOGUE_25;
		}
		break;
	}
	case DIALOGUE_23:
	{
		if (player->pos.x >= 2340.0f)
		{
			SetTutorialPause(NONE);
			g_tutorialStage = TUTORIAL_DEFEND;
			ScrollBG(300.0f, -300.0f, 200.0f);
			DisableAllPlayerAction(TRUE);
			g_NextDialogue = DIALOGUE_25;
		}

		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f
			&& g_tutorialStage == TUTORIAL_AIRJUMP)
		{
			SetTutorialPause(PAUSE_AIRJUMP);
		}

		if (player->pos.x >= 1860.0f && g_tutorialStage == TUTORIAL_AIRJUMP)
		{
			SetTutorialPause(NONE);
		}

		break;
	}
	case DIALOGUE_24:
	{

		if (player->pos.x >= 2340.0f && g_tutorialStage == TUTORIAL_EXPLORE2)
		{
			SetTutorialPause(NONE);
			ScrollBG(300.0f, -300.0f, 130.0f);
			DisableAllPlayerAction(TRUE);
		}

		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate
			&& g_tutorialStage == TUTORIAL_EXPLORE2)
		{
			SetTutorialPause(PAUSE_EXPLORE2);
		}

		break;
	}
	case DIALOGUE_26:
	{
		SetUpdateEnemyByIdx(TUTORIAL_CYCLOPS_ID, TRUE);
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.75f
			&& g_tutorialStage == TUTORIAL_DEFEND)
		{
			SetTutorialPause(PAUSE_DEFEND);
		}

		if (GetUpdateEnemy() == FALSE)
		{
			DisablePlayerDefendCount(TRUE);
			DisablePlayerDefend(FALSE);
		}

		break;
	}
	case DIALOGUE_27:
	{
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * 0.25f
			&& g_tutorialStage == TUTORIAL_PARRY)
		{
			DisablePlayerAttack(FALSE);
			SetTutorialPause(PAUSE_PARRY);
		}

		break;
	}
	case DIALOGUE_28:
	{
		DisableAllPlayerAction(FALSE);
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate
			&& g_tutorialStage == TUTORIAL_ENEMY2)
		{
			SetTutorialPause(PAUSE_ENEMY2);
		}
	}
	case DIALOGUE_29:
	{
		if (g_Dialogue.fadeCount > DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate
			&& g_tutorialStage == TUTORIAL_FINAL)
		{
			SetTutorialPause(PAUSE_FINAL);
		}
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

	int currentTime = g_Dialogue.fadeCount;
	float fadeTime = DIALOGUE_DISPLAY_TIME * DIALOGUE_FADE_TIME_RATIO * g_DialogueDisplaySpeedRate;
	float alpha = 1.0f;

	if (currentTime <= fadeTime)
		alpha = currentTime / fadeTime;
	else if (currentTime > fadeTime && currentTime <= DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate - fadeTime)
		alpha = 1.0f;
	else if (currentTime <= DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate)
		alpha =  1.0f - (currentTime - DIALOGUE_DISPLAY_TIME * g_DialogueDisplaySpeedRate * (1.0f - DIALOGUE_FADE_TIME_RATIO)) / fadeTime;
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
		*h *= 0.6f;
		break;
	case DIALOGUE_12:
		*w *= 1.1f;
		*h *= 0.9f;
		break;
	case DIALOGUE_13:
		*w *= 1.3f;
		*h *= 1.0f;
		break;
	case DIALOGUE_15:
		*w *= 0.7f;
		*h *= 0.8f;
		break;
	case DIALOGUE_18:
		*w *= 1.1f;
		*h *= 1.0f;
		break;
	case DIALOGUE_20:
		*w *= 1.4f;
		*h *= 1.4f;
		break;
	case DIALOGUE_21:
		*w *= 1.1f;
		*h *= 1.1f;
		break;
	case DIALOGUE_22:
		*w *= 1.2f;
		*h *= 1.0f;
		break;
	case DIALOGUE_28:
		*w *= 1.2f;
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

int GetTutorialStage(void)
{
	return g_tutorialStage;
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
	case TUTORIAL_COMBO:
		return TUTORIAL_MAGIC;
	case TUTORIAL_MAGIC:
		return TUTORIAL_EXPLORE;
	case TUTORIAL_EXPLORE:
		return TUTORIAL_AIRJUMP;
	case TUTORIAL_AIRJUMP:
		return TUTORIAL_EXPLORE2;
	case TUTORIAL_EXPLORE2:
		return TUTORIAL_DEFEND;
	case TUTORIAL_DEFEND:
		return TUTORIAL_PARRY;
	case TUTORIAL_PARRY:
		return TUTORIAL_ENEMY2;
	case TUTORIAL_ENEMY2:
		return TUTORIAL_FINAL;
	default:
		return -1;
	}
}

void DisableAllPlayerAction(BOOL disable)
{
	DisablePlayerLeftMove(disable);
	DisablePlayerRightMove(disable);
	DisablePlayerAttack(disable);
	DisablePlayerJump(disable);
	DisablePlayerMagicSwitch(disable);
	DisablePlayerHealing(disable);
	DisablePlayerFireBall(disable);
	DisablePlayerFlameblade(disable);
	DisablePlayerDash(disable);
	DisablePlayerDefend(disable);
	DisablePlayerRun(disable);
}