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
#include "score.h"
#include "sound.h"

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
#define	UI_MAX							(45)
#define	GAUGE_MAX						(3)

#define TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_X	(4)
#define TEXTURE_FLAMEBLADE_ICON_PATTERN_DIVIDE_Y	(2)
#define TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_X		(9)
#define TEXTURE_SKILL_ENABLED_PATTERN_DIVIDE_Y		(4)

#define ANIM_WAIT_FLAMEBLADE_ICON		(5)
#define ANIM_WAIT_SKILL_ENABLED			(8)

// タイトルUIテクスチャサイズ
#define TEXTURE_SYSTEM_MENU_BOX_TITLE_WIDTH			(450.0f)
#define TEXTURE_SYSTEM_MENU_BOX_TITLE_HEIGHT		(250.0f)
#define TEXTURE_BUTTON_TITLE_WIDTH					(260.0f)
#define TEXTURE_BUTTON_TITLE_HEIGHT					(40.0f)
// タイトルUIポジション
#define UI_TITLE_BUTTON_POS_X						(470.0f)
#define	UI_TITLE_BUTTON_POS_Y						(330.0f)
#define UI_TITLE_BUTTON_OFFSET_Y					(70.0f)


// メッセージボックステクスチャサイズ
#define TEXTURE_MESSAGE_BOX_WIDTH					(580.0f)
#define TEXTURE_MESSAGE_BOX_HEIGHT					(350.0f)
//メッセージボックスポジション
#define UI_MESSAGE_BOX_POS_X						(490.0f)
#define UI_MESSAGE_BOX_POS_Y						(300.0f)

// システムメニューボックステクスチャサイズ
#define TEXTURE_SYSTEM_MENU_BOX_WIDTH				(640.0f)
#define TEXTURE_SYSTEM_MENU_BOX_HEIGHT				(450.0f)
// システムメニューボックスポジション
#define UI_SYSTEM_MENU_BOX_POS_X					(520.0f)
#define UI_SYSTEM_MENU_BOX_POS_Y					(285.0f)

// システムメニューボタンテクスチャサイズ
#define TEXTURE_SYSTEM_MENU_BUTTON_WIDTH			(260.0f)
#define TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT			(40.0f)
// システムメニューボタンポジション
#define UI_SYSTEM_MENU_BUTTON_POS_X					(510.0f)
#define	UI_SYSTEM_MENU_BUTTON_POS_Y					(125.0f)
#define UI_SYSTEM_MENU_BUTTON_OFFSET_Y				(65.0f)

#define SYSTEM_MENU_BUTTON_NUM						(6)

#define TEXTURE_SUBMENU_BUTTON_WIDTH				(165.0f)
#define TEXTURE_SUBMENU_BUTTON_HEIGHT				(45.0f)
#define UI_CONFIRM_BUTTON_POS_X						(364.0f)
#define UI_CONFIRM_BUTTON_POS_Y						(474.0f)
#define UI_CANCEL_BUTTON_POS_X						(648.0f)
#define UI_CANCEL_BUTTON_POS_Y						(472.0f)

// ステータステクスチャサイズ
#define TEXTURE_PLAYER_STATUS_WIDTH					(580.0f)
#define TEXTURE_PLAYER_STATUS_HEIGHT				(480.0f)
// ステータスポジション
#define UI_PLAYER_STATUS_POS_X						(510.0f)
#define	UI_PLAYER_STATUS_POS_Y						(275.0f)
#define TEXTURE_NUMBER_WIDTH						(21.0f)
#define TEXTURE_NUMBER_HEIGHT						(27.0f)
#define NUMBER_LEVEL_POS_X							(761.0f)
#define NUMBER_LEVEL_POS_Y							(80.5f)
#define NUMBER_SKILL_POINT_POS_X					(551.0f)
#define NUMBER_SKILL_POINT_POS_Y					(80.5f)
#define NUMBER_CURRENT_EXP_POS_X					(551.0f)
#define NUMBER_CURRENT_EXP_POS_Y					(108.0f)
#define NUMBER_REQUIRED_EXP_POS_X					(551.0f)
#define NUMBER_REQUIRED_EXP_POS_Y					(138.0f)
#define NUMBER_SP_HP_POS_X							(481.0f)
#define NUMBER_SP_HP_POS_Y							(184.5f)
#define NUMBER_MAX_HP_POS_X							(752.0f)
#define NUMBER_MAX_HP_POS_Y							(184.5f)
#define NUMBER_STATUS_OFFSET_Y						(40.6f)
#define TEXTURE_ARROW_WIDTH							(31.0f)
#define TEXTURE_ARROW_HEIGHT						(21.0f)
#define UI_RIGHT_ARROW_POS_X						(515.0f)
#define UI_LEFT_ARROW_POS_X							(425.0f)
#define UI_ARROW_SELECTED_OFFSET					(15.5f)
#define UI_ARROW_MOVE_CNT_MAX						(125)

// マジックリストテクスチャサイズ
#define TEXTURE_MAGIC_LIST_WIDTH					(580.0f)
#define TEXTURE_MAGIC_LIST_HEIGHT					(480.0f)
// マジックリストポジション
#define UI_MAGIC_LIST_POS_X							(510.0f)
#define	UI_MAGIC_LIST_POS_Y							(275.0f)

#define TEXTURE_MAGIC_SLOT_WIDTH					(95.0f)
#define TEXTURE_MAGIC_SLOT_HEIGHT					(95.0f)
#define	UI_MAGIC_SLOT_POS_X							(291.0f)
#define UI_MAGIC_SLOT_POS_Y							(130.0f)
#define UI_MAGIC_SLOT_OFFSET_X						(110.0f)
#define UI_MAGIC_SLOT_NUM							(5)

#define UI_MAGIC_ICON_SCALE_CNT_MAX					(185)

#define TEXTURE_MAGIC_LIST_ICON_WIDTH				(65.0f)
#define TEXTURE_MAGIC_LIST_ICON_HEIGHT				(65.0f)
#define UI_MAGIC_LIST_ICON_INIT_POS_X				(291.0f)
#define UI_MAGIC_LIST_ICON_INIT_POS_Y				(270.0f)
#define UI_MAGIC_LIST_ICON_OFFSET_X					(95.0f)
#define UI_MAGIC_LIST_ICON_OFFSET_Y					(95.0f)
#define UI_MAGIC_LIST_ICON_NUM_PER_ROW				(5)

#define TEXTURE_MAGIC_LIST_ICON_BOX_WIDTH			(92.0f)
#define TEXTURE_MAGIC_LIST_ICON_BOX_HEIGHT			(92.0f)

// 一時停止UIテクスチャサイズ
#define	TEXTURE_BUTTON_PAUSE_WIDTH					(150.0f)
#define	TEXTURE_BUTTON_PAUSE_HEIGHT					(40.0f)
// 一時停止UIポジション
#define UI_PAUSE_BUTTON_POS_X						(390.0f)
#define UI_PAUSE_BUTTON_POS_Y						(295.0f)
#define UI_PAUSE_BUTTON_OFFSET_X					(200.0f)
#define UI_BUTTON_SELECTED_SCALE					(1.2f)

#define	FADE_RATE									(0.04f)			// フェード係数
#define CURSOR_DELTA_TIME							(3)

#define UI_RESPAWN_MESSAGE_OFFSET_Y					(40.0f)

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
	"data/TEXTURE/UI/hidden.png",
	"data/TEXTURE/UI/shield.png",
	"data/TEXTURE/UI/flameblade_icon.png",
	"data/TEXTURE/UI/skillEnabled.png",
	"data/TEXTURE/UI/messagebox_return_title.png",
	"data/TEXTURE/UI/button_new_game.png",
	"data/TEXTURE/UI/button_tutorial.png",
	"data/TEXTURE/UI/button_exit_game.png",
	"data/TEXTURE/UI/button_yes.png",
	"data/TEXTURE/UI/button_no.png",
	"data/TEXTURE/UI/messagebox_tutorial.png",
	"data/TEXTURE/UI/messagebox_respawn.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/UI/messagebox_tutorial_respawn.png",
	"data/TEXTURE/UI/systemMenuBox_no_message.png",
	"data/TEXTURE/UI/status_button.png",
	"data/TEXTURE/UI/magic_button.png",
	"data/TEXTURE/UI/teleport_button.png",
	"data/TEXTURE/UI/save_button.png",
	"data/TEXTURE/UI/load_button.png",
	"data/TEXTURE/UI/resume_button.png",
	"data/TEXTURE/UI/player_status.png",
	"data/TEXTURE/UI/magic_ui.png",
	"data/TEXTURE/UI/save_slot.png",
	"data/TEXTURE/UI/save_ui.png",
	"data/TEXTURE/UI/load_ui.png",
	"data/TEXTURE/UI/confirm_button.png",
	"data/TEXTURE/UI/return_button.png",
	"data/TEXTURE/UI/number.png",
	"data/TEXTURE/UI/right_arrow.png",
	"data/TEXTURE/UI/magic_slot.png",
	"data/TEXTURE/UI/Highlight_Box.png",
	"data/TEXTURE/UI/skill_box.png",
};

