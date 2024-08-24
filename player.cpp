//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_IDLE_WIDTH					(150/2)	// キャラサイズ
#define TEXTURE_IDLE_HEIGHT					(150/2)	// 
#define TEXTURE_NORMAL_ATTACK1_WIDTH		(285/2)	// キャラサイズ
#define TEXTURE_NORMAL_ATTACK1_HEIGHT		(240/2)	// 
#define TEXTURE_NORMAL_ATTACK3_WIDTH		(281/2)	// キャラサイズ
#define TEXTURE_NORMAL_ATTACK3_HEIGHT		(217/2)	//
#define TEXTURE_NORMAL_ATTACK4_WIDTH		(217/2)	// キャラサイズ
#define TEXTURE_NORMAL_ATTACK4_HEIGHT		(290/2)	//
#define TEXTURE_RUN_WIDTH					(165/2)	// キャラサイズ
#define TEXTURE_RUN_HEIGHT					(165/2)	// 
#define TEXTURE_JUMP_WIDTH					(210/2)	// キャラサイズ
#define TEXTURE_JUMP_HEIGHT					(210/2)	//
#define TEXTURE_MAX							(20)	// テクスチャの数

// アニメパターンのテクスチャ内分割数（X)
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


#define ANIM_PATTERN_NUM						(TEXTUREf_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT_WALK							(10)		// アニメーションの切り替わるWait値
#define ANIM_WAIT_IDLE							(30)		// アニメーションの切り替わるWait値
#define ANIM_WAIT_RUN							(5)		// アニメーションの切り替わるWait値
#define ANIM_WAIT_DASH							(1)		// アニメーションの切り替わるWait値
#define ANIM_WAIT_ATTACK						(5)		// アニメーションの切り替わるWait値
#define ANIM_WAIT_JUMP							(7)		// アニメーションの切り替わるWait値
#define ANIM_DASH_FRAME							(10)
#define ANIM_NORMAL_ATTACK1_FRAME				(7)
#define ANIM_NORMAL_ATTACK2_FRAME				(9)
#define ANIM_NORMAL_ATTACK3_FRAME				(8)
#define ANIM_NORMAL_ATTACK4_FRAME				(13)


#define TEXTURE_NORMAL_ATTACK1_OFFSET			XMFLOAT3(10.0f, -20.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK2_OFFSET			XMFLOAT3(5.0f, 0.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK3_OFFSET			XMFLOAT3(10.0f, -10.0f, 0.0f)
#define TEXTURE_NORMAL_ATTACK4_OFFSET			XMFLOAT3(10.0f, -20.0f, 0.0f)

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_IDLE_HEIGHT)

// ジャンプ処理
#define	PLAYER_JUMP_CNT_MAX			(60)		// 60フレームで着地する
#define	PLAYER_JUMP_Y_MAX			(7.5f)	// ジャンプの高さ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawPlayerOffset(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

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


static BOOL		g_Load = FALSE;				// 初期化を行ったかのフラグ
static PLAYER	g_Player[PLAYER_MAX];		// プレイヤー構造体
static int		g_PlayerCount = PLAYER_MAX;	// 生きてるプレイヤーの数

