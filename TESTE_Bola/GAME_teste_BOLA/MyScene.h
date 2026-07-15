#ifndef MYSCENE.H
#define MYSCENE.H

#include "NGL/NGLMain.h"
#include "NGL/NGLScene.h"

class CMyScene : public CNGLScene
{
	//SPRITE da bola
	CNGLSprite *p_Sprite;

	//GRAVIDADE
	float fGravity;

	//VETOR VELOCIDADE
	CNGLVector Speed;

public:

	//***CONSTRUTOR-PADRAO
	CMyScene(void);

	//DESTRUTOR
	~CMyScene(void);

	//INICIA a cena
	bool Initialize(void);

	//execute a cena
	void Execute(void);
};

#endif //MYSCENE.H
