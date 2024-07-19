//=============================================================================
//
// ファイル処理 [file.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "player.h"
#include "enemy.h"
#include "score.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/



/*******************************************************************************
* 構造体定義
*******************************************************************************/
struct	SAVEDATA				// セーブデータの構造体
{
	// プレイヤーデータを保存
	PLAYER	player[PLAYER_MAX];	// プレイヤーのMAX人数分セーブする

	// エネミーデータを保存
	ENEMY	enemy[ENEMY_MAX];	// エネミーのMAX人数分セーブする

	// スコアデータを保存
	int		score;				// スコアを保存する

	// 他にも何かセーブしたい物があればここに追加

	int		sum;				// チェックサムを入れる
};



/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void SaveData(void);
void LoadData(void);