static int      g_jumpCnt = 0;
static int		g_jump[PLAYER_JUMP_CNT_MAX] =
{
	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5,-4,-3,-2,-1,
	  1,   2,   3,   4,   5,   6,  7,  8,  9, 10, 11,12,13,14,15
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_PlayerCount = 0;						// 生きてるプレイヤーの数

	// プレイヤー構造体の初期化
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_PlayerCount++;
		g_Player[i].use = TRUE;
		g_Player[i].pos = XMFLOAT3(PLAYER_INIT_POS_X, PLAYER_INIT_POS_Y, 0.0f);	// 中心点から表示
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

		g_Player[i].move = XMFLOAT3(3.75f, 0.0f, 0.0f);		// 移動量

		g_Player[i].dir = CHAR_DIR_RIGHT;					// 右向きにしとくか
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

		// ジャンプの初期化
		g_Player[i].jump = FALSE;
		g_Player[i].jumpCnt = 0;
		g_Player[i].jumpYMax = PLAYER_JUMP_Y_MAX;

		g_Player[i].onAirCnt = 0;

		for (int j = 0; j < ACTION_QUEUE_SIZE; j++)
		{
			g_Player[i].actionQueue[j] = 0;
		}

		// 分身用
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
// 終了処理
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
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_Load == FALSE) return;
	g_PlayerCount = 0;				// 生きてるプレイヤーの数

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// 生きてるプレイヤーだけ処理をする
		if (g_Player[i].use == TRUE)
		{
			g_PlayerCount++;		// 生きてるプレイヤーの数

			if (g_Player[i].state == IDLE) g_Player[i].shadowY = g_Player[i].pos.y;

			// 地形との当たり判定用に座標のバックアップを取っておく
			XMFLOAT3 pos_old = g_Player[i].pos;

			// 分身用
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = pos_old;

			UpdateActionQueue();
			g_Player[i].dashCD++;

			// 現在のアニメーションが終了したかどうかを確認
			if (g_Player[i].playAnim == FALSE)
			{
				if (g_Player[i].actionQueueStart != g_Player[i].actionQueueEnd)
				{
					// キュー内の次のアクションを実行
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
				// 落ちる状態に戻る？
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

				// MAP外チェック
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

				// 左右の制限距離（画面の三分の一）
				float leftLimitX = SCREEN_WIDTH / 3;
				float rightLimitX = SCREEN_WIDTH * 2 / 3;

				// プレイヤーが画面内の移動範囲内かどうかを確認
				if (g_Player[i].pos.x < bg->pos.x + leftLimitX) {
					// プレイヤーが左の制限範囲を越えた場合は、背景を左にスクロール
					bg->pos.x = g_Player[i].pos.x - leftLimitX;
				}
				else if (g_Player[i].pos.x > bg->pos.x + rightLimitX) {
					// プレイヤーが右の制限範囲を越えた場合は、背景を右にスクロール
					bg->pos.x = g_Player[i].pos.x - rightLimitX;
				}

				// 背景のX座標を画面端で制限
				if (bg->pos.x < 0) bg->pos.x = 0;
				if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				// 上下の制限距離（画面の三分の一）
				float topLimitY = SCREEN_HEIGHT / 3;
				float bottomLimitY = SCREEN_HEIGHT * 2 / 3;

				// プレイヤーが画面内の移動範囲内かどうかを確認
				if (g_Player[i].pos.y < bg->pos.y + topLimitY) {
					// プレイヤーが上の制限範囲を越えた場合は、背景を上にスクロール
					bg->pos.y = g_Player[i].pos.y - topLimitY;
				}
				else if (g_Player[i].pos.y > bg->pos.y + bottomLimitY) {
					// プレイヤーが下の制限範囲を越えた場合は、背景を下にスクロール
					bg->pos.y = g_Player[i].pos.y - bottomLimitY;
				}

				// 背景のY座標を画面端で制限
				if (bg->pos.y < 0) bg->pos.y = 0;
				if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

				//// プレイヤーの立ち位置からMAPのスクロール座標を計算する
				//bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
				//if (bg->pos.x < 0) bg->pos.x = 0;
				//if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

				//bg->pos.y = g_Player[i].pos.y - PLAYER_DISP_Y;
				//if (bg->pos.y < 0) bg->pos.y = 0;
				//if (bg->pos.y > bg->h - SCREEN_HEIGHT) bg->pos.y = bg->h - SCREEN_HEIGHT;

				// 移動が終わったらエネミーとの当たり判定
				//{
				//	ENEMY* enemy = GetEnemy();

				//	// エネミーの数分当たり判定を行う
				//	for (int j = 0; j < ENEMY_MAX; j++)
				//	{
				//		// 生きてるエネミーと当たり判定をする
				//		if (enemy[j].use == TRUE)
				//		{
				//			BOOL ans = CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h,
				//				enemy[j].pos, enemy[j].w, enemy[j].h);
				//			// 当たっている？
				//			if (ans == TRUE)
				//			{
				//				// 当たった時の処理
				//				enemy[j].use = FALSE;
				//				AddScore(10);
				//			}
				//		}
				//	}
				//}



				// バレット処理
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

			// アニメーション  
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


	// 現状をセーブする
	if (GetKeyboardTrigger(DIK_S))
	{
		SaveData();
	}


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes


			DrawPlayerShadow();

			// プレイヤーの分身を描画
			if (g_Player[i].state == DASH)
			{	// ダッシュ中だけ分身処理
				DrawPlayerOffset(i);
			}

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[i].texNo]);

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_Player[i].w;		// プレイヤーの表示幅
			float ph = g_Player[i].h;		// プレイヤーの表示高さ

			AdjustAttackTexturePosition(px, py);

			// アニメーション用
			float tw, th, tx, ty;
			tw = 1.0f / GetTexturePatternDivideX(); // テクスチャの幅
			th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			ty = (float)(g_Player[i].patternAnim / GetTexturePatternDivideX()) * th;	// テクスチャの左上Y座標
			if (g_Player->invertTex)
			{
				tw *= -1.0f;
				tx = (float)(g_Player[i].patternAnim % GetTexturePatternDivideX()) * (-1 * tw) - tw;	// テクスチャの左上X座標
			}
			else
				tx = (float)(g_Player[i].patternAnim % GetTexturePatternDivideX()) * tw;

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Player[i].rot.z);

			// ポリゴン描画
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
		// もしアイドル状態なら、すぐにアクションを実行
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
		// 空中の状態
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
		// その他の状態の場合、アクションをキューに追加
		else if (!(g_Player->dashOnAir == TRUE && g_Player->onAirCnt > 0))
		{
			g_Player->actionQueue[g_Player->actionQueueEnd] = DASH;
			g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

			// キューのオーバーフローを防止
			if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
			{
				g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // 最も古いアクションを破棄
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
		// もしアイドル状態なら、すぐにアクションを実行
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
		// 空中の状態
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
		// その他の状態の場合、アクションをキューに追加
		else
		{
			g_Player->actionQueue[g_Player->actionQueueEnd] = JUMP;
			g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

			// キューのオーバーフローを防止
			if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
			{
				g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // 最も古いアクションを破棄
			}
		}
	}
	else if (GetKeyboardTrigger(DIK_X))
	{
		switch (g_Player->state)
		{
			// もしアイドル状態なら、すぐにアクションを実行
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
			// もしアイドル状態なら、すぐにアクションを実行
			if (g_Player->playAnim == FALSE)
			{
				g_Player->patternAnim = 0;
				g_Player->animFrameCount = 0;
				g_Player->playAnim = TRUE;
				g_Player->attackPattern++;
			}
			// 空中の状態
			else if (g_Player->jumpCnt > PLAYER_JUMP_CNT_MAX / 2 && g_Player->state != ATTACK)
			{
				g_Player->patternAnimOld = g_Player->patternAnim;
				g_Player->patternAnim = 0;
				g_Player->state = ATTACK;
				g_Player->animFrameCount = 0;
			}
			// その他の状態の場合、アクションをキューに追加
			else
			{
				g_Player->actionQueue[g_Player->actionQueueEnd] = ATTACK;
				g_Player->actionQueueEnd = (g_Player->actionQueueEnd + 1) % ACTION_QUEUE_SIZE;

				// キューのオーバーフローを防止
				if (g_Player->actionQueueEnd == g_Player->actionQueueStart)
				{
					g_Player->actionQueueStart = (g_Player->actionQueueStart + 1) % ACTION_QUEUE_SIZE; // 最も古いアクションを破棄
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
	// プレイヤーの位置を更新する前に接地を確認
	AABB* grounds = GetMap01AABB();
	BOOL onGround = false;

	if (g_Player->jumpCnt == 0)
	{
		if (g_Player->onAirCnt < PLAYER_JUMP_CNT_MAX * 0.5f && g_Player->onAirCnt != 0)
		{
			// sin関数を使用して下落速度を計算
			float angle = (XM_PI / PLAYER_JUMP_CNT_MAX) * g_Player->onAirCnt;

			g_Player->move.y = g_Player->jumpYMax * sinf(angle);

			// 滞空時間を増加させる
			g_Player->onAirCnt++;
		}
		else
		{
			// 滞空時間が最大に達したら、下落速度を最大値に保つ
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
// Player構造体の先頭アドレスを取得
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}


// 生きてるエネミーの数
int GetPlayerCount(void)
{
	return g_PlayerCount;
}

void DrawPlayerShadow(void)
{
	BG* bg = GetBG();

	// 影表示
	SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[CHAR_SHADOW]);

	float px = g_Player->pos.x - bg->pos.x;	// プレイヤーの表示位置X
	float py = g_Player->shadowY - bg->pos.y;	// プレイヤーの表示位置Y
	float pw = TEXTURE_IDLE_WIDTH;		// プレイヤーの表示幅
	float ph = TEXTURE_IDLE_WIDTH / 4;		// プレイヤーの表示高さ

	// 地面との衝突判定に基づく影の位置
	// プレイヤーの位置に対応する地面の高さを計算
	float shadowPosY = py;  // 影の最終的なY座標
	float highestGroundY = FLT_MAX;  // 最も高い地面のY座標を保持

	// 地面のAABB情報を取得
	AABB* ground = GetMap01AABB();
	BOOL isOnGround = false;

	// プレイヤーが地面にいるかどうかを確認
	for (int j = 0; j < MAP01_GROUND_MAX; j++)
	{
		float groundX = ground[j].pos.x;
		float groundY = ground[j].pos.y;
		float groundW = ground[j].w;
		float groundH = ground[j].h;

		// プレイヤーのx座標が地面の幅の範囲に入っているかどうかを確認
		if (g_Player->pos.x >= groundX - groundW / 2 && g_Player->pos.x <= groundX + groundW / 2)
		{
			// もっとも高い地面を探す
			if (groundY - groundH / 2 < highestGroundY)
			{
				highestGroundY = groundY - groundH / 2;  // 現在の地面が最も高いので更新
				shadowPosY = groundY - groundH / 2 - bg->pos.y - 50.0f; // 地面の上に影を表示
				isOnGround = true;  // プレイヤーが地面にいるとフラグを設定
			}
		}
	}
	py = shadowPosY + 50.0f;

	float tw = 1.0f;	// テクスチャの幅
	float th = 1.0f;	// テクスチャの高さ
	float tx = 0.0f;	// テクスチャの左上X座標
	float ty = 0.0f;	// テクスチャの左上Y座標



	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
}

//=============================================================================
// プレイヤーの分身を描画
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG* bg = GetBG();
	float alpha = 0.0f;

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Player[no].texNo]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Player[no].offset[j].x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Player[no].offset[j].y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Player[no].w;		// プレイヤーの表示幅
		float ph = g_Player[no].h;		// プレイヤーの表示高さ

		// アニメーション用
		float tw = 1.0f / GetTexturePatternDivideX();	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float ty = (float)(g_Player[no].patternAnim / GetTexturePatternDivideX()) * th;	// テクスチャの左上Y座標
		float tx;
		if (g_Player->invertTex)
		{
			tw *= -1.0f;
			tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * (-1 * tw) - tw;	// テクスチャの左上X座標
		}
		else
			tx = (float)(g_Player->patternAnim % GetTexturePatternDivideX()) * tw;

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// ポリゴン描画
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
		// パターンの切り替え
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
		// パターンの切り替え
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
		// パターンの切り替え
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
		// パターンの切り替え
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

	// 上方向か下方向かを判断して一度に処理
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
	// 壁のAABB情報を取得
	AABB* walls = GetMap01AABB();
	for (int i = 0; i < MAP01_GROUND_MAX; i++)
	{
		if (walls[i].tag == GROUND_AABB) continue;

		XMFLOAT3 wallPos = walls[i].pos;
		float wallW = walls[i].w;
		float wallH = walls[i].h;

		
		// 衝突確認
		switch (dir)
		{
		case CHAR_DIR_LEFT:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x + move, g_Player->pos.y, g_Player->pos.z);

			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH) &&
				g_Player->bodyAABB.pos.x > wallPos.x)
			{
				// プレイヤーが左に移動していて、壁が左側にある場合
				SET_PLAYER_POS_X(wallPos.x + wallW / 2 + g_Player->bodyAABB.w / 2 + 0.01f); // 左への進行を停止
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
				// プレイヤーが右に移動していて、壁が右側にある場合
				SET_PLAYER_POS_X(wallPos.x - wallW / 2 - g_Player->bodyAABB.w / 2 - 0.01f); // 右への進行を停止
				return false;
			}
			break;
		}

		case CHAR_DIR_UP:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x, g_Player->pos.y + move, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH))
			{
				// プレイヤーが上に移動していて、壁が上側にある場合
				SET_PLAYER_POS_Y(wallPos.y + wallH / 2 + g_Player->bodyAABB.h / 2); // 上への進行を停止
				return false;
			}
			break;
		}
		case CHAR_DIR_DOWN:
		{
			XMFLOAT3 newPos = XMFLOAT3(g_Player->pos.x, g_Player->pos.y + move, g_Player->pos.z);
			if (CollisionBB(newPos, g_Player->bodyAABB.w, g_Player->bodyAABB.h, wallPos, wallW, wallH))
			{
				// プレイヤーが上に移動していて、壁が下側にある場合
				SET_PLAYER_POS_Y(wallPos.y - wallH / 2 - g_Player->bodyAABB.h / 2); // 下への進行を停止
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
	// プレイヤーのAABB情報を取得
	XMFLOAT3 playerPos = g_Player->bodyAABB.pos;
	float playerW = g_Player->bodyAABB.w;
	float playerH = g_Player->bodyAABB.h;

	// 地面のAABB情報を取得
	XMFLOAT3 groundPos = ground->pos;
	float groundW = ground->w;
	float groundH = ground->h;

	if (groundPos.y < playerPos.y) return false;

	// 衝突確認
	BOOL isColliding = CollisionBB(playerPos, playerW, playerH, groundPos, groundW, groundH);
	// プレイヤーが地面の上でどれくらい重なっているかを計算
	float overlapLeft = max(playerPos.x - playerW / 2, groundPos.x - groundW / 2);
	float overlapRight = min(playerPos.x + playerW / 2, groundPos.x + groundW / 2);
	float overlapWidth = overlapRight - overlapLeft;

	// 落ちる閾値
	float threshold = playerW / FALLING_THRESHOLD;

	// プレイヤーのy方向の速度が0より小さい（プレイヤーが落下中）場合、かつ衝突が発生し、重なりが閾値を超えている場合
	if (isColliding && g_Player->move.y >= 0 && overlapWidth >= threshold) 
	{
		// プレイヤーが落下中でかつ地面に十分接触している、落下を止め
		g_Player->move.y = 0.0f;

		// プレイヤーの滞空時間をリセットする
		g_Player->onAirCnt = 0;
		// ジャンプの状態からidle状態に戻る
		if (g_Player->jumpCnt > 0)
		{
			g_Player->playAnim = FALSE;
			g_Player->state = IDLE;
			g_Player->jumpCnt = 0;
		}
		// 空中ダッシュ状態をリセット
		g_Player->dashOnAir = FALSE;
		// 空中ダッシュ回数をリセット
		g_Player->airDashCount = 0;
		g_Player->jumpOnAirCnt = 0;

		
		// プレイヤーの位置を地面の上に調整し、地面を通り抜けないようにする
		SET_PLAYER_POS_Y(groundPos.y - groundH * 0.5f - playerH * 0.5f + 0.01f); // 0.01fのマージンを追加

		ground->tag = GROUND_AABB;
		return true;
	}
	else if (isColliding && overlapWidth < threshold) 
	{
		// プレイヤーが地面の上にあまり重なっていないので落下を続行
		// プレイヤーを少し押し出して、地面と衝突しないようにする
		if (playerPos.x < groundPos.x) 
		{
			// プレイヤーが左側にある場合
			CHANGE_PLAYER_POS_X(-1.0f);// 少し左に押し出す
		}
		else 
		{
			// プレイヤーが右側にある場合
			CHANGE_PLAYER_POS_X(1.0f);// 少し右に押し出す
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