#ifndef _CNGLSPRITE_
#define _CNGLSPRITE_

//includes
#include "NGLobject.h"
#include "NGLvector.h"
#include "NGLanimation.h"
#include "NGLimage.h"

//protótipos
class CNGLMain;
class CNGLLayer;

//!Classe CNGLSprite
/*! 
	Esta classe contém o sprite configurado, com posição, animações e espelhamentos.
*/
class CNGLSprite :	public CNGLObject
{
public:

	//!Indica se o sprite está visível ou não.
	bool			bVisible;

	//!Ponteiro para o CNGLMain.
	CNGLMain	*pr_Main;

	//!Referência para a layer à qual este sprite pertence.
	CNGLLayer	*pr_Layer;

	//!Posição (x,y) do sprite na layer.
	CNGLVector Position;

	//!Lista de animações.
	vector<CNGLAnimation*> Animations;

	//!Indica a animação corrente do sprite.
	short sCurrentAnim;

	//!Referência para a imagem utilizada pelo sprite.
	CNGLImage *pr_Image;

	//!Indica o tipo de espelhamento (NGLMIRROR_NONE, NGLMIRROR_LEFTRIGHT, NGLMIRROR_UPDOWN).
	Byte byMirror;

public:
	/*!
	Construtor padrão.
	*/
	CNGLSprite(void);

	/*!
	Destrutor padrão.
	*/
	virtual ~CNGLSprite(void);

	/*!
		Finaliza a classe.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool Release(void);

	/*!
		Cria o sprite.
		\param pc_FileName: Nome do arquivo da imagem fonte.
		\param cFrameSize: Tamanho do quadro na imagem fonte.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool Create(const char *pc_FileName,const CNGLVector &cFrameSize);

	/*!
		Desenha o sprite no BackBuffer.
	*/
	void Draw(void);

	/*!
		Atualiza o sprite
	*/
	void Update(void);

	/*!
	Adiciona uma animação ao sprite.
	\param iFPS: Número de quadros por segundo.
	\param bRepeat: Indica se a animação é contínua ou finita.
	\param iFrameCount: Número de quadros da animação.
	\param ...: Índices dos quadros da animação.
	*/
	void AddAnimation(int iFPS, bool bRepeat, int iFrameCount,...);

	/*!
	Reinicia a animação corrente.
	*/
	void ResetAnimation(void);

	/*!
	Ajusta a animação corrente.
	\param sAnim: Indice da nova animação.
	*/
	void SetCurrentAnimation(short sAnim);

	/*!
	Exclui todas as animações do vetor "Animations".
	*/
	void ClearAnimations(void);

	/*!
	Retorna um clone do sprite.
	\param bAddToLayer: Flag indicando que o sprite deve ser adicionado a layer. Caso seja verdadeiro
	a layer automaticamente desalocará este clone. Senão deve-se deletar este clone usando "delete".
	\return Retorna um ponteiro para o sprite clonado.
	*/
	CNGLSprite *GetClone(bool bAddToLayer);

	/*!
		Retorna o frame atual do sprite.
		\return Retorna um CNGLFrame contendo o frame atual.
	*/
	CNGLFrame GetFrame(void);
};

#endif