//#ifndef MAIN_TESTE_BOLA.CPP;
//#define MAIN_TESTE_BOLA.CPP;

#include <windows.h>

#include "NGL/NGLAnimation.h"
#include "NGL/NGLBrick.h"
#include "NGL/NGLFrame.h"
#include "NGL/NGLImage.h"
#include "NGL/NGLInputManager.h"
#include "NGL/NGLLayer.h"
#include "NGL/NGLMain.h"
#include "NGL/NGLObject.h"
#include "NGL/NGLScene.h"
#include "NGL/NGLSound.h"
#include "NGL/NGLSoundManager.h"
#include "NGL/NGLSprite.h"
#include "NGL/NGLTimeAccumulator.h"
#include "NGL/NGLTimeHandler.h"
#include "NGL/NGLVector.h"
#include "NGL/NGLVideoManager.h"
#include "NGL/StlFiles.h"
#include "CMyScene.h"


#pragma region

//VAR global que indica SE a janela do jogo esta ativa
bool g_bActive = true;

//PTR globla p/ a classe proncipal da LIB NGL 
CNGLMain *g_pr_Main = NULL;

//----------------------------------------------------------------------------------------------------------------------------------
//NOME: WindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//DESC: FUNCAO que trata as mensagens do windows recebidas pela aplicacao
//----------------------------------------------------------------------------------------------------------------------------------

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_ERASEBKGND:
		case WM_NCPAINT:
		case WM_SYNCPAINT:
		{
			if (g_pr_Main->VideoManager.bFullScreen)
			{
				//***SE estiver em tela CHEIA, retorna 0 p/ essas tres mensagens
				//(EVITA "flick" na tela cheia do jogo)
				return 0;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			//ATUALIZACAO DA posicao do mouse
			g_pr_Main->InputManager.SetWindowsMousePos(CNGLVector((float)LOWORD(lParam), (float)HIWORD(lParam)));
			break;
		}
		case WM_MOVE:
		{
		//ATUALIZA o frame CASO a janela seja movida
			//VERSAO ANTIGA
			/*(g_pr_Main) ? g_pr_Main->VideoManager.UpdateFrame() : 0;
			break;*/

			//VERSAO NOVA
			if (g_pr_Main) 
			{
				g_pr_Main->VideoManager.UpdateFrame();
				break;
			}

		}
		case WM_PAINT:
		{
		//SE A janela nao estiver ATIVA:
			if (g_pr_Main && !g_bActive)
			{
				//PEGA o estado de pausa do jogo
				bool bIsPaused = g_pr_Main->IsPaused();

				//O codigo abaixo PRINTA o jogo com ele pausado, pois nao 
				//deve haver atualizacoes de tempo e dos objetos
				if (!bIsPaused)
				{
					//PAUSA O JOGO (se o mesmo nao estiver PAUSADO)
					g_pr_Main->Pause();
				}
				//DESENHA O JOGO!!!!
				g_pr_Main->Loop();
				if (!bIsPaused)
				{
					//CONTINUA o jogo
					g_pr_Main->Resume();
				}
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
		}
		case WM_GETDLGCODE:
		{
		//AVISA AO WINDOWS QUE ESTE APLICATIVO REQUER TECLADO
			return DLGC_WANTALLKEYS;
			break;
		}
		case WM_SETCURSOR:
		{
		//FAZ o ponteiro do MOUSE desaparecer
		//quando o mouse estiver "sobre" a janela da aplicacao
			SetCursor(NULL);
			return TRUE;
			break;
		}
		case WM_SIZE:
		{
			//VERSAO ANTIGA! (VCR6)
			/*ATUALIZA o frame do video se a janela for redimensionada
			(g_pr_Main) ? g_pr_Main->VideoManager.UpdateFrame() : 0;
			return 0;
			break;*/

			//***VERSAO ATUALIZADA
			// Verifica se a janela não foi minimizada antes de atualizar o frame
			if (wParam != SIZE_MINIMIZED)
			{
				if (g_pr_Main)
				{
					g_pr_Main->VideoManager.UpdateFrame();
				}
			}
			return 0;
		}
		case WM_ACTIVATE:
		{
		//INFORMA SE a aplicacao ganhou ou perdeu o foco
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				//PERDA de FOCO
				g_bActive = false;
			}
			else
			{
				if (HIWORD(wParam) == 0)
				{
					//APLIC GANHOU FOCO
					//RESETAR o tempo da NGL p/ os objetos do jogo
					//mantenham-se na mesma posicao qndo ao janela for desativada

					//VERSAO ANTIGA
					/*(g_pr_Main) ? g_pr_Main->TimeHandler.Reset() : 0;
					g_bActive = true;*/

					//***VERSAO NOVA
					if (g_pr_Main) 
					{
						g_pr_Main->TimeHandler.Reset();
						g_bActive = true;
					}
				}
			}
			break;
		}
		case WM_EXITMENULOOP:
		{
		//Qndo sair do menu da janela
			//VERSOA ANTIGA
			/*(g_pr_Main) ? g_pr_Main->TimeHandler.Reset() : 0;
			break;*/

			//VERSAO NOVA
			if (g_pr_Main) 
			{
				g_pr_Main->TimeHandler.Reset();
				break;
			}
		}
		case WM_EXITSIZEMOVE:
		{
		//Qndo PARAR de mover OU redimensionar a janela
			//VERSAO ANTIGA
			/*(g_pr_Main) ? g_pr_Main->TimeHandler.Reset() : 0;
			break;*/

			//VERSAO NOVA
			if (g_pr_Main)
			{
				g_pr_Main->TimeHandler.Reset();
				break;
			}
		}
		case WM_SYSCOMMAND:
		{
			//PREVINE contra MOVIMENTACAO/REDIMENSIONAMENTO/MAXIMIXACAO
			// e tambem DESLIGAMENTO do MONITOR (PROTETOR DE TELA) DA JANELA ATIVA
			switch (wParam)
			{
				case SC_MOVE:
				case SC_SIZE:
				case SC_MAXIMIZE:
				case SC_MONITORPOWER:

				return TRUE;
			}
			break;
		}
		case WM_CLOSE:
		{
			//CASO seja aplicado/clicado o "X" da janela ou as teclas "ALT+F4"
			//deve-se FECHAR a aplicacao
			PostQuitMessage(0);
			return 0;
			break;
		}
	}

	//CHAMAR o tratamento padrao de MENSAGENS
	return DefWindowProc(hwnd, msg, wParam, lParam);
}//FIM FUNCAO: WindowProc

