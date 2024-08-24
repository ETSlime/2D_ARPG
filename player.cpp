//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "file.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_IDLE_WIDTH					(150/2)	// �L�����T�C�Y
#define TEXTURE_IDLE_HEIGHT					(150/2)	// 
#define TEXTURE_NORMAL_ATTACK1_WIDTH		(285/2)	// �L�����T�C�Y
#define TEXTURE_NORMAL_ATTACK1_HEIGHT		(240/2)	// 
#define TEXTURE_NORMAL_ATTACK3_WIDTH		(281/2)	// �L�����T�C�Y
#define TEXTURE_NORMAL_ATTACK3_HEIGHT		(217/2)	//
#define TEXTURE_NORMAL_ATTACK4_WIDTH		(217/2)	// �L�����T�C�Y
#define TEXTURE_NORMAL_ATTACK4_HEIGHT		(290/2)	//
#define TEXTURE_RUN_WIDTH					(165/2)	// �L�����T�C�Y
#define TEXTURE_RUN_HEIGHT					(165/2)	// 
#define TEXTURE_JUMP_WIDTH					(210/2)	// �L�����T�C�Y
#define TEXTURE_JUMP_HEIGHT					(210/2)	//
#define TEXTURE_MAX							(20)	// �e�N�X�`���̐�

// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_IDLE_PATTERN_DIVIDE_X			(4)		
#define TEXTURE_WALK_PATTERN_DIVIDE_X			(4)
#define TEXTURE_RUN_PATTERN_DIVIDE_X			(8)
#define TEXTURE_DASH_PATTERN_DIVIDE_X			(1)
#define TEXTURE_JUMP_PATTERN_DIVIDE_X			(10)
#define TEXTURE_NORMAL_ATTACK1_PATTERN_DIVIDE_X	(7)
#define TEXTURE_NORMAL_ATTACK2_PATTERN_DIVIDE_X	(9)
#define TEXTURE_NORMAL_ATTACK3_PATTERN_DIVIDE_X	(8)
#define TEXTURE_NORMAL_ATTACK4_PATTERN_DIVIDE_X	(13)
#define TEXTURE_PATTERN_DIVIDE_Y				(1)


#define ANIM_PATTERN_NUM						(TEXTUREf_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT_WALK							(10)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_IDLE							(30)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_RUN							(5)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_DASH							(1)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_ATTACK						(5)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_JUMP							(7)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_DASH_FRAME							(10)
#define ANIM_NORMAL_ATTACK1_FRAME				(7)
#define ANIM_NORMAL_ATTACK2_FRAME				(9)
#define ANIM_NORMAL_ATTACK3_FRAME				(8)
#define ANIM_NORMAL_ATTACK4_FRAME				(13)


#define TEXTURE_NORMAL_ATTACK1_OFFSET			XMFLOAT3(10.0f, -20.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK2_OFFSET			XMFLOAT3(5.0f, 0.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK3_OFFSET			XMFLOAT3(10.0f, -10.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK4_OFFSET			XMFLOAT3(10.0f, -20.0f, 0.0f)

// �v���C���[�̉�ʓ��z�u���W
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_IDLE_HEIGHT)

// �W�����v����
#define	PLAYER_JUMP_CNT_MAX			(60)		// 60�t���[���Œ��n����
#define	PLAYER_JUMP_Y_MAX			(7.5f)	// �W�����v�̍���


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawPlayerOffset(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

#ifdef _DEBUG	
static ID3D11Buffer* g_AABBVertexBuffer = NULL;
#endif

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/char/char_idle_right.png",
	"data/TEXTURE/char/char_walkforward.png",
	"data/TEXTURE/char/char_runforward.png",
	"data/TEXTURE/char/char_dashforward.png",
	"data/TEXTURE/char/char_attack1.png",
	"data/TEXTURE/char/char_attack2.png",
	"data/TEXTURE/char/char_attack3.png",
	"data/TEXTURE/char/char_attack4.png",
	"data/TEXTURE/char/char_jump.png",
	"data/TEXTURE/shadow000.jpg",
};


static BOOL		g_Load = FALSE;				// ���������s�������̃t���O
static PLAYER	g_Player[PLAYER_MAX];		// �v���C���[�\����
static int		g_PlayerCount = PLAYER_MAX;	// �����Ă�v���C���[�̐�

