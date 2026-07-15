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
#include "NGLinputmanager.h"
#include "NGLMain.h"
#include "windows.h"

//Essa macro retorna se uma tecla está pressionada. No DirectInput cada tecla é representada por um Byte.
//Caso o bit mais alto deste Byte estiver em 1, quer dizer que a tecla está pressionada (por isso se faz um AND com 0x80).
#define NGLKEYDOWN(KeyBuffer, key) (KeyBuffer[key] & 0x80)

//Essa macro retorna se um botão do mouse está pressionado. No DirectInput cada botão é representado por um Byte.
//Caso o bit mais alto deste Byte estiver em 1, quer dizer que o botão está pressionado (por isso se faz um AND com 0x80).
#define NGLMOUBTNDOWN(MouBuffer, btn) (MouBuffer.rgbButtons[btn] & 0x80)

//---------------------------------------------------------------------------- 
// Nome: CNGLInputManager(void)
// Desc: construtor padrão
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLInputManager::CNGLInputManager(void)
{
	p_DInput			= NULL;
	p_DevMouse		= NULL;
	p_DevKeyboard	= NULL;
	pr_Main				= NULL;
	ZeroMemory(KeyboardState,sizeof(KeyboardState));
	ZeroMemory(LastKeyboardState,sizeof(LastKeyboardState));	
	ZeroMemory(&MouseState,sizeof(MouseState));
	ZeroMemory(&LastMouseState,sizeof(LastMouseState));
}

//---------------------------------------------------------------------------- 
// Nome: ~CNGLInputManager(void)
// Desc: destrutor padrão
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLInputManager::~CNGLInputManager(void)
{
	Release();
}

//---------------------------------------------------------------------------- 
// Nome: Release(void)
// Desc: finaliza o input manager
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::Release(void)
{
	//liberando o teclado
	if(p_DevKeyboard)
  { 
	  //Sempre chamar unacquire de dispositivo antes de chamar o Release()
		p_DevKeyboard->Unacquire();
		p_DevKeyboard->Release();
    p_DevKeyboard = NULL;
	}

	//liberando o mouse
	if(p_DevMouse)
  { 
	  //Sempre chamar unacquire de dispositivo antes de chamar o Release()
		p_DevMouse->Unacquire();
		p_DevMouse->Release();
    p_DevMouse = NULL;
	}

	//liberando o objeto DirectInput (esse deve ser o último a ser destruído
	//por causa da ordem de alocação).
	if(p_DInput)
	{   
		p_DInput->Release();
		p_DInput = NULL;
  } 

	return true;
}

//---------------------------------------------------------------------------- 
// Nome: Initialize(void)
// Desc: Inicia o teclado e mouse, usando DirectInput.
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::Initialize(void)
{
	//inicia objeto do DirectInput
	if(FAILED(DirectInputCreate((HINSTANCE)pr_Main->pr_Inst,DIRECTINPUT_VERSION,&p_DInput, NULL)))
  {
		//erro de criação do objeto DirectInput
    return false;
  }

	//cria o teclado
	CreateKeyboard();
	
	//cria o mouse
	CreateMouse();

	return true;
}

//---------------------------------------------------------------------------- 
// Nome: CreateKeyboard(void)
// Desc: Cria o dispositivo de teclado.
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::CreateKeyboard(void)
{
	if(p_DInput)
	{
		//criando dispositivo de teclado
		if(FAILED(p_DInput->CreateDevice(GUID_SysKeyboard,&p_DevKeyboard,NULL)))
		{
			return false;
		}
		
		//configurando formato de dados do teclado
		if(FAILED(p_DevKeyboard->SetDataFormat(&c_dfDIKeyboard))) 
		{
			return false;
		}

		//deve-se fazer a primeira requisição de leitura do dispositivo
		p_DevKeyboard->Acquire();

		//tudo foi criado sem erros
		return true;
	}

	//objeto do Direct Input não foi criado...
	return false;
}

