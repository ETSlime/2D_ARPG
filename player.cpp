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
#define TEXTURE_WIDTH						(200/2)	// �L�����T�C�Y
#define TEXTURE_HEIGHT						(200/2)	// 
#define TEXTURE_NORMAL_ATTACK1_WIDTH		(380/2)	// �L�����T�C�Y
#define TEXTURE_NORMAL_ATTACK1_HEIGHT		(320/2)	// 
#define TEXTURE_NORMAL_ATTACK3_WIDTH		(375/2)	// �L�����T�C�Y
#define TEXTURE_NORMAL_ATTACK3_HEIGHT		(290/2)	//
#define TEXTURE_NORMAL_ATTACK4_WIDTH		(290/2)	// �L�����T�C�Y
#define TEXTURE_NORMAL_ATTACK4_HEIGHT		(290/2)	//
#define TEXTURE_JUMP_WIDTH					(280/2)	// �L�����T�C�Y
#define TEXTURE_JUMP_HEIGHT					(280/2)	//
#define TEXTURE_MAX							(20)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X				(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_WALK_PATTERN_DIVIDE_X			(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_RUN_PATTERN_DIVIDE_X			(8)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_DASH_PATTERN_DIVIDE_X			(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_JUMP_PATTERN_DIVIDE_X			(10)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_NORMAL_ATTACK1_PATTERN_DIVIDE_X	(7)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_NORMAL_ATTACK2_PATTERN_DIVIDE_X	(9)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_NORMAL_ATTACK3_PATTERN_DIVIDE_X	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_NORMAL_ATTACK4_PATTERN_DIVIDE_X	(13)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y				(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)


#define ANIM_PATTERN_NUM						(TEXTUREf_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT								(10)		// �A�j���[�V�����̐؂�ւ��Wait�l
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
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

// �W�����v����
#define	PLAYER_JUMP_CNT_MAX			(60)		// 60�t���[���Œ��n����
#define	PLAYER_JUMP_Y_MAX			(10.0f)	// �W�����v�̍���


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
	"data/TEXTURE/char/char_idle_left.png",
	"data/TEXTURE/char/char_walkforward.png",
	"data/TEXTURE/char/char_walkbackward.png",	
	"data/TEXTURE/char/char_runforward.png",	
	"data/TEXTURE/char/char_runrollback.png",
	"data/TEXTURE/char/char_dashforward.png",
	"data/TEXTURE/char/char_dashbackward.png",
	"data/TEXTURE/char/char_attack1.png",
	"data/TEXTURE/char/char_attack2.png",
	"data/TEXTURE/char/char_attack3.png",
	"data/TEXTURE/char/char_attack4.png",
	"data/TEXTURE/char/char_jump.png",
	"data/TEXTURE/shadow000.jpg",
	"data/TEXTURE/char01.png",
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
		g_Player[i].w = TEXTURE_WIDTH;
		g_Player[i].h = TEXTURE_HEIGHT;
		g_Player[i].shadowY = g_Player[i].pos.y;
		g_Player[i].texNo = 0;
		g_Player[i].state = IDLE;

		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;
		g_Player[i].patternAnimOld = 0;
		g_Player[i].animFrameCount = 0;
		g_Player[i].dashCount = 0;

		g_Player[i].move = XMFLOAT3(5.0f, 0.0f, 0.0f);		// �ړ���

		g_Player[i].dir = CHAR_DIR_RIGHT;					// �E�����ɂ��Ƃ���
		g_Player[i].running = FALSE;
		g_Player[i].playAnim = FALSE;
		g_Player[i].dashOnAir = FALSE;
		g_Player[i].patternAnim = CHAR_IDLE_RIGHT;

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
			g_Player[i].dashCount++;

			// ���݂̃A�j���[�V�������I���������ǂ������m�F
			if (g_Player[i].playAnim == FALSE)
			{
				if (g_Player[i].actionQueueStart != g_Player[i].actionQueueEnd)
				{
					// �L���[���̎��̃A�N�V���������s
					int action = g_Player[i].actionQueue[g_Player[i].actionQueueStart];
					g_Player[i].actionQueueStart = (g_Player[i].actionQueueStart + 1) % ACTION_QUEUE_SIZE;

					g_Player[i].patternAnim = 0;
					g_Player[i].state = action;
					g_Player->animFrameCount = 0;
					g_Player->playAnim = TRUE;
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

				

			// �L�[���͂ňړ�
			//if (g_Player[i].state == IDLE)
			{
				float speed = g_Player[i].move.x;
				
#ifdef _DEBUG
				if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_LCONTROL))
				{
					CHANGE_PLAYER_POS_Y(-speed * 5);
					g_Player[i].dir = CHAR_DIR_RIGHT;
					g_Player[i].state = WALK;
				}
				else if (GetKeyboardPress(DIK_DOWN) && GetKeyboardPress(DIK_LCONTROL))
				{
					CHANGE_PLAYER_POS_Y(speed * 5);
					g_Player[i].dir = CHAR_DIR_LEFT;
					g_Player[i].state = WALK;
				}

				if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_LCONTROL))
				{
					CHANGE_PLAYER_POS_X(-speed * 5);
					g_Player[i].dir = CHAR_DIR_RIGHT;
					g_Player[i].state = WALK;
				}
				else if (GetKeyboardPress(DIK_RIGHT) && GetKeyboardPress(DIK_LCONTROL))
				{
					CHANGE_PLAYER_POS_X(speed * 5);
					g_Player[i].dir = CHAR_DIR_LEFT;
					g_Player[i].state = WALK;
				}