static int      g_jumpCnt = 0;
static int		g_jump[PLAYER_JUMP_CNT_MAX] =
{
	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5,-4,-3,-2,-1,
	  1,   2,   3,   4,   5,   6,  7,  8,  9, 10, 11,12,13,14,15
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
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

	g_PlayerCount = 0;						// �����Ă�v���C���[�̐�

	// �v���C���[�\���̂̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_PlayerCount++;
		g_Player[i].use = TRUE;
		g_Player[i].pos = XMFLOAT3(PLAYER_INIT_POS_X, PLAYER_INIT_POS_Y, 0.0f);	// ���S�_����\��
		g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player[i].w = TEXTURE_IDLE_WIDTH;
		g_Player[i].h = TEXTURE_IDLE_HEIGHT;
		g_Player[i].shadowY = g_Player[i].pos.y;
		g_Player[i].texNo = 0;
		g_Player[i].state = IDLE;
		g_Player[i].invertTex = FALSE;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;
		g_Player[i].patternAnimOld = 0;
		g_Player[i].animFrameCount = 0;
		g_Player[i].dashCount = 0;
		g_Player[i].dashCD = 0;
		g_Player[i].airDashCount = 0;
		g_Player[i].maxDashCount = MAX_DASH_COUNT;

		g_Player[i].move = XMFLOAT3(3.75f, 0.0f, 0.0f);		// �ړ���

		g_Player[i].dir = CHAR_DIR_RIGHT;					// �E�����ɂ��Ƃ���
		g_Player[i].running = FALSE;
		g_Player[i].playAnim = FALSE;
		g_Player[i].dashOnAir = FALSE;
		g_Player[i].jumpOnAir = TRUE;
		g_Player[i].jumpOnAirCnt = 0;
		g_Player[i].patternAnim = CHAR_IDLE;

		g_Player[i].attackPattern = NONE;
		g_Player[i].actionQueueStart = 0;
		g_Player[i].actionQueueEnd = 0;
		g_Player[i].actionQueueClearTime = 0;

		// �W�����v�̏�����
		g_Player[i].jump = FALSE;
		g_Player[i].jumpCnt = 0;
		g_Player[i].jumpYMax = PLAYER_JUMP_Y_MAX;

		g_Player[i].onAirCnt = 0;

		for (int j = 0; j < ACTION_QUEUE_SIZE; j++)
		{
			g_Player[i].actionQueue[j] = 0;
		}

		// ���g�p
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}

		// AABB
		g_Player[i].bodyAABB.pos = g_Player[i].pos;
		g_Player[i].bodyAABB.w = g_Player[i].w;
		g_Player[i].bodyAABB.h = g_Player[i].h;
		g_Player[i].bodyAABB.tag = PLAYER_BODY_AABB;
	}

