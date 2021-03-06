#include "Player.h"



Player::Player()
{
	DirectGraphics::GetpInstance()->InitGraphics("image\\Character\\Character.png", GetTexture());
	DirectGraphics::GetpInstance()->InitGraphics("image\\Fire\\Fire.png", GetBulletTexture());
	WindowPos.x = 300;
	WindowPos.y = 300;
	Hp = 5;
	/* WorldPos;*/
	 Size.Width=90.0f;
	 Size.Hight=180.0f;
	 Speed=2.0f;
	 IsRight = false;
	 IsAtk = false;
	 IsMoving = false;
	 IsActive=true;
	 MovementX = 0;
	 MovementY = 0;
	Alive = true;
}


Player::~Player()
{
}

void Player::Draw()
{

	CUSTOMVERTEX player[4];
	static CUSTOMVERTEX PlayerVertex[4]
	{
		//{プレイヤーのX座標 ,    プレイヤーのY座標 ,  1固定,  ?,   カラー    ,tu,tv}
		{ -Size.Width / 2,-Size.Hight / 2, 1.f,1.f, 0xFFFFFFFF, 0.f, 0.f },
		{ +Size.Width / 2,-Size.Hight / 2, 1.f,1.f, 0xFFFFFFFF, PlayerTu, 0.f },
		{ +Size.Width / 2,+Size.Hight / 2, 1.f,1.f, 0xFFFFFFFF, PlayerTu, PlayerTv },
		{ -Size.Width / 2,+Size.Hight / 2, 1.f,1.f, 0xFFFFFFFF, 0.f, PlayerTv }

	};
	for (int i = 0; i < 4; ++i)
	{
		player[i] = PlayerVertex[i];
		player[i].x += WindowPos.x;
		player[i].y += WindowPos.y;

	}
	if (IsActive&&Alive)
	{
		static int StayCount = 0;
		static int MoveCount = 0;
		if (IsMoving == false)
		{

			static int const StayAnimationFreamMax = 60;

			if (StayCount >= StayAnimationFreamMax / 2 && StayCount < StayAnimationFreamMax)
			{
				for (int i = 0; i < 4; ++i)
				{
					player[i].tu += PlayerTu;

				}
			}

			if (IsRight)
			{
				//反転する関数を呼ぶ
				DirectGraphics::GetpInstance()->InvertedRight(player);
			}
			else
			{

			}
			if (StayCount == 60)
			{
				StayCount = 0;
			}
			++StayCount;
		}
		if (IsMoving)
		{

			static int const MoveAnimationFreamCount = 120;
			if (0 <= MoveCount && MoveCount < MoveAnimationFreamCount * 1 / 4)
			{
				for (int i = 0; i < 4; ++i)
				{
					player[i].tu += PlayerTu * 0;
					player[i].tv += PlayerTv * 1;

				}
			}
			if (MoveAnimationFreamCount * 1 / 4 <= MoveCount && MoveCount < MoveAnimationFreamCount * 2 / 4)
			{
				for (int i = 0; i < 4; ++i)
				{
					player[i].tu += PlayerTu * 1;
					player[i].tv += PlayerTv * 1;

				}
			}
			if (MoveAnimationFreamCount * 2 / 4 <= MoveCount && MoveCount < MoveAnimationFreamCount * 3 / 4)
			{
				for (int i = 0; i < 4; ++i)
				{
					player[i].tu += PlayerTu * 2;
					player[i].tv += PlayerTv * 1;

				}
			}
			if (MoveAnimationFreamCount * 3 / 4 <= MoveCount && MoveCount < MoveAnimationFreamCount * 4 / 4)
			{
				for (int i = 0; i < 4; ++i)
				{
					player[i].tu += PlayerTu * 1;
					player[i].tv += PlayerTv * 1;

				}
			}


			if (MoveCount == MoveAnimationFreamCount)
			{
				MoveCount = 0;
			}
			++MoveCount;

			if (!IsRight)
			{

			}
			if (IsRight)
			{
				//反転する関数を呼ぶ
				DirectGraphics::GetpInstance()->InvertedRight(player);


			}
		}
		if (IsMoving)
		{
			StayCount = 0;
		}
		else
		{
			MoveCount = 0;
		}

		DirectGraphics::GetpInstance()->Draw(&TextureID, player);
	}

}

void Player::Update()
{
	Control();
	Move();
	if (pBullet != NULL)
	{
		pBullet->Update(BulletTexture, WindowPos, IsRight, Size);
	}
	LifeOrDeath();

}

void Player::Control()
{
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_LEFT], DIK_LEFT);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_RIGHT], DIK_RIGHT);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_UP], DIK_UP);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_DOWN], DIK_DOWN);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_Z], DIK_Z);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_A], DIK_A);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_W], DIK_W);
	DirectInput::GetInstance()->KeyCheck(&Key[KEY_SPACE], DIK_SPACE);

	if (MovementX != 0 || MovementY != 0)
	{
		MovementX = 0;
		MovementY = 0;
	}

	if (Key[KEY_UP] == KEY_ON) {
		MovementY = -Speed;
	}
	if (Key[KEY_DOWN] == KEY_ON) {
		MovementY = +Speed;
	}
	if (Key[KEY_LEFT] == KEY_ON) {
		MovementX = -Speed;
		IsRight = false;
	}
	if (Key[KEY_RIGHT] == KEY_ON) {
		MovementX = +Speed;
		IsRight = true;
	}

	if (MovementX != 0 || MovementY != 0)
	{
		IsMoving = true;
	}
	else
	{
		IsMoving = false;
	}

	if (Key[KEY_Z] == KEY_ON) {
		IsAtk = true;
	}
	if (IsAtk)
	{
		static int AtkFcount = 0;
		++AtkFcount;
		if(AtkFcount==1)
		{
			pBullet = new Bullet(BulletTexture,WindowPos,IsRight,Size);
		}
		if (AtkFcount == 60)
		{
			AtkFcount = 0;
			IsAtk = false;
			delete pBullet;
			pBullet = NULL;
		}
	}
}

void Player::Move()
{
	WindowPos.x += MovementX;
	WindowPos.y += MovementY;

}

void Player::SetPlayerHp(int* Atk)
{
	Hp -= *Atk;
}
void Player::LifeOrDeath()
{
	if (Hp <= 0)
	{
		IsActive = false;
		Alive = false;
	}
}
	