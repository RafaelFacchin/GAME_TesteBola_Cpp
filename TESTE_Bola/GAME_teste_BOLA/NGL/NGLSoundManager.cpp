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
#include "NGLsoundmanager.h"
#include "NGLMain.h"


//---------------------------------------------------------------------------- 
// Nome: CNGLSoundManager(void)
// Desc: construtor padrão
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLSoundManager::CNGLSoundManager(void)
{
	p_DSound  = NULL;
	p_Primary = NULL;
}

//---------------------------------------------------------------------------- 
// Nome: ~CNGLSoundManager(void)
// Desc: destrutor padrão
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLSoundManager::~CNGLSoundManager(void)
{
	Release();
}

//---------------------------------------------------------------------------- 
// Nome: Release(void)
// Desc: finaliza o sound manager
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLSoundManager::Release(void)
{
	// limpa o vetor de musicas tocando
	PlayMusic.clear();

	// libera memoria de todos os sons alocados
	for (int i = (int)Sounds.size() - 1; i >=0; i--)
	{
		Sounds[i]->Release();
		delete Sounds[i];
		Sounds[i] = NULL;
	}
	// limpa o vetor de sons
	Sounds.clear();

	// deletando o buffer primário (Mixer)
	if (p_Primary)
	{
		p_Primary->Stop();
		p_Primary->Release();
		p_Primary = NULL;
	}
	// deletando o DirectSound
	if (p_DSound)
	{
		p_DSound->Release();
		p_DSound = NULL;
	}	

	return true;
}

//---------------------------------------------------------------------------- 
// Nome: DeleteSound(CNGLLayer **p_Layer)
// Desc: Deleta um som e retira do sound manager
// Pams: referência ao ponteiro do som
//---------------------------------------------------------------------------- 
bool CNGLSoundManager::DeleteSound(CNGLSound **p_Sound)
{
	if(p_Sound && *p_Sound)
	{
		for(int i = (int)Sounds.size()-1; i >=0; i--)
		{
			if(Sounds[i] == *p_Sound)
			{
				Sounds[i]->Release();
				delete Sounds[i];
				*p_Sound = NULL;
				Sounds.erase(Sounds.begin()+i);
				return true;
			}
		}
	}

	//objeto de som não encontrado
	return false;
}

//---------------------------------------------------------------------------- 
// Nome: LoadSound(char *cp_chFileName,bool bol bIsFX)
// Desc: Insere um novo som no vetor de sons do Manager o inicializa
// Pams: cp_chFileName nome do arquivo de som (.wav)
//       BIsFX é variavel que diz se o som é musica ou efeito sonoro.
//---------------------------------------------------------------------------- 
CNGLSound *CNGLSoundManager::LoadSound(char *cp_chFileName,bool bIsFX)
{
	//tenta encontrar uma som já lido
	// (varre o vetor de traz pra frente)
	for(int i = (int)Sounds.size()-1; i>=0; i--)
	{
		//Se já existe som alocado
		if(!Sounds[i]->FileName.compare(cp_chFileName))
		{
			// cria o objeto CNGLSound
			CNGLSound *p_Sound = new CNGLSound(this,cp_chFileName,bIsFX);
			// faz o ponteiro para som apontar para o mesmo
			//buffer de Sounds[i]
			p_Sound->DuplicateSound(Sounds[i]);
			// inclui o som no vetor stl de sons
			Sounds.push_back(p_Sound);
			return p_Sound;
		}
	}

	//Se não existe ainda o som, então aloca o objeto 
	// e carrega o arquivo de som
	CNGLSound *p_Sound = new CNGLSound(this,cp_chFileName,bIsFX);
	if(p_Sound)
	{
		if (p_Sound->LoadFromFile(cp_chFileName))
		{
			//Adiciona a lista
			Sounds.push_back(p_Sound);
		}
		else // não conseguiu carregar som. Deleta a classe e retorna null
		{
			delete p_Sound;
			p_Sound = NULL;
		}
	}
	//retorna o Som
	return p_Sound;
}

//---------------------------------------------------------------------------- 
// Nome: Initialize(void)
// Desc: Inicializa o DirectSound e cria o buffer primário 
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLSoundManager::Initialize(void)
{
	// Descritores para o Buffer Primário
	DSBUFFERDESC   stBufferDesc;
	WAVEFORMATEX   wfm;

	// Cria o principal objeto directsound
	if (DirectSoundCreate(NULL, &p_DSound,NULL) != DS_OK)
	{
		return false;
	}

	//ajusta cooperative level usado para interagir com outras aplicações windows
	if (p_DSound->SetCooperativeLevel((HWND)pr_Main->pr_Wnd,DSSCL_PRIORITY ) != DS_OK)
	{
		Release();
		return false;
	}
	
	//preenche descritor para buffer primário
	memset(&stBufferDesc,0,sizeof(DSBUFFERDESC));
	stBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	stBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	stBufferDesc.dwBufferBytes = 0;
	stBufferDesc.lpwfxFormat = NULL;

	// preenche estrutura do Buffer primário para o formato desejado
	memset(&wfm,0,sizeof(WAVEFORMATEX));
	wfm.wFormatTag			= WAVE_FORMAT_PCM;  // sem compressão
	wfm.nChannels				= 2;                // stereo
	wfm.nSamplesPerSec	= 44100;						// 44.1 Khz
	wfm.wBitsPerSample	= 16;								// 16 bits de som
	wfm.nBlockAlign			= wfm.wBitsPerSample * wfm.nChannels / 8; // bytes lidos por vez
	wfm.nAvgBytesPerSec	= wfm.nSamplesPerSec * wfm.nBlockAlign; // média de bytes por segundo

	// cria o buffer primário
	if (p_DSound->CreateSoundBuffer(&stBufferDesc,&p_Primary,NULL) != DS_OK)
	{
		Release();
		return false;
	}	

	// Ajusta o formato para o mixer (buffer primário)	
	if (p_Primary->SetFormat(&wfm) != 0)
	{
		Release();
		return false;
	}
	
	// faz o buffer primário tocar continuamente
	if (p_Primary->Play(0,0,DSBPLAY_LOOPING) != DS_OK)
	{
		Release();
		return false;
	}

	return true; // tudo inicializado com sucesso!
}

