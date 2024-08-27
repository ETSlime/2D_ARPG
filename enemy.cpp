//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �L�����T�C�Y
#define TEXTURE_WIDTH_CYCLOPS		(540/2)
#define TEXTURE_HEIGHT_CYCLOPS		(400/2)
#define TEXTURE_WIDTH_GARGOYLE		(280/2)
#define TEXTURE_HEIGHT_GARGOYLE		(210/2)
#define TEXTURE_WIDTH_GNOLL			(465/2)
#define TEXTURE_HEIGHT_GNOLL		(375/2)
#define TEXTURE_WIDTH_GOBLIN		(300/2)
#define TEXTURE_HEIGHT_GOBLIN		(250/2)
#define TEXTURE_WIDTH_GOLEM			(720/2)
#define TEXTURE_HEIGHT_GOLEM		(600/2)
#define TEXTURE_WIDTH_IMP			(300/2)
#define TEXTURE_HEIGHT_IMP			(250/2)
#define TEXTURE_WIDTH_MUMMY			(450/2)
#define TEXTURE_HEIGHT_MUMMY		(375/2)
#define TEXTURE_WIDTH_OGRE			(650/2)
#define TEXTURE_HEIGHT_OGRE			(470/2)
#define TEXTURE_WIDTH_SKELL			(390/2)
#define TEXTURE_HEIGHT_SKELL		(297/2)
#define TEXTURE_MAX					(20)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_IDLE_PATTERN_NUM		(1)
#define ANIM_WALK_PATTERN_NUM		(4)
#define ANIM_ATTACK_PATTERN_NUM		(3)
#define ANIM_ATTACK_OFFSET			(6)
#define ANIM_IDLE_OFFSET			(9)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_WALK				(20)
#define ANIM_WAIT_ATTACK			(15)
#define ANIM_WAIT_IDLE				(25)
#define IDLE_WAIT					(50)

#define ATTACK_COOLDOWN_TIME		(100)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

#ifdef _DEBUG	
static ID3D11Buffer* g_bodyAABBVertexBuffer = NULL;
static ID3D11Buffer* g_attackAABBVertexBuffer = NULL;
#endif

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/monster/M_cyclops.png",
	"data/TEXTURE/monster/M_gargoyle.png",
	"data/TEXTURE/monster/M_gnoll_sword.png",
	"data/TEXTURE/monster/M_goblin_swordman.png",
	"data/TEXTURE/monster/M_golem.png",
	"data/TEXTURE/monster/M_imp_sword_.png",
	"data/TEXTURE/monster/M_mummy.png",
	"data/TEXTURE/monster/M_ogre.png",
	"data/TEXTURE/monster/M_skell_warrior.png",
	"data/TEXTURE/bar_white.png",
};


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static ENEMY	g_Enemy[ENEMY_MAX];		// �G�l�~�[�\����

static int		g_EnemyCount = ENEMY_MAX;

static XMFLOAT3		g_EnemyInitPos[ENEMY_MAX] = {
	XMFLOAT3(787.0f,  1314.0f, 0.0f),/*
	XMFLOAT3(1087.0f,  1314.0f, 0.0f),
	XMFLOAT3(1462.0f,  1314.0f, 0.0f),
	XMFLOAT3(1837.0f,  1314.0f, 0.0f),
	XMFLOAT3(2212.0f,  1314.0f, 0.0f),
	XMFLOAT3(2587.0f,  1314.0f, 0.0f),
	XMFLOAT3(2962.0f,  1314.0f, 0.0f),
	XMFLOAT3(3187.0f,  1314.0f, 0.0f),
	XMFLOAT3(3487.0f,  1314.0f, 0.0f),*/
};