static UISprite					g_UI[UI_MAX];
static int						g_flamebladeIconCountAnim;
static int						g_flamebladePatternAnim;
static int						g_skillEnabledCountAnim;
static int						g_skillEnabledPatternAnim;
static BOOL						g_Load = FALSE;
static int						g_Cursor;
static int						g_MenuUIModule;
static int						g_MenuUIModuleNext;
static BOOL						g_IsFading;
static int						g_Pause;
static int						g_cursorCurretTime;
static int						g_cursorActionTime;
static BOOL						g_PlayerRespawnMsg;
static BOOL						g_RenderGauge;
static BOOL						g_RenderSkillIcon;
static BOOL						g_RenderBladeIcon;
static BOOL						g_RenderJumpIcon;
static BOOL						g_RenderMenuUI;
static float					g_ArrowMove;
static int						g_ArrowMoveCnt;
static float					g_NewMagicScale;
static int						g_NewMagicScaleCnt;
static int						g_MagicLearnedCnt;
static LevelUpStatus			g_LevelUpStatus[SP_CNT_MAX + STATUS_CNT_MAX];
static BOOL						g_ChangeMagic;
static int						g_MagicListNew[MAGIC_SLOT_MAX];

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
		g_UI[i].fade = FADE_NONE;
		g_UI[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		g_UI[i].UIModule = UI_MODULE_NONE;
	}

	// ゲージの初期化
	for (int i = 0; i < GAUGE_MAX; i++)
	{
		g_UI[i].use = TRUE;
		g_UI[i].height = TEXTURE_GAUGE_HEIGHT;
		g_UI[i].ratio = 1.0f;
	}

	g_PlayerRespawnMsg = FALSE;
	g_RenderGauge = TRUE;
	g_RenderSkillIcon = TRUE;
	g_RenderBladeIcon = TRUE;
	g_RenderJumpIcon = TRUE;
	g_RenderMenuUI = FALSE;
	g_ChangeMagic = FALSE;

	g_flamebladeIconCountAnim = 0;
	g_flamebladePatternAnim = 0;
	g_skillEnabledCountAnim = 0;
	g_skillEnabledPatternAnim = 0;

	g_ArrowMove = 0;
	g_ArrowMoveCnt = 0;

	g_MagicLearnedCnt = 0;
	g_NewMagicScaleCnt = 0;

	g_MenuUIModule = UI_MODULE_SYSTEM_MENU;
	g_MenuUIModule = UI_MODULE_SYSTEM_MENU;
	g_IsFading = FALSE;

	g_UI[UI_HP].width = TEXTURE_HP_WIDTH;
	g_UI[UI_HP].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y, 0.0f);
	g_UI[UI_HP].texNo = UI_HP;

	g_UI[UI_MP].width = TEXTURE_MP_WIDTH;
	g_UI[UI_MP].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT, 0.0f);
	g_UI[UI_MP].texNo = UI_MP;

	g_UI[UI_ST].width = TEXTURE_ST_WIDTH;
	g_UI[UI_ST].pos = XMFLOAT3(UI_GAUGE_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT * 2, 0.0f);
	g_UI[UI_ST].texNo = UI_ST;

	g_UI[UI_JUMP_ICON].width = TEXTURE_JUMP_ICON_WIDTH;
	g_UI[UI_JUMP_ICON].height = TEXTURE_JUMP_ICON_HEIGHT;
	g_UI[UI_JUMP_ICON].pos = XMFLOAT3(UI_JUMP_ICON_POS_X, UI_GAUGE_POS_Y + TEXTURE_GAUGE_HEIGHT * 3, 0.0f);
	g_UI[UI_JUMP_ICON].texNo = UI_JUMP_ICON;

	g_UI[UI_FLAMEBLADE_ICON].width = TEXTURE_FLAMEBLADE_ICON_WIDTH;
	g_UI[UI_FLAMEBLADE_ICON].height = TEXTURE_FLAMEBLADE_ICON_HEIGHT;
	g_UI[UI_FLAMEBLADE_ICON].pos = XMFLOAT3(UI_FLAMEBLADE_ICON_POS_X, UI_FLAMEBLADE_ICON_POS_Y, 0.0f);
	g_UI[UI_FLAMEBLADE_ICON].texNo = UI_FLAMEBLADE_ICON;

	g_UI[UI_MESSAGEBOX_RETURN_TITLE].width = TEXTURE_MESSAGE_BOX_WIDTH;
	g_UI[UI_MESSAGEBOX_RETURN_TITLE].height = TEXTURE_MESSAGE_BOX_HEIGHT;
	g_UI[UI_MESSAGEBOX_RETURN_TITLE].pos = XMFLOAT3(UI_MESSAGE_BOX_POS_X, UI_MESSAGE_BOX_POS_Y, 0.0f);
	g_UI[UI_MESSAGEBOX_RETURN_TITLE].texNo = UI_MESSAGEBOX_RETURN_TITLE;

	g_UI[UI_BUTTON_NEW_GAME].width = TEXTURE_BUTTON_TITLE_WIDTH;
	g_UI[UI_BUTTON_NEW_GAME].height = TEXTURE_BUTTON_TITLE_HEIGHT;
	g_UI[UI_BUTTON_NEW_GAME].pos = XMFLOAT3(UI_TITLE_BUTTON_POS_X, UI_TITLE_BUTTON_POS_Y, 0.0f);
	g_UI[UI_BUTTON_NEW_GAME].texNo = UI_BUTTON_NEW_GAME;

	g_UI[UI_BUTTON_TUTORIAL].width = TEXTURE_BUTTON_TITLE_WIDTH;
	g_UI[UI_BUTTON_TUTORIAL].height = TEXTURE_BUTTON_TITLE_HEIGHT;
	g_UI[UI_BUTTON_TUTORIAL].pos = XMFLOAT3(UI_TITLE_BUTTON_POS_X, UI_TITLE_BUTTON_POS_Y + UI_TITLE_BUTTON_OFFSET_Y, 0.0f);
	g_UI[UI_BUTTON_TUTORIAL].texNo = UI_BUTTON_TUTORIAL;

	g_UI[UI_BUTTON_EXIT_GAME].width = TEXTURE_BUTTON_TITLE_WIDTH;
	g_UI[UI_BUTTON_EXIT_GAME].height = TEXTURE_BUTTON_TITLE_HEIGHT;
	g_UI[UI_BUTTON_EXIT_GAME].pos = XMFLOAT3(UI_TITLE_BUTTON_POS_X, UI_TITLE_BUTTON_POS_Y + UI_TITLE_BUTTON_OFFSET_Y * 2, 0.0f);
	g_UI[UI_BUTTON_EXIT_GAME].texNo = UI_BUTTON_EXIT_GAME;

	g_UI[UI_BUTTON_YES].width = TEXTURE_BUTTON_PAUSE_WIDTH;
	g_UI[UI_BUTTON_YES].height = TEXTURE_BUTTON_PAUSE_HEIGHT;
	g_UI[UI_BUTTON_YES].pos = XMFLOAT3(UI_PAUSE_BUTTON_POS_X, UI_PAUSE_BUTTON_POS_Y, 0.0f);
	g_UI[UI_BUTTON_YES].texNo = UI_BUTTON_YES;

	g_UI[UI_BUTTON_NO].width = TEXTURE_BUTTON_PAUSE_WIDTH;
	g_UI[UI_BUTTON_NO].height = TEXTURE_BUTTON_PAUSE_HEIGHT;
	g_UI[UI_BUTTON_NO].pos = XMFLOAT3(UI_PAUSE_BUTTON_POS_X + UI_PAUSE_BUTTON_OFFSET_X, UI_PAUSE_BUTTON_POS_Y, 0.0f);
	g_UI[UI_BUTTON_NO].texNo = UI_BUTTON_NO;

	g_UI[UI_SYSTEM_MENU_BOX].width = TEXTURE_SYSTEM_MENU_BOX_WIDTH;
	g_UI[UI_SYSTEM_MENU_BOX].height = TEXTURE_SYSTEM_MENU_BOX_HEIGHT;
	g_UI[UI_SYSTEM_MENU_BOX].pos = XMFLOAT3(UI_SYSTEM_MENU_BOX_POS_X, UI_SYSTEM_MENU_BOX_POS_Y, 0.0f);
	g_UI[UI_SYSTEM_MENU_BOX].texNo = UI_SYSTEM_MENU_BOX;

	g_UI[UI_BUTTON_STATUS].width = TEXTURE_SYSTEM_MENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_STATUS].height = TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_STATUS].pos = XMFLOAT3(UI_SYSTEM_MENU_BUTTON_POS_X, UI_SYSTEM_MENU_BUTTON_POS_Y, 0.0f);
	g_UI[UI_BUTTON_STATUS].texNo = UI_BUTTON_STATUS;

	g_UI[UI_BUTTON_MAGIC].width = TEXTURE_SYSTEM_MENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_MAGIC].height = TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_MAGIC].pos = XMFLOAT3(UI_SYSTEM_MENU_BUTTON_POS_X, UI_SYSTEM_MENU_BUTTON_POS_Y + UI_SYSTEM_MENU_BUTTON_OFFSET_Y, 0.0f);
	g_UI[UI_BUTTON_MAGIC].texNo = UI_BUTTON_MAGIC;

	g_UI[UI_BUTTON_TELEPORT].width = TEXTURE_SYSTEM_MENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_TELEPORT].height = TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_TELEPORT].pos = XMFLOAT3(UI_SYSTEM_MENU_BUTTON_POS_X, UI_SYSTEM_MENU_BUTTON_POS_Y + UI_SYSTEM_MENU_BUTTON_OFFSET_Y * 2, 0.0f);
	g_UI[UI_BUTTON_TELEPORT].texNo = UI_BUTTON_TELEPORT;

	g_UI[UI_BUTTON_SAVE].width = TEXTURE_SYSTEM_MENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_SAVE].height = TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_SAVE].pos = XMFLOAT3(UI_SYSTEM_MENU_BUTTON_POS_X, UI_SYSTEM_MENU_BUTTON_POS_Y + UI_SYSTEM_MENU_BUTTON_OFFSET_Y * 3, 0.0f);
	g_UI[UI_BUTTON_SAVE].texNo = UI_BUTTON_SAVE;

	g_UI[UI_BUTTON_LOAD].width = TEXTURE_SYSTEM_MENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_LOAD].height = TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_LOAD].pos = XMFLOAT3(UI_SYSTEM_MENU_BUTTON_POS_X, UI_SYSTEM_MENU_BUTTON_POS_Y + UI_SYSTEM_MENU_BUTTON_OFFSET_Y * 4, 0.0f);
	g_UI[UI_BUTTON_LOAD].texNo = UI_BUTTON_LOAD;

	g_UI[UI_BUTTON_RESUME_GAME].width = TEXTURE_SYSTEM_MENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_RESUME_GAME].height = TEXTURE_SYSTEM_MENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_RESUME_GAME].pos = XMFLOAT3(UI_SYSTEM_MENU_BUTTON_POS_X, UI_SYSTEM_MENU_BUTTON_POS_Y + UI_SYSTEM_MENU_BUTTON_OFFSET_Y * 5, 0.0f);
	g_UI[UI_BUTTON_RESUME_GAME].texNo = UI_BUTTON_RESUME_GAME;
	
	g_UI[UI_PLAYER_STATUS].width = TEXTURE_PLAYER_STATUS_WIDTH;
	g_UI[UI_PLAYER_STATUS].height = TEXTURE_PLAYER_STATUS_HEIGHT;
	g_UI[UI_PLAYER_STATUS].pos = XMFLOAT3(UI_PLAYER_STATUS_POS_X, UI_PLAYER_STATUS_POS_Y, 0.0f);
	g_UI[UI_PLAYER_STATUS].texNo = UI_PLAYER_STATUS;
	g_UI[UI_PLAYER_STATUS].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);


	g_UI[UI_RIGHT_ARROW].width = TEXTURE_ARROW_WIDTH;
	g_UI[UI_RIGHT_ARROW].height = TEXTURE_ARROW_HEIGHT;
	g_UI[UI_RIGHT_ARROW].pos = XMFLOAT3(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y, 0.0f);
	g_UI[UI_RIGHT_ARROW].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[UI_NUM].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[UI_MAGIC_LIST].width = TEXTURE_MAGIC_LIST_WIDTH;
	g_UI[UI_MAGIC_LIST].height = TEXTURE_MAGIC_LIST_HEIGHT;
	g_UI[UI_MAGIC_LIST].pos = XMFLOAT3(UI_MAGIC_LIST_POS_X, UI_MAGIC_LIST_POS_Y, 0.0f);
	g_UI[UI_MAGIC_LIST].texNo = UI_MAGIC_LIST;
	g_UI[UI_MAGIC_LIST].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[UI_BUTTON_CONFIRM].width = TEXTURE_SUBMENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_CONFIRM].height = TEXTURE_SUBMENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_CONFIRM].pos = XMFLOAT3(UI_CONFIRM_BUTTON_POS_X, UI_CONFIRM_BUTTON_POS_Y, 0.0f);
	g_UI[UI_BUTTON_CONFIRM].texNo = UI_BUTTON_CONFIRM;
	g_UI[UI_BUTTON_CONFIRM].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[UI_BUTTON_CANCEL].width = TEXTURE_SUBMENU_BUTTON_WIDTH;
	g_UI[UI_BUTTON_CANCEL].height = TEXTURE_SUBMENU_BUTTON_HEIGHT;
	g_UI[UI_BUTTON_CANCEL].pos = XMFLOAT3(UI_CANCEL_BUTTON_POS_X, UI_CANCEL_BUTTON_POS_Y, 0.0f);
	g_UI[UI_BUTTON_CANCEL].texNo = UI_BUTTON_CANCEL;
	g_UI[UI_BUTTON_CANCEL].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[UI_MAGIC_SLOT].width = TEXTURE_MAGIC_SLOT_WIDTH;
	g_UI[UI_MAGIC_SLOT].height = TEXTURE_MAGIC_SLOT_HEIGHT;
	g_UI[UI_MAGIC_SLOT].pos = XMFLOAT3(UI_MAGIC_SLOT_POS_X, UI_MAGIC_SLOT_POS_Y, 0.0f);
	g_UI[UI_MAGIC_SLOT].texNo = UI_MAGIC_SLOT;
	g_UI[UI_MAGIC_SLOT].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[UI_HIGHLIGHT_BOX].width = TEXTURE_MAGIC_LIST_ICON_BOX_WIDTH;
	g_UI[UI_HIGHLIGHT_BOX].height = TEXTURE_MAGIC_LIST_ICON_BOX_WIDTH;
	g_UI[UI_HIGHLIGHT_BOX].pos = XMFLOAT3(UI_MAGIC_LIST_ICON_INIT_POS_X, UI_MAGIC_LIST_ICON_INIT_POS_Y, 0.0f);
	g_UI[UI_HIGHLIGHT_BOX].texNo = UI_HIGHLIGHT_BOX;
	g_UI[UI_HIGHLIGHT_BOX].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	g_UI[BG_FADE].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	g_UI[BG_FADE].pos = XMFLOAT3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	g_UI[BG_FADE].width = SCREEN_WIDTH;
	g_UI[BG_FADE].height = SCREEN_HEIGHT;
	g_UI[BG_FADE].texNo = BG_FADE;

	if (GetMode() == MODE_MESSAGEBOX)
	{
		g_Cursor = UI_BUTTON_YES;
		g_cursorCurretTime = 0;
		g_cursorActionTime = 0;
	}
	else if (GetMode() == MODE_MENU)
	{
		g_Cursor = 0;
		g_cursorCurretTime = 0;
		g_cursorActionTime = 0;
	}

	ClearLevelUpStatus();

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
	// タイトル画面の処理
	case MODE_TITLE:
	{
		g_Cursor = GetTitleCursor();
		break;
	}
	// メッセージボックスの処理
	case MODE_MESSAGEBOX:
	{
		// 左右キーの入力がトリガーされた場合、かつ前回のアクションから一定時間が経過している場合
		int deltaTime = g_cursorCurretTime - g_cursorActionTime;
		if ((GetKeyboardTrigger(DIK_LEFT) || GetKeyboardTrigger(DIK_RIGHT)) && deltaTime >= CURSOR_DELTA_TIME)
		{
			// アクション時間を更新し、カーソル位置を切り替える
			g_cursorActionTime = g_cursorCurretTime;
			g_Cursor = g_Cursor == UI_BUTTON_NO ? UI_BUTTON_YES : UI_BUTTON_NO;
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}

		// ENTERキーがリリースされ、かつ前回のアクションから一定時間が経過している場合
		if (GetKeyboardRelease(DIK_RETURN) && deltaTime >= CURSOR_DELTA_TIME)
		{
			g_cursorActionTime = g_cursorCurretTime;
			if (g_Cursor == UI_BUTTON_YES)
			{
				// チュートリアルのマップに移動し、プレイヤーの初期位置を設定
				SetCurrentMap(TUTORIAL_01);
				InitTeleportInitPos(TUTORIAL_01);
				SetPlayerInitPosByMap(TUTORIAL_01, INITPOS_01);
				SetFade(FADE_OUT, MODE_TUTORIAL);
				PlaySound(SOUND_LABEL_SE_ENTER);
			}
			else
			{
				// 通常のゲームマップに移動し、プレイヤーの初期位置を設定
				SetCurrentMap(MAP_01);
				InitTeleportInitPos(MAP_01);
				SetPlayerInitPosByMap(MAP_01, INITPOS_01);
				SetFade(FADE_OUT, MODE_GAME);
				PlaySound(SOUND_LABEL_SE_ENTER);

			}
		}

		g_cursorCurretTime++;
		break;
	}
	case MODE_TUTORIAL:
	case MODE_GAME:
	{
		// プレイヤーがリスポーンメッセージ表示中の場合
		if (g_PlayerRespawnMsg == TRUE)
		{
			if (GetKeyboardRelease(DIK_LEFT) || GetKeyboardRelease(DIK_RIGHT) )
			{
				g_Cursor = g_Cursor == UI_BUTTON_NO ? UI_BUTTON_YES : UI_BUTTON_NO;
				PlaySound(SOUND_LABEL_SE_CURSOR);
			}


			if (GetKeyboardRelease(DIK_RETURN))
			{
				PlaySound(SOUND_LABEL_SE_ENTER);
				// ゲームモード
				if (GetMode() == MODE_GAME)
				{
					// スコアを減らしてリスポーン
					if (g_Cursor == UI_BUTTON_YES)
					{
						int score = GetScore();
						score *= 0.7f;
						SetScore(score);
						SetRespawn(TRUE);
						SetFade(FADE_OUT, MODE_GAME);
					}
					else
						SetFade(FADE_OUT, MODE_RESULT);  // リザルト画面に移行
				}
				// チュートリアルモード
				else if (GetMode() == MODE_TUTORIAL)
				{
					// プレイヤーを直接リスポーン
					if (g_Cursor == UI_BUTTON_YES)
					{
						g_PlayerRespawnMsg = FALSE;
						PlayerRespawnDirectly();
					}
					else
						SetFade(FADE_OUT, MODE_TITLE);	// リザルト画面に移行
				}
			}
		}

		// ポーズ状態を切り替える
		if (GetKeyboardRelease(DIK_P) && g_PlayerRespawnMsg == FALSE)
		{
			BOOL pause = GetPause() == TRUE ? FALSE : TRUE;
			SetPause(pause);
			g_Cursor = UI_BUTTON_NO;
			PlaySound(SOUND_LABEL_SE_ENTER);
		}
		// ポーズ状態を更新
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
			{
				g_Cursor = g_Cursor == UI_BUTTON_NO ? UI_BUTTON_YES : UI_BUTTON_NO;
				PlaySound(SOUND_LABEL_SE_CURSOR);
			}


			if (GetKeyboardRelease(DIK_RETURN))
			{
				PlaySound(SOUND_LABEL_SE_ENTER);
				if (g_Cursor == UI_BUTTON_YES)
				{
					SetFade(FADE_OUT, MODE_TITLE);
				}
					
				SetPause(FALSE);
			}
		}

		break;
	}
	case MODE_MENU:
	{
		switch (g_MenuUIModule)
		{
		case UI_MODULE_STATUS:
			UpdateStatusUI();
			break;
		case UI_MODULE_MAGIC_LIST:
			UpdateMagicList();
			break;
		case UI_MODULE_SYSTEM_MENU:
			UpdateSystemMenu();
			break;
		default:
			break;
		}
		break;
	}

	default:
		break;
	}

	UpdateUIFade();

}

