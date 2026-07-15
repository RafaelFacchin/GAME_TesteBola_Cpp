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
#ifndef _CNGLTIMEHANDLER_
#define _CNGLTIMEHANDLER_

#include "NGLobject.h"
#include "stlfiles.h"

//!Classe CNGLTimeHandler
/*!
	Esta classe controla o relógio do jogo.
*/
class CNGLTimeHandler :	public CNGLObject
{
	
private:

	//!Armazena o último tempo obtido (em milisegundos).
	unsigned int uiLastTime;

	//!Tempo acumulado desde o último tempo obtido (em milisegundos).
	int iTimeAcum;

	//!Número de quadros que se passaram durante este período ou o número de chamadas da rotina Update().
	int	iFrameCount;

	//!Número de quadros por segundos.
	int	iFPS;

public:
	
	//!Ligar esse flag para prevenir slowdown.
	bool bPreventLowdown;

	//!Armazena o tempo passado desde o último quadro (em milisegundos).
	int iFrameTime;

	//!Armazena o tempo passado desde o último quadro (em segundos).
	float fFrameTime;

	/*!
		Recomeça a contagem de tempo.
	*/
	void Reset();

	/*! 
		Retorna o número de frames por segundo.
		\return Número de frames por segundo.
	*/
	int	GetFPS();
	
	/*!
		Atualiza o tempo. Este método deve ser chamado a cada quadro da aplicação.
	*/
	void Update();
	
	/*!
		Inicializa variáveis internas.
	*/
	void Initialize();
  
	/*! 
		Finaliza a classe.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool Release(void);
	
	/*! 
		Construtor padrão.
	*/
	CNGLTimeHandler(void);

	/*!
		Destrutor padrão.
	*/
	virtual ~CNGLTimeHandler(void);
};

#endif