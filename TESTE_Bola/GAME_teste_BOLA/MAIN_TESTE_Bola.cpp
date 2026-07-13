#include <windows.h>

#include "NGLMain.h"
#include "MyScene.h"

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
			(g_pr_Main) ? g_pr_Main->VideoManager.UpdateFrame() : 0;
			break;
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
		//ATUALIZA o frame do video se a janela for redimensionada
			(g_pr_Main) ? g_pr_Main->VideoManager.UpdateFrame() : 0;
			return 0;
			break;
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
					(g_pr_Main) ? g_pr_Main->Timehandler.Reset() : 0;
					g_bActive = true;
				}
			}
			break;
		}
		case WM_EXITMENULOOP:
		{
		//Qndo sair do menu da janela
			(g_pr_Main) ? g_pr_Main->Timehandler.Reset() : 0;
			break;
		}
		case WM_EXITSIZEMOVE:
		{
		//Qndo PARAR de mover OU redimensionar a janela
			(g_pr_Main) ? g_pr_Main->Timehandler.Reset() : 0;
			break;
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