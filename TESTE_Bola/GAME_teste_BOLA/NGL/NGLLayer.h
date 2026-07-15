/*NGL v1.0 - Nology Game Library 
Copyright (C) 2003, 2004 Nology Softwares SC Ltda. Todos os direitos reservados

O uso desta biblioteca de software em forma de código fonte ou arquivo binário, 
com ou sem modificações, é permitido contanto que sejam atendidas as seguintes condições:

1. A redistribuição deste software em qualquer forma, seu uso comercial 
e em treinamentos de qualquer natureza estão sujeitos a aprovação 
prévia por escrito da Nology Softwares, estando estas ações 
proibidas em quaisquer outras condições.

2. A alteração do conteúdo deste software está autorizada contanto que 
sejam mantidas as informações de copyright dos arquivos originais.

3. O uso deste software é permitido para uso educacional livre 
de quaisquer obrigações desde que seja atendida a condição número 1.

4. A Nology Softwares, detentora dos direitos autorais sobre este software, 
não oferece nenhuma garantia de funcionamento e não pode ser responsabilizada 
por quaisquer danos diretos, indiretos, acidentais, especiais ou 
específicos causados pelo uso deste software. 
*/
#ifndef _CNGLLAYER_
#define _CNGLLAYER_

//includes
#include "NGLobject.h"
#include "NGLvector.h"
#include "NGLbrick.h"
#include "NGLsprite.h"
#include "stlfiles.h"

//protótipos
class CNGLScene;

//!NGLBrickConfig
/*!
Usada para criar uma layer a partir de um bitmap.
Indica que, quando a cor for encontrada deve-se utilizar a
imagem e o quadro no brick
*/
typedef struct stNGLBrickConfig
{
	//!Cor a ser utilizada para esta configuração
	unsigned int	uiColor;
	//!indice da imagem na cena
	int						iImage;
	//!indice do quadro na imagem
	int						iFrame;
}NGLBrickConfig;

//!Classe CNGLLayer
/*!
	Controla a criação e exibição da layer no jogo.
*/
class CNGLLayer :	public CNGLObject
{
public:
	
	//!Ponteiro para a cena.
	CNGLScene	*pr_Scene;

	//!Número de bricks em x e y na layer.
	CNGLVector NumBricks;

	//!Tamanho dos bricks na layer.
	CNGLVector BrickSize;

	//!Layer offset - deslocamento da layer em relação ao vídeo.
	CNGLVector Offset;

	//!Velocidade da layer no Scroll.
	CNGLVector Speed;

	//!Vetor de Bricks (tijolos) que preenchem a layer.
	CNGLBrick	**p_Bricks;

	//!Lista de sprites da layer.
	vector<CNGLSprite*> Sprites;

	//!Flag que indica se a layer está visível ou não
	bool bVisible;

public:

	/*!
		Construtor padrão.
	*/
	CNGLLayer(void);

	/*!
		Destrutor padrão.
	*/
	virtual ~CNGLLayer(void);

	/*!
		Finaliza a classe.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool Release(void);

	/*!
	Atualiza a layer.
	*/
	void Update(void);

	/*!
	Desenha a layer.
	*/
	void Draw(void);

	/*!
	Cria um sprite e adiciona ele à layer.
	\param cp_chFileName: Imagem de origem do sprite.
	\param cFrameSize: Tamanho do quadro do sprite.
	\return Ponteiro para o sprite criado.
	*/
	CNGLSprite* CreateSprite(const char* cp_chFileName,const CNGLVector &cFrameSize);

	/*!
		Deleta um sprite e o remove da layer.
		\param p_Sprite: referência ao ponteiro para um sprite que será desalocado.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool DeleteSprite(CNGLSprite **p_Sprite);

	/*!
		Faz o Scroll (deslocamento) da layer.
		\param cScroll: Tamanho do deslocamento.
	*/
	void Scroll(const CNGLVector & cScroll);

	/*!
		Cria os bricks da layer.
		\param cNumBricks: Tamanho da Matriz de Bricks (ex: 40x40).
	*/
	void CreateBricks(const CNGLVector &cNumBricks);

	/*!
		Apaga todos os bricks da layer.
	*/
	void ClearBricks(void);

	/*!
		Ajusta a configuração de um brick.
		\param cBrickPos: posição do brick no vetor de Bricks.
		\param iImage: indice da imagem na cena.
		\param iFrame: indice do quadro na imagem.
	*/
	void SetBrick(const CNGLVector& cBrickPos, int iImage, int iFrame);
	
	/*!
		Cria uma layer com bricks a partir de um bitmap.
		\param cp_chFileName: imagem que contém a disposição dos bricks.
		\param pr_BrickConfigs: configuração de cada brick (onde se associa a cor do pixel com o brick correspondente).
		\param iConfigCount: número de configurações de bricks.
	*/
	void CreateBricksFromBitmap(const char * cp_chFileName, stNGLBrickConfig* pr_BrickConfigs, int iConfigCount);

	/*!
		Retorna o tamanho da layer em pixels
		\return Vetor com o tamanho da layer em pixels(x,y)
	*/
	CNGLVector GetLayerSize(void);

	/*!
		Retorna a posição na layer dada a posição no back buffer
		\param BkBuffPos: Posição no back buffer
		\return Vetor com a posiçao na layer
	*/
	CNGLVector GetLayerPos(const CNGLVector &BkBuffPos);
	/*!
		Retorna a posição no back buffer dada a posição na layer e o tamanho
		do quadro do objeto que utilizará esta posição.
		\param LayerPos: Posição na layer
		\param FrameSize: Tamanho do quadro do objeto 
	*/
	CNGLVector GetBackBufferPos(const CNGLVector &LayerPos, const CNGLVector &FrameSize = CNGLVector(0.0f,0.0f));
};

#endif