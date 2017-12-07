#include "Game.h"



Game::Game()
{
	pPlayer = new Player;
	pZombie = new Zombie;
	pCollision = new CollisionJudgment;
	pSceneManager = new SceneManager;
}


Game::~Game()
{

}
void Game::Update()
{
	pSceneManager->SceneUpdate();
	pPlayer->Update();
	pZombie->Update();
	bool Hit = pCollision->RectangleCollosion(GetPlayer()->GetPos(), GetPlayer()->GetSize(), GetZombie()->GetPos(), GetZombie()->GetSize());
}

void Game::Draw()
{
	DirectGraphics::GetpInstance()->StartRender();
	pSceneManager->SceneDraw();
	pPlayer->Draw();
	pZombie->Draw();
	DirectGraphics::GetpInstance()->EndRender();
}
void Game::Run()
{
	Update();
	Draw();
}