static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W									��]��							�g�嗦					����
	{ XMFLOAT3(1800.0f, 1314.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	300 },
	{ XMFLOAT3(1450.0f,  1314.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	300 },
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(1700.0f,   0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(1700.0f,  SCREEN_HEIGHT, 0.0f),XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	60 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(3000.0f, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(3000 + SCREEN_WIDTH, 100.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 6.28f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,

};

static AttackAABBTBL cyclopsAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(95.0f, 0.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-95.0f, 10.0f, 0.0f), 50.0f, 140.0f},
	{XMFLOAT3(0.0f, -80.0f, 0.0f), 160.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-95.0f, 10.0f, 0.0f), 50.0f, 140.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL gargoyleAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 20.0f, 0.0f), 20.0f, 60.0f},
	{XMFLOAT3(0.0f, 30.0f, 0.0f), 80.0f, 30.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL gnollAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(75.0f, 0.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, -80.0f, 0.0f), 160.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 50.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL goblinAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(55.0f, 0.0f, 0.0f), 45.0f, 85.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-55.0f, 10.0f, 0.0f), 45.0f, 85.0f},
	{XMFLOAT3(0.0f, -40.0f, 0.0f), 130.0f, 45.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-55.0f, 10.0f, 0.0f), 45.0f, 85.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL golemAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(105.0f, -100.0f, 0.0f), 80.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-105.0f, 10.0f, 0.0f), 50.0f, 250.0f},
	{XMFLOAT3(0.0f, -120.0f, 0.0f), 160.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-105.0f, 10.0f, 0.0f), 50.0f, 250.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL impAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-35.0f, 10.0f, 0.0f), 30.0f, 90.0f},
	{XMFLOAT3(-20.0f, -40.0f, 0.0f), 60.0f, 30.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL mummyAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(0.0f, -65.0f, 0.0f), 120.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-85.0f, 10.0f, 0.0f), 40.0f, 140.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL ogreAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(115.0f, 0.0f, 0.0f), 80.0f, 180.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-115.0f, 10.0f, 0.0f), 80.0f, 180.0f},
	{XMFLOAT3(-10.0f, -80.0f, 0.0f), 250.0f, 70.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-115.0f, 10.0f, 0.0f), 80.0f, 180.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL skellAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(65.0f, 0.0f, 0.0f), 50.0f, 110.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-65.0f, 10.0f, 0.0f), 50.0f, 110.0f},
	{XMFLOAT3(0.0f, -60.0f, 0.0f), 160.0f, 40.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-65.0f, 10.0f, 0.0f), 50.0f, 110.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
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


	// �G�l�~�[�\���̂̏�����
	g_EnemyCount = 0;
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_EnemyCount++;
		g_Enemy[i].use = TRUE;
		g_Enemy[i].pos = g_EnemyInitPos[i];
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].move = XMFLOAT3(2.0f, 0.0f, 0.0f);		// �ړ���
		g_Enemy[i].idleCount = 0;
		g_Enemy[i].state = ENEMY_IDLE;
		g_Enemy[i].dir = CHAR_DIR_LEFT;
		g_Enemy[i].attackCooldown = 0;
		g_Enemy[i].returnPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].canFly = FALSE;
		g_Enemy[i].stepBack = FALSE;

		g_Enemy[i].damage = 16;

		g_Enemy[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Enemy[i].enemyType = GOLEM;
		g_Enemy[i].w = GetCurrentTextureSizeW(g_Enemy[i].enemyType);
		g_Enemy[i].h = GetCurrentTextureSizeH(g_Enemy[i].enemyType);

		// AABB
		g_Enemy[i].bodyAABB.pos = g_Enemy[i].pos;
		g_Enemy[i].bodyAABB.w = g_Enemy[i].w * 0.5f;
		g_Enemy[i].bodyAABB.h = g_Enemy[i].h * 0.7f;
		g_Enemy[i].bodyAABB.tag = ENEMY_BODY_AABB;

		for (int j = 0; j < MAX_ATTACK_AABB; j++)
		{
			g_Enemy[i].attackAABB[j].pos = g_Enemy[i].pos;
			g_Enemy[i].attackAABB[j].w = 0;
			g_Enemy[i].attackAABB[j].h = 0;
			g_Enemy[i].attackAABB[j].tag = ENEMY_ATTACK_AABB;
		}
	}

	//// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Enemy[0].pos = g_MoveTbl0[0].pos;

	//// 1�Ԃ������`��Ԃœ������Ă݂�
	//g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//// 2�Ԃ������`��Ԃœ������Ă݂�
	//g_Enemy[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