#endif

				g_Player[i].running = FALSE;

				if (GetKeyboardPress(DIK_RIGHT) && g_Player[i].playAnim == FALSE)
				{
					CHANGE_PLAYER_POS_X(speed);
					g_Player[i].dir = CHAR_DIR_RIGHT;
					g_Player[i].state = WALK;
					if (GetKeyboardPress(DIK_LSHIFT))
					{
						CHANGE_PLAYER_POS_X(speed * 0.5f);
						g_Player[i].running = TRUE;
						g_Player[i].state = RUN;
					}

					std::cout << g_Player[i].pos.x << " ";
					std::cout << g_Player[i].pos.y << std::endl;
				}
				else if (GetKeyboardPress(DIK_LEFT) && g_Player[i].playAnim == FALSE)
				{
					CHANGE_PLAYER_POS_X(-speed);
					g_Player[i].dir = CHAR_DIR_LEFT;
					g_Player[i].state = WALK;
					if (GetKeyboardPress(DIK_LSHIFT))
					{
						CHANGE_PLAYER_POS_X(-speed * 0.5f);
						g_Player[i].running = TRUE;
						g_Player[i].state = RUN;
					}
					std::cout << g_Player[i].pos.x << " ";
					std::cout << g_Player[i].pos.y << std::endl;
				}

				if (GetKeyboardTrigger(DIK_C) && g_Player[i].dashCount >= DASH_CD_TIME)
				{
					// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
					if (g_Player[i].playAnim == FALSE)
					{
						g_Player[i].patternAnim = 0;
						g_Player[i].state = DASH;
						g_Player->animFrameCount = 0;
						g_Player[i].dashCount = 0;
						g_Player->playAnim = TRUE;
					}
					// �󒆂̏��
					else if (g_Player[i].jumpCnt > PLAYER_JUMP_CNT_MAX / 2)
					{
						g_Player[i].patternAnimOld = g_Player[i].patternAnim;
						g_Player[i].patternAnim = 0;
						g_Player[i].state = DASH;
						g_Player[i].dashOnAir = TRUE;
						g_Player[i].dashCount = 0;
						g_Player->animFrameCount = 0;
					}
					// ���̑��̏�Ԃ̏ꍇ�A�A�N�V�������L���[�ɒǉ�
					else
					{
						g_Player[i].actionQueue[g_Player[i].actionQueueEnd] = DASH;
						g_Player[i].actionQueueEnd = (g_Player[i].actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

						// �L���[�̃I�[�o�[�t���[��h�~
						if (g_Player[i].actionQueueEnd == g_Player[i].actionQueueStart)
						{
							g_Player[i].actionQueueStart = (g_Player[i].actionQueueStart + 1) % ACTION_QUEUE_SIZE; // �ł��Â��A�N�V������j��
						}
					}

				}
				else if (GetKeyboardTrigger(DIK_SPACE))
				{
					// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
					if (g_Player[i].playAnim == FALSE)
					{
						g_Player[i].patternAnim = 0;
						g_Player[i].state = JUMP;
						g_Player->animFrameCount = 0;
						g_Player->playAnim = TRUE;

						g_Player[i].state = JUMP;
						g_Player[i].jumpCnt = 0;
					}
					// ���̑��̏�Ԃ̏ꍇ�A�A�N�V�������L���[�ɒǉ�
					else
					{
						g_Player[i].actionQueue[g_Player[i].actionQueueEnd] = JUMP;
						g_Player[i].actionQueueEnd = (g_Player[i].actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

						// �L���[�̃I�[�o�[�t���[��h�~
						if (g_Player[i].actionQueueEnd == g_Player[i].actionQueueStart)
						{
							g_Player[i].actionQueueStart = (g_Player[i].actionQueueStart + 1) % ACTION_QUEUE_SIZE; // �ł��Â��A�N�V������j��
						}
					}
				}
				else if (GetKeyboardTrigger(DIK_X))
				{
					switch (g_Player[i].state)
					{
						// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
					case IDLE:
					case WALK:
					case RUN:
						g_Player[i].patternAnim = 0;
						g_Player[i].state = ATTACK;
						g_Player->animFrameCount = 0;
						g_Player->playAnim = TRUE;
						g_Player->attackPattern = NORMAL_ATTACK1;
						break;

					case DASH:
					case ATTACK:
					case JUMP:
						// �����A�C�h����ԂȂ�A�����ɃA�N�V���������s
						if (g_Player->playAnim == FALSE)
						{
							g_Player[i].patternAnim = 0;
							g_Player->animFrameCount = 0;
							g_Player->playAnim = TRUE;
							g_Player->attackPattern++;
						}
						// �󒆂̏��
						else if (g_Player[i].jumpCnt > PLAYER_JUMP_CNT_MAX / 2 && g_Player[i].state != ATTACK)
						{
							g_Player[i].patternAnimOld = g_Player[i].patternAnim;
							g_Player[i].patternAnim = 0;
							g_Player[i].state = ATTACK;
							g_Player->animFrameCount = 0;
						}
						// ���̑��̏�Ԃ̏ꍇ�A�A�N�V�������L���[�ɒǉ�
						else
						{
							g_Player[i].actionQueue[g_Player[i].actionQueueEnd] = ATTACK;
							g_Player[i].actionQueueEnd = (g_Player[i].actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

							// �L���[�̃I�[�o�[�t���[��h�~
							if (g_Player[i].actionQueueEnd == g_Player[i].actionQueueStart)
							{
								g_Player[i].actionQueueStart = (g_Player[i].actionQueueStart + 1) % ACTION_QUEUE_SIZE; // �ł��Â��A�N�V������j��
							}
						}
	
						break;
					default:
						break;
					}

				}

				// �Q�[���p�b�h�łňړ�����

				if (IsButtonPressed(0, BUTTON_RIGHT))
				{
					g_Player[i].pos.x += speed;
					g_Player[i].dir = CHAR_IDLE_RIGHT;
				}
				else if (IsButtonPressed(0, BUTTON_LEFT))
				{
					g_Player[i].pos.x -= speed;
					g_Player[i].dir = CHAR_IDLE_LEFT;
				}

				if (GetKeyboardPress(DIK_Z))
				{
					// �v���C���[�̈ʒu���X�V����O�ɐڒn���m�F
					AABB* grounds = GetMap01AABB();
					BOOL onGround = false;
					for (int j = 0; j < MAP01_GROUND_MAX; j++)
					{ 
						if (CheckGroundCollision(g_Player, grounds[j]))
						{
							onGround = true;
							break;
						} 
					}
					if (!onGround)
					{
						if (g_Player->onAirCnt < PLAYER_JUMP_CNT_MAX * 0.5f)
						{
							// cos�֐����g�p���ĉ������x���v�Z
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
					}
					
				}

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

				// �v���C���[�̗����ʒu����MAP�̃X�N���[�����W���v�Z����
				bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
				if (bg->pos.x < 0) bg->pos.x = 0;
				if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
				if (bg->pos.y < 0) bg->pos.y = 0;
				if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

				// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
				{
					ENEMY* enemy = GetEnemy();

					// �G�l�~�[�̐��������蔻����s��
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						// �����Ă�G�l�~�[�Ɠ����蔻�������
						if (enemy[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
								enemy[j].pos, enemy[j].w, enemy[j].h);
							// �������Ă���H
							if (ans == TRUE)
							{
								// �����������̏���
								enemy[j].use = FALSE;
								AddScore(10);
							}
						}
					}
				}



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

			{	// �e�\��
				SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����

				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[CHAR_SHADOW]);

				float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
				float py = g_Player[i].shadowY - bg->pos.y;	// �v���C���[�̕\���ʒuY
				float pw = TEXTURE_WIDTH;		// �v���C���[�̕\����
				float ph = TEXTURE_WIDTH /4;		// �v���C���[�̕\������
				py += 50.0f;		// �����ɕ\��

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

			//py += g_Player[i].jumpY;		// �W�����v���̍����𑫂�

			// �A�j���[�V�����p
			float tw = 1.0f / GetTexturePatternDivideX();	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Player[i].patternAnim % GetTexturePatternDivideX()) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Player[i].patternAnim / GetTexturePatternDivideX()) * th;	// �e�N�X�`���̍���Y���W

			//float tw = 1.0f;	// �e�N�X�`���̕�
			//float th = 1.0f;	// �e�N�X�`���̍���
			//float tx = 0.0f;	// �e�N�X�`���̍���X���W
			//float ty = 0.0f;	// �e�N�X�`���̍���Y���W

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
		float tx = (float)(g_Player[no].patternAnim % GetTexturePatternDivideX()) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Player[no].patternAnim / GetTexturePatternDivideX()) * th;	// �e�N�X�`���̍���Y���W


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
	g_Player->w = TEXTURE_WIDTH;
	g_Player->h = TEXTURE_HEIGHT;

	switch (g_Player->dir)
	{
	case CHAR_DIR_LEFT:
		g_Player->texNo = CHAR_RUN_LEFT;
		break;
	case CHAR_DIR_RIGHT:
		g_Player->texNo = CHAR_RUN_RIGHT;
		break;
	default:
		break;
	}
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
	g_Player->w = TEXTURE_WIDTH;
	g_Player->h = TEXTURE_HEIGHT;

	g_Player->countAnim += 1.0f;
	switch (g_Player->dir)
	{
	case CHAR_DIR_LEFT:
		g_Player->texNo = CHAR_IDLE_LEFT;
		break;
	case CHAR_DIR_RIGHT:
		g_Player->texNo = CHAR_IDLE_RIGHT;
		break;
	default:
		break;
	}

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
	g_Player->w = TEXTURE_WIDTH;
	g_Player->h = TEXTURE_HEIGHT;

	float speed = g_Player->move.x;
	switch (g_Player->dir)
	{
	case CHAR_DIR_LEFT:
		g_Player->texNo = CHAR_DASH_LEFT;
		//g_Player->pos.x -= speed * 2;
		CHANGE_PLAYER_POS_X(-speed * 2);
		break;
	case CHAR_DIR_RIGHT:
		g_Player->texNo = CHAR_DASH_RIGHT;
		//g_Player->pos.x += speed * 2;
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

		if (g_Player->jumpCnt > 0)	
			g_Player->state = JUMP;
		else
		{
			g_Player->state = IDLE;
			g_Player->playAnim = FALSE;
		}

	}
}