#ifdef _DEBUG	
	{
		int aabbCount = MAP01_GROUND_MAX;
		const int maxVertices = MAP01_GROUND_MAX * 4;

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
void UninitPlayer(void)
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
void UpdatePlayer(void)
{
	if (g_Load == FALSE) return;
	g_PlayerCount = 0;				// �����Ă�v���C���[�̐�

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// �����Ă�v���C���[��������������
		if (g_Player[i].use == TRUE)
		{
			g_PlayerCount++;		// �����Ă�v���C���[�̐�

			if (g_Player[i].state == IDLE) g_Player[i].shadowY = g_Player[i].pos.y;

			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			XMFLOAT3 pos_old = g_Player[i].pos;

			// ���g�p
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = pos_old;

			UpdateActionQueue();
			g_Player[i].dashCD++;

			// ���݂̃A�j���[�V�������I���������ǂ������m�F
			if (g_Player[i].playAnim == FALSE)
			{
				if (g_Player[i].actionQueueStart != g_Player[i].actionQueueEnd)
				{
					// �L���[���̎��̃A�N�V���������s
					int action = g_Player[i].actionQueue[g_Player[i].actionQueueStart];

					BOOL canExecuteAction = FALSE;
					switch (action)
					{
					case JUMP:
						if (g_Player->onAirCnt > 0)
						{
							if (g_Player->jumpOnAirCnt == 1)
								canExecuteAction = FALSE;
							else
							{
								canExecuteAction = TRUE;
								g_Player->jumpOnAirCnt = 1;
							}
						}
						else
							canExecuteAction = TRUE;
						break;
					case DASH:
						if (g_Player[i].dashCD > DASH_CD_TIME || g_Player[i].dashCount < g_Player[i].maxDashCount)
						{
							g_Player[i].dashCD = 0;
							g_Player[i].dashCount++;
							canExecuteAction = TRUE;
						}
						else
							canExecuteAction = FALSE;
						break;
					default:
						canExecuteAction = TRUE;
						break;
					}

					if (canExecuteAction)
					{
							
						g_Player[i].actionQueueStart = (g_Player[i].actionQueueStart + 1) % ACTION_QUEUE_SIZE;
						g_Player[i].patternAnim = 0;
						g_Player[i].state = action;
						g_Player->animFrameCount = 0;
						g_Player->playAnim = TRUE;
					}

				}
				// �������Ԃɖ߂�H
				//else if (g_Player[i].jumpCnt > 0)
				//{
				//	g_Player[i].state = JUMP;
				//	g_Player[i].patternAnim = g_Player[i].patternAnimOld;
				//}
				else
				{
					g_Player[i].state = IDLE;
					g_Player[i].attackPattern = NONE;
				}

			}

				

			{

				UpdateGroundCollision();
				UpdateKeyboardInput();
				UpdateGamepadInput();

				
				if (g_Player[i].dashCD >= DASH_CD_TIME)
				{
					g_Player[i].dashCount = 0;
				}
				if (g_Player[i].move.y >= g_Player->jumpYMax * 0.5f && g_Player[i].state == IDLE)
					g_Player[i].state = FALL;

				// MAP�O�`�F�b�N
				BG* bg = GetBG();

				if (g_Player[i].bodyAABB.pos.x < 0.0f)
				{
					SET_PLAYER_POS_X(0.0f);
				}

				if (g_Player[i].bodyAABB.pos.x > bg->w)
				{
					SET_PLAYER_POS_X(bg->w);
				}

				if (g_Player[i].bodyAABB.pos.y < 0.0f)
				{
					SET_PLAYER_POS_Y(0.0f);
				}

				if (g_Player[i].bodyAABB.pos.y > bg->h)
				{
					SET_PLAYER_POS_Y(bg->h);
				}

				// ���E�̐��������i��ʂ̎O���̈�j
				float leftLimitX = SCREEN_WIDTH / 3;
				float rightLimitX = SCREEN_WIDTH * 2 / 3;

				// �v���C���[����ʓ��̈ړ��͈͓����ǂ������m�F
				if (g_Player[i].pos.x < bg->pos.x + leftLimitX) {
					// �v���C���[�����̐����͈͂��z�����ꍇ�́A�w�i�����ɃX�N���[��
					bg->pos.x = g_Player[i].pos.x - leftLimitX;
				}
				else if (g_Player[i].pos.x > bg->pos.x + rightLimitX) {
					// �v���C���[���E�̐����͈͂��z�����ꍇ�́A�w�i���E�ɃX�N���[��
					bg->pos.x = g_Player[i].pos.x - rightLimitX;
				}

				// �w�i��X���W����ʒ[�Ő���
				if (bg->pos.x < 0) bg->pos.x = 0;
				if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				// �㉺�̐��������i��ʂ̎O���̈�j
				float topLimitY = SCREEN_HEIGHT / 3;
				float bottomLimitY = SCREEN_HEIGHT * 2 / 3;

				// �v���C���[����ʓ��̈ړ��͈͓����ǂ������m�F
				if (g_Player[i].pos.y < bg->pos.y + topLimitY) {
					// �v���C���[����̐����͈͂��z�����ꍇ�́A�w�i����ɃX�N���[��
					bg->pos.y = g_Player[i].pos.y - topLimitY;
				}
				else if (g_Player[i].pos.y > bg->pos.y + bottomLimitY) {
					// �v���C���[�����̐����͈͂��z�����ꍇ�́A�w�i�����ɃX�N���[��
					bg->pos.y = g_Player[i].pos.y - bottomLimitY;
				}

				// �w�i��Y���W����ʒ[�Ő���
				if (bg->pos.y < 0) bg->pos.y = 0;
				if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

				//// �v���C���[�̗����ʒu����MAP�̃X�N���[�����W���v�Z����
				//bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
				//if (bg->pos.x < 0) bg->pos.x = 0;
				//if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				//bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
				//if (bg->pos.y < 0) bg->pos.y = 0;
				//if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

				// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
				//{
				//	ENEMY* enemy = GetEnemy();

				//	// �G�l�~�[�̐��������蔻����s��
				//	for (int j = 0; j < ENEMY_MAX; j++)
				//	{
				//		// �����Ă�G�l�~�[�Ɠ����蔻�������
				//		if (enemy[j].use == TRUE)
				//		{
				//			BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
				//				enemy[j].pos, enemy[j].w, enemy[j].h);
				//			// �������Ă���H
				//			if (ans == TRUE)
				//			{
				//				// �����������̏���
				//				enemy[j].use = FALSE;
				//				AddScore(10);
				//			}
				//		}
				//	}
				//}



				// �o���b�g����
				//if (GetKeyboardTrigger(DIK_SPACE))
				//{
				//	XMFLOAT3 pos = g_Player[i].pos;
				//	pos.y += g_Player[i].jumpY;
				//	SetBullet(pos);
				//}

				//if (IsButtonTriggered(0, BUTTON_B))
				//{
				//	XMFLOAT3 pos = g_Player[i].pos;
				//	pos.y += g_Player[i].jumpY;
				//	SetBullet(pos);
				//}

			}

			// �A�j���[�V����  
			switch (g_Player[i].state)
			{
			case IDLE:
				PlayIdleAnim();
				break;
			case WALK:
				PlayWalkAnim();
				break;
			case RUN:
				PlayRunningAnim();
				break;
			case DASH:
				PlayDashAnim();
				break;
			case ATTACK:
				PlayAttackAnim();
				break;
			case JUMP:
				PlayJumpAnim();
				break;
			case FALL:
				PlayFallAnim();
				break;
			default:
				break;
			}
		}
	}


	// ������Z�[�u����
	if (GetKeyboardTrigger(DIK_S))
	{
		SaveData();
	}


#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
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

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes


			DrawPlayerShadow();

			// �v���C���[�̕��g��`��
			if (g_Player[i].state == DASH)
			{	// �_�b�V�����������g����
				DrawPlayerOffset(i);
			}

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_Player[i].w;		// �v���C���[�̕\����
			float ph = g_Player[i].h;		// �v���C���[�̕\������

			AdjustAttackTexturePosition(px, py);

			// �A�j���[�V�����p
			float tw, th, tx, ty;
			tw = 1.0f / GetTexturePatternDivideX(); // �e�N�X�`���̕�
			th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			ty = (float)(g_Player[i].patternAnim / GetTexturePatternDivideX()) * th;	// �e�N�X�`���̍���Y���W
			if (g_Player->invertTex)
			{
				tw *= -1.0f;
				tx = (float)(g_Player[i].patternAnim % GetTexturePatternDivideX()) * (-1 * tw) - tw;	// �e�N�X�`���̍���X���W
			}
			else
				tx = (float)(g_Player[i].patternAnim % GetTexturePatternDivideX()) * tw;

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Player[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);


#ifdef _DEBUG
			{
				MATERIAL materialAABB;
				ZeroMemory(&materialAABB, sizeof(materialAABB));
				materialAABB.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				materialAABB.noTexSampling = 1;

				SetMaterial(materialAABB);
				GetDeviceContext()->IASetVertexBuffers(0, 1, &g_AABBVertexBuffer, &stride, &offset);
				for (int i = 0; i < MAP01_GROUND_MAX; ++i)
				{
					SetSpriteColorRotation(g_AABBVertexBuffer, g_Player[i].bodyAABB.pos.x - bg->pos.x, 
						g_Player[i].bodyAABB.pos.y - bg->pos.y, g_Player[i].bodyAABB.w, g_Player[i].bodyAABB.h,
						0.0f, 0.0f, 0.0f, 0.0f,
						XMFLOAT4(0.0f, 1.0f, 0.0f, 0.2f),
						0.0f);

					GetDeviceContext()->Draw(4, i * 4);
				}
			}
#endif
		}
	}
}