//---------------------------------------------------------------------------- 
// Nome: CreateMouse(void)
// Desc: Cria o dispositivo de mouse.
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::CreateMouse(void)
{
	if(p_DInput)
	{
		//criando dispositivo de teclado
		if(FAILED(p_DInput->CreateDevice(GUID_SysMouse,&p_DevMouse,NULL)))
		{
			return false;
		}
		
		//configurando formato de dados do teclado
		if(FAILED(p_DevMouse->SetDataFormat(&c_dfDIMouse))) 
		{
			return false;
		}

		//deve-se fazer a primeira requisição de leitura do dispositivo
		p_DevMouse->Acquire();

		//seta posição inicial do mouse no centro da tela
		MousePos.fx = pr_Main->VideoManager.VideoSize.fx/2;
		MousePos.fy = pr_Main->VideoManager.VideoSize.fy/2;

		//tudo foi criado sem erros
		return true;
	}

	//objeto do Direct Input não foi criado...
	return false;
}

//---------------------------------------------------------------------------- 
// Nome: Read(void)
// Desc: Lê entradas do teclado e mouse.
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::Read(void)
{
	if(p_DevKeyboard)
	{
		if(!ReadKeyboard())
		{
			return false;
		}
	}
	if(p_DevMouse)
	{
		if(!ReadMouse())
		{
			return false;
		}
	}

	return true;
}


