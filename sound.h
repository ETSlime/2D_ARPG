//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_sample000,	// BGM0
	SOUND_LABEL_BGM_sample001,	// BGM1
	SOUND_LABEL_BGM_sample002,	// BGM2
	SOUND_LABEL_SE_bomb000,		// 爆発音
	SOUND_LABEL_SE_defend000,	// 
	SOUND_LABEL_SE_defend001,	// 
	SOUND_LABEL_SE_hit000,		// 
	SOUND_LABEL_SE_laser000,	// 
	SOUND_LABEL_SE_lockon000,	// 
	SOUND_LABEL_SE_shot000,		// 
	SOUND_LABEL_SE_shot001,		// 

	SOUND_LABEL_BGM_title,		//
	SOUND_LABEL_BGM_tutorial,
	SOUND_LABEL_BGM_bossFight,
	SOUND_LABEL_SE_CURSOR,
	SOUND_LABEL_SE_ENTER,
	SOUND_LABEL_SE_PARRY,
	SOUND_LABEL_SE_HEAL,
	SOUND_LABEL_SE_FIRE,
	SOUND_LABEL_SE_CAST,
	SOUND_LABEL_SE_BOOM,
	SOUND_LABEL_SE_TELEPORT,
	SOUND_LABEL_SE_BOSS_SCREAM,
	SOUND_LABEL_SE_THUNDER,
	SOUND_LABEL_SE_ATTACK,
	SOUND_LABEL_SE_DASH,


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

