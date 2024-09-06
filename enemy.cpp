//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "map.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "score.h"
#include "magic.h"
#include "tutorial.h"
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
#define TEXTURE_MAX					(15)		// �e�N�X�`���̐�

#define	ENEMY_HP_GAUGE_TEXTURE		(9)
#define ENEMY_SHADOW_TEXTURE		(11)
#define LIGHTPOINT_TEXTURE			(12)

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_IDLE_PATTERN_NUM		(1)
#define ANIM_WALK_PATTERN_NUM		(4)
#define ANIM_ATTACK_PATTERN_NUM		(3)
#define ANIM_ATTACK_OFFSET			(6)
#define ANIM_DIE_OFFSET				(4)
#define	ANIM_HIT_OFFSET				(9)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

// �A�j���[�V�����̐؂�ւ��Wait�l
#define ANIM_WAIT_WALK				(20)
#define ANIM_WAIT_ATTACK			(15)
#define ANIM_WAIT_IDLE				(25)
#define ANIM_WAIT_DIE				(150)
#define IDLE_WAIT					(50)


// lightpoint
#define TEXTURE_WIDTH_LIGHTPOINT	(150.0f)
#define TEXTURE_HEIGHT_LIGHTPOINT	(150.0f)
#define LIGHPOINT_SPEED_MIN			(2)
#define LIGHTPOINT_SPEED_MAX		(5)
#define LIGHTPOINT_FLY_TIME			(25)
#define LIGHTPOINT_STAY_TIME		(45)
#define LIGHTPOINT_REACH_PLAYER_TIME (80.0f)
#define LIGHTPOINT_TARGET_OFFSET_X	(-70.0f)
#define LIGHTPOINT_TARGET_OFFSET_Y	(-70.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void EnemyTakeDamage(ENEMY* enemy, Magic* magic = nullptr);

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
	"data/TEXTURE/monster/EnemyHPGauge.png",
	"data/TEXTURE/monster/EnemyHPGauge_bg.png",
	"data/TEXTURE/char/shadow000.jpg",
	"data/TEXTURE/lightpoint.png",
};


static BOOL			g_Load = FALSE;			// ���������s�������̃t���O
static ENEMY		g_Enemy[ENEMY_MAX];		// �G�l�~�[�\����
static LightPoint 	g_LightPoint[LIGHTPOINT_MAX];

static int		g_EnemyCount = ENEMY_MAX;
static int		g_LightPointInterval = 10;
static BOOL		g_Update = TRUE;

static EnemyAttributes cyclopsAttributes = {
	100.0f,	// float hp
	100.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes gargoyleAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes gnollAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes goblinAttributes = {
	130.0f,	// float hp
	130.0f,	// float maxhp
	5,			// int damage
	5,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	75.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes golemAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes impAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes mummyAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes ogreAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes skellAttributes = {
	1000.0f,	// float hp
	1000.0f,	// float maxhp
	15,			// int damage
	25,			// int staggerResistance
	80,			// int staggerRecoveryTime
	35,			// int stunTime
	100.0f,		// float attackRange
	100,		// int attackCooldown
	FALSE,		// BOOL canFly
	XMFLOAT3(2.0f, 0.0f, 0.0f) // XMFLOAT3 move
};

static EnemyAttributes* attributesTbl[] =
{
	&cyclopsAttributes,
	&gargoyleAttributes,
	&gnollAttributes,
	&goblinAttributes,
	&golemAttributes,
	&impAttributes,
	&mummyAttributes,
	&ogreAttributes,
	&skellAttributes
};


static INTERPOLATION_DATA* g_MoveTblAdr[MAP_ENEMY_MAX];

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
	{XMFLOAT3(55.0f, 0.0f, 0.0f), 31.0f, 85.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-55.0f, 10.0f, 0.0f), 31.0f, 85.0f},
	{XMFLOAT3(0.0f, -40.0f, 0.0f), 130.0f, 45.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-55.0f, 10.0f, 0.0f), 31.0f, 85.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
};