void UpdateStatusUI(void)
{
	g_ArrowMoveCnt++;
	g_ArrowMoveCnt = fmod(g_ArrowMoveCnt, UI_ARROW_MOVE_CNT_MAX);

	float angle = (XM_PI * 2.0f * g_ArrowMoveCnt) / UI_ARROW_MOVE_CNT_MAX;

	g_ArrowMove = UI_ARROW_SELECTED_OFFSET * (sin(angle) * 0.65f + 0.35f);

	int deltaTime = g_cursorCurretTime - g_cursorActionTime;
	PlayerData* playerData = GetPlayerData();


	if (GetKeyboardRelease(DIK_UP) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == 0)
			g_Cursor = CURSOR_CONFIRM;
		else if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
			g_Cursor = SP_CNT_MAX - 1;
		else
			g_Cursor = (g_Cursor + SP_CNT_MAX) % (SP_CNT_MAX + 1);
	}
	else if (GetKeyboardRelease(DIK_DOWN) && deltaTime >= CURSOR_DELTA_TIME)
	{

		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == SP_CNT_MAX - 1)
			g_Cursor = CURSOR_CONFIRM;
		else if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
			g_Cursor = 0;
		else
			g_Cursor = (g_Cursor + 1) % (SP_CNT_MAX + 1);
	}
	else if (GetKeyboardRelease(DIK_LEFT) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
		{
			g_Cursor = g_Cursor == CURSOR_CONFIRM ? CURSOR_CANCEL : CURSOR_CONFIRM;
		}
		else
		{
			UpdateSPStatus(g_Cursor, CURSOR_LEFT);
		}
	}
	else if (GetKeyboardRelease(DIK_RIGHT) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
		{
			g_Cursor = g_Cursor == CURSOR_CONFIRM ? CURSOR_CANCEL : CURSOR_CONFIRM;
		}
		else
		{
			UpdateSPStatus(g_Cursor, CURSOR_RIGHT);
		}
	}
	else if (GetKeyboardRelease(DIK_X) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		HandleStatusUIButton();
	}
	else if (GetKeyboardRelease(DIK_RETURN) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CANCEL)
		{
			HandleStatusUIButton();
		}
		else
		{
			for (int i = 0; i < SP_CNT_MAX + STATUS_CNT_MAX; i++)
			{
				if (g_LevelUpStatus[i].oldValue != g_LevelUpStatus[i].newValue)
				{
					UpdatePlayerStatus(i);
				}
			}
			ClearLevelUpStatus();
		}


	}

	g_cursorCurretTime++;
}

