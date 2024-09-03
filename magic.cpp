//=============================================================================
//
// UI���� [UI.cpp]
// Author : 
//
//=============================================================================
#include "magic.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define TEXTURE_FIRE_BALL_WIDTH					(155)
#define TEXTURE_FIRE_BALL_HEIGHT				(95)
#define TEXTURE_BOOM_WIDTH						(250)
#define TEXTURE_BOOM_HEIGHT						(250)
#define TEXTURE_HEAL_WIDTH						(150)
#define TEXTURE_HEAL_HEIGHT						(210)

#define MAGIC_FIRE_BALL_OFFSET					(50.0f)

#define	ANIM_WAIT_FIRE_BALL						(3)
#define	ANIM_WAIT_BOOM							(6)
#define ANIM_WAIT_HEAL							(4)
#define	TEXTURE_FIRE_BALL_PATTERN_DIVIDE_X		(27)
#define TEXTURE_FIRE_BALL_PATTERN_DIVIDE_Y		(1)
#define	TEXTURE_BOOM_PATTERN_DIVIDE_X			(4)
#define	TEXTURE_BOOM_PATTERN_DIVIDE_Y			(4)
#define	TEXTURE_HEAL_PATTERN_DIVIDE_X			(26)
#define	TEXTURE_HEAL_PATTERN_DIVIDE_Y			(1)

#define TEXTURE_MAX								(8)


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

#ifdef _DEBUG	
static ID3D11Buffer* g_AABBVertexBuffer = NULL;
#endif

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/magic/heal.png",
	"data/TEXTURE/magic/fireball.png",
	"data/TEXTURE/UI/flameblade_icon.png",
	"data/TEXTURE/magic/boom.png",

};

static Magic					g_Magic[MAGIC_MAX];	// ���@�̍\����
static BOOL						g_Load = FALSE;	

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMagic(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// �ϐ��̏�����
	for (int i = 0; i < MAGIC_MAX; i++)
	{
		ClearMagic(&g_Magic[i]);
	}

#ifdef _DEBUG	
	{
		int aabbCount = MAGIC_MAX;
		int maxVertices = MAGIC_MAX * 4;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * maxVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_AABBVertexBuffer);
	}
#endif

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagic(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < MAGIC_MAX; i++)
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
// �X�V����
//=============================================================================

