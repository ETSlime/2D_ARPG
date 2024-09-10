//=============================================================================
//
// sprite処理 [sprite.h]
// Author : GP11B132 99 外岡高明
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define PI 3.14159265358979323846f
#define MAX_SEGMENTS 100 // セグメント数の最大値

struct AABB
{
	XMFLOAT3	pos;
	float		w, h;
	int			tag;
};

// AABB tag
enum
{
	GROUND_AABB,
	WALL_AABB,
	PLAYER_BODY_AABB,
	PLAYER_ATTACK_AABB,
	ENEMY_BODY_AABB,
	ENEMY_ATTACK_AABB,
	PLAYER_MAGIC_AABB,
	ENEMY_MAGIC_AABB,
	TELEPORT_AABB,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteColor(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color);

void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot, int vertexOffset = 0);

void SetSpriteLeftTop(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color);


void SetSpriteTopToBottomRevealColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color, float progress);


void SetSpriteRightToLeftDisappearColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color, float progress);