void UpdateKeyboardInput(void)
{
	float speed = g_Player->move.x;

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_LCONTROL))
	{
		CHANGE_PLAYER_POS_Y(-speed * 5);
		g_Player->dir = CHAR_DIR_RIGHT;
		g_Player->state = WALK;
	}
	else if (GetKeyboardPress(DIK_DOWN) && GetKeyboardPress(DIK_LCONTROL))
	{
		CHANGE_PLAYER_POS_Y(speed * 5);
		g_Player->dir = CHAR_DIR_LEFT;
		g_Player->state = WALK;
	}

	if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_LCONTROL))
	{
		CHANGE_PLAYER_POS_X(-speed * 5);
		g_Player->dir = CHAR_DIR_RIGHT;
		g_Player->state = WALK;
	}
	else if (GetKeyboardPress(DIK_RIGHT) && GetKeyboardPress(DIK_LCONTROL))
	{
		CHANGE_PLAYER_POS_X(speed * 5);
		g_Player->dir = CHAR_DIR_LEFT;
		g_Player->state = WALK;
	}
#endif

	g_Player->running = FALSE;

	if (GetKeyboardPress(DIK_RIGHT) && g_Player->playAnim == FALSE)
	{
		g_Player->dir = CHAR_DIR_RIGHT;
		if (g_Player->move.y == 0)
		{
			g_Player->state = WALK;
			if (CheckMoveCollision(speed, g_Player->dir))
				CHANGE_PLAYER_POS_X(speed);
			if (GetKeyboardPress(DIK_LSHIFT))
			{
				g_Player->running = TRUE;
				g_Player->state = RUN;
				if (CheckMoveCollision(speed * 0.5f, g_Player->dir))
					CHANGE_PLAYER_POS_X(speed * 0.5f);
			}
		}
		else if (CheckMoveCollision(speed * 0.8f, g_Player->dir))
			CHANGE_PLAYER_POS_X(speed * 0.8f);

		std::cout << g_Player->pos.x << " ";
		std::cout << g_Player->pos.y << std::endl;
	}
	else if (GetKeyboardPress(DIK_LEFT) && g_Player->playAnim == FALSE)
	{
		g_Player->dir = CHAR_DIR_LEFT;
		if (g_Player->move.y == 0)
		{
			g_Player->state = WALK;
			if (CheckMoveCollision(-speed, g_Player->dir))
				CHANGE_PLAYER_POS_X(-speed);
			if (GetKeyboardPress(DIK_LSHIFT))
			{
				if (CheckMoveCollision(-speed * 0.5f, g_Player->dir))
					CHANGE_PLAYER_POS_X(-speed * 0.5f);
				g_Player->running = TRUE;
				g_Player->state = RUN;
			}
		}
		else if (CheckMoveCollision(-speed * 0.8f, g_Player->dir))
			CHANGE_PLAYER_POS_X(-speed * 0.8f);

		std::cout << g_Player->pos.x << " ";
		std::cout << g_Player->pos.y << std::endl;
	}

	if (GetKeyboardTrigger(DIK_C) && g_Player->dashCount < g_Player->maxDashCount)
	{
		// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
		if (g_Player->playAnim == FALSE)
		{
			if (g_Player->onAirCnt == 0 || (g_Player->onAirCnt > 0 && g_Player->dashOnAir == FALSE))
			{
				g_Player->patternAnim = 0;
				g_Player->state = DASH;
				g_Player->animFrameCount = 0;
				g_Player->dashCD = 0;
				g_Player->playAnim = TRUE;
				g_Player->dashCount++;

				if (g_Player->onAirCnt > 0 && g_Player->airDashCount < g_Player->maxDashCount)
				{
					g_Player->dashOnAir = TRUE;
					g_Player->airDashCount++;
				}
					
			}
		}
		// �󒆂̏��
		else if (g_Player->onAirCnt > 0 && g_Player->airDashCount < g_Player->maxDashCount)
		{
			g_Player->patternAnimOld = g_Player->patternAnim;
			g_Player->patternAnim = 0;
			g_Player->state = DASH;
			g_Player->dashOnAir = TRUE;
			g_Player->dashCD = 0;
			g_Player->animFrameCount = 0;
			g_Player->dashCount++;
			g_Player->airDashCount++;
		}
		// ���̑��̏�Ԃ̏ꍇ�A�A�N�V�������L���[�ɒǉ�
		else if (!(g_Player->dashOnAir == TRUE && g_Player->onAirCnt > 0))
		{
			g_Player->actionQueue[g_Player->actionQueueEnd] = DASH;
			g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

			// �L���[�̃I�[�o�[�t���[��h�~
			if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
			{
				g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // �ł��Â��A�N�V������j��
			}
		}

	}
	else if (GetKeyboardTrigger(DIK_SPACE) && g_Player->jumpOnAirCnt == 0)
	{
		//if (g_Player->state == FALL || g_Player->jumpOnAir == 1)
		//std::cout << g_Player->state << std::endl;
		//std::cout << "g_Player->jumpCnt " << g_Player->jumpCnt << std::endl;
		//std::cout << "g_Player->jumpOnAir " << g_Player->jumpOnAir << std::endl;
		//std::cout << "g_Player->jumpOnAir " << g_Player->jumpOnAir << std::endl;
		// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
		if (g_Player->playAnim == FALSE)
		{
			if (g_Player->onAirCnt == 0 || (g_Player->jumpOnAir && g_Player->jumpOnAirCnt == 0))
			{
				g_Player->patternAnim = 0;
				g_Player->state = JUMP;
				g_Player->animFrameCount = 0;
				g_Player->playAnim = TRUE;

				g_Player->state = JUMP;
				g_Player->jumpCnt = 0;

				if (g_Player->onAirCnt > 0)
					g_Player->jumpOnAirCnt = 1;
			}
			
		}
		// �󒆂̏��
		else if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX / 2 && g_Player->jumpOnAir && g_Player->jumpOnAirCnt == 0)
		{
			g_Player->patternAnim = 0;
			g_Player->state = JUMP;
			g_Player->animFrameCount = 0;
			g_Player->playAnim = TRUE;

			g_Player->state = JUMP;
			g_Player->jumpCnt = 0;

			g_Player->jumpOnAirCnt = 1;
				
		}
		// ���̑��̏�Ԃ̏ꍇ�A�A�N�V�������L���[�ɒǉ�
		else
		{
			g_Player->actionQueue[g_Player->actionQueueEnd] = JUMP;
			g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

			// �L���[�̃I�[�o�[�t���[��h�~
			if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
			{
				g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // �ł��Â��A�N�V������j��
			}
		}
	}
	else if (GetKeyboardTrigger(DIK_X))
	{
		switch (g_Player->state)
		{
			// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
		case IDLE:
		case WALK:
		case RUN:
			g_Player->patternAnim = 0;
			g_Player->state = ATTACK;
			g_Player->animFrameCount = 0;
			g_Player->playAnim = TRUE;
			g_Player->attackPattern = NORMAL_ATTACK1;
			break;

		case DASH:
		case ATTACK:
		case JUMP:
		case FALL:
			// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
			if (g_Player->playAnim == FALSE)
			{
				g_Player->patternAnim = 0;
				g_Player->animFrameCount = 0;
				g_Player->playAnim = TRUE;
				g_Player->attackPattern++;
			}
			// �󒆂̏��
			else if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX / 2 && g_Player->state != ATTACK)
			{
				g_Player->patternAnimOld = g_Player->patternAnim;
				g_Player->patternAnim = 0;
				g_Player->state = ATTACK;
				g_Player->animFrameCount = 0;
			}
			// ���̑��̏�Ԃ̏ꍇ�A�A�N�V�������L���[�ɒǉ�
			else
			{
				g_Player->actionQueue[g_Player->actionQueueEnd] = ATTACK;
				g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

				// �L���[�̃I�[�o�[�t���[��h�~
				if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
				{
					g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // �ł��Â��A�N�V������j��
				}
			}

			break;
		default:
			break;
		}

	}
}