void UpdateMagic(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < MAGIC_MAX; i++)
	{
		if (g_Magic[i].use == TRUE)
		{
			// �A�j���[�V����  
			g_Magic[i].countAnim++;
			if ((g_Magic[i].countAnim % GetMagicAnimWait(&g_Magic[i])) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Magic[i].patternAnim = (g_Magic[i].patternAnim + 1) % 
					(GetMagicTexturePatternDivideX(&g_Magic[i]) * GetMagicTexturePatternDivideY(&g_Magic[i]));

				if (g_Magic[i].isAnimRepeat == FALSE &&
					g_Magic[i].patternAnim == GetMagicTexturePatternDivideX(&g_Magic[i]) * GetMagicTexturePatternDivideY(&g_Magic[i]) - 1)
				{
					g_Magic[i].use = FALSE;
					ClearMagic(&g_Magic[i]);
				}
			}

			// �ړ�����
			if (g_Magic[i].followPlayer == TRUE)
			{
				g_Magic[i].pos = player->pos;
			}
			else
			{
				XMVECTOR pos = XMLoadFloat3(&g_Magic[i].pos);
				XMVECTOR move = XMLoadFloat3(&g_Magic[i].move);
				pos += move;
				XMStoreFloat3(&g_Magic[i].pos, pos);
			}

			// ��ʊO�܂Ői�񂾁H
			BG* bg = GetBG();
			if (g_Magic[i].pos.x < (-g_Magic[i].width / 2))		// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			{
				g_Magic[i].use = FALSE;
				ClearMagic(&g_Magic[i]);
			}
			if (g_Magic[i].pos.x > (bg->w + g_Magic[i].width / 2))	// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			{
				g_Magic[i].use = FALSE;
				ClearMagic(&g_Magic[i]);
			}

			// �����蔻�菈��
			if (g_Magic[i].isCollision == TRUE)
			{
				SetUpMagicCollision(&g_Magic[i]);

				HandleMagicCollision(&g_Magic[i]);

			}

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagic(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < MAGIC_MAX; i++)
	{
		if (g_Magic[i].use == TRUE)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Magic[i].magicType]);

			//���@�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Magic[i].pos.x - bg->pos.x;	// ���@�̕\���ʒuX
			float py = g_Magic[i].pos.y - bg->pos.y;	// ���@�̕\���ʒuY
			float pw = g_Magic[i].width;		// ���@�̕\����
			float ph = g_Magic[i].height;		// ���@�̕\������

			float tw = 1.0f / GetMagicTexturePatternDivideX(&g_Magic[i]);	// �e�N�X�`���̕�
			float th = 1.0f / GetMagicTexturePatternDivideY(&g_Magic[i]);	// �e�N�X�`���̍���
			// �e�N�X�`���̍���X���W
			float tx;
			// �e�N�X�`���̍���Y���W
			float ty = (float)(g_Magic[i].patternAnim / GetMagicTexturePatternDivideY(&g_Magic[i])) * th;
			if (g_Magic[i].invertTex)
			{
				tw *= -1.0f;
				tx = (float)(g_Magic[i].patternAnim % GetMagicTexturePatternDivideX(&g_Magic[i])) * (-1 * tw) - tw;
			}
			else
				tx = (float)(g_Magic[i].patternAnim % GetMagicTexturePatternDivideX(&g_Magic[i])) * tw;
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Magic[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

#ifdef _DEBUG
	{
		MATERIAL materialAABB;
		ZeroMemory(&materialAABB, sizeof(materialAABB));
		materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materialAABB.noTexSampling = 1;

		SetMaterial(materialAABB);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < MAGIC_MAX; ++i)
		{
			if (g_Magic[i].use == FALSE) continue;

			int vertexOffset = i * 4;

			SetSpriteColorRotation(g_AABBVertexBuffer, g_Magic[i].magicAABB.pos.x - bg->pos.x,
				g_Magic[i].magicAABB.pos.y - bg->pos.y, g_Magic[i].magicAABB.w, g_Magic[i].magicAABB.h,
				0.0f, 0.0f, 0.0f, 0.0f,
				XMFLOAT4(0.0f, 1.0f, 1.0f, 0.2f),
				0.0f,
				vertexOffset);

			GetDeviceContext()->Draw(4, vertexOffset);
		}
	}
#endif

}