#ifdef _DEBUG	
	{
		int aabbCount = ENEMY_MAX;
		int maxVertices = ENEMY_MAX * 4;
		int maxAttackVertices = maxVertices * MAX_ATTACK_AABB;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * maxVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_bodyAABBVertexBuffer);

		bd.ByteWidth = sizeof(VERTEX_3D) * maxAttackVertices;
		GetDevice()->CreateBuffer(&bd, NULL, &g_attackAABBVertexBuffer);
	}
#endif

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{
	if (g_Load == FALSE) return;
	g_EnemyCount = 0;			// �����Ă�G�l�~�[�̐�

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// �����Ă�G�l�~�[��������������
		if (g_Enemy[i].use == TRUE)
		{
			g_EnemyCount++;		// �����Ă��G�̐�
			
			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			XMFLOAT3 pos_old = g_Enemy[i].pos;

			// �A�j���[�V����
			switch (g_Enemy[i].state)
			{
			case ENEMY_IDLE:
			case ENEMY_COOLDOWN:
				PlayEnemyIdleAnim(&g_Enemy[i]);
				break;
			case ENEMY_WALK:
			case ENEMY_CHASE:
			case ENEMY_WALK_BACK:
			case ENEMY_RETREAT:
				PlayEnemyWalkAnim(&g_Enemy[i]);
				break;
			case ENEMY_ATTACK:
				PlayEnemyAttackAnim(&g_Enemy[i]);
				break;
			default:
				break;
			}

			if (g_Enemy[i].state != ENEMY_ATTACK && g_Enemy[i].finishAttack == FALSE)
			{
				for (int j = 0; j < MAX_ATTACK_AABB; j++)
				{
					g_Enemy[i].attackAABB[j].w = 0;
					g_Enemy[i].attackAABB[j].h = 0;
				}
				g_Enemy[i].finishAttack = TRUE;
			}
			

			// �ړ�����
			if (g_Enemy[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	
				PLAYER* player = GetPlayer();
				// ���݂̍��W��ۑ����Ă������߂̕ϐ�
				float oldPosX = g_Enemy[i].pos.x;

				// �G�ƃv���C���[�̋������v�Z����
				XMVECTOR enemyPos = XMLoadFloat3(&g_Enemy[i].pos);
				XMVECTOR playerPos = XMLoadFloat3(&player->pos);
				XMVECTOR direction = XMVector3Normalize(playerPos - enemyPos);
				XMVECTOR returnPos = XMLoadFloat3(&g_Enemy[i].returnPos);
				if (g_Enemy[i].canFly == FALSE)
					direction = XMVectorSetY(direction, 0);  // Y�����̑��x��0�ɐݒ�
				float distanceToPlayer = XMVectorGetX(XMVector3Length(playerPos - enemyPos));
				float distanceToReturnPos = XMVectorGetX(XMVector3Length(returnPos - enemyPos));

				switch (g_Enemy[i].state)
				{
				case ENEMY_CHASE:
				{

					//ChasePlayer(&g_Enemy[i]);

					if (distanceToPlayer < ENEMY_ATTACK_RADIUS)
					{
						g_Enemy[i].state = ENEMY_ATTACK;
					}
					// �ő�ǐՋ����𒴂����ꍇ�ARETREAT��ԂɈڍs
					else if (distanceToReturnPos > MAX_CHASE_DISTANCE) 
					{
						g_Enemy[i].state = ENEMY_RETREAT;  // �|�ޏ�ԂɈڍs
					}
					else if (distanceToPlayer > ENEMY_VISION_RADIUS)
					{
						// �v���C���[������͈͊O�ɏo���ꍇ�A���̈ʒu�ɖ߂邽��WALK_BACK��ԂɈڍs
						g_Enemy[i].state = ENEMY_WALK_BACK;
					}
					else
					{
						// �ǐՂ��s��
						g_Enemy[i].stepBack = FALSE;
						float speed = g_Enemy[i].move.x;
						XMVECTOR moveStep = direction * speed;
						enemyPos += moveStep;
						XMStoreFloat3(&g_Enemy[i].pos, enemyPos);
						XMStoreFloat3(&g_Enemy[i].bodyAABB.pos, enemyPos);
					}
					break;
				}

				case ENEMY_RETREAT:
				{
					

					// �v���C���[������͈͊O�ɏo���ꍇ�AWALK_BACK��ԂɑJ��
					if (distanceToPlayer > ENEMY_VISION_RADIUS) 
					{
						g_Enemy[i].state = ENEMY_WALK_BACK;  // �v���C���[������͈͊O�Ȃ猳�̃p�X�ɖ߂�
					}
					// �v���C���[���ő�ǐՋ������ɖ߂����ꍇ�ACHASE��ԂɑJ��
					else if (CheckChasingPlayer(&g_Enemy[i]))
					{
						g_Enemy[i].state = ENEMY_CHASE;  // �ő�ǐՋ������Ƀv���C���[������ꍇ�A�ĂђǐՂ���
					}
					else
					{
						// �|�ޕ����i�v���C���[�Ƃ͋t�j
						XMVECTOR direction = XMVector3Normalize(enemyPos - playerPos);
						if (g_Enemy[i].canFly == FALSE)
							direction = XMVectorSetY(direction, 0);  // Y�����̑��x��0�ɐݒ�
						XMVECTOR moveStep = direction * g_Enemy[i].move.x * RETREAT_SPEED_RATE;
						enemyPos += moveStep;

						XMStoreFloat3(&g_Enemy[i].pos, enemyPos);
						XMStoreFloat3(&g_Enemy[i].bodyAABB.pos, enemyPos);
						// �|�ޒ��ł������̓v���C���[�Ɍ�����
						g_Enemy[i].stepBack = TRUE;

						// MIN_RETURN_DISTANCE�܂œ|�ނ�����AWALK_BACK��Ԃɖ߂�
						if (distanceToReturnPos <= MIN_RETURN_DISTANCE)
						{
							g_Enemy[i].state = ENEMY_WALK_BACK;  
						}
					}

					break;
				}
				case ENEMY_COOLDOWN:
				{
					if (distanceToPlayer > ENEMY_VISION_RADIUS) 
					{
						// �v���C���[������͈͊O�ɏo���ꍇ�A���̈ʒu�ɖ߂邽��WALK_BACK��ԂɈڍs
						g_Enemy[i].state = ENEMY_WALK_BACK;
					}
					else if (distanceToReturnPos > MAX_CHASE_DISTANCE)	// �ő�ǐՋ����𒴂����ꍇ
					{
						g_Enemy[i].state = ENEMY_RETREAT;  // �|�ޏ�ԂɈڍs
					}
					else if (distanceToPlayer > ENEMY_ATTACK_RADIUS) 
					{
						// �U���͈͊O�Ȃ�ǐՂ𑱂���
						g_Enemy[i].stepBack = FALSE;
						float speed = g_Enemy[i].move.x;  // �ǐՑ��x

						XMVECTOR moveStep = direction * speed * 0.5f;
						enemyPos += moveStep;

						XMStoreFloat3(&g_Enemy[i].pos, enemyPos);
						XMStoreFloat3(&g_Enemy[i].bodyAABB.pos, enemyPos);
					}
					
					// �U���͈͓��ɂ���ꍇ�͒�~���A�N�[���_�E����҂�
					// �N�[���_�E�����W�b�N
					g_Enemy[i].attackCooldown--;
					if (g_Enemy[i].attackCooldown <= 0)
					{
						// �N�[���_�E���I����A�ĂѓG�̍s��������
						if (distanceToPlayer < ENEMY_ATTACK_RADIUS)
						{
							g_Enemy[i].state = ENEMY_ATTACK;  // �v���C���[���U���͈͓��ɂ���ꍇ
						}
						else if (CheckChasingPlayer(&g_Enemy[i]))
						{
							g_Enemy[i].state = ENEMY_CHASE;   // �v���C���[��������ɂ���ꍇ
						}
						else
						{
							g_Enemy[i].state = ENEMY_IDLE;    // ����O�ɂ���ꍇ�͑ҋ@���
						}
					}
					break;
				}

				case ENEMY_IDLE:
				case ENEMY_WALK:
				{
					// �v���C���[��������ɂ��āA�G���v���C���[�������Ă���ꍇ��CHASE��ԂɑJ��
					if (distanceToPlayer < ENEMY_VISION_RADIUS && CheckChasingPlayer(&g_Enemy[i]))
					{
						g_Enemy[i].state = ENEMY_CHASE;
						g_Enemy[i].returnPos = g_Enemy[i].pos;  // �s���p�X���痣���O�̈ʒu���L�^
					}
					else
					{
						g_Enemy[i].stepBack = FALSE;

						// ���`��Ԃ̏���
						int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
						int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
						int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

						if (g_Enemy[i].state == ENEMY_IDLE)
						{
							g_Enemy[i].idleCount++;
							if (g_Enemy[i].idleCount == IDLE_WAIT)
							{
								g_Enemy[i].idleCount = 0;
								g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
								g_Enemy[i].state = ENEMY_WALK;
							}
						}
						else
						{
							XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
							XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
							XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

							XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
							XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
							XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

							float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

							Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
							Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
							Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���



							// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
							XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);
							XMStoreFloat3(&g_Enemy[i].bodyAABB.pos, nowPos + Pos);

							// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
							XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

							// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
							XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);
							g_Enemy[i].w = GetCurrentTextureSizeW(g_Enemy[i].enemyType) * g_Enemy[i].scl.x;
							g_Enemy[i].h = GetCurrentTextureSizeH(g_Enemy[i].enemyType) * g_Enemy[i].scl.y;

							// frame���g�Ď��Ԍo�ߏ���������
							g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
							if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
							{
								g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
							}

							if (fabs(g_Enemy[i].time - nowNo) < EPSILON && g_Enemy[i].state == ENEMY_WALK)
							{
								g_Enemy[i].state = ENEMY_IDLE;
							}
						}
					}
					break;
				}

				case ENEMY_WALK_BACK:
				{
					
					// �v���C���[��������ɂ��邩���`�F�b�N
					if (distanceToPlayer < ENEMY_VISION_RADIUS && CheckChasingPlayer(&g_Enemy[i]))
					{
						// �v���C���[���Ăю�����ɓ������ꍇ�ACHASE��ԂɑJ��
						g_Enemy[i].state = ENEMY_CHASE;
					}
					else
					{
						// ���̍s���p�X�ɖ߂邽�߂ɁA�L�^�����ʒu�Ɉړ�����
						g_Enemy[i].stepBack = FALSE;
						XMVECTOR enemyPos = XMLoadFloat3(&g_Enemy[i].pos);
						XMVECTOR returnPos = XMLoadFloat3(&g_Enemy[i].returnPos);
						XMVECTOR direction = XMVector3Normalize(returnPos - enemyPos);
						if (g_Enemy[i].canFly == FALSE)
							direction = XMVectorSetY(direction, 0);  // Y�����̑��x��0�ɐݒ�
						float speed = g_Enemy[i].move.x;  // �����Ė߂鑬�x
						XMVECTOR moveStep = direction * speed;
						enemyPos += moveStep;

						XMStoreFloat3(&g_Enemy[i].pos, enemyPos);
						XMStoreFloat3(&g_Enemy[i].bodyAABB.pos, enemyPos);

						XMVECTOR delta = returnPos - enemyPos;
						float deltaX = fabs(XMVectorGetX(delta));
						float deltaY = fabs(XMVectorGetY(delta));

						// ���̈ʒu�ɋ߂Â��������m�F����
						if (deltaX <= g_Enemy[i].move.x && deltaY <= g_Enemy[i].move.y)
						{
							XMStoreFloat3(&g_Enemy[i].pos, returnPos);
							XMStoreFloat3(&g_Enemy[i].bodyAABB.pos, returnPos);
							// ���̈ʒu�ɖ߂�����A���̍s���p�X�ɖ߂�
							g_Enemy[i].state = ENEMY_WALK;  // �ړ��p�X�ɖ߂�
						}
					}
					break;
				}

				default:
					break;
				}



				// �G���E�Ɉړ����Ă��邩�A���Ɉړ����Ă��邩�𔻒f	
				if (oldPosX < g_Enemy[i].pos.x)
					g_Enemy[i].dir = g_Enemy[i].stepBack == TRUE ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
				else if (oldPosX > g_Enemy[i].pos.x)
					g_Enemy[i].dir = g_Enemy[i].stepBack == TRUE ? CHAR_DIR_RIGHT : CHAR_DIR_LEFT;
			}

			// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
			//{
			//	PLAYER* player = GetPlayer();

			//	// �G�l�~�[�̐��������蔻����s��
			//	for (int j = 0; j < ENEMY_MAX; j++)
			//	{
			//		// �����Ă�G�l�~�[�Ɠ����蔻�������
			//		if (player[j].use == TRUE)
			//		{
			//			BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
			//				player[j].pos, player[j].w, player[j].h);
			//			// �������Ă���H
			//			if (ans == TRUE)
			//			{
			//				// �����������̏���
			//				player[j].use = FALSE;	// �f�o�b�O�ňꎞ�I�ɖ��G�ɂ��Ă�����
			//			}
			//		}
			//	}
			//}
		}
	}


	//// �G�l�~�[�S�Ń`�F�b�N
	//if (g_EnemyCount <= 0)
	//{
 //		SetFade(FADE_OUT, MODE_RESULT);
	//}