void UpdateGamepadInput(void)
{
	//float speed = g_Player->move.x;

	//if (IsButtonPressed(0, BUTTON_RIGHT))
	//{
	//	g_Player->pos.x += speed;
	//	g_Player->dir = CHAR_IDLE_RIGHT;
	//}
	//else if (IsButtonPressed(0, BUTTON_LEFT))
	//{
	//	g_Player->pos.x -= speed;
	//	g_Player->dir = CHAR_IDLE_LEFT;
	//}
}

void UpdateGroundCollision(void)
{
	// �v���C���[�̈ʒu���X�V����O�ɐڒn���m�F
	AABB* grounds = GetMap01AABB();
	BOOL onGround = false;

	if (g_Player->jumpCnt == 0)
	{
		if (g_Player->onAirCnt < PLAYER_JUMP_CNT_MAX * 0.5f && g_Player->onAirCnt != 0)
		{
			// sin�֐����g�p���ĉ������x���v�Z
			float angle = (XM_PI / PLAYER_JUMP_CNT_MAX) * g_Player->onAirCnt;

			g_Player->move.y = g_Player->jumpYMax * sinf(angle);

			// �؋󎞊Ԃ𑝉�������
			g_Player->onAirCnt++;
		}
		else
		{
			// �؋󎞊Ԃ��ő�ɒB������A�������x���ő�l�ɕۂ�
			g_Player->move.y = g_Player->jumpYMax;
		}


		CHANGE_PLAYER_POS_Y(g_Player->move.y);
		for (int j = 0; j < MAP01_GROUND_MAX; j++)
		{
			if (CheckGroundCollision(g_Player, &grounds[j]))
				onGround = TRUE;
		}
		if (g_Player->playAnim == TRUE || onGround)
			CHANGE_PLAYER_POS_Y(-g_Player->move.y);
	}
	else
	{
		for (int j = 0; j < MAP01_GROUND_MAX; j++)
		{
			CheckGroundCollision(g_Player, &grounds[j]);
		}
	}

	//if (GetKeyboardPress(DIK_Z))
	//{

	//}
}


//=============================================================================
// Player�\���̂̐擪�A�h���X���擾
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}


// �����Ă�G�l�~�[�̐�
int GetPlayerCount(void)
{
	return g_PlayerCount;
}

void DrawPlayerShadow(void)
{
	BG* bg = GetBG();

	// �e�\��
	SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[CHAR_SHADOW]);

	float px = g_Player->pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
	float py = g_Player->shadowY - bg->pos.y;	// �v���C���[�̕\���ʒuY
	float pw = TEXTURE_IDLE_WIDTH;		// �v���C���[�̕\����
	float ph = TEXTURE_IDLE_WIDTH / 4;		// �v���C���[�̕\������

	// �n�ʂƂ̏Փ˔���Ɋ�Â��e�̈ʒu
	// �v���C���[�̈ʒu�ɑΉ�����n�ʂ̍������v�Z
	float shadowPosY = py;  // �e�̍ŏI�I��Y���W
	float highestGroundY = FLT_MAX;  // �ł������n�ʂ�Y���W��ێ�

	// �n�ʂ�AABB�����擾
	AABB* ground = GetMap01AABB();
	BOOL isOnGround = false;

	// �v���C���[���n�ʂɂ��邩�ǂ������m�F
	for (int j = 0; j < MAP01_GROUND_MAX; j++)
	{
		float groundX = ground[j].pos.x;
		float groundY = ground[j].pos.y;
		float groundW = ground[j].w;
		float groundH = ground[j].h;

		// �v���C���[��x���W���n�ʂ̕��͈̔͂ɓ����Ă��邩�ǂ������m�F
		if (g_Player->pos.x >= groundX - groundW / 2 && g_Player->pos.x <= groundX + groundW / 2)
		{
			// �����Ƃ������n�ʂ�T��
			if (groundY - groundH / 2 < highestGroundY)
			{
				highestGroundY = groundY - groundH / 2;  // ���݂̒n�ʂ��ł������̂ōX�V
				shadowPosY = groundY - groundH / 2 - bg->pos.y - 50.0f; // �n�ʂ̏�ɉe��\��
				isOnGround = true;  // �v���C���[���n�ʂɂ���ƃt���O��ݒ�
			}
		}
	}
	py = shadowPosY + 50.0f;

	float tw = 1.0f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.0f;	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W



	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

	SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�
}

