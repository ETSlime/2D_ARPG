//=============================================================================
//
// sprite���� [sprite.h]
// Author : GP11B132 99 �O������
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************

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
	MONSTER_BODY_AABB
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteColor(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color);

void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot);

void SetSpriteLeftTop(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color);