//---------------------------------------------------------------------------- 
// Nome: Update(void)
// Desc: rotina que é chamada a cada laço, faz o update do som de musica
//       quando o play cursor chegar na metade, atualiza a primeira porção
//       quando o play cursor chegar no fim, atualiza a segunda porção
// Pams: nenhum
//---------------------------------------------------------------------------- 
void CNGLSoundManager::Update(void)
{
	// se houver musica tocando, então vamos dar um "refresh"
	for (int iIndex = (int)PlayMusic.size() - 1 ; iIndex >= 0 ; iIndex--)
	{
		CNGLSound *p_Music = PlayMusic[iIndex];
		DWORD dwPlay        = 0;
		DWORD dwMidBuffer   = 0;
		DWORD dwStartOfs    = 0;
		void         *lpData       = NULL;
		DWORD dwBytesLocked = 0;
		DWORD dwTotalLocked = 0;
			
		// pega a posicão do play cursor no buffer de som
		p_Music->p_Buffer->GetCurrentPosition(&dwPlay,&dwStartOfs);
			
		// copia o valor da metade do buffer (só pra facilitar)
		dwMidBuffer = p_Music->GetMidOfBuffer();

		// trata fim do som, não precisa mais preencher com dados.
		if (p_Music->bSameSeg)
		{
			if ((dwPlay >= p_Music->uiEndMark) ||
				 (dwPlay > dwMidBuffer && p_Music->uiEndMark < dwMidBuffer) ||
				 (dwPlay < dwMidBuffer && p_Music->uiEndMark > dwMidBuffer))
			{
				p_Music->Stop();
				continue;
			}
			continue;
		}
		// se tem que atualizar o buffer...
		if (((dwPlay >= dwMidBuffer) && (p_Music->uiLastPlayPos < dwMidBuffer)) || (dwPlay < p_Music->uiLastPlayPos))
		{
			dwStartOfs = (dwPlay >= dwMidBuffer)? 0: dwMidBuffer;
			
			// se um marco já foi dado ao segmento, então o dwplay está no mesmo segmento para finalizar
			// enquanto isso, já limpa o outro pedaço do segmento
			if (p_Music->uiEndMark)
			{
				p_Music->bSameSeg = true;
				continue;
			}
			// enquanto não travou toda a metade do buffer...
			while (dwTotalLocked < dwMidBuffer)				
			{
				// tranca a metade do buffer
				p_Music->p_Buffer->Lock(dwStartOfs+dwTotalLocked,
											dwMidBuffer - dwTotalLocked,
											&lpData,
											&dwBytesLocked,
											NULL,
											NULL,
											0);		
				// verifica se não chegou ao fim do arquivo
				if ((p_Music->uiWavCurPos + dwBytesLocked) < p_Music->GetDataSize())
				{
					fread((unsigned char *)lpData,dwBytesLocked,sizeof(unsigned char),p_Music->p_File);
					p_Music->uiWavCurPos += dwBytesLocked;
				}
				else // alcançou o fim
				{
					unsigned int uiRestBytes;
					// reseta uma metade do buffer
					memset((Byte *)lpData,0,sizeof(Byte)*dwBytesLocked);
					// pega a quantidade de bytes que faltam para fim de arquivo;
					uiRestBytes = p_Music->GetDataSize() - p_Music->uiWavCurPos;
					// le o pedaço de som da wave que faltava
					fread((unsigned char *)lpData,uiRestBytes,sizeof(unsigned char),p_Music->p_File);
					// faz o ponteiro do arquivo voltar para o inicio dos dados
					p_Music->SeekToWaveData();

					// musica deve estar em loop? sim - não retira da playMusic List.
					if (!p_Music->bMusicLoop)
					{
						p_Music->uiEndMark = dwStartOfs + p_Music->GetDataSize() - p_Music->uiWavCurPos;
					}
					else
					{
						// lê a primeira parte da musica para concatenar com o fim do som
						fread((unsigned char*)lpData + uiRestBytes,dwBytesLocked - uiRestBytes,sizeof(unsigned char),p_Music->p_File);
						// atualiza posição do cursor na wave
						p_Music->uiWavCurPos = dwBytesLocked - uiRestBytes;
					}
				}
				// destrava o pedaço de buffer trancado
				p_Music->p_Buffer->Unlock(lpData,dwBytesLocked,NULL,0);
				// soma ao total travado
				dwTotalLocked += dwBytesLocked;
			} // end while
		} // end if play half buffer
	// atualiza a ultima posicao do play cursor.
	p_Music->uiLastPlayPos = dwPlay;
	} //  end for
}