//=============================================================================
// �v���C���[�̕��g��`��
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG* bg = GetBG();
	float alpha = 0.0f;

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].texNo]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Player[no].offset[j].x - bg->pos.x;	// �v���C���[�̕\���ʒuX
		float py = g_Player[no].offset[j].y - bg->pos.y;	// �v���C���[�̕\���ʒuY
		float pw = g_Player[no].w;		// �v���C���[�̕\����
		float ph = g_Player[no].h;		// �v���C���[�̕\������

		// �A�j���[�V�����p
		float tw = 1.0f / GetTexturePatternDivideX();	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float ty = (float)(g_Player[no].patternAnim / GetTexturePatternDivideX()) * th;	// �e�N�X�`���̍���Y���W
		float tx;
		if (g_Player->invertTex)
		{
			tw *= -1.0f;
			tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * (-1 * tw) - tw;	// �e�N�X�`���̍���X���W
		}
		else
			tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * tw;

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

void PlayRunningAnim(void)
{
	g_Player->w = TEXTURE_RUN_WIDTH;
	g_Player->h = TEXTURE_RUN_HEIGHT;

	g_Player->texNo = CHAR_RUN;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_RUN)
	{
		g_Player->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayIdleAnim(void)
{
	g_Player->w = TEXTURE_IDLE_WIDTH;
	g_Player->h = TEXTURE_IDLE_HEIGHT;

	g_Player->countAnim += 1.0f;
	g_Player->texNo = CHAR_IDLE;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_IDLE)
	{
		g_Player->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayDashAnim(void)
{
	g_Player->w = TEXTURE_RUN_WIDTH;
	g_Player->h = TEXTURE_RUN_HEIGHT;

	float speed = g_Player->move.x;

	g_Player->texNo = CHAR_DASH;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	switch (g_Player->dir)
	{
	case CHAR_DIR_LEFT:
		if (CheckMoveCollision(-speed * 2, g_Player->dir))
			CHANGE_PLAYER_POS_X(-speed * 2);
			
		break;
	case CHAR_DIR_RIGHT:
		if (CheckMoveCollision(speed * 2, g_Player->dir))
			CHANGE_PLAYER_POS_X(speed * 2);
		break;
	default:
		break;
	}
	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_DASH)
	{
		g_Player->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
		g_Player->animFrameCount++;
	}
	if (g_Player->animFrameCount == ANIM_DASH_FRAME)
	{
		g_Player->animFrameCount = 0;

		if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX * 0.5f)	
			g_Player->state = JUMP;
		else
		{
			g_Player->state = IDLE;
			g_Player->playAnim = FALSE;
			g_Player->jumpCnt = 0;
		}

	}
}

void PlayAttackAnim(void)
{
	if (g_Player->attackPattern == ATTACK_PATTERN_MAX ||
		g_Player->attackPattern == NONE)
		g_Player->attackPattern = NORMAL_ATTACK1;


	AdjustAttackTextureSize();

	//std::cout << g_Player->attackPattern << std::endl;

	int attackFrame = 0;

	float speed = g_Player->move.x;
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		g_Player->texNo = CHAR_NORMAL_ATTACK1;
		attackFrame = ANIM_NORMAL_ATTACK1_FRAME;
		break;
	case NORMAL_ATTACK2:
		g_Player->texNo = CHAR_NORMAL_ATTACK2;
		attackFrame = ANIM_NORMAL_ATTACK2_FRAME;
		break;
	case NORMAL_ATTACK3:
		g_Player->texNo = CHAR_NORMAL_ATTACK3;
		attackFrame = ANIM_NORMAL_ATTACK3_FRAME;
		break;
	case NORMAL_ATTACK4:
		g_Player->texNo = CHAR_NORMAL_ATTACK4;
		attackFrame = ANIM_NORMAL_ATTACK4_FRAME;
		break;
	default:
		break;
	}
	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_ATTACK)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 2)
			g_Player->patternAnim = GetTexturePatternDivideX() - 2;
		g_Player->animFrameCount++;
	}
	if (g_Player->animFrameCount == attackFrame)
	{
		g_Player->animFrameCount = 0;

		if (g_Player->jumpCnt > 0)
			g_Player->state = JUMP;
		else
		{
			g_Player->state = IDLE;
			g_Player->playAnim = FALSE;
		}

		g_Player->attackPattern++;
	}
}