#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
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

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].enemyType]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Enemy[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_Enemy[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_Enemy[i].w;		// �G�l�~�[�̕\����
			float ph = g_Enemy[i].h;		// �G�l�~�[�̕\������

			// �A�j���[�V�����p
			float tw, th, tx, ty;
			tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X; // �e�N�X�`���̕�
			th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			ty = (float)(g_Enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W
			if (g_Enemy[i].invertTex)
			{
				tw *= -1.0f;
				tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * (-1 * tw) - tw;	// �e�N�X�`���̍���X���W
			}
			else
				tx = (float)(g_Enemy[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Enemy[i].rot.z);

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
		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_bodyAABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < ENEMY_MAX; ++i)
		{
			int vertexOffset = i * 4;

			SetSpriteColorRotation(g_bodyAABBVertexBuffer, g_Enemy[i].bodyAABB.pos.x - bg->pos.x,
				g_Enemy[i].bodyAABB.pos.y - bg->pos.y, g_Enemy[i].bodyAABB.w, g_Enemy[i].bodyAABB.h,
				0.0f, 0.0f, 0.0f, 0.0f,
				XMFLOAT4(0.0f, 0.0f, 1.0f, 0.2f),
				0.0f,
				vertexOffset);

			GetDeviceContext()->Draw(4, vertexOffset);
		}

		GetDeviceContext()->IASetVertexBuffers(0, 1, &g_attackAABBVertexBuffer, &stride, &offset);
		for (int i = 0; i < ENEMY_MAX; ++i)
		{
			int vertexOffset = i * 4;

			for (int j = 0; j < MAX_ATTACK_AABB; j++)
			{
				SetSpriteColorRotation(g_attackAABBVertexBuffer, g_Enemy[i].attackAABB[j].pos.x - bg->pos.x,
					g_Enemy[i].attackAABB[j].pos.y - bg->pos.y, g_Enemy[i].attackAABB[j].w, g_Enemy[i].attackAABB[j].h,
					0.0f, 0.0f, 0.0f, 0.0f,
					XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f),
					0.0f,
					vertexOffset);

				GetDeviceContext()->Draw(4, vertexOffset);
			}

		}
	}
#endif

	// �Q�[�W�̃e�X�g
	{
		// ���~���̃Q�[�W�i�g�I�ȕ��j
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = 600.0f;		// �Q�[�W�̕\���ʒuX
		float py =  10.0f;		// �Q�[�W�̕\���ʒuY
		float pw = 300.0f;		// �Q�[�W�̕\����
		float ph =  30.0f;		// �Q�[�W�̕\������

		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;	// �e�N�X�`���̍���
		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


		// �G�l�~�[�̐��ɏ]���ăQ�[�W�̒�����\�����Ă݂�
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		pw = pw * ((float)g_EnemyCount / ENEMY_MAX);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


	}




}