//---------------------------------------------------------------------------- 
// Nome: ReadKeyboard(void)
// Desc: Lê dados do teclado.
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::ReadKeyboard(void)
{
	HRESULT hr;

	//copia o estado atual para o estado anterior
	memcpy(LastKeyboardState,KeyboardState,sizeof(LastKeyboardState));	

	//zera os estados do vetor
	ZeroMemory(KeyboardState,sizeof(KeyboardState));
	
	//pega o estado corrente do teclado
	hr = p_DevKeyboard->GetDeviceState(sizeof(KeyboardState),(LPVOID)&KeyboardState);
	
	//verifica se deu erro
	if(FAILED(hr))
	{ 
		//se falhou, provavelmente o dispositivo foi perdido (ex: a aplicação perdeu o foco).
		
		//Então deve-se tentar recuperá-lo aqui.
		hr = p_DevKeyboard->Acquire();
		
		//laço até conseguir acesso ao dispositivo
		while(hr == DIERR_INPUTLOST)
		{
			hr = p_DevKeyboard->Acquire();
		}

		//houve algum outro erro qualquer
		if(FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------- 
// Nome: ReadMouse(void)
// Desc: Lê dados do mouse.
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::ReadMouse(void)
{
	HRESULT hr;

	//copia o estado atual para o estado anterior
	memcpy(&LastMouseState,&MouseState,sizeof(LastMouseState));	

	//zera os estados do vetor
	ZeroMemory(&MouseState,sizeof(MouseState));
	
	//pega o estado corrente do mouse
	hr = p_DevMouse->GetDeviceState(sizeof(MouseState),(LPVOID)&MouseState);
	
	//verifica se deu erro
	if(FAILED(hr))
	{ 
		//se falhou, provavelmente o dispositivo foi perdido (ex: a aplicação perdeu o foco).
		//Então deve-se tentar recuperá-lo aqui.
		hr = p_DevMouse->Acquire();
		
		//laço até conseguir acesso ao dispositivo
		while(hr == DIERR_INPUTLOST)
		{
			hr = p_DevMouse->Acquire();
		}

		//houve algum outro erro qualquer
		if(FAILED(hr))
		{
			return false;
		}
	}

	if(pr_Main->VideoManager.WindowFrame.GetWidth() > 0 && pr_Main->VideoManager.WindowFrame.GetHeight() > 0)
	{
		//converte a coordenada do mouse do Windows para a coordenada equivalente no BackBuffer
		CNGLVector NewPos     = MouseWindowsPos;

		NewPos.fx = (NewPos.fx*pr_Main->VideoManager.VideoSize.fx)/(float)pr_Main->VideoManager.WindowFrame.GetWidth();
		NewPos.fy = (NewPos.fy*pr_Main->VideoManager.VideoSize.fy)/(float)pr_Main->VideoManager.WindowFrame.GetHeight();

		//essa é a nova posição do mouse
		MousePos = NewPos;
	}

	return true;
}

//---------------------------------------------------------------------------- 
// Nome: IsMouseOn(void)
// Desc: Retorna verdadeiro se o mouse estiver ligado, senão retorna falso
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLInputManager::IsMouseOn(void)
{
	return (p_DevMouse != NULL);
}

//---------------------------------------------------------------------------- 
// Nome: KeyDown(const Byte cbyKeyCode)
// Desc: Verifica se uma tecla está abaixada.
// Pams: código da tecla
//---------------------------------------------------------------------------- 
bool CNGLInputManager::KeyDown(const Byte cbyKeyCode)
{
	return (NGLKEYDOWN(KeyboardState,cbyKeyCode)) ? true : false;
}

//---------------------------------------------------------------------------- 
// Nome: KeyPressed(const Byte cbyKeyCode)
// Desc: Verifica se uma tecla foi pressionada neste instante.
// Pams: código da tecla
//---------------------------------------------------------------------------- 
bool CNGLInputManager::KeyPressed(const Byte cbyKeyCode)
{
	//se o estado anterior da tecla for não pressionado e o estado atual for pressionado,
	//logo a tecla foi pressionada exatamente neste momento.
	return (!NGLKEYDOWN(LastKeyboardState,cbyKeyCode) && NGLKEYDOWN(KeyboardState,cbyKeyCode)) ? true : false;
}

//---------------------------------------------------------------------------- 
// Nome: KeyReleased(const Byte cbyKeyCode)
// Desc: Verifica se uma tecla foi largada neste instante.
// Pams: código da tecla
//---------------------------------------------------------------------------- 
bool CNGLInputManager::KeyReleased(const Byte cbyKeyCode)
{
	//se o estado anterior da tecla for pressionado e o estado atual for não pressionado,
	//logo a tecla foi largada exatamente neste momento.
	return (NGLKEYDOWN(LastKeyboardState,cbyKeyCode) && !NGLKEYDOWN(KeyboardState,cbyKeyCode)) ? true : false;
}

//---------------------------------------------------------------------------- 
// Nome: MouBtnDown(Byte cbyBtnCode)
// Desc: Verifica se um botão do mouse está abaixado.
// Pams: código do botão (ex: 0, 1, 2)
//---------------------------------------------------------------------------- 
bool CNGLInputManager::MouBtnDown(const Byte cbyBtnCode)
{	
	return (NGLMOUBTNDOWN(MouseState,cbyBtnCode)) ? true : false;
}

//---------------------------------------------------------------------------- 
// Nome: MouBtnPressed(Byte cbyBtnCode)
// Desc: Verifica se um botão do mouse foi pressionado neste instante.
// Pams: código do botão (ex: 0, 1, 2)
//---------------------------------------------------------------------------- 
bool CNGLInputManager::MouBtnPressed(const Byte cbyBtnCode)
{
	return (!NGLMOUBTNDOWN(LastMouseState,cbyBtnCode) && NGLMOUBTNDOWN(MouseState,cbyBtnCode)) ? true : false;
}

//---------------------------------------------------------------------------- 
// Nome: MouBtnReleased(Byte cbyBtnCode)
// Desc: Verifica se um botão do mouse foi largado neste instante.
// Pams: código do botão (ex: 0, 1, 2)
//---------------------------------------------------------------------------- 
bool CNGLInputManager::MouBtnReleased(const Byte cbyBtnCode)
{
	return (NGLMOUBTNDOWN(LastMouseState,cbyBtnCode) && !NGLMOUBTNDOWN(MouseState,cbyBtnCode)) ? true : false;
}

//---------------------------------------------------------------------------- 
// Nome: SetWindowsMousePos(const CNGLVector &cNewPos)
// Desc: Ajusta a nova posição do mouse vinda da aplicação Windows.
// Pams: posição
//---------------------------------------------------------------------------- 
void CNGLInputManager::SetWindowsMousePos(const CNGLVector &cNewPos)
{
	MouseWindowsPos = cNewPos;
}