void PlayWalkAnim(void)
{
	g_Player->w = TEXTURE_IDLE_WIDTH;
	g_Player->h = TEXTURE_IDLE_HEIGHT;

	g_Player->texNo = CHAR_WALK;
	g_Player->invertTex = g_Player->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	g_Player->countAnim++;
	if (g_Player->countAnim > ANIM_WAIT_WALK)
	{
		g_Player->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayJumpAnim()
{
	if (GetKeyboardPress(DIK_RIGHT))
	{
		g_Player->dir = CHAR_DIR_RIGHT;
		g_Player->invertTex = FALSE;
		if (CheckMoveCollision(g_Player->move.x * 0.8f, CHAR_DIR_RIGHT))
			CHANGE_PLAYER_POS_X(g_Player->move.x * 0.8f);
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		g_Player->dir = CHAR_DIR_LEFT;
		g_Player->invertTex = TRUE;
		if (CheckMoveCollision(-g_Player->move.x * 0.8f, CHAR_DIR_LEFT))
			CHANGE_PLAYER_POS_X(-g_Player->move.x * 0.8f);
	}
		

	g_Player->w = TEXTURE_JUMP_WIDTH;
	g_Player->h = TEXTURE_JUMP_HEIGHT;

	g_Player->texNo = CHAR_JUMP;

	float angle = (XM_PI / PLAYER_JUMP_CNT_MAX) * g_Player->jumpCnt;
	g_Player->move.y = -1.0f * g_Player->jumpYMax * cosf(angle);

	// ����������������𔻒f���Ĉ�x�ɏ���
	int direction = (g_Player->move.y < 0) ? CHAR_DIR_UP : CHAR_DIR_DOWN;
	if (CheckMoveCollision(g_Player->move.y, direction))
	{
		CHANGE_PLAYER_POS_Y(g_Player->move.y);
	}
	else if (direction == CHAR_DIR_DOWN)
	{
		g_Player->jumpCnt = PLAYER_JUMP_CNT_MAX;
	}

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_JUMP)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 3)
			g_Player->patternAnim = GetTexturePatternDivideX() - 4;
		g_Player->animFrameCount++;
	}

	g_Player->jumpCnt++;
	g_Player->onAirCnt = g_Player->jumpCnt * 0.5f;
	if (g_Player->jumpCnt >= PLAYER_JUMP_CNT_MAX)
	{
		g_Player->playAnim = FALSE;
		g_Player->state = IDLE;
		g_Player->jumpCnt = 0;
	}
}

void PlayFallAnim()
{
	g_Player->w = TEXTURE_JUMP_WIDTH;
	g_Player->h = TEXTURE_JUMP_HEIGHT;

	g_Player->texNo = CHAR_JUMP;

	g_Player->countAnim += 1.0f;
	if (g_Player->countAnim > ANIM_WAIT_JUMP)
	{
		g_Player->countAnim = 0.0f;
		g_Player->patternAnim++;
		if (g_Player->patternAnim > GetTexturePatternDivideX() - 3)
			g_Player->patternAnim = GetTexturePatternDivideX() - 4;
		g_Player->animFrameCount++;
	}
}

int GetTexturePatternDivideX()
{
	switch (g_Player->texNo)
	{
	case CHAR_IDLE:
		return TEXTURE_IDLE_PATTERN_DIVIDE_X;
	case CHAR_WALK:
		return TEXTURE_WALK_PATTERN_DIVIDE_X;
	case CHAR_RUN:
		return TEXTURE_RUN_PATTERN_DIVIDE_X;
	case CHAR_DASH:
		return TEXTURE_DASH_PATTERN_DIVIDE_X;
	case CHAR_JUMP:
		return TEXTURE_JUMP_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK1:
		return TEXTURE_NORMAL_ATTACK1_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK2:
		return TEXTURE_NORMAL_ATTACK2_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK3:
		return TEXTURE_NORMAL_ATTACK3_PATTERN_DIVIDE_X;
	case CHAR_NORMAL_ATTACK4:
		return TEXTURE_NORMAL_ATTACK4_PATTERN_DIVIDE_X;
	default:
		return -1;
	}
}

void UpdateActionQueue(void)
{
	g_Player->actionQueueClearTime++;
	if (g_Player->actionQueueClearTime == ACTION_QUEUE_CLEAR_WAIT)
	{
		g_Player->actionQueueStart = 0;
		g_Player->actionQueueEnd = 0;
		g_Player->actionQueueClearTime = 0;
	}
}

BOOL CheckMoveCollision(float move, int dir)
{
	// �ǂ�AABB�����擾
	AABB* walls = GetMap01AABB();
	for (int i = 0; i < MAP01_GROUND_MAX; i++)
	{
		if (walls[i].tag == GROUND_AABB) continue;

		XMFLOAT3 wallPos = walls[i].pos;
		float wallW = walls[i].w;
		float wallH = walls[i].h;

		
		// �Փˊm�F
		switch (dir)
		{
		case CHAR_DIR_LEFT:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x + move, g_Player->pos.y, g_Player->pos.z);

			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH) &&
				g_Player->bodyAABB.pos.x > wallPos.x)
			{
				// �v���C���[�����Ɉړ����Ă��āA�ǂ������ɂ���ꍇ
				SET_PLAYER_POS_X(wallPos.x + wallW / 2 + g_Player->bodyAABB.w / 2 + 0.01f); // ���ւ̐i�s���~
				return false;
			}
			break;
		}

		case CHAR_DIR_RIGHT:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x + move, g_Player->pos.y, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH) &&
				g_Player->bodyAABB.pos.x < wallPos.x)
			{
				// �v���C���[���E�Ɉړ����Ă��āA�ǂ��E���ɂ���ꍇ
				SET_PLAYER_POS_X(wallPos.x - wallW / 2 - g_Player->bodyAABB.w / 2 - 0.01f); // �E�ւ̐i�s���~
				return false;
			}
			break;
		}

		case CHAR_DIR_UP:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x, g_Player->pos.y + move, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH))
			{
				// �v���C���[����Ɉړ����Ă��āA�ǂ��㑤�ɂ���ꍇ
				SET_PLAYER_POS_Y(wallPos.y + wallH / 2 + g_Player->bodyAABB.h / 2); // ��ւ̐i�s���~
				return false;
			}
			break;
		}
		case CHAR_DIR_DOWN:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x, g_Player->pos.y + move, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH))
			{
				// �v���C���[����Ɉړ����Ă��āA�ǂ������ɂ���ꍇ
				SET_PLAYER_POS_Y(wallPos.y - wallH / 2 - g_Player->bodyAABB.h / 2); // ���ւ̐i�s���~
				return false;
			}
			break;
		}
		default:
			break;
		}
	}

	return true;
}