void TriggerMagic(int magicType, XMFLOAT3 pos)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < MAGIC_MAX; i++)
	{
		if (g_Magic[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{
			g_Magic[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Magic[i].magicType = magicType;
			g_Magic[i].countAnim = 0;
			g_Magic[i].patternAnim = 0;
			switch (magicType)
			{
			case MAGIC_FIRE_BALL:
			{
				g_Magic[i].pos = player->pos;	// ���W���Z�b�g
				g_Magic[i].pos.x += player->dir == CHAR_DIR_RIGHT ? MAGIC_FIRE_BALL_OFFSET : -MAGIC_FIRE_BALL_OFFSET;
				g_Magic[i].width = TEXTURE_FIRE_BALL_WIDTH;
				g_Magic[i].height = TEXTURE_FIRE_BALL_HEIGHT;
				float speed = player->dir == CHAR_DIR_RIGHT ? FIRE_BALL_SPEED : -FIRE_BALL_SPEED;
				g_Magic[i].move = XMFLOAT3(speed, 0.0f, 0.0f);
				g_Magic[i].invertTex = player->invertTex;
				g_Magic[i].isAnimRepeat = TRUE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				break;
			}
			case MAGIC_HEALING:
			{
				g_Magic[i].pos = player->pos;	// ���W���Z�b�g
				g_Magic[i].width = TEXTURE_HEAL_WIDTH;
				g_Magic[i].height = TEXTURE_HEAL_HEIGHT;
				g_Magic[i].invertTex = player->invertTex;
				g_Magic[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = TRUE;
				g_Magic[i].isCollision = FALSE;
				break;
			}
			case MAGIC_BOOM:
			{
				g_Magic[i].pos = pos;	// ���W���Z�b�g
				g_Magic[i].width = TEXTURE_BOOM_WIDTH;
				g_Magic[i].height = TEXTURE_BOOM_HEIGHT;
				g_Magic[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				g_Magic[i].isAnimRepeat = FALSE;
				g_Magic[i].followPlayer = FALSE;
				g_Magic[i].isCollision = TRUE;
				break;
			}
			default:
				break;
			}
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

int GetMagicTexturePatternDivideX(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		return TEXTURE_FIRE_BALL_PATTERN_DIVIDE_X;
	case MAGIC_BOOM:
		return TEXTURE_BOOM_PATTERN_DIVIDE_X;
	case MAGIC_HEALING:
		return TEXTURE_HEAL_PATTERN_DIVIDE_X;
	default:
		return -1;
	}
}

int GetMagicTexturePatternDivideY(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		return TEXTURE_FIRE_BALL_PATTERN_DIVIDE_Y;
	case MAGIC_BOOM:
		return TEXTURE_BOOM_PATTERN_DIVIDE_Y;
	case MAGIC_HEALING:
		return TEXTURE_HEAL_PATTERN_DIVIDE_Y;
	default:
		return -1;
	}
}

int GetMagicAnimWait(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		return ANIM_WAIT_FIRE_BALL;
	case MAGIC_BOOM:
		return ANIM_WAIT_BOOM;
	case MAGIC_HEALING:
		return ANIM_WAIT_HEAL;
	default:
		return -1;
	}
}

void ClearMagic(Magic* magic)
{
	magic->use = FALSE;
	magic->invertTex = FALSE;
	magic->isAnimRepeat = FALSE;
	magic->followPlayer = FALSE;
	magic->isCollision = FALSE;
	magic->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	magic->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	magic->width = 0.0f;
	magic->height = 0.0f;
	magic->magicType = MAGIC_NONE;

	magic->countAnim = 0;
	magic->patternAnim = 0;

	magic->move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������
	magic->magicAABB.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	magic->magicAABB.w = 0.0f;
	magic->magicAABB.h = 0.0f;
}

void SetUpMagicCollision(Magic* magic)
{
	switch (magic->magicType)
	{
	case MAGIC_FIRE_BALL:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.tag = PLAYER_MAGIC_AABB;
		magic->magicAABB.w = magic->width * 0.3f;
		magic->magicAABB.h = magic->height * 0.5f;
		break;
	case MAGIC_BOOM:
		magic->magicAABB.pos = magic->pos;
		magic->magicAABB.tag = PLAYER_MAGIC_AABB;
		magic->magicAABB.w = magic->width;
		magic->magicAABB.h = magic->height;
		break;
	default:
		break;
	}
}

void HandleMagicCollision(Magic* magic)
{
	// �n�ʂ�AABB�����擾
	AABB* grounds = GetMap01AABB();
	for (int j = 0; j < MAP01_GROUND_MAX; j++)
	{
		if (grounds[j].tag == GROUND_AABB) continue;
		if (CollisionBB(magic->magicAABB.pos, magic->magicAABB.w, magic->magicAABB.h,
			grounds[j].pos, grounds[j].w, grounds[j].h))
		{
			switch (magic->magicType)
			{
			case MAGIC_FIRE_BALL:
				TriggerMagic(MAGIC_BOOM, magic->pos);
				magic->use = FALSE;
				break;
			case MAGIC_BOOM:
				break;
			default:
				break;
			}

			return;
		}
	}

	if (magic->magicAABB.tag == PLAYER_MAGIC_AABB)
	{
		ENEMY* enemy = GetEnemy();
		// �G�l�~�[�̐��������蔻����s��
		for (int j = 0; j < ENEMY_MAX; j++)
		{
			// �����Ă�G�l�~�[�Ɠ����蔻�������
			if (enemy[j].use == TRUE && CollisionBB(magic->magicAABB.pos, magic->magicAABB.w, magic->magicAABB.h,
				enemy[j].pos, enemy[j].w, enemy[j].h))
			{
				switch (magic->magicType)
				{
				case MAGIC_FIRE_BALL:
					TriggerMagic(MAGIC_BOOM, magic->pos);
					magic->use = FALSE;
					break;
				case MAGIC_BOOM:
					break;
				default:
					break;
				}

				return;
			}
		}
	}

}

Magic* GetMagic(void)
{
	return &g_Magic[0];
}