void HandleStatusUIButton(void)
{
	BOOL resume = FALSE;
	for (int i = 0; i < SP_CNT_MAX; i++)
	{
		if (g_LevelUpStatus[i].oldValue != g_LevelUpStatus[i].newValue)
		{
			resume = TRUE;
			break;
		}
	}

	ClearLevelUpStatus();
	if (resume == FALSE)
	{
		SetUIFade(UI_MODULE_STATUS, FADE_OUT);
		g_MenuUIModuleNext = UI_MODULE_SYSTEM_MENU;
		g_Cursor = GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_STATUS);
	}
}

void UpdateSPStatus(int cursor, int dir)
{
	int sign = dir == CURSOR_RIGHT ? 1 : -1;
	PlayerData* playerData = GetPlayerData();
	PLAYER* player = GetPlayer();

	if (g_LevelUpStatus[STATUS_SP_LEFT].oldValue == -1)
	{
		g_LevelUpStatus[STATUS_SP_LEFT].oldValue = playerData->skillPointLeft;
		g_LevelUpStatus[STATUS_SP_LEFT].newValue = playerData->skillPointLeft;
	}
	if (g_LevelUpStatus[STATUS_LEVEL].oldValue == -1)
	{
		g_LevelUpStatus[STATUS_LEVEL].oldValue = playerData->level;
		g_LevelUpStatus[STATUS_LEVEL].newValue = playerData->level;
	}
	if (g_LevelUpStatus[STATUS_CURRENT_EXP].oldValue == -1)
	{
		g_LevelUpStatus[STATUS_CURRENT_EXP].oldValue = GetScore();
		g_LevelUpStatus[STATUS_CURRENT_EXP].newValue = GetScore();
	}
	if (g_LevelUpStatus[STATUS_REQUIRED_EXP].oldValue == -1)
	{
		g_LevelUpStatus[STATUS_REQUIRED_EXP].oldValue = CalculateExpForLevel(playerData->level);
		g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue = CalculateExpForLevel(playerData->level);
	}

	if (dir == CURSOR_RIGHT)
	{
		if (g_LevelUpStatus[STATUS_SP_LEFT].newValue > 0)
			g_LevelUpStatus[STATUS_SP_LEFT].newValue--;
		else if (g_LevelUpStatus[STATUS_CURRENT_EXP].newValue < g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue) return;
		else
		{
			g_LevelUpStatus[STATUS_LEVEL].newValue++;
			g_LevelUpStatus[STATUS_CURRENT_EXP].newValue = g_LevelUpStatus[STATUS_CURRENT_EXP].newValue - g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue;
			g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue = CalculateExpForLevel(g_LevelUpStatus[STATUS_LEVEL].newValue);
		}
	}


	switch (cursor)
	{
	case CURSOR_SP_HP:
		if (g_LevelUpStatus[CURSOR_SP_HP].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_HP].oldValue = playerData->spHP;
			g_LevelUpStatus[CURSOR_SP_HP].newValue = playerData->spHP;
			g_LevelUpStatus[STATUS_MAX_HP].oldValue = playerData->maxHP;
			g_LevelUpStatus[STATUS_MAX_HP].newValue = playerData->maxHP;
		}

		if (g_LevelUpStatus[CURSOR_SP_HP].newValue == PLAYER_INIT_SP_HP && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_HP].newValue += sign;
		g_LevelUpStatus[STATUS_MAX_HP].newValue += sign * SP_HP_PER_LV;

		break;
	case CURSOR_SP_MP:
		if (g_LevelUpStatus[CURSOR_SP_MP].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_MP].oldValue = playerData->spMP;
			g_LevelUpStatus[CURSOR_SP_MP].newValue = playerData->spMP;
			g_LevelUpStatus[STATUS_MAX_MP].oldValue = playerData->maxMP;
			g_LevelUpStatus[STATUS_MAX_MP].newValue = playerData->maxMP;
		}

		if (g_LevelUpStatus[CURSOR_SP_MP].newValue == PLAYER_INIT_SP_MP && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_MP].newValue += sign;
		g_LevelUpStatus[STATUS_MAX_MP].newValue += sign * SP_MP_PER_LV;

		break;
	case CURSOR_SP_ST:
		if (g_LevelUpStatus[CURSOR_SP_ST].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_ST].oldValue = playerData->spST;
			g_LevelUpStatus[CURSOR_SP_ST].newValue = playerData->spST;
			g_LevelUpStatus[STATUS_MAX_ST].oldValue = playerData->maxST;
			g_LevelUpStatus[STATUS_MAX_ST].newValue = playerData->maxST;
		}

		if (g_LevelUpStatus[CURSOR_SP_ST].newValue == PLAYER_INIT_SP_ST && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_ST].newValue += sign;
		g_LevelUpStatus[STATUS_MAX_ST].newValue += sign * SP_ST_PER_LV;

		break;
	case CURSOR_SP_ATK:
		if (g_LevelUpStatus[CURSOR_SP_ATK].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_ATK].oldValue = playerData->spATK;
			g_LevelUpStatus[CURSOR_SP_ATK].newValue = playerData->spATK;
			g_LevelUpStatus[STATUS_ATK].oldValue = playerData->ATK;
			g_LevelUpStatus[STATUS_ATK].newValue = playerData->ATK;
		}

		if (g_LevelUpStatus[CURSOR_SP_ATK].newValue == PLAYER_INIT_SP_ATK && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_ATK].newValue += sign;
		g_LevelUpStatus[STATUS_ATK].newValue += sign * SP_ATK_PER_LV;

		break;
	case CURSOR_SP_MAT:
		if (g_LevelUpStatus[CURSOR_SP_MAT].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_MAT].oldValue = playerData->spMAT;
			g_LevelUpStatus[CURSOR_SP_MAT].newValue = playerData->spMAT;
			g_LevelUpStatus[STATUS_MAT].oldValue = playerData->MAT;
			g_LevelUpStatus[STATUS_MAT].newValue = playerData->MAT;
		}

		if (g_LevelUpStatus[CURSOR_SP_MAT].newValue == PLAYER_INIT_SP_MAT && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_MAT].newValue += sign;
		g_LevelUpStatus[STATUS_MAT].newValue += sign * SP_MAT_PER_LV;

		break;
	case CURSOR_SP_DEF:
		if (g_LevelUpStatus[CURSOR_SP_DEF].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_DEF].oldValue = playerData->spDEF;
			g_LevelUpStatus[CURSOR_SP_DEF].newValue = playerData->spDEF;
			g_LevelUpStatus[STATUS_DEF].oldValue = playerData->DEF;
			g_LevelUpStatus[STATUS_DEF].newValue = playerData->DEF;
		}

		if (g_LevelUpStatus[CURSOR_SP_DEF].newValue == PLAYER_INIT_SP_DEF && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_DEF].newValue += sign;
		g_LevelUpStatus[STATUS_DEF].newValue += sign * SP_DEF_PER_LV;

		break;
	case CURSOR_SP_MDF:
		if (g_LevelUpStatus[CURSOR_SP_MDF].oldValue == -1)
		{
			g_LevelUpStatus[CURSOR_SP_MDF].oldValue = playerData->spMDF;
			g_LevelUpStatus[CURSOR_SP_MDF].newValue = playerData->spMDF;
			g_LevelUpStatus[STATUS_MDF].oldValue = playerData->MDF;
			g_LevelUpStatus[STATUS_MDF].newValue = playerData->MDF;
		}

		if (g_LevelUpStatus[CURSOR_SP_MDF].newValue == PLAYER_INIT_SP_MDF && sign == -1)
			return;
		g_LevelUpStatus[CURSOR_SP_MDF].newValue += sign;
		g_LevelUpStatus[STATUS_MDF].newValue += sign * SP_MDF_PER_LV;

		break;
	default:
		break;
	}

	if (dir == CURSOR_LEFT)
	{
		if (g_LevelUpStatus[STATUS_REQUIRED_EXP].oldValue != g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue)
		{
			g_LevelUpStatus[STATUS_LEVEL].newValue--;
			g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue = CalculateExpForLevel(g_LevelUpStatus[STATUS_LEVEL].newValue);
			g_LevelUpStatus[STATUS_CURRENT_EXP].newValue += g_LevelUpStatus[STATUS_REQUIRED_EXP].newValue;
		}
		else
			g_LevelUpStatus[STATUS_SP_LEFT].newValue++;
	}
}