BOOL CheckGroundCollision(PLAYER* g_Player, AABB* ground)
{
	// �v���C���[��AABB�����擾
	XMFLOAT3 playerPos = g_Player->bodyAABB.pos;
	float playerW = g_Player->bodyAABB.w;
	float playerH = g_Player->bodyAABB.h;

	// �n�ʂ�AABB�����擾
	XMFLOAT3 groundPos = ground->pos;
	float groundW = ground->w;
	float groundH = ground->h;

	if (groundPos.y < playerPos.y) return false;

	// �Փˊm�F
	BOOL isColliding = CollisionBB(playerPos, playerW, playerH, groundPos, groundW, groundH);
	// �v���C���[���n�ʂ̏�łǂꂭ�炢�d�Ȃ��Ă��邩���v�Z
	float overlapLeft = max(playerPos.x - playerW / 2, groundPos.x - groundW / 2);
	float overlapRight = min(playerPos.x + playerW / 2, groundPos.x + groundW / 2);
	float overlapWidth = overlapRight - overlapLeft;

	// ������臒l
	float threshold = playerW / FALLING_THRESHOLD;

	// �v���C���[��y�����̑��x��0��菬�����i�v���C���[���������j�ꍇ�A���Փ˂��������A�d�Ȃ肪臒l�𒴂��Ă���ꍇ
	if (isColliding && g_Player->move.y >= 0 && overlapWidth >= threshold) 
	{
		// �v���C���[���������ł��n�ʂɏ\���ڐG���Ă���A�������~��
		g_Player->move.y = 0.0f;

		// �v���C���[�̑؋󎞊Ԃ����Z�b�g����
		g_Player->onAirCnt = 0;
		// �W�����v�̏�Ԃ���idle��Ԃɖ߂�
		if (g_Player->jumpCnt > 0)
		{
			g_Player->playAnim = FALSE;
			g_Player->state = IDLE;
			g_Player->jumpCnt = 0;
		}
		// �󒆃_�b�V����Ԃ����Z�b�g
		g_Player->dashOnAir = FALSE;
		// �󒆃_�b�V���񐔂����Z�b�g
		g_Player->airDashCount = 0;
		g_Player->jumpOnAirCnt = 0;

		
		// �v���C���[�̈ʒu��n�ʂ̏�ɒ������A�n�ʂ�ʂ蔲���Ȃ��悤�ɂ���
		SET_PLAYER_POS_Y(groundPos.y - groundH * 0.5f - playerH * 0.5f + 0.01f); // 0.01f�̃}�[�W����ǉ�

		ground->tag = GROUND_AABB;
		return true;
	}
	else if (isColliding && overlapWidth < threshold) 
	{
		// �v���C���[���n�ʂ̏�ɂ��܂�d�Ȃ��Ă��Ȃ��̂ŗ����𑱍s
		// �v���C���[�����������o���āA�n�ʂƏՓ˂��Ȃ��悤�ɂ���
		if (playerPos.x < groundPos.x) 
		{
			// �v���C���[�������ɂ���ꍇ
			CHANGE_PLAYER_POS_X(-1.0f);// �������ɉ����o��
		}
		else 
		{
			// �v���C���[���E���ɂ���ꍇ
			CHANGE_PLAYER_POS_X(1.0f);// �����E�ɉ����o��
		}
	}
	ground->tag = WALL_AABB;

	return false;
}

void AdjustAttackTextureSize()
{
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		g_Player->w = TEXTURE_NORMAL_ATTACK1_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK1_HEIGHT;
		break;
	case NORMAL_ATTACK2:
		g_Player->w = TEXTURE_NORMAL_ATTACK1_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK1_HEIGHT;
		break;
	case NORMAL_ATTACK3:
		g_Player->w = TEXTURE_NORMAL_ATTACK3_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK3_HEIGHT;
		break;
	case NORMAL_ATTACK4:
		g_Player->w = TEXTURE_NORMAL_ATTACK4_WIDTH;
		g_Player->h = TEXTURE_NORMAL_ATTACK4_HEIGHT;
		break;
	default:
		break;
	}
}

void AdjustAttackTexturePosition(float& px, float& py)
{
	switch (g_Player->attackPattern)
	{
	case NORMAL_ATTACK1:
		px += TEXTURE_NORMAL_ATTACK1_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK1_OFFSET.y;
		break;
	case NORMAL_ATTACK2:
		px += TEXTURE_NORMAL_ATTACK2_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK2_OFFSET.y;
		break;
	case NORMAL_ATTACK3:
		px += TEXTURE_NORMAL_ATTACK3_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK3_OFFSET.y;
		break;
	case NORMAL_ATTACK4:
		px += TEXTURE_NORMAL_ATTACK4_OFFSET.x;
		py += TEXTURE_NORMAL_ATTACK4_OFFSET.y;
		break;
	default:
		break;
	}
}