static AttackAABBTBL golemAttackTbl[MAX_ATTACK_AABB * ANIM_ATTACK_PATTERN_NUM] = {
	{XMFLOAT3(105.0f, -100.0f, 0.0f), 80.0f, 130.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 125.0f, 250.0f},
	{XMFLOAT3(0.0f, -120.0f, 0.0f), 160.0f, 50.0f},
	{XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f},

	{XMFLOAT3(-75.0f, 10.0f, 0.0f), 125.0f, 250.0f},
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

	g_Update = TRUE;

	EnemyConfig* enemyConfig = GetEnemyConfig(GetCurrentMap());
	// �G�l�~�[�\���̂̏�����
	g_EnemyCount = 0;
	for (int i = 0; i < MAP_ENEMY_MAX; i++)
	{
		if (enemyConfig[i].moveTbl == nullptr) break;

		g_MoveTblAdr[i] = enemyConfig[i].moveTbl;

		g_Enemy[i].enemyType = enemyConfig[i].enemyType;
		g_Enemy[i].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = i;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy[i].tblMax = enemyConfig[i].moveTblSize;	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
		g_Enemy[i].pos = enemyConfig[i].moveTbl[0].pos;
		g_Enemy[i].returnPos = g_Enemy[i].pos;

		SetupEnemyAttributes(&g_Enemy[i]);

		g_EnemyCount++;
		g_Enemy[i].use = TRUE;
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		g_Enemy[i].idleCount = 0;
		g_Enemy[i].state = ENEMY_IDLE;
		g_Enemy[i].dir = CHAR_DIR_RIGHT;
		g_Enemy[i].oldDir = CHAR_DIR_RIGHT;
		g_Enemy[i].stepBack = FALSE;
		g_Enemy[i].onAirCnt = 0;
		g_Enemy[i].isFalling = FALSE;

		// battle
		g_Enemy[i].isHit = FALSE;
		g_Enemy[i].hitTimer = 0.0f;
		g_Enemy[i].hitCD = 0.0f;
		g_Enemy[i].diePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].dieInitSpeedX = 0.0f;
		g_Enemy[i].dieInitSpeedY = 0.0f;

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

	for (int i = 0; i < LIGHTPOINT_MAX; i++)
	{
		g_LightPoint[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_LightPoint[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_LightPoint[i].w = TEXTURE_WIDTH_LIGHTPOINT;
		g_LightPoint[i].h = TEXTURE_HEIGHT_LIGHTPOINT;
		g_LightPoint[i].timeToPlayer = 0.0f;
		g_LightPoint[i].use = FALSE;
		g_LightPoint[i].flyingToPlayer = FALSE;
		g_LightPoint[i].distanceToPlayer = 0.0f;
		g_LightPoint[i].timeToFly = 0.0f;
		g_LightPoint[i].desiredToPlayerTime = LIGHTPOINT_REACH_PLAYER_TIME;
	}


	//// 1�Ԃ������`��Ԃœ������Ă݂�
	//g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	//g_Enemy[1].pos = g_MoveTbl1[0].pos;

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

void SetupEnemyAttributes(ENEMY* enemy)
{
	enemy->attributes.hp = attributesTbl[enemy->enemyType]->hp;
	enemy->attributes.maxHp = attributesTbl[enemy->enemyType]->maxHp;
	enemy->attributes.damage = attributesTbl[enemy->enemyType]->damage;
	enemy->attributes.staggerResistance = attributesTbl[enemy->enemyType]->staggerResistance;
	enemy->attributes.staggerRecoveryTime = attributesTbl[enemy->enemyType]->staggerRecoveryTime;
	enemy->attributes.stunTime = attributesTbl[enemy->enemyType]->stunTime;
	enemy->attributes.attackRange = attributesTbl[enemy->enemyType]->attackRange;
	enemy->attributes.attackCooldown = attributesTbl[enemy->enemyType]->attackCooldown;
	enemy->attributes.canFly = attributesTbl[enemy->enemyType]->canFly;
	enemy->attributes.move = attributesTbl[enemy->enemyType]->move;

	enemy->w = GetCurrentTextureSizeW(enemy->enemyType);
	enemy->h = GetCurrentTextureSizeH(enemy->enemyType);
}

const EnemyAttributes* GetEnemyAttributes(ENEMY* enemy)
{
	return attributesTbl[enemy->enemyType];
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

	for (int i = 0; i < MAP_ENEMY_MAX; i++)
	{
		ClearEnemy(&g_Enemy[i]);
		g_MoveTblAdr[i] = nullptr;
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
			case ENEMY_HIT:
				PlayEnemyHitAnim(&g_Enemy[i]);
				break;
			case ENEMY_DIE:
				PlayEnemyDieAnim(&g_Enemy[i]);
				break;
			default:
				break;
			}

			UpdateEnemyStates(&g_Enemy[i]);

			BOOL isMoveable = g_Enemy[i].state != ENEMY_HIT && g_Enemy[i].state != ENEMY_DIE && g_Enemy[i].onAirCnt < 5;
			// �ړ�����
			if (g_Enemy[i].tblMax > 0 && isMoveable)
			{	
				PLAYER* player = GetPlayer();
				// ���݂̍��W��ۑ����Ă������߂̕ϐ�
				float oldPosX = g_Enemy[i].pos.x;

				// �G�ƃv���C���[�̋������v�Z����
				XMVECTOR enemyPos = XMLoadFloat3(&g_Enemy[i].pos);
				XMVECTOR playerPos = XMLoadFloat3(&player->pos);
				XMVECTOR direction = XMVector3Normalize(playerPos - enemyPos);
				XMVECTOR returnPos = XMLoadFloat3(&g_Enemy[i].returnPos);
				if (g_Enemy[i].attributes.canFly == FALSE)
					direction = XMVectorSetY(direction, 0);  // Y�����̑��x��0�ɐݒ�
				float distanceToPlayer = XMVectorGetX(XMVector3Length(playerPos - enemyPos));
				float distanceToReturnPos = XMVectorGetX(XMVector3Length(returnPos - enemyPos));

				switch (g_Enemy[i].state)
				{
				case ENEMY_CHASE:
				{
					if (distanceToPlayer < g_Enemy[i].attributes.attackRange && player->state != DIE)
					{
						g_Enemy[i].stateOld = g_Enemy[i].state;
						g_Enemy[i].state = ENEMY_ATTACK;
						g_Enemy[i].dir = g_Enemy[i].pos.x - player->pos.x > 0.0f ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
						g_Enemy[i].patternAnim = ANIM_ATTACK_OFFSET;
						g_Enemy[i].countAnim = 0.0f;
						g_Enemy[i].finishAttack = FALSE;
					}
					// �ő�ǐՋ����𒴂����ꍇ�ARETREAT��ԂɈڍs
					else if (distanceToReturnPos > MAX_CHASE_DISTANCE) 
					{
						g_Enemy[i].state = ENEMY_RETREAT;  // �|�ޏ�ԂɈڍs
						g_Enemy[i].patternAnim = ANIM_WALK_PATTERN_NUM - 1;
					}
					else if (distanceToPlayer > ENEMY_VISION_RADIUS)
					{
						// �v���C���[������͈͊O�ɏo���ꍇ�A���̈ʒu�ɖ߂邽��WALK_BACK��ԂɈڍs
						g_Enemy[i].state = ENEMY_WALK_BACK;
					}
					else if (player->state != DIE)
					{
						// �ǐՂ��s��
						g_Enemy[i].stepBack = FALSE;
						float speed = g_Enemy[i].attributes.move.x;
						XMVECTOR moveStep = direction * speed;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);
						
					}
					else
						g_Enemy[i].state = ENEMY_WALK_BACK;
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
					else if (player->state != DIE && CheckChasingPlayer(&g_Enemy[i]))
					{
						g_Enemy[i].state = ENEMY_CHASE;  // �ő�ǐՋ������Ƀv���C���[������ꍇ�A�ĂђǐՂ���
					}
					else
					{
						// �|�ޕ����i�v���C���[�Ƃ͋t�j
						XMVECTOR direction = XMVector3Normalize(enemyPos - playerPos);
						if (g_Enemy[i].attributes.canFly == FALSE)
							direction = XMVectorSetY(direction, 0);  // Y�����̑��x��0�ɐݒ�
						XMVECTOR moveStep = direction * g_Enemy[i].attributes.move.x * RETREAT_SPEED_RATE;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);

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
						g_Enemy[i].patternAnim = ANIM_WALK_PATTERN_NUM - 1;
					}
					else if (distanceToPlayer > g_Enemy[i].attributes.attackRange 
						&& distanceToReturnPos < MAX_CHASE_DISTANCE
						&& player->state != DIE)
					{
						// �U���͈͊O�Ȃ�ǐՂ𑱂���
						g_Enemy[i].stepBack = FALSE;
						float speed = g_Enemy[i].attributes.move.x;  // �ǐՑ��x

						XMVECTOR moveStep = direction * speed * 0.5f;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);
					}
					
					// �U���͈͓��ɂ���ꍇ�͒�~���A�N�[���_�E����҂�
					// �N�[���_�E�����W�b�N
					g_Enemy[i].attributes.attackCooldown--;
					if (g_Enemy[i].attributes.attackCooldown <= 0)
					{
						// �N�[���_�E���I����A�ĂѓG�̍s��������
						if (distanceToPlayer < g_Enemy[i].attributes.attackRange && player->state != DIE)
						{
							g_Enemy[i].stateOld = g_Enemy[i].state;
							g_Enemy[i].state = ENEMY_ATTACK;  // �v���C���[���U���͈͓��ɂ���ꍇ
							g_Enemy[i].dir = g_Enemy[i].pos.x - player->pos.x > 0.0f ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
							g_Enemy[i].patternAnim = ANIM_ATTACK_OFFSET;
							g_Enemy[i].countAnim = 0.0f;
							g_Enemy[i].finishAttack = FALSE;
						}
						else if (CheckChasingPlayer(&g_Enemy[i]) && player->state != DIE)
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

				// ���`��Ԃ����s����
				case ENEMY_IDLE:
				case ENEMY_WALK:
				{
					// �v���C���[��������ɂ��āA�G���v���C���[�������Ă���ꍇ��CHASE��ԂɑJ��
					if (player->state != DIE 
						&& distanceToPlayer < ENEMY_VISION_RADIUS 
						&& CheckChasingPlayer(&g_Enemy[i]))
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
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[i];	// �s���e�[�u���̃A�h���X���擾
						//if (nowNo != 0)

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


							enemyPos = nowPos + Pos;
							XMFLOAT3 newPos;
							XMStoreFloat3(&newPos, enemyPos);

							// �G���E�Ɉړ����Ă��邩�A���Ɉړ����Ă��邩�𔻒f	
							g_Enemy[i].dir = XMVectorGetX(Pos) < 0 ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;

							if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							{
								// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
								SET_ENEMY_POS((&g_Enemy[i]), newPos);

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
					if (player->state != DIE 
						&& distanceToPlayer < ENEMY_VISION_RADIUS 
						&& CheckChasingPlayer(&g_Enemy[i]))
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
						if (g_Enemy[i].attributes.canFly == FALSE)
							direction = XMVectorSetY(direction, 0);  // Y�����̑��x��0�ɐݒ�
						float speed = g_Enemy[i].attributes.move.x;  // �����Ė߂鑬�x
						XMVECTOR moveStep = direction * speed;
						enemyPos += moveStep;

						XMFLOAT3 newPos;
						XMStoreFloat3(&newPos, enemyPos);
						if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
							SET_ENEMY_POS((&g_Enemy[i]), newPos);

						XMVECTOR delta = returnPos - enemyPos;
						float deltaX = fabs(XMVectorGetX(delta));
						float deltaY = fabs(XMVectorGetY(delta));

						// ���̈ʒu�ɋ߂Â��������m�F����
						if (deltaX <= g_Enemy[i].attributes.move.x && deltaY <= g_Enemy[i].attributes.move.y)
						{
							XMFLOAT3 newPos;
							XMStoreFloat3(&newPos, enemyPos);
							if (CheckEnemyMoveCollision(&g_Enemy[i], newPos, g_Enemy[i].dir))
								SET_ENEMY_POS((&g_Enemy[i]), newPos);
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

				g_Enemy[i].invertTex = g_Enemy[i].dir == CHAR_DIR_RIGHT ? FALSE : TRUE;
			}

			UpdateEnemyGroundCollision(&g_Enemy[i]);

			// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
			PLAYER* player = GetPlayer();

			if (g_Enemy[i].hitCD <= 0.0f && g_Enemy[i].state != ENEMY_DIE)
			{
				// �U���p�̕�̓{�b�N�X���擾
				for (int j = 0; j < MAX_ATTACK_AABB; j++)
				{
					AABB attackBox = player->attackAABB[j];

					// �G��AABB�����擾
					XMFLOAT3 enemyPos = g_Enemy[i].bodyAABB.pos;
					float enemyW = g_Enemy[i].bodyAABB.w;
					float enemyH = g_Enemy[i].bodyAABB.h;

					// �U����AABB�����擾
					XMFLOAT3 attackPos = attackBox.pos;
					float attackW = attackBox.w;
					float attackH = attackBox.h;

					// �G�l�~�[�̕�̓{�b�N�X�ƃv���C���[�̍U���͈͂��d�Ȃ��Ă��邩���m�F
					BOOL isColliding = CollisionBB(enemyPos, enemyW, enemyH, attackPos, attackW, attackH);

					if (isColliding)
					{
						// �����蔻�肪�������ꍇ�A�G�Ƀ_���[�W��^����
						EnemyTakeDamage(&g_Enemy[i]);
						break;
					}
				}

				// ���@�̕�̓{�b�N�X���擾
				Magic* magic = GetMagic();
				for (int j = 0; j < MAX_ATTACK_AABB; j++)
				{
					if (magic[j].use == FALSE) continue;

					if (magic[j].magicAABB.tag == PLAYER_MAGIC_AABB)
					{
						AABB attackBox = magic[j].magicAABB;

						// �G��AABB�����擾
						XMFLOAT3 enemyPos = g_Enemy[i].bodyAABB.pos;
						float enemyW = g_Enemy[i].bodyAABB.w;
						float enemyH = g_Enemy[i].bodyAABB.h;

						// ���@��AABB�����擾
						XMFLOAT3 attackPos = attackBox.pos;
						float attackW = attackBox.w;
						float attackH = attackBox.h;

						// �G�l�~�[�̕�̓{�b�N�X�ƃv���C���[�̍U���͈͂��d�Ȃ��Ă��邩���m�F
						BOOL isColliding = CollisionBB(enemyPos, enemyW, enemyH, attackPos, attackW, attackH);

						if (isColliding)
						{
							// �����蔻�肪�������ꍇ�A�G�Ƀ_���[�W��^����
							EnemyTakeDamage(&g_Enemy[i], &magic[j]);
							break;
						}
					}
				}
			}
		}
	}

	UpdateLightPoint();

	//// �G�l�~�[�S�Ń`�F�b�N
	//if (g_EnemyCount <= 0)
	//{
 //		SetFade(FADE_OUT, MODE_RESULT);
	//}

#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

void UpdateEnemyStates(ENEMY* enemy)
{
	// �G���U����Ԃł͂Ȃ��A���U�����I�����Ă��Ȃ��ꍇ
	if (enemy->state != ENEMY_ATTACK && enemy->finishAttack == FALSE)
	{
		// �U�������AABB�����Z�b�g
		for (int j = 0; j < MAX_ATTACK_AABB; j++)
		{
			enemy->attackAABB[j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			enemy->attackAABB[j].w = 0.0f;
			enemy->attackAABB[j].h = 0.0f;
		}
		enemy->finishAttack = TRUE;
	}

	if (enemy->hitCD > 0.0f) 
	{
		enemy->hitCD--;
	}
	// �G����e�����ꍇ
	if (enemy->isHit)
	{
		enemy->hitTimer--;
		if (enemy->hitTimer <= 0.0f)
		{
			enemy->isHit = false;
			enemy->hitTimer = 0.0f;
		}
	}

	enemy->attributes.staggerRecoveryTime--;
	if (enemy->attributes.staggerRecoveryTime <= 0)
		enemy->attributes.staggerResistance = GetEnemyAttributes(enemy)->staggerResistance;

	if (enemy->attributes.attackCooldown > 0 && enemy->state != ENEMY_COOLDOWN && enemy->state != ENEMY_HIT && enemy->state != ENEMY_DIE)
		enemy->state = ENEMY_COOLDOWN;
}

void UpdateEnemyGroundCollision(ENEMY* enemy)
{
	if (enemy->attributes.canFly == TRUE) return;

	AABB* grounds = GetMap01AABB();

	if (enemy->onAirCnt >= ENEMY_FALL_CNT_MAX)
		enemy->attributes.move.y = ENEMY_FALL_SPEED;
	else
	{
		// sin�֐����g�p���ĉ������x���v�Z
		float angle = (XM_PI / ENEMY_FALL_CNT_MAX) * enemy->onAirCnt;
		enemy->attributes.move.y = ENEMY_FALL_SPEED * sinf(angle);
	}
	if (enemy->attributes.move.y < 2.0f)
		enemy->attributes.move.y = 2.0f;
	// �؋󎞊Ԃ𑝉�������
	enemy->onAirCnt++;

	XMVECTOR enemyPos = XMLoadFloat3(&enemy->pos);
	XMVECTOR direction = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	enemyPos += direction * enemy->attributes.move.y;

	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, enemyPos);
	if (CheckEnemyMoveCollision(enemy, newPos, CHAR_DIR_DOWN))
	{
		SET_ENEMY_POS(enemy, newPos);
		enemy->isFalling = TRUE;
		enemy->dir = CHAR_DIR_DOWN;
	}
	else
	{
		enemy->isFalling = FALSE;
		enemy->onAirCnt = 0;
	}
		

}

void UpdateEnemyMoveTbl(ENEMY* enemy, float newPosY)
{
	int size = enemy->tblMax;

	INTERPOLATION_DATA* moveTbl = g_MoveTblAdr[enemy->tblNo];

	for (int i = 0; i < size; ++i) 
	{
		moveTbl[i].pos.y = newPosY;
	}
}

void EnemyTakeDamage(ENEMY* enemy, Magic* magic)
{
	PLAYER* player = GetPlayer();
	enemy->dir = enemy->pos.x - player->pos.x > 0 ? CHAR_DIR_LEFT : CHAR_DIR_RIGHT;
	enemy->isHit = true;
	enemy->hitTimer = ENEMY_HIT_TIMER;
	enemy->hitCD = ENEMY_HIT_CD;
	enemy->attributes.staggerRecoveryTime = GetEnemyAttributes(enemy)->staggerRecoveryTime;

	if (magic == nullptr)
	{
		if (player->attackPattern == PARRY)
			enemy->hitCD = ENEMY_HIT_CD * 0.5f;


		enemy->attributes.staggerResistance -= GetPoiseDamage();
		enemy->attributes.hp -= GetPlayerDamage();
	}
	else
	{
		switch (magic->magicType)
		{
		case MAGIC_FIRE_BALL:
			enemy->attributes.staggerResistance -= 20;
			enemy->attributes.hp -= 15;
			break;
		default:
			return;
		}
	}

	if (enemy->attributes.staggerResistance <= 0)
	{
		if (enemy->state != ENEMY_ATTACK)
			enemy->stateOld = enemy->state;
		enemy->attributes.attackCooldown = GetEnemyAttributes(enemy)->attackCooldown + GetRand(10, 60);  // �N�[���_�E���^�C�������Z�b�g
		enemy->attributes.staggerResistance = GetEnemyAttributes(enemy)->staggerResistance; // ���x�������Z�b�g
		enemy->state = ENEMY_HIT;
		enemy->countAnim = 0.0f;
	}

	if (enemy->attributes.hp <= 0)
	{
		enemy->state = ENEMY_DIE;
		enemy->countAnim = 0;
		enemy->diePos = enemy->pos;
		enemy->pos.y -= 70.0f;
		enemy->dieInitSpeedX = 0.2f * (float)GetRand(-10, 10);
		enemy->dieInitSpeedY = 0.1f * (float)GetRand(1, 5);
		EnemyDieOnTrigger(enemy);
	}

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

			// �G�̉e
			DrawEnemyShadow(&g_Enemy[i]);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Enemy[i].enemyType]);

			DrawEnemySprite(&g_Enemy[i]);
			if (g_Enemy[i].state == ENEMY_DIE)
			{
				g_Enemy[i].patternAnim = ANIM_DIE_OFFSET + 1;
				DrawEnemySprite(&g_Enemy[i], TRUE);
			}

			// �G��HP�Q�[�W
			DrawEnemyHPGauge(&g_Enemy[i]);

		}
	}

	DrawLightPoint();


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
			if (g_Enemy[i].use == FALSE) continue;

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
			if (g_Enemy[i].use == FALSE) continue;

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
}

void DrawEnemySprite(const ENEMY* enemy, BOOL die)
{
	BG* bg = GetBG();

	float px, py, pw, ph;
	if (die)
	{
		px = enemy->diePos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
		py = enemy->diePos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
		pw = enemy->w;		// �G�l�~�[�̕\����
		ph = enemy->h;		// �G�l�~�[�̕\������
	}
	else
	{
		px = enemy->pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
		py = enemy->pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
		pw = enemy->w;		// �G�l�~�[�̕\����
		ph = enemy->h;		// �G�l�~�[�̕\������
	}


	// �A�j���[�V�����p
	float tw, th, tx, ty;
	tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X; // �e�N�X�`���̕�
	th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
	ty = (float)(enemy->patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W
	if (enemy->invertTex)
	{
		tw *= -1.0f;
		tx = (float)(enemy->patternAnim % TEXTURE_PATTERN_DIVIDE_X) * (-1 * tw) - tw;	// �e�N�X�`���̍���X���W
	}
	else
		tx = (float)(enemy->patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	if (enemy->isHit)
	{
		color = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
		//if (fmod(enemy->hitTimer, 3.0f) < 1.5f) 
		//{
		//	color.w = 0.0f;
		//}
	}
	if (enemy->state == ENEMY_DIE)
	{
		color.w = 1.0f - enemy->countAnim / ANIM_WAIT_DIE;
	}

	float rotation;
	if (die)
		rotation = 0.0f;
	else
		rotation = enemy->rot.z;
	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		color,
		rotation);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}

void DrawEnemyHPGauge(const ENEMY* enemy)
{
	BG* bg = GetBG();

	float posX, posY;
	if (enemy->state == ENEMY_DIE)
	{
		posX = enemy->diePos.x;
		posY = enemy->diePos.y;
	}
	else
	{
		posX = enemy->bodyAABB.pos.x;
		posY = enemy->bodyAABB.pos.y;
	}
	// HP�o�[�̈ʒu���v�Z
	float px = posX - enemy->bodyAABB.w * 0.5f - bg->pos.x;  // HP�o�[��X���W�i�G�̒����ɕ\���j
	float py = posY - enemy->bodyAABB.h * 0.7f - bg->pos.y;  // HP�o�[��Y���W
	float pw = enemy->bodyAABB.w;   // HP�o�[�̕�
	float ph = 10.0f;    // HP�o�[�̍���

	float tw = 1.0f;     // �e�N�X�`���̕�
	float th = 1.0f;     // �e�N�X�`���̍���
	float tx = 0.0f;     // �e�N�X�`����X���W
	float ty = 0.0f;     // �e�N�X�`����Y���W

	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);  // �w�i�𔒐F�ŕ\��
	if (enemy->state == ENEMY_DIE)
	{
		color.w = 1.0f - enemy->countAnim / ANIM_WAIT_DIE;
	}

	// 1. HP�o�[�̔w�i��`��i�G�̑�HP��\���A_bg�e�N�X�`�����g�p�j
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ENEMY_HP_GAUGE_TEXTURE + 1]);
	SetSpriteLTColor(g_VertexBuffer,
		px, py, pw, ph,
		tx, ty, tw, th,
		color);
	GetDeviceContext()->Draw(4, 0);


	// 2. HP�o�[�̑O�i��`��i�G�̌��݂�HP��\���AENEMY_HP_GAUGE_TEXTURE�e�N�X�`�����g�p�j
	float hpRatio = (float)enemy->attributes.hp / (float)enemy->attributes.maxHp;  // HP�䗦���v�Z
	float hpWidth = pw * hpRatio;  // HP�䗦�Ɋ�Â���HP�o�[�̒������v�Z

	// �O�i�̐F��ݒ�i�ʏ�͗ΐF�A���݂�HP��\���j
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ENEMY_HP_GAUGE_TEXTURE]);
	SetSpriteLTColor(g_VertexBuffer,
		px, py, hpWidth, ph,         // �O�i�̕���HP�Ɋ�Â��ďk��
		tx, ty, tw * hpRatio, th,    // HP�䗦�ɉ����ăe�N�X�`���̕����k��
		color);
	GetDeviceContext()->Draw(4, 0);
}

