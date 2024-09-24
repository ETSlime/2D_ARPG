//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "map.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)
#define TEXTURE_TITLE_WIDTH			(850)
#define TEXTURE_TITLE_HEIGHT		(333)
#define TEXTURE_MAX					(3)				// �e�N�X�`���̐�


#define	TITLE_POS_X					(45.0f)
#define TITLE_POS_Y					(-10.0f)

#define ANIM_WAIT_BG				(5)
#define	ANIM_WAIT_TITLE				(5)
#define ANIM_BG_PATTERN_DIVID_X		(6)
#define ANIM_BG_PATTERN_DIVID_Y		(5)
#define	ANIM_TITLE_PATTERN_DIVID_X	(5)
#define	BUTTON_NUM					(3)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title_bg.png",
	"data/TEXTURE/title.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�
static int						g_Cursor;
static int						g_BG_patternAnim;
static int						g_BG_countAnim;
static int						g_title_patternAnim;
static int						g_title_countAnim;

float	alpha;
BOOL	flag_alpha;

static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	ID3D11Device *pDevice = GetDevice();

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
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = XMFLOAT3(g_w/2, g_h/2, 0.0f);
	g_TexNo = 0;
	g_Cursor = 0;
	g_Load = TRUE;

	g_BG_countAnim = 0;
	g_BG_patternAnim = 0;
	g_title_patternAnim = 0;
	g_title_countAnim = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
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
// �X�V����
//=============================================================================
void UpdateTitle(void)
{

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		HandleButtonPressed();
	}
	// �Q�[���p�b�h�œ��͏���
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		HandleButtonPressed();
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		HandleButtonPressed();
	}

	if (GetKeyboardRelease(DIK_UP))
	{
		g_Cursor = (g_Cursor + BUTTON_NUM - 1) % BUTTON_NUM;
	}
	else if (GetKeyboardRelease(DIK_DOWN))
	{
		g_Cursor = (g_Cursor + 1) % BUTTON_NUM;
	}

	g_BG_countAnim++;
	if (g_BG_countAnim == ANIM_WAIT_BG)
	{
		g_BG_countAnim = 0;
		g_BG_patternAnim = (g_BG_patternAnim + 1) % (ANIM_BG_PATTERN_DIVID_X * ANIM_BG_PATTERN_DIVID_Y);
	}

	g_title_countAnim++;
	if (g_title_countAnim == ANIM_WAIT_TITLE)
	{
		g_title_countAnim = 0;
		g_title_patternAnim = (g_title_patternAnim + 1) % ANIM_TITLE_PATTERN_DIVID_X;
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
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

	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		float px = 0.0f;
		float py = 0.0f;
		float pw = TEXTURE_WIDTH;
		float ph = TEXTURE_HEIGHT;

		float tw = 1.0f / ANIM_BG_PATTERN_DIVID_X;	// �e�N�X�`���̕�
		float th = 1.0f / ANIM_BG_PATTERN_DIVID_Y;							// �e�N�X�`���̍���

		float tx = g_BG_patternAnim % ANIM_BG_PATTERN_DIVID_X * tw;
		float ty = g_BG_patternAnim / ANIM_BG_PATTERN_DIVID_X * th;

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �^�C�g����`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		float px = TITLE_POS_X;
		float py = TITLE_POS_Y;
		float pw = TEXTURE_TITLE_WIDTH;
		float ph = TEXTURE_TITLE_HEIGHT;

		float tw = 1.0f / ANIM_TITLE_PATTERN_DIVID_X;	// �e�N�X�`���̕�
		float th = 1.0f;								// �e�N�X�`���̍���

		float tx = g_title_patternAnim % ANIM_TITLE_PATTERN_DIVID_X * tw;
		float ty = 0.0f;

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

void HandleButtonPressed(void)
{
	switch (g_Cursor)
	{
	case NEW_GANE:
		SetFade(FADE_OUT, MODE_MESSAGEBOX);
		break;
	case TUTORIAL:
		SetFade(FADE_OUT, MODE_TUTORIAL);
		SetCurrentMap(TUTORIAL_01);
		InitTeleportInitPos(TUTORIAL_01);
		SetPlayerInitPosByMap(TUTORIAL_01, INITPOS_01);
		break;
	case EXIT_GAME:
		ExitGame();
		break;
	default:
		break;
	}
}

int GetTitleCursor(void)
{
	return g_Cursor;
}