//=============================================================================
// Enemy�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}


// �����Ă�G�l�~�[�̐�
int GetEnemyCount(void)
{
	return g_EnemyCount;
}


void PlayEnemyWalkAnim(ENEMY* enemy)
{
	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	int animWait = g_Enemy->stepBack == TRUE ? ANIM_WAIT_WALK * 2 : ANIM_WAIT_WALK;
	enemy->countAnim++;
	if (enemy->countAnim > animWait)
	{
		enemy->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		if (g_Enemy->state == ENEMY_RETREAT)
		{
			// ��ޏ�Ԃł̓A�j���[�V�������t���ɍĐ�����
			if (enemy->patternAnim == 0) 
			{
				enemy->patternAnim = ANIM_WALK_PATTERN_NUM - 1;  // �ŏ��̃t���[���ł���΍Ō�̃t���[���ɖ߂�
			}
			else 
			{
				enemy->patternAnim--;  // �t���[�����t�����ɐi�߂�
			}
		}
		else
		{
			enemy->patternAnim = (enemy->patternAnim + 1) % ANIM_WALK_PATTERN_NUM;
		}

	}
}

void PlayEnemyAttackAnim(ENEMY* enemy)
{
	PLAYER* player = GetPlayer();
	// �v���C���[�̈ʒu�ƓG�̈ʒu���r���āA�����𒲐�
	if (player->pos.x > enemy->pos.x) 
	{
		// �v���C���[���E���ɂ���ꍇ
		enemy->dir = CHAR_DIR_RIGHT;
	}
	else {
		// �v���C���[�������ɂ���ꍇ
		enemy->dir = CHAR_DIR_LEFT;
	}

	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;


	enemy->countAnim++;
	if (enemy->countAnim > ANIM_WAIT_ATTACK)
	{
		enemy->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		if (enemy->patternAnim == 0)
			enemy->patternAnim = ANIM_ATTACK_OFFSET + ANIM_ATTACK_PATTERN_NUM - 1;
		enemy->patternAnim = (enemy->patternAnim + 1) % ANIM_ATTACK_PATTERN_NUM + ANIM_ATTACK_OFFSET;
	}

	// �U������̍X�V
	UpdateEnemyAttackAABB(enemy);

	// �A�j���[�V�������I�����������`�F�b�N
	if (enemy->patternAnim == ANIM_ATTACK_PATTERN_NUM + ANIM_ATTACK_OFFSET - 1 && enemy->countAnim == ANIM_WAIT_ATTACK)
	{
		// �A�j���[�V�����I����ɍU�������s
		//PlayerTakeDamage();  // �v���C���[���_���[�W���󂯂鏈��
		enemy->attackCooldown = ATTACK_COOLDOWN_TIME;  // �N�[���_�E���^�C�������Z�b�g
		enemy->state = ENEMY_COOLDOWN;  // �N�[���_�E����ԂɈڍs

		for (int i = 0; i < MAX_ATTACK_AABB; i++)
		{
			enemy->attackAABB[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			enemy->attackAABB[i].h = 0;
			enemy->attackAABB[i].w = 0;
		}
	}
}

void PlayEnemyIdleAnim(ENEMY* enemy)
{
	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	enemy->countAnim++;
	if (enemy->countAnim > ANIM_WAIT_IDLE)
	{
		enemy->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		enemy->patternAnim = (enemy->patternAnim == 0) ? 2 : 0;
	}
}

int GetCurrentTextureSizeW(int enemyType)
{
	switch (enemyType)
	{
	case CYCLOPS:
		return TEXTURE_WIDTH_CYCLOPS;
	case GARGOYLE:
		return TEXTURE_WIDTH_GARGOYLE;
	case GNOLL:
		return TEXTURE_WIDTH_GNOLL;
	case GOBLIN:
		return TEXTURE_WIDTH_GOBLIN;
	case GOLEM:
		return TEXTURE_WIDTH_GOLEM;
	case IMP:
		return TEXTURE_WIDTH_IMP;
	case MUMMY:
		return TEXTURE_WIDTH_MUMMY;
	case OGRE:
		return TEXTURE_WIDTH_OGRE;
	case SKELL:
		return TEXTURE_WIDTH_SKELL;
	default:
		return -1;
	}
}
int GetCurrentTextureSizeH(int enemyType)
{
	switch (enemyType)
	{
	case CYCLOPS:
		return TEXTURE_HEIGHT_CYCLOPS;
	case GARGOYLE:
		return TEXTURE_HEIGHT_GARGOYLE;
	case GNOLL:
		return TEXTURE_HEIGHT_GNOLL;
	case GOBLIN:
		return TEXTURE_HEIGHT_GOBLIN;
	case GOLEM:
		return TEXTURE_HEIGHT_GOLEM;
	case IMP:
		return TEXTURE_HEIGHT_IMP;
	case MUMMY:
		return TEXTURE_HEIGHT_MUMMY;
	case OGRE:
		return TEXTURE_HEIGHT_OGRE;
	case SKELL:
		return TEXTURE_HEIGHT_SKELL;
	default:
		return -1;
	}
}

void UpdateEnemyAttackAABB(ENEMY* enemy)
{
	AttackAABBTBL* attackTable = nullptr;

	switch (enemy->enemyType)
	{
	case CYCLOPS:
		attackTable = cyclopsAttackTbl;
		break;
	case GARGOYLE:
		attackTable = gargoyleAttackTbl;
		break;
	case GNOLL:
		attackTable = gnollAttackTbl;
		break;
	case GOBLIN:
		attackTable = goblinAttackTbl;
		break;
	case GOLEM:
		attackTable = golemAttackTbl;
		break;
	case IMP:
		attackTable = impAttackTbl;
		break;
	case MUMMY:
		attackTable = mummyAttackTbl;
		break;
	case OGRE:
		attackTable = ogreAttackTbl;
		break;
	case SKELL:
		attackTable = skellAttackTbl;
		break;
	default:
		return;
	}

	for (int i = 0; i < MAX_ATTACK_AABB; i++)
	{
		int tableIdx = (enemy->patternAnim - ANIM_ATTACK_OFFSET) * MAX_ATTACK_AABB + i;

		// �G�̌����𔻒肷��: �������̏ꍇ��dir��1�A�E�����̏ꍇ��dir��-1
		int dir = enemy->dir == CHAR_DIR_LEFT ? 1 : -1;

		// �����ɉ�����X�����̃I�t�Z�b�g�𒲐�
		XMFLOAT3 offset = attackTable[tableIdx].posOffset;
		offset.x *= dir;

		// attackAABB�̈ʒu���X�V
		enemy->attackAABB[i].pos = XMFLOAT3_ADD(enemy->bodyAABB.pos, offset);
		enemy->attackAABB[i].h = attackTable[tableIdx].h;
		enemy->attackAABB[i].w = attackTable[tableIdx].w;
	}
}

BOOL CheckChasingPlayer(const ENEMY* enemy)
{
	PLAYER* player = GetPlayer();

	if (enemy->state == ENEMY_RETREAT || enemy->state == ENEMY_CHASE || enemy->state == ENEMY_WALK_BACK)
	{
		XMVECTOR returnPos = XMLoadFloat3(&enemy->returnPos);
		XMVECTOR playerPos = XMLoadFloat3(&player->pos);
		float distancePlayerToReturnPos = XMVectorGetX(XMVector3Length(returnPos - playerPos));
		if (enemy->state == ENEMY_RETREAT)
		{
			std::cout << distancePlayerToReturnPos << std::endl;
		}
		if (distancePlayerToReturnPos > MAX_CHASE_DISTANCE)
			return FALSE;
	}

	if (player->state == RUN || player->state == DASH)
		return TRUE;

	BOOL isChasingPlayer = FALSE;
	if (enemy->dir == CHAR_DIR_RIGHT && player->pos.x > enemy->pos.x)
	{
		// �G���E�������Ă��āA�v���C���[���E���ɂ���ꍇ
		isChasingPlayer = true;
	}
	else if (enemy->dir == CHAR_DIR_LEFT && player->pos.x < enemy->pos.x)
	{
		// �G�����������Ă��āA�v���C���[�������ɂ���ꍇ
		isChasingPlayer = true;
	}
	return isChasingPlayer;
}