void PlayAttackAnim(void)
{
	//if (g_Player->attackPattern == ATTACK_PATTERN_MAX)
	//{
	//	g_Player->animFrameCount = 0;
	//	g_Player->state = IDLE;
	//	g_Player->playAnim = FALSE;
	//	return;
	//}

	if (g_Player->attackPattern == ATTACK_PATTERN_MAX ||
		g_Player->attackPattern == NONE)
		g_Player->attackPattern = NORMAL_ATTACK1;


	AdjustAttackTextureSize();

	std::cout << g_Player->w << std::endl;
	std::cout << g_Player->attackPattern << std::endl;

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
	g_Player->w = TEXTURE_WIDTH;
	g_Player->h = TEXTURE_HEIGHT;

	switch (g_Player->dir)
	{
	case CHAR_DIR_LEFT:
		g_Player->texNo = CHAR_WALK_LEFT;
		break;
	case CHAR_DIR_RIGHT:
		g_Player->texNo = CHAR_WALK_RIGHT;
		break;
	default:
		break;
	}
	g_Player->countAnim++;
	if (g_Player->countAnim > ANIM_WAIT)
	{
		g_Player->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		g_Player->patternAnim = (g_Player->patternAnim + 1) % GetTexturePatternDivideX();
	}
}

void PlayJumpAnim()
{
	if (GetKeyboardPress(DIK_RIGHT))
		CHANGE_PLAYER_POS_X(g_Player->move.x * 0.8);
	else if (GetKeyboardPress(DIK_LEFT))
		CHANGE_PLAYER_POS_X(-g_Player->move.x * 0.8);

	g_Player->w = TEXTURE_JUMP_WIDTH;
	g_Player->h = TEXTURE_JUMP_HEIGHT;

	g_Player->texNo = CHAR_JUMP;

	float angle = (XM_PI / PLAYER_JUMP_CNT_MAX) * g_Player->jumpCnt;
	g_Player->move.y = g_Player->jumpYMax * cosf(angle);
	CHANGE_PLAYER_POS_Y(-g_Player->move.y);

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
	if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX)
	{
		g_Player->dashOnAir = FALSE;
		g_Player->playAnim = FALSE;
		g_Player->state = IDLE;
		g_Player->jumpCnt = 0;
	}

}