void UpdatePlayerStatus(int category)
{
	PlayerData* playerData = GetPlayerData();
	switch (category)
	{
	case STATUS_MAX_HP:
		playerData->maxHP = g_LevelUpStatus[STATUS_MAX_HP].newValue;
		break;
	case STATUS_MAX_MP:
		playerData->maxMP = g_LevelUpStatus[STATUS_MAX_MP].newValue;
		break;
	case STATUS_MAX_ST:
		playerData->maxST = g_LevelUpStatus[STATUS_MAX_ST].newValue;
		break;
	case STATUS_ATK:
		playerData->ATK = g_LevelUpStatus[STATUS_ATK].newValue;
		break;
	case STATUS_MAT:
		playerData->MAT = g_LevelUpStatus[STATUS_MAT].newValue;
		break;
	case STATUS_DEF:
		playerData->DEF = g_LevelUpStatus[STATUS_DEF].newValue;
		break;
	case STATUS_MDF:
		playerData->MDF = g_LevelUpStatus[STATUS_MDF].newValue;
		break;
	case STATUS_LEVEL:
		playerData->level = g_LevelUpStatus[STATUS_LEVEL].newValue;
		break;
	case STATUS_SP_LEFT:
		playerData->skillPointLeft = g_LevelUpStatus[STATUS_SP_LEFT].newValue;
		break;
	case CURSOR_SP_HP:
		playerData->spHP = g_LevelUpStatus[CURSOR_SP_HP].newValue;
		break;
	case CURSOR_SP_MP:
		playerData->spMP = g_LevelUpStatus[CURSOR_SP_MP].newValue;
		break;
	case CURSOR_SP_ST:
		playerData->spST = g_LevelUpStatus[CURSOR_SP_ST].newValue;
		break;
	case CURSOR_SP_ATK:
		playerData->spATK = g_LevelUpStatus[CURSOR_SP_ATK].newValue;
		break;
	case CURSOR_SP_MAT:
		playerData->spMAT = g_LevelUpStatus[CURSOR_SP_MAT].newValue;
		break;
	case CURSOR_SP_DEF:
		playerData->spDEF = g_LevelUpStatus[CURSOR_SP_DEF].newValue;
		break;
	case CURSOR_SP_MDF:
		playerData->spMDF = g_LevelUpStatus[CURSOR_SP_MDF].newValue;
		break;
	case STATUS_CURRENT_EXP:
		SetScore(g_LevelUpStatus[STATUS_CURRENT_EXP].newValue);
		break;
	default:
		break;
	}
}

void UpdateMagicList(void)
{
	g_NewMagicScaleCnt++;
	g_NewMagicScaleCnt = fmod(g_NewMagicScaleCnt, UI_MAGIC_ICON_SCALE_CNT_MAX);

	float angle = (XM_PI * 2.0f * g_NewMagicScaleCnt) / UI_MAGIC_ICON_SCALE_CNT_MAX;

	g_NewMagicScale = (sin(angle) + 1.0f) * (1.1f - 0.8f) / 2.0f + 0.8f;

	int deltaTime = g_cursorCurretTime - g_cursorActionTime;

	if (GetKeyboardRelease(DIK_LEFT) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
		{
			g_Cursor = g_Cursor == CURSOR_CONFIRM ? CURSOR_CANCEL : CURSOR_CONFIRM;
		}
		else if (g_Cursor == 0)
		{
			g_Cursor = g_MagicLearnedCnt - 1;
		}
		else
			g_Cursor--;
	}
	else if (GetKeyboardRelease(DIK_RIGHT) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
		{
			g_Cursor = g_Cursor == CURSOR_CONFIRM ? CURSOR_CANCEL : CURSOR_CONFIRM;
		}
		else if (g_Cursor == g_MagicLearnedCnt - 1)
		{
			g_Cursor = 0;
		}
		else
			g_Cursor++;
	}
	else if (GetKeyboardRelease(DIK_UP) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
		{
			g_Cursor = g_MagicLearnedCnt - 1;
		}
		else if (g_Cursor < UI_MAGIC_LIST_ICON_NUM_PER_ROW)
		{
			g_Cursor = CURSOR_CONFIRM;
		}
		else
			g_Cursor -= UI_MAGIC_LIST_ICON_NUM_PER_ROW;
	}
	else if (GetKeyboardRelease(DIK_DOWN) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		if (g_Cursor == CURSOR_CONFIRM || g_Cursor == CURSOR_CANCEL)
		{
			g_Cursor = 0;
		}
		else if (g_Cursor >= g_MagicLearnedCnt - UI_MAGIC_LIST_ICON_NUM_PER_ROW)
		{
			g_Cursor = CURSOR_CONFIRM;
		}
		else
			g_Cursor += UI_MAGIC_LIST_ICON_NUM_PER_ROW;
	}
	else if (GetKeyboardRelease(DIK_RETURN) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;

		PlayerData* playerData = GetPlayerData();
		PLAYER* player = GetPlayer();
		if (g_Cursor == CURSOR_CONFIRM)
		{
			BOOL change = FALSE;
			for (int i = 0; i < MAGIC_SLOT_MAX; i++)
			{
				if (g_MagicListNew[i] != playerData->magicList[i])
				{
					playerData->magicList[i] = g_MagicListNew[i];
					player->magicList[i] = g_MagicListNew[i];
					change = TRUE;
				}
			}
			if (change == FALSE)
			{
				SetUIFade(UI_MODULE_MAGIC_LIST, FADE_OUT);
				g_MenuUIModuleNext = UI_MODULE_SYSTEM_MENU;
			}
			else
			{
				player->currentMagicIdx = 0;
			}
		}
		else if (g_Cursor == CURSOR_CANCEL)
		{
			BOOL change = FALSE;
			for (int i = 0; i < MAGIC_SLOT_MAX; i++)
			{
				if (g_MagicListNew[i] != playerData->magicList[i])
				{
					g_MagicListNew[i] = playerData->magicList[i];
					change = TRUE;
				}
			}
			if (change == FALSE)
			{
				SetUIFade(UI_MODULE_MAGIC_LIST, FADE_OUT);
				g_MenuUIModuleNext = UI_MODULE_SYSTEM_MENU;
			}
		}
		else
		{
			BOOL remove = FALSE;
			for (int i = 0; i < MAGIC_SLOT_MAX; i++)
			{
				if (g_MagicListNew[i] == playerData->magicLearned[g_Cursor])
				{
					RemoveMagicListElementAt(i);
					remove = TRUE;
					break;
				}
			}
			BOOL add = FALSE;
			if (remove == FALSE)
			{
				add = InsertMagicListElement(playerData->magicLearned[g_Cursor]);
			}
		}
	}
	else if (GetKeyboardRelease(DIK_X) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;

		PlayerData* playerData = GetPlayerData();
		BOOL change = FALSE;
		for (int i = 0; i < MAGIC_SLOT_MAX; i++)
		{
			if (g_MagicListNew[i] != playerData->magicList[i])
			{
				g_MagicListNew[i] = playerData->magicList[i];
				change = TRUE;
			}
		}

		if (change == FALSE)
		{
			SetUIFade(UI_MODULE_MAGIC_LIST, FADE_OUT);
			g_MenuUIModuleNext = UI_MODULE_SYSTEM_MENU;
		}
	}

	int x_idx = g_Cursor % UI_MAGIC_LIST_ICON_NUM_PER_ROW;
	int y_idx = g_Cursor / UI_MAGIC_LIST_ICON_NUM_PER_ROW;
	g_UI[UI_HIGHLIGHT_BOX].pos.x = UI_MAGIC_LIST_ICON_INIT_POS_X + x_idx * UI_MAGIC_LIST_ICON_OFFSET_X;
	g_UI[UI_HIGHLIGHT_BOX].pos.y = UI_MAGIC_LIST_ICON_INIT_POS_Y + y_idx * UI_MAGIC_LIST_ICON_OFFSET_Y;

	g_cursorCurretTime++;
}

