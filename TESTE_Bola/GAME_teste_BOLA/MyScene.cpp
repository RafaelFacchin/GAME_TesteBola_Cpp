#ifndef MYSCENE.CPP
#define MYSCENE.CPP

#include "MyScene.h"
#include "NGL/NGLMain.h"
#include "NGL/NGLMain.h"
#include "NGL/NGLScene.h"
#include "math.h"

//-----------------------------------------------------------------------------------------------------
// NAME: CMySceneBola(void)
// DESC: Construtor padrao
// Pams: nenhum
//-----------------------------------------------------------------------------------------------------
CMyScene::CMyScene(void)
{
}

//-----------------------------------------------------------------------------------------------------
// NAME: ~CMySceneBola(void)
// DESC: Destrutor
// Pams: nenhum
//-----------------------------------------------------------------------------------------------------
CMyScene::~CMyScene(void)
{
}

//-----------------------------------------------------------------------------------------------------
// NAME: Initialize(void)
// DESC: INICIA a cena
// Pams: nenhum
//-----------------------------------------------------------------------------------------------------
bool CMyScene:Initialize(void) 
{
	CNGLLayer *p_Layer = CreateLayer(CNGLVector(40, 40), CNGL(1, 1));
	p_Sprite = p_Layer->CreateSprite("Surfaces/spr_bola.bmp", CNGLVector(33, 33));

	fGravity = 700.0f;
	Speed = CNGLVector(100, 0);
	return true;
}

//-----------------------------------------------------------------------------------------------------
// NAME: execute(void)
// DESC: EXECUTA a cena
// Pams: nenhum
//-----------------------------------------------------------------------------------------------------
#define FTIME pr_Main->TimeHandler.fFrameTime
void CMyScene::Execute(void)
{
	/*MOVIMENTO EM X*/
	p_Sprite->Position.fx += Speed.fx * FTIME; 

	/*MOVIMENTO EM Y*/
	//S = S0 + V0*T + (A*T*T)/2
	p_Sprite->Position.fy += (Speed.fy * FTIME) + ((fGravity * FTIME * FTIME) * 0.5f);
	Speed.fy += fGravity * FTIME;
	Speed.fy = (Speed.fy > 600.0f) ? 600.0f : Speed.fy;
	if (p_Sprite->Position.fy > 420.0f) 
	{
		Speed.fy = -fabs(Speed.fy) * 0.7f;
		Speed.fx *= 0.95f;
		p_Sprite->Position.fy = 420.0f;
		if (Speed.fy > -30.0f)
		{
			Speed.fy = 0.0f;
		}
	}
}

#endif //MYSCENE.CPP