void DrawEnemyShadow(const ENEMY* enemy)
{
	BG* bg = GetBG();

	// �e�\��
	SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ENEMY_SHADOW_TEXTURE]);

	float px = enemy->pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
	float py = enemy->pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
	float pw = enemy->bodyAABB.w;		// �v���C���[�̕\����
	float ph = enemy->bodyAABB.w / 3;		// �v���C���[�̕\������

	// �n�ʂƂ̏Փ˔���Ɋ�Â��e�̈ʒu
	// �v���C���[�̈ʒu�ɑΉ�����n�ʂ̍������v�Z
	float shadowPosY = py;  // �e�̍ŏI�I��Y���W
	float highestGroundY = FLT_MAX;  // �ł������n�ʂ�Y���W��ێ�

	// �n�ʂ�AABB�����擾
	AABB* ground = GetMap01AABB();
	BOOL isOnGround = false;

	// �v���C���[���n�ʂɂ��邩�ǂ������m�F
	for (int j = 0; j < MAP_GROUND_MAX; j++)
	{
		float groundX = ground[j].pos.x;
		float groundY = ground[j].pos.y;
		float groundW = ground[j].w;
		float groundH = ground[j].h;

		if (groundY < enemy->pos.y) continue;

		// �v���C���[��x���W���n�ʂ̕��͈̔͂ɓ����Ă��邩�ǂ������m�F
		if (enemy->pos.x >= groundX - groundW / 2 && enemy->pos.x <= groundX + groundW / 2)
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

void DrawLightPoint(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < LIGHTPOINT_MAX; i++)
	{
		if (g_LightPoint[i].use == TRUE)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[LIGHTPOINT_TEXTURE]);

			float px = g_LightPoint[i].pos.x - bg->pos.x;
			float py = g_LightPoint[i].pos.y - bg->pos.y;
			float pw = g_LightPoint[i].w;
			float ph = g_LightPoint[i].h;

			float tw = 1.0f;     // �e�N�X�`���̕�
			float th = 1.0f;     // �e�N�X�`���̍���
			float tx = 0.0f;     // �e�N�X�`����X���W
			float ty = 0.0f;     // �e�N�X�`����Y���W

			XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				color);
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

