//#ifndef CMYSCENE.H;
//#define CMYSCENE.H;

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
	CMyScene();

	//DESTRUTOR
	~CMyScene();

	//INICIA a cena
	bool Initialize();

	//execute a cena
	void Execute();
};

//#endif //CMYSCENE.H