void UpdateSystemMenu(void)
{
	int deltaTime = g_cursorCurretTime - g_cursorActionTime;
	if (GetKeyboardRelease(DIK_UP) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		g_Cursor = (g_Cursor + SYSTEM_MENU_BUTTON_NUM - 1) % SYSTEM_MENU_BUTTON_NUM;
	}
	else if (GetKeyboardRelease(DIK_DOWN) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		g_Cursor = (g_Cursor + 1) % SYSTEM_MENU_BUTTON_NUM;
	}
	else if ((GetKeyboardRelease(DIK_RETURN) || GetKeyboardRelease(DIK_C)) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		switch (g_Cursor)
		{
		case GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_STATUS):
			SetUIFade(UI_MODULE_SYSTEM_MENU, FADE_OUT);
			g_MenuUIModuleNext = UI_MODULE_STATUS;
			break;
		case GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_MAGIC):
			SetUIFade(UI_MODULE_SYSTEM_MENU, FADE_OUT);
			g_MenuUIModuleNext = UI_MODULE_MAGIC_LIST;
			g_MagicLearnedCnt = GetMagicLearnedCount();
			InitMagicList();
			break;
		case GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_RESUME_GAME):
			SetUIFade(UI_MODULE_SYSTEM_MENU, FADE_OUT);
			SetUIFade(BG_FADE, FADE_OUT);
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardRelease(DIK_X) && deltaTime >= CURSOR_DELTA_TIME)
	{
		g_cursorActionTime = g_cursorCurretTime;
		SetUIFade(UI_MODULE_SYSTEM_MENU, FADE_OUT);
		SetUIFade(BG_FADE, FADE_OUT);
	}

	g_cursorCurretTime++;
}

void UpdateUIFade(void)
{
	BOOL isFadingOut = FALSE;
	for (int i = 0; i < UI_MAX; i++)
	{
		if (g_UI[i].fade == FADE_IN)
		{
			// α値を加算して画面を浮き上がらせる
			if (g_UI[i].texNo == BG_FADE)
				g_UI[i].color.w += FADE_RATE * 0.5f;
			else
				g_UI[i].color.w += FADE_RATE;

			if (g_UI[i].color.w >= 1.0f)
			{
				// フェード処理終了
				g_UI[i].color.w = 1.0f;
				g_UI[i].fade = FADE_NONE;
			}
		}
		else if (g_UI[i].fade == FADE_OUT)
		{
			// α値を減算して画面を消していく
			if (g_UI[i].texNo == BG_FADE)
				g_UI[i].color.w -= FADE_RATE * 0.5f;
			else
				g_UI[i].color.w -= FADE_RATE;

			if (g_UI[i].color.w <= 0.0f)
			{
				// フェード処理終了
				g_UI[i].color.w = 0.0f;
				g_UI[i].fade = FADE_NONE;

				if (g_UI[i].texNo == BG_FADE && g_MenuUIModuleNext == g_MenuUIModule)
				{
					SetMode(MODE_GAME);
					g_RenderMenuUI = FALSE;
				}

			}
			isFadingOut = TRUE;
		}
	}

	if (isFadingOut == FALSE)
	{
		if (g_MenuUIModuleNext != g_MenuUIModule)
		{
			g_Cursor = 0;
			SetUIFade(g_MenuUIModuleNext, FADE_IN);
		}
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
	case MODE_MESSAGEBOX:
		DrawMessageBox();
		break;
	case MODE_TUTORIAL:
	case MODE_GAME:
	case MODE_MENU:
		DrawInGameUI();

		if (g_PlayerRespawnMsg == TRUE)
			DrawMessageBox();
		else if (GetPause())
			DrawPauseUI();

		if (g_RenderMenuUI == TRUE)
			DrawMenuUI();

		break;
	default:
		break;
	}
}

void DrawTitleUI(void)
{
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
	if (g_RenderGauge == TRUE)
		DrawPlayerGauge();
	if (g_RenderJumpIcon == TRUE)
		DrawPlayerJumpIcon();
	if (g_RenderSkillIcon == TRUE)
		DrawSkillIcon();
	if (g_RenderBladeIcon == TRUE)
		DrawFlamebladeIcon();
}

void DrawMenuUI(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[BG_FADE].texNo]);
	SetSpriteColor(g_VertexBuffer,
		g_UI[BG_FADE].pos.x, g_UI[BG_FADE].pos.y,
		g_UI[BG_FADE].width, g_UI[BG_FADE].height,
		0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[BG_FADE].color);
	GetDeviceContext()->Draw(4, 0);

	switch (g_MenuUIModule)
	{
	case UI_MODULE_STATUS:
		DrawStatusUI();
		break;
	case UI_MODULE_MAGIC_LIST:
		DrawMagicList();
		break;
	case UI_MODULE_SYSTEM_MENU:
		DrawSystemMenuUI();
		break;
	default:
		break;
	}


}

void DrawStatusUI(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_PLAYER_STATUS].texNo]);
	SetSpriteColor(g_VertexBuffer,
		g_UI[UI_PLAYER_STATUS].pos.x, g_UI[UI_PLAYER_STATUS].pos.y,
		g_UI[UI_PLAYER_STATUS].width, g_UI[UI_PLAYER_STATUS].height,
		0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[UI_PLAYER_STATUS].color);
	GetDeviceContext()->Draw(4, 0);

	PLAYER* player = GetPlayer();
	PlayerData* playerData = GetPlayerData();
	int score = GetScore();
	DrawNumber(NUMBER_LEVEL_POS_X, NUMBER_LEVEL_POS_Y, playerData->level, STATUS_LEVEL);
	DrawNumber(NUMBER_SKILL_POINT_POS_X, NUMBER_SKILL_POINT_POS_Y, playerData->skillPointLeft, STATUS_SP_LEFT);
	DrawNumber(NUMBER_CURRENT_EXP_POS_X, NUMBER_CURRENT_EXP_POS_Y, score, STATUS_CURRENT_EXP);
	int requiredExp = CalculateExpForLevel(playerData->level);
	DrawNumber(NUMBER_REQUIRED_EXP_POS_X, NUMBER_REQUIRED_EXP_POS_Y, requiredExp, STATUS_REQUIRED_EXP);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y, playerData->spHP, CURSOR_SP_HP, 2);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y + NUMBER_STATUS_OFFSET_Y, playerData->spMP, CURSOR_SP_MP, 2);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 2, playerData->spST, CURSOR_SP_ST, 2);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 3, playerData->spATK, CURSOR_SP_ATK, 2);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 4, playerData->spMAT, CURSOR_SP_MAT, 2);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 5, playerData->spDEF, CURSOR_SP_DEF, 2);
	DrawNumber(NUMBER_SP_HP_POS_X, NUMBER_SP_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 6, playerData->spMDF, CURSOR_SP_MDF, 2);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_RIGHT_ARROW]);
	for (int i = 0; i < SP_CNT_MAX; i++)
	{
		float posX = UI_RIGHT_ARROW_POS_X;
		if (i == g_Cursor)
			posX += g_ArrowMove;

		SetSpriteColorRotation(g_VertexBuffer,
			posX, g_UI[UI_RIGHT_ARROW].pos.y + i * NUMBER_STATUS_OFFSET_Y,
			g_UI[UI_RIGHT_ARROW].width, g_UI[UI_RIGHT_ARROW].height,
			0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[UI_RIGHT_ARROW].color, 0.0f);
		GetDeviceContext()->Draw(4, 0);

		posX = UI_LEFT_ARROW_POS_X;
		if (i == g_Cursor)
			posX -= g_ArrowMove;
		SetSpriteColorRotation(g_VertexBuffer,
			posX, g_UI[UI_RIGHT_ARROW].pos.y + i * NUMBER_STATUS_OFFSET_Y,
			g_UI[UI_RIGHT_ARROW].width, g_UI[UI_RIGHT_ARROW].height,
			0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[UI_RIGHT_ARROW].color, PI);
		GetDeviceContext()->Draw(4, 0);
	}

	

	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y, playerData->maxHP, STATUS_MAX_HP);
	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y + NUMBER_STATUS_OFFSET_Y, playerData->maxMP, STATUS_MAX_MP);
	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 2, playerData->maxST, STATUS_MAX_ST);
	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 3, playerData->ATK, STATUS_ATK);
	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 4, playerData->MAT, STATUS_MAT);
	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 5, playerData->DEF, STATUS_DEF);
	DrawNumber(NUMBER_MAX_HP_POS_X, NUMBER_MAX_HP_POS_Y + NUMBER_STATUS_OFFSET_Y * 6, playerData->MDF, STATUS_MDF);
	
	

	BOOL selected = g_Cursor == CURSOR_CONFIRM ? TRUE : FALSE;
	DrawButton(UI_BUTTON_CONFIRM, selected);
	selected = g_Cursor == CURSOR_CANCEL ? TRUE : FALSE;
	DrawButton(UI_BUTTON_CANCEL, selected);
}