//----------------------------------------------------------------------------------------------------------------------------------
//NOME: WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//DESC: FUNCAO de entrada da aplicacao
//----------------------------------------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CNGLMain Main; //OBJ principal LIB NGL
	WNDCLASS winclass; //DESCREVE a classe da janela de apresentacao
	MSG msg; //ESTRUTURA de mensagem do windows
	HWND hwnd; //HANDLE da janela de aplicacao
	char chName[11] = "NGL_Window"; //NOME da janela de aplicacao

	//AJUSTE do ponteiro global da CNGLMain
	g_pr_Main = &Main;

	//COLOCA os parametros na janela a ser criada
	winclass.style         = CS_HREDRAW | CS_VREDRAW;
	//winclass.lpfwndProc    = WindowProc;//VERSAO VC6
	winclass.lpfnWndProc = WindowProc; //VERSAO VC +14
	winclass.cbClsExtra    = 0;
	//winclass.cbwndextra    = 0;//VERSAO VC6
	winclass.cbClsExtra    = 0;//VERSAO VC +14
	winclass.hInstance     = hInstance;
	winclass.hIcon         = NULL;
	winclass.hCursor       = NULL;
	winclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	winclass.lpszMenuName  = NULL;
	winclass.lpszClassName  = L"chName";

	//REGISTRO de classe
	if (!RegisterClass(&winclass)) 
	{
		return -1;
	}

	//AJUSTE de dimensao do video; PADRAO:(640 x 480)
	Main.VideoManager.VideoSize = CNGLVector(640, 480);

	//CRIA a janela PRINCIPAL do jogo, com a dimensao ajustada acima
	if (!(hwnd = CreateWindowEx(0,L"chName", L"NGL Game", WS_CAPTION | WS_SYSMENU | WS_POPUP | WS_VISIBLE, 0, 0, (int)Main.VideoManager.VideoSize.fx,
		(int)Main.VideoManager.VideoSize.fy,NULL,NULL,hInstance,NULL))) //***EDIT: +L"chName", +L"NGL Game" (Add o "L")
	{
		//RETORNA SE DER ERRO!!!
		return -1;
	}

	//ATRIBUI instancia e window handle da aplicacao (VALORES UTILIZADOS pelo DirecX)
	Main.pr_Inst = (void*)hInstance;
	Main.pr_Wnd = (void*)hwnd;

	//CENA do jogo
	CMyScene Scene;

	//ler arquivo de opções do jogo para a LIB NGL saber se 
	//deve iniciar em Janela ou Tela Cheia
	//Level.Menu.loadOptions(&Main);

	//ADICIONA 2 cenas a LIB NGL
	Main.AddScene(&Scene);

	//INICIALIZA o MAIN
	if (!Main.Initialize())
	{
		//RETORNA ERRO!
		Main.Release();
		return -1;
	}
#pragma endregion

	//LACO de recebimento e encaminhamento de mensagens do WINDOWS
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		//MENSAGEM DE SAIDA (TERMINA A APLICACAO)
		if (WM_QUIT == msg.message)
		{
			//CHAMADA de saida da NGL
			Main.ExitNGL();
		}
		//ENCAMINHA mensagem p/ o WindowProc p/ tratamento
		DispatchMessage(&msg);
	}

	//SE a janela estiver ATIVA
	if (g_bActive) 
	{
		//ATUALIZA a NGL
		Main.Loop();
	}
	else 
	{
		//SE a janela nao esta ativa, deve-se sempre reinicializar o tempo do jogo
		Main.TimeHandler.Reset();
	}

	DeleteObject(winclass.hbrBackground);
	ClipCursor(NULL);

	return 0;
}

//#endif // MAIN_TESTE_BOLA.CPP 