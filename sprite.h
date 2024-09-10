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

#define PI 3.14159265358979323846f
#define MAX_SEGMENTS 100 // �Z�O�����g���̍ő�l

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
// �v���g�^�C�v�錾
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