void DrawMagicList(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_MAGIC_LIST].texNo]);
	SetSpriteColor(g_VertexBuffer,
		g_UI[UI_MAGIC_LIST].pos.x, g_UI[UI_MAGIC_LIST].pos.y,
		g_UI[UI_MAGIC_LIST].width, g_UI[UI_MAGIC_LIST].height,
		0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[UI_MAGIC_LIST].color);
	GetDeviceContext()->Draw(4, 0);

	for (int i = 0; i < UI_MAGIC_SLOT_NUM; i++)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_MAGIC_SLOT].texNo]);
		SetSpriteColor(g_VertexBuffer,
			g_UI[UI_MAGIC_SLOT].pos.x + i * UI_MAGIC_SLOT_OFFSET_X, g_UI[UI_MAGIC_SLOT].pos.y,
			g_UI[UI_MAGIC_SLOT].width, g_UI[UI_MAGIC_SLOT].height,
			0.0f, 0.0f, 1.0f, 1.0f,
			g_UI[UI_MAGIC_SLOT].color);
		GetDeviceContext()->Draw(4, 0);

		if (g_MagicListNew[i] != MAGIC_NONE)
		{
			XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
			int texNo = GetMagicTexNo(g_MagicListNew[i]);
			color.w = g_UI[texNo].color.w;
			float scale = 0.56f;
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[texNo]);
			SetSpriteColor(g_VertexBuffer,
				g_UI[UI_MAGIC_SLOT].pos.x + i * UI_MAGIC_SLOT_OFFSET_X, g_UI[UI_MAGIC_SLOT].pos.y,
				g_UI[UI_MAGIC_SLOT].width * scale, g_UI[UI_MAGIC_SLOT].height * scale,
				0.0f, 0.0f, 1.0f, 1.0f,
				color);
			GetDeviceContext()->Draw(4, 0);
		}
	}

	PLAYER* player = GetPlayer();
	PlayerData* playerData = GetPlayerData();
	int count = 0;
	for (int i = 0; i < MAGIC_NUM_MAX; i++)
	{
		if (playerData->magicLearned[i] == MAGIC_NONE) continue;

		int x_idx = count % UI_MAGIC_LIST_ICON_NUM_PER_ROW;
		int y_idx = count / UI_MAGIC_LIST_ICON_NUM_PER_ROW;
		int magicTexNo = GetMagicTexNo(playerData->magicLearned[i]);
		XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		float scale = 1.0f;
		BOOL equipped = FALSE;
		BOOL newAdd = TRUE;
		for (int j = 0; j < MAGIC_SLOT_MAX; j++)
		{
			if (g_MagicListNew[j] == playerData->magicLearned[i])
			{
				equipped = TRUE;
				for (int k = 0; k < MAGIC_SLOT_MAX; k++)
				{
					if (g_MagicListNew[j] == playerData->magicList[k])
					{
						newAdd = FALSE;
						break;
					}
				}
			}
		}
		if (equipped)
		{
			scale = 0.8f;
			if (newAdd)
				scale *= g_NewMagicScale;
			color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		}

		color.w = g_UI[magicTexNo].color.w;
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[magicTexNo]);
		SetSpriteColor(g_VertexBuffer,
			UI_MAGIC_LIST_ICON_INIT_POS_X + x_idx * UI_MAGIC_LIST_ICON_OFFSET_X, 
			UI_MAGIC_LIST_ICON_INIT_POS_Y + y_idx * UI_MAGIC_LIST_ICON_OFFSET_Y,
			TEXTURE_MAGIC_LIST_ICON_WIDTH * scale, TEXTURE_MAGIC_LIST_ICON_HEIGHT * scale,
			0.0f, 0.0f, 1.0f, 1.0f,
			color);
		GetDeviceContext()->Draw(4, 0);
		g_UI[magicTexNo].UIModule = UI_MODULE_MAGIC_LIST;
		count++;
	}

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_HIGHLIGHT_BOX].texNo]);
	SetSpriteColor(g_VertexBuffer,
		g_UI[UI_HIGHLIGHT_BOX].pos.x, g_UI[UI_HIGHLIGHT_BOX].pos.y,
		g_UI[UI_HIGHLIGHT_BOX].width, g_UI[UI_HIGHLIGHT_BOX].height,
		0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[UI_HIGHLIGHT_BOX].color);
	GetDeviceContext()->Draw(4, 0);

	BOOL selected = g_Cursor == CURSOR_CONFIRM ? TRUE : FALSE;
	DrawButton(UI_BUTTON_CONFIRM, selected);
	selected = g_Cursor == CURSOR_CANCEL ? TRUE : FALSE;
	DrawButton(UI_BUTTON_CANCEL, selected);
}

void DrawSystemMenuUI(void)
{
	// システムメニューボックス
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_SYSTEM_MENU_BOX].texNo]);
	SetSpriteColor(g_VertexBuffer,
		g_UI[UI_SYSTEM_MENU_BOX].pos.x, g_UI[UI_SYSTEM_MENU_BOX].pos.y,
		g_UI[UI_SYSTEM_MENU_BOX].width, g_UI[UI_SYSTEM_MENU_BOX].height,
		0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[UI_SYSTEM_MENU_BOX].color);
	GetDeviceContext()->Draw(4, 0);

	// ボタン
	BOOL selected = g_Cursor == 0;
	DrawButton(UI_BUTTON_STATUS, selected);

	selected = g_Cursor == GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_MAGIC);
	DrawButton(UI_BUTTON_MAGIC, selected);

	selected = g_Cursor == GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_TELEPORT);
	DrawButton(UI_BUTTON_TELEPORT, selected);

	selected = g_Cursor == GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_SAVE);
	DrawButton(UI_BUTTON_SAVE, selected);

	selected = g_Cursor == GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_LOAD);
	DrawButton(UI_BUTTON_LOAD, selected);

	selected = g_Cursor == GET_SYSTEM_MENU_CURSOR_IDX(UI_BUTTON_RESUME_GAME);
	DrawButton(UI_BUTTON_RESUME_GAME, selected);
}

void DrawPlayerGauge(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.9f);
	SetMaterial(material);

	PLAYER* player = GetPlayer();

	// HPゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_HP].texNo]);
	SetSpriteLeftTop(g_VertexBuffer, 
		g_UI[UI_HP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_HP].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_HP].width * UI_GAUGE_SCALE_X * g_UI[UI_HP].ratio, g_UI[UI_HP].height * UI_GAUGE_SCALE_Y, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	float greyHPRatio = player->greyHP / player->maxHP;
	SetSpriteLeftTop(g_VertexBuffer,
		g_UI[UI_HP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_HP].pos.y + UI_GAUGE_OFFSET_Y,
		g_UI[UI_HP].width * UI_GAUGE_SCALE_X * greyHPRatio, g_UI[UI_HP].height * UI_GAUGE_SCALE_Y,
		0.0f, 0.0f, 1.0f, 0.5f);
	GetDeviceContext()->Draw(4, 0);

	// MPゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_MP].texNo]);
	SetSpriteLeftTop(g_VertexBuffer, g_UI[UI_MP].pos.x + UI_GAUGE_OFFSET_X, g_UI[UI_MP].pos.y + UI_GAUGE_OFFSET_Y, 
		g_UI[UI_MP].width * UI_GAUGE_SCALE_X * g_UI[UI_MP].ratio, g_UI[UI_MP].height * UI_GAUGE_SCALE_Y, 
		0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);

	// STゲージ
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_ST].texNo]);
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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_JUMP_ICON].texNo]);
		SetSpriteLeftTop(g_VertexBuffer, 
			g_UI[UI_JUMP_ICON].pos.x + TEXTURE_JUMP_ICON_WIDTH, g_UI[UI_JUMP_ICON].pos.y,
			g_UI[UI_JUMP_ICON].width, g_UI[UI_JUMP_ICON].height, 
			0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}
	if (player->dashCount < 2)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[UI_JUMP_ICON].texNo]);
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
	int currentMagic = UI_EMPYT_MAGIC_BOX, leftMagic = UI_EMPYT_MAGIC_BOX, rightMagic = UI_EMPYT_MAGIC_BOX;
	currentMagic = GetMagicTexNo(player->magicList[player->currentMagicIdx]);

	int magicCount = GetMagicEquippedCount();
	int leftMagicIdx = -1, rightMagicIdx = -1;
	if (magicCount > 1)
	{
		leftMagicIdx = (player->currentMagicIdx + magicCount - 1) % magicCount;
		while (player->magicList[player->currentMagicIdx] == MAGIC_NONE)
			leftMagicIdx = (player->currentMagicIdx + magicCount - 1) % magicCount;
		rightMagicIdx = (player->currentMagicIdx + 1) % magicCount;
		while (player->magicList[player->currentMagicIdx] == MAGIC_NONE)
			rightMagicIdx = (player->currentMagicIdx + 1) % magicCount;
	}
	if (leftMagicIdx >= 0)
		leftMagic = GetMagicTexNo(player->magicList[leftMagicIdx]);
	else
		leftMagic = UI_EMPYT_MAGIC_BOX;
	if (rightMagicIdx >= 0)
		rightMagic = GetMagicTexNo(player->magicList[rightMagicIdx]);
	else
		rightMagic = UI_EMPYT_MAGIC_BOX;

	float healingCDProgress = 1.0f - player->healingCD / HEALING_CD_TIME;
	float fireBallCDProgress = 1.0f - player->fireBallCD / FIRE_BALL_CD_TIME;
	float leftMagicCD = GetMagicCooldown(leftMagic, healingCDProgress, fireBallCDProgress);
	float currentMagicCD = GetMagicCooldown(currentMagic, healingCDProgress, fireBallCDProgress);
	float rightMagicCD = GetMagicCooldown(rightMagic, healingCDProgress, fireBallCDProgress);

	XMFLOAT4 leftColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 middleColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 rightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	if (IsSkillIconActive(leftMagic) == FALSE)
		leftColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	if (IsSkillIconActive(currentMagic) == FALSE)
		middleColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	if (IsSkillIconActive(rightMagic) == FALSE)
		rightColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	// 中央のマジックアイコン
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[currentMagic]);
	SetSpriteTopToBottomRevealColor(g_VertexBuffer, 
		UI_MAGIC_ICON_MIDDLE_POS_X, UI_MAGIC_ICON_MIDDLE_POS_Y,
		TEXTURE_MAGIC_ICON_SIZE, TEXTURE_MAGIC_ICON_SIZE, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		middleColor, currentMagicCD);
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
		leftColor, leftMagicCD);
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
		rightColor, rightMagicCD);
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

