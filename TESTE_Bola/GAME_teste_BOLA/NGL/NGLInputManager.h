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
#ifndef _CNGLINPUTMANAGER_
#define _CNGLINPUTMANAGER_

#ifndef DIRECTINPUT_VERSION
	#define DIRECTINPUT_VERSION         0x0300
#endif

#include "dinput.h"
#include "NGLobject.h"
#include "NGLVector.h"

class CNGLMain;

//!Número de teclas do teclado
#define NGLNUM_KEYS													256

//!Classe CNGLInputManager
/*!
	Gerencia as atividades de Input (teclado e mouse).
*/
class CNGLInputManager : public CNGLObject
{
public:

	//!Ponteiro de referência para a CNGLMain
	CNGLMain						*pr_Main;
	
	/*!
		Construtor padrão.
	*/
	CNGLInputManager(void);

	/*!
		Destrutor padrão.
	*/
	virtual ~CNGLInputManager(void);

	/*!
		Finaliza a classe.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool Release(void);

	/*!
		Inicia o teclado e mouse, usando DirectInput.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/
	bool Initialize(void);

	/*!
		Lê entradas do teclado e mouse.
		\return verdadeiro se a operação foi bem sucedida, caso contrário retorna falso.
	*/	
	bool Read(void);

	/*!
		Retorna posição atual do cursor mouse.
		\return CNGLVector contedo a posição do cursor do mouse.
	*/	
	inline CNGLVector GetMousePos() { return MousePos; };

private:
	/*!
		Lê dados do teclado.
		\return verdadeiro se conseguiu ler, senão retorna falso.
	*/
	bool ReadKeyboard(void);
	/*!
		Lê dados do mouse.
		\return verdadeiro se conseguiu ler, senão retorna falso.
	*/
	bool ReadMouse(void);

private:
	//!Objeto do DirectInput.
	IDirectInput					*p_DInput;

	//!Dispositivo de teclado do DirectInput.
	IDirectInputDevice		*p_DevKeyboard;

	//!Dispositivo de mouse do DirectInput.
	IDirectInputDevice		*p_DevMouse;

	//!Estrutura de dados que guarda estado atual do teclado.
	Byte KeyboardState[NGLNUM_KEYS];

	//!Estrutura de dados que guarda estado anterior do teclado. Usada para verificar se uma tecla foi recém pressionada ou largada.
	Byte LastKeyboardState[NGLNUM_KEYS];

	//!Estrutura de dados que guarda estado atual do mouse.
	DIMOUSESTATE MouseState;

	//!Estrutura de dados que guarda estado anterior do mouse.  Usada para verificar se um botão foi recém pressionado ou largado.
	DIMOUSESTATE LastMouseState;

	//!Posição atual do mouse.
	CNGLVector MousePos;

	//!Posição do mouse vinda da aplicação Windows.
	CNGLVector MouseWindowsPos;

public:
	/*!
		Cria o dispositivo de teclado.
		\return verdadeiro se criou com sucesso, senão retorna falso.
	*/
	bool CreateKeyboard(void);

	/*!
		Cria o dispositivo de mouse.
		\return verdadeiro se criou com sucesso, senão retorna falso.
	*/
	bool CreateMouse(void);

	/*!
		Verifica se o mouse está ligado.
		\return verdadeiro se o mouse estiver ligado, senão retorna falso.
	*/
	bool IsMouseOn(void);

	/*!
		Verifica se uma tecla está pressionada.
		\param cbyKeyCode: código da tecla. Usar tabela de constantes de teclas do DirectInput.
		\return verdadeiro se a tecla estiver pressionada, senão retorna falso.
	*/
	bool KeyDown(const Byte cbyKeyCode);

	/*!
		Verifica se uma tecla foi pressionada neste instante.
		\param cbyKeyCode: código da tecla. Usar tabela de constantes de teclas do DirectInput.
		\return verdadeiro se a tecla foi pressionada neste instante, senão retorna falso.
	*/
	bool KeyPressed(const Byte cbyKeyCode);

	/*!
		Verifica se uma tecla foi largada neste instante.
		\param cbyKeyCode: código da tecla. Usar tabela de constantes de teclas do DirectInput.
		\return verdadeiro se a tecla foi largada neste instante, senão retorna falso.
	*/
	bool KeyReleased(const Byte cbyKeyCode);

	/*!
		Verifica se um botão do mouse está pressionado.
		\param cbyBtnCode: código do botão (ex: 0, 1, 2)
		\return verdadeiro se o botão estiver pressionado, senão retorna falso.
	*/
	bool MouBtnDown(const Byte cbyBtnCode);

	/*!
		Verifica se um botão do mouse foi pressionado neste instante.
		\param cbyBtnCode: código do botão (ex: 0, 1, 2).
		\return verdadeiro se o botão foi pressionado neste instante, senão retorna falso.
	*/
	bool MouBtnPressed(const Byte cbyBtnCode);

	/*!
		Verifica se um botão do mouse foi largado neste instante.
		\param cbyBtnCode: código do botão (ex: 0, 1, 2).
		\return verdadeiro se o botão foi largado neste instante, senão retorna falso.
	*/
	bool MouBtnReleased(const Byte cbyBtnCode);	

	/*!
		Ajusta a nova posição do mouse vinda da aplicação Windows.
		\param cNewPos: CNGLVector contendo a nova posição do mouse.
	*/
	void SetWindowsMousePos(const CNGLVector &cNewPos);
};

#endif