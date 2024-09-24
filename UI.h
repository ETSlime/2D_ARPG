//=============================================================================
//
// UI処理 [UI.h]
// Author : 
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define COUNT_DIGITS(number) ((number) == 0 ? 1 : (int)(log10(abs(number)) + 1))
#define CHECK_MODIFIED_VALUE(category, value) (g_LevelUpStatus[category].newValue == -1 ? value : g_LevelUpStatus[category].newValue)
#define	GET_SYSTEM_MENU_CURSOR_IDX(cursor) (cursor - UI_BUTTON_STATUS)

#define SP_CNT_MAX									(7)
#define STATUS_CNT_MAX								(11)
#define SP_HP_PER_LV								(120.0f)
#define SP_MP_PER_LV								(150.0f)
#define SP_ST_PER_LV								(90.0f)
#define SP_ATK_PER_LV								(4.0f)
#define SP_MAT_PER_LV								(2.0f)
#define SP_DEF_PER_LV								(3.0f)
#define SP_MDF_PER_LV								(3.0f)

struct UISprite
{
	BOOL use;
	int texNo;
	float width, height;
	float ratio;
	XMFLOAT3 pos;
	int fade;
	XMFLOAT4 color;
};

struct LevelUpStatus
{
	float oldValue;
	float newValue;
};

// texture num
enum
{
	UI_HP,
	UI_MP,
	UI_ST,
	UI_GAUGE_COVER,
	UI_JUMP_ICON,
	UI_MAGIC_HEAL,
	UI_MAGIC_FLAMEBLADE,
	UI_MAGIC_FIRE_BALL,
	UI_MAGIC_HIDDEN,
	UI_MAGIC_SHIELD,
	UI_FLAMEBLADE_ICON,
	UI_SKILL_ENABLED,
	UI_MESSAGEBOX_RETURN_TITLE,
	UI_BUTTON_NEW_GAME,
	UI_BUTTON_TUTORIAL,
	UI_BUTTON_EXIT_GAME,
	UI_BUTTON_YES,
	UI_BUTTON_NO,
	UI_MESSAGEBOX_TUTORIAL,
	UI_MESSAGEBOX_RESPAWN,
	BG_FADE,
	UI_MESSAGEBOX_TUTORIAL_RESPAWN,
	UI_SYSTEM_MENU_BOX,
	UI_BUTTON_STATUS,
	UI_BUTTON_MAGIC,
	UI_BUTTON_TELEPORT,
	UI_BUTTON_SAVE,
	UI_BUTTON_LOAD,
	UI_BUTTON_RESUME_GAME,
	UI_PLAYER_STATUS,
	UI_MAGIC_LIST,
	UI_SAVE_SLOT,
	UI_SAVE_PAGE,
	UI_LOAD_PAGE,
	UI_BUTTON_CONFIRM,
	UI_BUTTON_CANCEL,
	UI_NUM,
	UI_RIGHT_ARROW,
	UI_MAGIC_SLOT,
	UI_HIGHLIGHT_BOX,
	UI_NONE,
};

enum
{
	UI_MODULE_SYSTEM_MENU,
	UI_MODULE_STATUS,
	UI_MODULE_MAGIC_LIST,
	UI_MODULE_SAVE,
	UI_MODULE_LOAD,
};

enum
{
	SKILL_ICON_LEFT,
	SKILL_ICON_MIDDLE,
	SKILL_ICON_RIGHT,
};

enum
{
	CURSOR_SP_HP,
	CURSOR_SP_MP,
	CURSOR_SP_ST,
	CURSOR_SP_ATK,
	CURSOR_SP_MAT,
	CURSOR_SP_DEF,
	CURSOR_SP_MDF,
	CURSOR_CONFIRM = 99,
	CURSOR_CANCEL,
};

enum
{
	STATUS_LEVEL = SP_CNT_MAX,
	STATUS_SP_LEFT,
	STATUS_CURRENT_EXP,
	STATUS_REQUIRED_EXP,
	STATUS_MAX_HP,
	STATUS_MAX_MP,
	STATUS_MAX_ST,
	STATUS_ATK,
	STATUS_MAT,
	STATUS_DEF,
	STATUS_MDF,
};

enum
{
	CURSOR_LEFT,
	CURSOR_RIGHT,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUI(void);
void InitSystemMenu(void);
void UninitUI(void);
void UpdateUI(void);
void UpdateUIFade(void);
void UpdateStatusUI(void);
void UpdateMagicList(void);
void UpdateSystemMenu(void);
void UpdateSPStatus(int cursor, int dir);
void UpdatePlayerStatus(int category);
void DrawUI(void);
void DrawTitleUI(void);
void DrawMessageBox(void);
void DrawInGameUI(void);
void DrawPauseUI(void);
void DrawMenuUI(void);
void DrawStatusUI(void);
void DrawMagicList(void);
void DrawSystemMenuUI(void);
void DrawButton(int button, BOOL selected);
void DrawPlayerGauge(void);
void DrawPlayerJumpIcon(void);
void DrawSkillIcon(void);
void DrawFlamebladeIcon(void);
void DrawNumber(float posX, float posY, int num, int category, int numDigit = 0);

void SetRespawnMessageBox(BOOL render);
void SetRenderGauge(BOOL render);
void SetRenderSkillIcon(BOOL render);
void SetRenderJumpIcon(BOOL render);
void SetRenderBladeIcon(BOOL render);
void SetRenderMenuUI(BOOL render);
void SetUIFade(int module, int fade);
void ResetCursor(void);

void ClearLevelUpStatus(void);

void HandleStatusUIButton(void);
BOOL IsSkillIconActive(int magic);
int GetSkillIconTexNo(int magic);
float GetMagicCooldown(int magicType, float healingCDProgress, float fireBallCDProgress);