void DrawMessageBox(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	if (GetMode() == MODE_MESSAGEBOX)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG_FADE]);
		SetSpriteColor(g_VertexBuffer,
			SCREEN_CENTER_X, SCREEN_CENTER_Y,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		//メッセージボックス
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_MESSAGEBOX_TUTORIAL]);
		SetSpriteColor(g_VertexBuffer,
			UI_MESSAGE_BOX_POS_X, UI_MESSAGE_BOX_POS_Y,
			TEXTURE_MESSAGE_BOX_WIDTH, TEXTURE_MESSAGE_BOX_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
	else
	{
		//メッセージボックス
		float px, py, tw, th;
		int texNo;
		px = UI_MESSAGE_BOX_POS_X;
		if (g_PlayerRespawnMsg == TRUE)
		{
			if (GetMode() == MODE_TUTORIAL)
				texNo = UI_MESSAGEBOX_TUTORIAL_RESPAWN;
			else if (GetMode() == MODE_GAME)
				texNo = UI_MESSAGEBOX_RESPAWN;
			py = UI_MESSAGE_BOX_POS_Y + UI_RESPAWN_MESSAGE_OFFSET_Y;
		}
		else
		{
			texNo = UI_MESSAGEBOX_TUTORIAL;
			py = UI_MESSAGE_BOX_POS_Y;
		}

		tw = TEXTURE_MESSAGE_BOX_WIDTH;
		th = TEXTURE_MESSAGE_BOX_HEIGHT;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[texNo]);
		SetSpriteColor(g_VertexBuffer,
			px, py,
			tw, th,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}

	// ボタン
	BOOL selected = g_Cursor == UI_BUTTON_YES;
	DrawButton(UI_BUTTON_YES, selected);

	selected = g_Cursor == UI_BUTTON_NO;
	DrawButton(UI_BUTTON_NO, selected);
}

void DrawPauseUI(void)
{
	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG_FADE]);
	SetSpriteColor(g_VertexBuffer,
		SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f));
	GetDeviceContext()->Draw(4, 0);

	// システムメニューボックス
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_MESSAGEBOX_RETURN_TITLE]);
	SetSpriteColor(g_VertexBuffer,
		UI_MESSAGE_BOX_POS_X, UI_MESSAGE_BOX_POS_Y,
		TEXTURE_MESSAGE_BOX_WIDTH, TEXTURE_MESSAGE_BOX_HEIGHT,
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[button].texNo]);
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
	float buttonPosX, buttonPosY;
	buttonPosX = g_UI[button].pos.x;
	if (g_PlayerRespawnMsg == TRUE)
		buttonPosY = g_UI[button].pos.y + UI_RESPAWN_MESSAGE_OFFSET_Y;
	else
		buttonPosY = g_UI[button].pos.y;

	SetSpriteColor(g_VertexBuffer,
		buttonPosX, buttonPosY,
		buttonWidth, buttonHeight,
		0.0f, 0.0f, 1.0f, 1.0f,
		g_UI[button].color);
	GetDeviceContext()->Draw(4, 0);
}

void DrawNumber(float posX, float posY, int num, int category, int numDigit)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_NUM]);

	num = CHECK_MODIFIED_VALUE(category, num);

	if (numDigit == 0)
		numDigit = COUNT_DIGITS(num);

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	if (num > g_LevelUpStatus[category].oldValue && g_LevelUpStatus[category].oldValue != -1)
		color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	else if (num < g_LevelUpStatus[category].oldValue && g_LevelUpStatus[category].oldValue != -1)
		color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	color.w = g_UI[UI_NUM].color.w;

	// 桁数分処理する
	for (int i = 0; i < numDigit; i++)
	{
		// 今回表示する桁の数字
		float x = (float)(num % 10);

		// スコアの位置やテクスチャー座標を反映
		float px = posX - TEXTURE_NUMBER_WIDTH * i;	// スコアの表示位置X
		float py = posY;							// スコアの表示位置Y
		float pw = TEXTURE_NUMBER_WIDTH;				// スコアの表示幅
		float ph = TEXTURE_NUMBER_HEIGHT;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = x * tw;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			color);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		num /= 10;
	}
}

void ClearLevelUpStatus(void)
{
	for (int i = 0; i < STATUS_CNT_MAX + SP_CNT_MAX; i++)
	{
		g_LevelUpStatus[i].newValue = -1;
		g_LevelUpStatus[i].oldValue = -1;
	}
}

float GetMagicCooldown(int magicType, float healingCDProgress, float fireBallCDProgress)
{
	if (magicType == UI_MAGIC_HEAL)
	{
		return healingCDProgress;
	}
	else if (magicType == UI_MAGIC_FLAMEBLADE)
	{
		return 1.0f;
	}
	else if (magicType == UI_MAGIC_FIRE_BALL)
	{
		return fireBallCDProgress;
	}
	return 0.0f;
}

void SetUIFade(int UIModule, int fade)
{
	switch (UIModule)
	{
	case UI_MODULE_SYSTEM_MENU:
		g_MenuUIModule = UIModule;
		g_UI[UI_SYSTEM_MENU_BOX].fade = fade;
		g_UI[UI_BUTTON_STATUS].fade = fade;
		g_UI[UI_BUTTON_MAGIC].fade = fade;
		g_UI[UI_BUTTON_TELEPORT].fade = fade;
		g_UI[UI_BUTTON_SAVE].fade = fade;
		g_UI[UI_BUTTON_LOAD].fade = fade;
		g_UI[UI_BUTTON_RESUME_GAME].fade = fade;
		break;
	case UI_MODULE_STATUS:
		g_MenuUIModule = UIModule;
		g_UI[UI_PLAYER_STATUS].fade = fade;
		g_UI[UI_BUTTON_CONFIRM].fade = fade;
		g_UI[UI_BUTTON_CANCEL].fade = fade;
		g_UI[UI_RIGHT_ARROW].fade = fade;
		g_UI[UI_NUM].fade = fade;
		break;
	case UI_MODULE_MAGIC_LIST:
		g_MenuUIModule = UIModule;
		g_UI[UI_MAGIC_LIST].fade = fade;
		g_UI[UI_BUTTON_CONFIRM].fade = fade;
		g_UI[UI_BUTTON_CANCEL].fade = fade;
		g_UI[UI_MAGIC_SLOT].fade = fade;
		g_UI[UI_HIGHLIGHT_BOX].fade = fade;
		for (int i = 0; i < UI_MAX; i++)
		{
			if (g_UI[i].UIModule == UI_MODULE_MAGIC_LIST)
				g_UI[i].fade = fade;
		}
		break;
	default:
		g_UI[UIModule].fade = fade;
		break;
	}
}

int GetMagicTexNo(int magic)
{
	switch (magic)
	{
	case MAGIC_FIRE_BALL:
		return UI_MAGIC_FIRE_BALL;
	case MAGIC_FLAMEBLADE:
		return UI_MAGIC_FLAMEBLADE;
	case MAGIC_HEALING:
		return UI_MAGIC_HEAL;
	case MAGIC_SHIELD:
		return UI_MAGIC_SHIELD;
	case MAGIC_HIDDEN:
		return UI_MAGIC_HIDDEN;
	default:
		return UI_EMPYT_MAGIC_BOX;
	}
}

void InitSystemMenu(void)
{
	g_Cursor = 0;
	g_MenuUIModule = UI_MODULE_SYSTEM_MENU;
	g_MenuUIModuleNext = UI_MODULE_SYSTEM_MENU;
	g_UI[UI_SYSTEM_MENU_BOX].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[UI_BUTTON_STATUS].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[UI_BUTTON_MAGIC].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[UI_BUTTON_TELEPORT].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[UI_BUTTON_SAVE].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[UI_BUTTON_LOAD].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[UI_BUTTON_RESUME_GAME].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	g_UI[BG_FADE].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
}

void InitMagicList(void)
{
	PlayerData* playerData = GetPlayerData();
	for (int i = 0; i < MAGIC_SLOT_MAX; i++)
	{
		g_MagicListNew[i] = playerData->magicList[i];
	}
}

BOOL IsSkillIconActive(int magic)
{
	if (magic == UI_EMPYT_MAGIC_BOX) return TRUE;

	if (magic == UI_MAGIC_HEAL && GetMagicActive(MAGIC_HEALING) == FALSE
		|| magic == UI_MAGIC_FIRE_BALL && GetMagicActive(MAGIC_FIRE_BALL) == FALSE
		|| magic == UI_MAGIC_FLAMEBLADE && GetMagicActive(MAGIC_FLAMEBLADE) == FALSE)
		return FALSE;
	else
		return TRUE;
}


void RemoveMagicListElementAt(int idx)
{
	int magicCount = 0;
	for (int i = 0; i < MAGIC_SLOT_MAX; i++)
	{
		if (g_MagicListNew[i] != MAGIC_NONE)
			magicCount++;
	}
	if (idx < 0 || idx >= magicCount) return;

	// 指定された位置から、後続の有効な要素を前に移動
	for (int i = idx; i < magicCount - 1; i++)
	{
		g_MagicListNew[i] = g_MagicListNew[i + 1];
	}

	// 最後の有効な要素を無効にする
	g_MagicListNew[magicCount - 1] = MAGIC_NONE;
	magicCount--;
}
BOOL InsertMagicListElement(int magic)
{
	for (int i = 0; i < MAGIC_SLOT_MAX; i++)
	{
		if (g_MagicListNew[i] == MAGIC_NONE)
		{
			g_MagicListNew[i] = magic;
			return TRUE;
		}
	}

	return FALSE;
}

void SetRespawnMessageBox(BOOL render)
{
	g_PlayerRespawnMsg = render;
	if (render == TRUE)
		g_Cursor = UI_BUTTON_YES;
}

void SetRenderGauge(BOOL render)
{
	g_RenderGauge = render;
}
void SetRenderSkillIcon(BOOL render)
{
	g_RenderSkillIcon = render;
}
void SetRenderJumpIcon(BOOL render)
{
	g_RenderJumpIcon = render;
}

void SetRenderBladeIcon(BOOL render)
{
	g_RenderBladeIcon = render;
}

void SetRenderMenuUI(BOOL render)
{
	g_RenderMenuUI = render;
}

void ResetCursor(void)
{
	g_Cursor = 0;
}