void PlayFallAnim()
{

}

int GetTexturePatternDivideX()
{
	switch (g_Player->texNo)
	{
	case CHAR_IDLE_RIGHT:
	case CHAR_IDLE_LEFT:
		return TEXTURE_PATTERN_DIVIDE_X;
	case CHAR_WALK_RIGHT:
	case CHAR_WALK_LEFT:
		return TEXTURE_WALK_PATTERN_DIVIDE_X;
	case CHAR_RUN_RIGHT:
	case CHAR_RUN_LEFT:
		return TEXTURE_RUN_PATTERN_DIVIDE_X;
	case CHAR_DASH_RIGHT:
	case CHAR_DASH_LEFT:
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
		return 0;
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

BOOL CheckGroundCollision(PLAYER* g_Player, AABB ground)
{
	// �v���C���[��AABB�����擾
	XMFLOAT3 playerPos = g_Player->bodyAABB.pos;
	float playerW = g_Player->bodyAABB.w;
	float playerH = g_Player->bodyAABB.h;

	// �n�ʂ�AABB�����擾
	XMFLOAT3 groundPos = ground.pos;
	float groundW = ground.w;
	float groundH = ground.h;

	// �Փˊm�F
	BOOL isColliding = CollisionBB(playerPos, playerW, playerH, groundPos, groundW, groundH);

	std::cout << isColliding << std::endl;

	// �v���C���[���n�ʂ̏�łǂꂭ�炢�d�Ȃ��Ă��邩���v�Z
	float overlapLeft = max(playerPos.x - playerW / 2, groundPos.x - groundW / 2);
	float overlapRight = min(playerPos.x + playerW / 2, groundPos.x + groundW / 2);
	float overlapWidth = overlapRight - overlapLeft;

	// ������臒l
	float threshold = playerW / FALLING_THRESHOLD;

	// �v���C���[��y�����̑��x��0��菬�����i�v���C���[���������j�ꍇ�A���Փ˂��������A�d�Ȃ肪臒l�𒴂��Ă���ꍇ
	if (isColliding && g_Player->move.y > 0 && overlapWidth >= threshold) 
	{
		// �v���C���[���������ł��n�ʂɏ\���ڐG���Ă���A�������~��
		g_Player->move.y = 0.0f;

		// �v���C���[�̑؋󎞊Ԃ����Z�b�g����
		g_Player->onAirCnt = 0;

		// �v���C���[�̈ʒu��n�ʂ̏�ɒ������A�n�ʂ�ʂ蔲���Ȃ��悤�ɂ���
		SET_PLAYER_POS_Y(groundPos.y - groundH / 2 - playerH / 2);

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