BOOL CheckEnemyMoveCollision(ENEMY* enemy, XMFLOAT3 newPos, int dir)
{
	// �ǂ�AABB�����擾
	AABB* walls = GetMap01AABB();
	for (int i = 0; i < MAP_GROUND_MAX; i++)
	{
		XMFLOAT3 wallPos = walls[i].pos;
		float wallW = walls[i].w;
		float wallH = walls[i].h;

		// �Փˊm�F
		switch (dir)
		{
		case CHAR_DIR_LEFT:
		{

			if (CollisionBB(newPos, enemy->bodyAABB.w, enemy->bodyAABB.h, wallPos, wallW, wallH) &&
				enemy->bodyAABB.pos.x > wallPos.x)
			{
				// ���Ɉړ����Ă��āA�ǂ������ɂ���ꍇ
				//XMFLOAT3 enemyPos = enemy->pos;
				//enemyPos.x = wallPos.x + wallW / 2 + enemy->bodyAABB.w / 2 + 0.01f;
				//SET_ENEMY_POS(enemy, enemyPos); // ���ւ̐i�s���~
				return false;
			}
			break;
		}

		case CHAR_DIR_RIGHT:
		{
			if (CollisionBB(newPos, enemy->bodyAABB.w, enemy->bodyAABB.h, wallPos, wallW, wallH) &&
				enemy->bodyAABB.pos.x < wallPos.x)
			{
				// �E�Ɉړ����Ă��āA�ǂ��E���ɂ���ꍇ
				//XMFLOAT3 enemyPos = enemy->pos;
				//enemyPos.x = wallPos.x - wallW / 2 - enemy->bodyAABB.w / 2 - 0.01f;
				//SET_ENEMY_POS(enemy, enemyPos); // �E�ւ̐i�s���~
				return false;
			}
			break;
		}
		case CHAR_DIR_DOWN:
		{
			if (CollisionBB(newPos, enemy->bodyAABB.w, enemy->bodyAABB.h, wallPos, wallW, wallH))
			{
				// �G�l�~�[����Ɉړ����Ă��āA�ǂ������ɂ���ꍇ
				XMFLOAT3 enemyPos = enemy->pos;
				enemyPos.y = wallPos.y - wallH / 2 - enemy->bodyAABB.h / 2 - 0.01f;
				SET_ENEMY_POS(enemy, enemyPos); // ���ւ̐i�s���~
				UpdateEnemyMoveTbl(enemy, enemyPos.y);
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
	if (enemy->patternAnim == ANIM_ATTACK_OFFSET && GetTutorialPause() == PAUSE_ENEMY)
	{
		g_Update = FALSE;
	}

	PLAYER* player = GetPlayer();

	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;

	enemy->countAnim++;
	int attackInterval = enemy->patternAnim == ANIM_ATTACK_OFFSET ? ANIM_WAIT_ATTACK * 2 : ANIM_WAIT_ATTACK;
	if (enemy->countAnim > attackInterval)
	{
		enemy->countAnim = 0.0f;
		// �p�^�[���̐؂�ւ�
		enemy->patternAnim = (enemy->patternAnim + 1) % ANIM_ATTACK_PATTERN_NUM + ANIM_ATTACK_OFFSET;
	}

	// �U������̍X�V
	UpdateEnemyAttackAABB(enemy);

	// �A�j���[�V�������I�����������`�F�b�N
	if (enemy->patternAnim == ANIM_ATTACK_PATTERN_NUM + ANIM_ATTACK_OFFSET - 1 && enemy->countAnim == ANIM_WAIT_ATTACK)
	{
		// �A�j���[�V�����I����ɍU�������s
		enemy->attributes.attackCooldown = GetEnemyAttributes(enemy)->attackCooldown + GetRand(10, 60);  // �N�[���_�E���^�C�������Z�b�g
		enemy->state = ENEMY_COOLDOWN;  // �N�[���_�E����ԂɈڍs

		for (int i = 0; i < MAX_ATTACK_AABB; i++)
		{
			enemy->attackAABB[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			enemy->attackAABB[i].h = 0;
			enemy->attackAABB[i].w = 0;
		}
	}


	HandleEnemyMagic(enemy);
}

void PlayEnemyHitAnim(ENEMY* enemy)
{
	enemy->invertTex = enemy->dir == CHAR_DIR_RIGHT ? FALSE : TRUE;
	int dir = enemy->dir == CHAR_DIR_RIGHT ? -1 : 1;
	enemy->patternAnim = ANIM_HIT_OFFSET;
	float rotationAngle = dir * 0.25f * enemy->countAnim / GetEnemyAttributes(enemy)->stunTime;
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, rotationAngle);
	XMStoreFloat3(&enemy->rot, XMLoadFloat3(&rotation));


	enemy->countAnim++;
	if (enemy->countAnim > GetEnemyAttributes(enemy)->stunTime)
	{
		enemy->countAnim = 0.0f;
		XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMStoreFloat3(&enemy->rot, XMLoadFloat3(&rotation));
		enemy->patternAnim = 0;
		enemy->state = enemy->stateOld;
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

void PlayEnemyDieAnim(ENEMY* enemy)
{
	enemy->patternAnim = ANIM_DIE_OFFSET;

	float gravity = 0.1f;
	float t = (float)enemy->countAnim * 0.05f;
	float airResistance = 0.98f;
	float currentSpeedX = enemy->dieInitSpeedX * pow(airResistance, t);

	enemy->pos.x += currentSpeedX;
	enemy->pos.y += enemy->dieInitSpeedY * t + 0.5f * gravity * t * t;

	float relativeX = enemy->pos.x - enemy->diePos.x;
	float rotationAngle = relativeX * 0.05f;
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, rotationAngle);
	XMStoreFloat3(&enemy->rot, XMLoadFloat3(&rotation));

	enemy->countAnim++;
	if (enemy->countAnim > ANIM_WAIT_DIE)
	{
		enemy->use = FALSE;
	}
}

float GetCurrentTextureSizeW(int enemyType)
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
float GetCurrentTextureSizeH(int enemyType)
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

void EnemyDieOnTrigger(ENEMY* enemy)
{
	// �G�̃T�C�Y�Ɋ�Â��Č��_�̐������肷��
	int numLightPoints = (int)enemy->w * enemy->h * 0.00025f;

	// ���_�̐����ő�l�𒴂��Ȃ��悤�ɐ�������
	if (numLightPoints > LIGHTPOINT_MAX)
		numLightPoints = LIGHTPOINT_MAX;

	for (int i = 0; i < LIGHTPOINT_MAX; ++i)
	{
		if (g_LightPoint[i].use == TRUE) continue;

		numLightPoints--;

		// ���_�����U����p�x�𒲐����A������ɂ�葽���̌��_���������悤�ɂ���
		int directionBias = GetRand(0, 100);  // �����̃o�C�A�X�����肷�郉���_���l
		float angle; 
		if (directionBias < 70) 
		{  // 70%�̊m���ŏ�����ɔ��U
			angle = GetRand(180, 360) * PI / 180.0f;
		}
		else 
		{  // 30%�̊m���ŉ������ɔ��U
			angle = GetRand(0, 180) * PI / 180.0f;
		}
		float speed = GetRand(LIGHPOINT_SPEED_MIN, LIGHTPOINT_SPEED_MAX);
		XMFLOAT3 velocity(cosf(angle) * speed, sinf(angle) * speed, 0.0f);

		g_LightPoint[i].timeToFly = LIGHTPOINT_FLY_TIME;
		g_LightPoint[i].use = TRUE;
		g_LightPoint[i].timeToPlayer = LIGHTPOINT_STAY_TIME;
		g_LightPoint[i].move = velocity;
		g_LightPoint[i].pos = enemy->bodyAABB.pos;
		if (numLightPoints == 0)
			break;
	}
}

void UpdateLightPoint(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < LIGHTPOINT_MAX; i++)
	{
		if (g_LightPoint[i].use == FALSE) continue;

		// ���_���v���C���[�Ɍ������Ĕ��ł���ꍇ
		if (g_LightPoint[i].flyingToPlayer == TRUE)
		{
			XMVECTOR posVec = XMLoadFloat3(&g_LightPoint[i].pos);
			XMFLOAT3 targetPos = player->bodyAABB.pos;
			targetPos.x += LIGHTPOINT_TARGET_OFFSET_X;
			targetPos.y += LIGHTPOINT_TARGET_OFFSET_Y;
			XMVECTOR playerVec = XMLoadFloat3(&targetPos);
			XMVECTOR direction = XMVector3Normalize(XMVectorSubtract(playerVec, posVec));
			XMVECTOR distanceVec = XMVectorSubtract(playerVec, posVec);
			float distance = XMVectorGetX(XMVector3Length(distanceVec));

			// �v���C���[�ɓ��B����܂ł̎��Ԃ��v�Z���A�K�؂ȑ��x��ݒ�
			float desiredTime = g_LightPoint[i].desiredToPlayerTime;
			if (g_LightPoint[i].desiredToPlayerTime > 1.0f)
				g_LightPoint[i].desiredToPlayerTime--;
			float adjustedSpeed = distance / desiredTime;

			XMVECTOR velocityVec = XMVectorScale(direction, adjustedSpeed);
			XMStoreFloat3(&g_LightPoint[i].move, velocityVec);

			posVec = XMVectorAdd(posVec, XMVectorScale(velocityVec, 1.0f));
			XMStoreFloat3(&g_LightPoint[i].pos, posVec);
		}
		
		else if (g_LightPoint[i].timeToFly > 0.0f)
		{
			// ���_���l���ɔ��U������
			XMVECTOR posVec = XMLoadFloat3(&g_LightPoint[i].pos);
			XMVECTOR velocityVec = XMLoadFloat3(&g_LightPoint[i].move);

			posVec = XMVectorAdd(posVec, velocityVec);
			XMStoreFloat3(&g_LightPoint[i].pos, posVec);

			g_LightPoint[i].timeToFly--;

			XMVECTOR playerVec = XMLoadFloat3(&player->bodyAABB.pos);
			XMVECTOR distanceVec = XMVectorSubtract(posVec, playerVec);
			g_LightPoint[i].distanceToPlayer = XMVectorGetX(XMVector3Length(distanceVec));
		}
		// ���_���܂��Î~���Ă���ꍇ
		else if (g_LightPoint[i].timeToFly <= 0.0f)
		{
			g_LightPoint[i].timeToPlayer--;
			// �v���C���[�Ɍ������Ĕ��
			if (g_LightPoint[i].timeToPlayer <= 0.0f)
				g_LightPoint[i].flyingToPlayer = TRUE;
		}

		// ���_���v���C���[�ɓ��B�������ǂ������`�F�b�N
		XMVECTOR posVec = XMLoadFloat3(&g_LightPoint[i].pos);
		XMFLOAT3 targetPos = player->bodyAABB.pos;
		targetPos.x += LIGHTPOINT_TARGET_OFFSET_X;
		targetPos.y += LIGHTPOINT_TARGET_OFFSET_Y;
		XMVECTOR playerVec = XMLoadFloat3(&targetPos);
		XMVECTOR distanceVec = XMVectorSubtract(posVec, playerVec);
		float distance = XMVectorGetX(XMVector3Length(distanceVec));
		if (distance < player->w * 0.3f)
		{
			g_LightPoint[i].use = FALSE;
			int score = GetRand(888, 999);
			AddScore(score);
		}
	}
}

void HandleEnemyMagic(ENEMY* enemy)
{
	switch (enemy->enemyType)
	{
	case GOLEM:
		if (enemy->patternAnim == ANIM_ATTACK_OFFSET + 1 && enemy->countAnim == 0)
			TriggerMagic(MAGIC_STOMP, enemy->pos, enemy);
		break;
	default:
		break;
	}
}

void ClearEnemy(ENEMY* enemy)
{
	enemy->enemyType = 0;
	enemy->time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	enemy->tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	enemy->tblMax = 0;
	enemy->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemy->returnPos = enemy->pos;

	enemy->attributes.hp = 0.0f;
	enemy->attributes.maxHp = 0.0f;
	enemy->attributes.damage = 0;
	enemy->attributes.staggerResistance = 0;
	enemy->attributes.staggerRecoveryTime = 0;
	enemy->attributes.stunTime = 0;
	enemy->attributes.attackRange = 0.0f;
	enemy->attributes.attackCooldown = 0;
	enemy->attributes.canFly = FALSE;
	enemy->attributes.move = XMFLOAT3(0.0f, 0.0f, 0.0f);

	enemy->w = 0.0f;
	enemy->h = 0.0f;

	enemy->use = FALSE;
	enemy->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemy->scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	enemy->countAnim = 0;
	enemy->patternAnim = 0;

	enemy->idleCount = 0;
	enemy->state = ENEMY_IDLE;
	enemy->dir = CHAR_DIR_RIGHT;
	enemy->oldDir = CHAR_DIR_RIGHT;
	enemy->stepBack = FALSE;
	enemy->onAirCnt = 0;
	enemy->isFalling = FALSE;

	// battle
	enemy->isHit = FALSE;
	enemy->hitTimer = 0.0f;
	enemy->hitCD = 0.0f;
	enemy->diePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	enemy->dieInitSpeedX = 0.0f;
	enemy->dieInitSpeedY = 0.0f;

	// AABB
	enemy->bodyAABB.pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
	enemy->bodyAABB.w = 0.0f;
	enemy->bodyAABB.h = 0.0f;
	enemy->bodyAABB.tag = ENEMY_BODY_AABB;

	for (int j = 0; j < MAX_ATTACK_AABB; j++)
	{
		enemy->attackAABB[j].pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
		enemy->attackAABB[j].w = 0.0f;
		enemy->attackAABB[j].h = 0.0f;
		enemy->attackAABB[j].tag = ENEMY_ATTACK_AABB;
	}
}

BOOL GetUpdateEnemy(void)
{
	return g_Update;

}
void SetUpdateEnemy(BOOL update)
{
	g_Update = update;
}