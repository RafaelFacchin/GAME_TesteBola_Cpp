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
#include "NGLsound.h"

#include "NGLSoundManager.h"

#include <windows.h> // constantes do windows (WORD, DWORD, etc...)
#include <mmreg.h>  // constantes multimidia do windows

//---------------------------------------------------------------------------- 
// Nome: CNGLSound(CNGLSoundManager *pr_SoundManagerPam,char pc_chFileNamePam,bool bIsFXPam)
// Desc: construtor da classe de som
//       ela já inicializa referencia pra SoundManager, Nome do Arquivo, e tipo de som
// Pams: Referencia para SoundManager, Nome do Arquivo, Flag se som é musica ou FX
//---------------------------------------------------------------------------- 
CNGLSound::CNGLSound(CNGLSoundManager *pr_SoundManagerPam,const char *cp_chFileNamePam,bool bIsFXPam)
{
	pr_SoundManager = pr_SoundManagerPam;
	FileName = cp_chFileNamePam;
	bIsFX = bIsFXPam;
	p_File = NULL;

	uiDataSize       = 0;    // tamanho dos dados (em bytes)
	uiWavCurPos		 = 0;    // Posicao do cursor de dados lidos na wav de som
	uiLastPlayPos    = 0;    // ultima posicao do play cursor no update sound
	uiMidBuffer      = 0;    // Contém o tamanho do buffer / 2 (streamming)
	p_Buffer         = NULL; // ponteiro para o buffer de som
	usChannels       = 0;    // numero de canais 
	uiFrequency      = 0;    // frequencia do som
	usBitsPerSamples = 0;    // bits (8 ou 16)
	uiAvgBytesPerSec = 0;    // media de bytes por segundo
	bMusicLoop       = false; // musica vai tocar em loop?
	uiEndMark        = 0; // flag para finalização streamming
	bSameSeg         = false;  // outro flag para finalização do streamming
}

//---------------------------------------------------------------------------- 
// Nome: ~CNGLSound(void)
// Desc: destrutor padrão (automaticamente executa a funcao release
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLSound::~CNGLSound(void)
{
	Release();
}

//---------------------------------------------------------------------------- 
// Nome: Release(void)
// Desc: finaliza um som
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLSound::Release(void)
{
	// se arquivo estiver aberto (streamming) então fecha
	if (p_File)
	{
		fclose(p_File);
	}
	// se existir um buffer alocado, então desaloca
	if (p_Buffer)
	{
		// para o som caso ele esteja tocando
		Stop();
		// libera o ponteiro do Buffer
		// DirectX cuida o Reference count
		p_Buffer->Release();
		p_Buffer = NULL;
	}
	return true;
}

//---------------------------------------------------------------------------- 
// Nome: LoadFromFile(char *cp_chFileName)
// Desc: le um arquivo wav de sons.
//       Abre o .wav
//       Verifica-se o tipo de som (musica ou FX)
//		 Cria o buffer DirectSoud conforme o tipo de som.
// Pams: ponteiro para vetor contante de caracteres
//---------------------------------------------------------------------------- 
bool CNGLSound::LoadFromFile(const char *cp_chFileName)
{
	if(!pr_SoundManager->p_DSound)
	{
		return false;
	}
	// estruturas necessárias para criar buffer de som
	WAVEFORMATEX wfm;             // windows audio format
	DSBUFFERDESC stBufferDesc;    // directx buffer descritor
	HRESULT hr;

	// header de uma Wave
	char	        chRId[4] = {0,0,0,0};
	unsigned int    uiRLen;
	char	        chWId[4] = {0,0,0,0};
	char	        chFId[4] = {0,0,0,0};
	unsigned int	uiFLen;
	unsigned short  usFormatTag;
	unsigned short  usBlockAlign;
	char	        chDId[4] = {0,0,0,0};
	
	
	// reseta WAVEFORMATEX e DSBUFFERDESC
	memset(&stBufferDesc,0,sizeof(DSBUFFERDESC));
	memset(&wfm,0,sizeof(WAVEFORMATEX));


	// abre o arquivo de som .wav
	p_File = fopen(cp_chFileName,"rb");

	if (p_File)
	{
		// começa a ler dados da wave
		//lê ID
		fread(&chRId,4,sizeof(char),p_File);
		// lê o tamanho do arquivo
		fread(&uiRLen,4,sizeof(char),p_File);
		// lê mais ID
		fread(&chWId,4,sizeof(char),p_File);
		// lê mais ID
		fread(&chFId,4,sizeof(char),p_File);
		// le o FLEn
		fread(&uiFLen,1,sizeof(unsigned int),p_File);
		// Le o FormatTag
		fread(&usFormatTag,1,sizeof(unsigned short),p_File);
		// le o numero de canais (1 - mono, 2 - stereo)
		fread(&usChannels,1,sizeof(unsigned short),p_File);
		// Lê a frequencia do som
		fread(&uiFrequency,1,sizeof(unsigned int),p_File);
		// Lê a média de bytes por segundo (Freq * Bits * nChannels)
		fread(&uiAvgBytesPerSec,1,sizeof(unsigned int),p_File);
		// lê o block Align ( canais * BitsPerSample / 8)
		fread(&usBlockAlign,1,sizeof(unsigned short),p_File);
		// lê o numero de bits por Sample ( 8 ou 16 bits)
		fread(&usBitsPerSamples,1,sizeof(unsigned short),p_File);
		// le mais um identificador
		fread(&chDId,4,sizeof(char),p_File);
		// lê o tamanho dos dados (somente dados, sem header)
		fread(&uiDataSize,1,sizeof(unsigned int),p_File);

		// preenche a estrutura WAVEFORMATEX necessária para criar
		// o buffer de som
		wfm.wBitsPerSample = usBitsPerSamples;
		wfm.nAvgBytesPerSec = uiAvgBytesPerSec;
		wfm.nBlockAlign = usBlockAlign;
		wfm.wFormatTag  = usFormatTag;
		wfm.nChannels   = usChannels;
		wfm.nSamplesPerSec = uiFrequency;
		wfm.cbSize = 0;

		// agora preenche a estrutura DSBUFFERDESC (descritor do Buffer de som)
		stBufferDesc.dwSize = sizeof(DSBUFFERDESC);
		stBufferDesc.dwBufferBytes = (bIsFX)? uiDataSize: wfm.nAvgBytesPerSec * 2;      // tamanho da wave (dados)
		stBufferDesc.lpwfxFormat = &wfm;              // seta referencia para WAVEFORMATEX
		stBufferDesc.dwFlags = (bIsFX)? DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME : DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;

		// Cria finalmente o sound Buffer (vazio)
		pr_SoundManager->p_DSound->CreateSoundBuffer(&stBufferDesc,&p_Buffer,NULL);

		// variaveis utilizadas para preencher buffer com som
		void *p_StartMemory = NULL;
		DWORD uiBytesLocked = 0;
		DWORD uiTotalLocked = 0;


		// se som for FX, cria de um jeito
		if (bIsFX)
		{
			// agora vamos travar o bufffer (lock) para preenchê-lo com dados (o som)
			while (uiTotalLocked < uiDataSize)
			{
				hr = p_Buffer->Lock(
									uiTotalLocked,  // inicio do lock no buffer
									uiDataSize - uiTotalLocked, // o quanto deve trancar
									&p_StartMemory, // memoria trancada
									&uiBytesLocked, // quanto trancou
										NULL,
									0,
									DSBLOCK_FROMWRITECURSOR);
				// le bytes (dados da wav para o sound buffer
				fread(p_StartMemory,uiBytesLocked,sizeof(unsigned char),p_File);
				// soma os bytes trancados ao total já trancado
				uiTotalLocked += uiBytesLocked;

				// libera a porção do buffer já preenchida com dados
				p_Buffer->Unlock(p_StartMemory,uiBytesLocked,NULL,0);
			} // repete a operação enquanto a quantidade de bytes trancadas é menor que o tamanho do buffer

			// fecha o arquivo, não precisamos mais.
			fclose(p_File);
			return true;
		}
		else   // música
		{
			// Clear Sound Buffer
			ClearSoundBuffer();
			// set Mid of Buffer
			uiMidBuffer = stBufferDesc.dwBufferBytes / 2;		
			return true;
		}
	}
	return false;
}



//---------------------------------------------------------------------------- 
// Nome: Stop()
// Desc: Pára de tocar um som, não faz o retorno do cursor de play
// caso outro play for dado, vai continuar de onde parou
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLSound::Stop(void)
{
	if(p_Buffer)
	{
		p_Buffer->Stop();
		if (!bIsFX)
		{
			// reseta variaveis utilizadas para finalizar o play streamming
			bSameSeg = false;
			uiEndMark = 0;

			// retira o som da lista de sons que estão tocando
			int iSize = (int)pr_SoundManager->PlayMusic.size();
			for (int i = 0; i < iSize; i++)
			{
				if (pr_SoundManager->PlayMusic[i] == this)
				{
					pr_SoundManager->PlayMusic.erase(pr_SoundManager->PlayMusic.begin()+i);
					break;
				}
			}
			// limpa o buffer
			ClearSoundBuffer();
			// reseta o File Pointer para o inicio da área de dados
			SeekToWaveData();
			return true;
		}
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------- 
// Nome: Play(bool bLoopPam)
// Desc: Começa a reproduzir o som.
// Pams: verdadeiro se som vai tocar em loop, falso caso contrário
//---------------------------------------------------------------------------- 
bool CNGLSound::Play(bool bLoopPam)
{
	HRESULT hr;

	// existe buffer de som?
	if (p_Buffer)
	{
		// som é do tipo efeito sonoro?
		if (bIsFX)
		{
			if (bLoopPam)
			{
				hr = p_Buffer->Play(0,0,DSBPLAY_LOOPING);
			}
			else
			{
				hr = p_Buffer->Play(0,0,0);
			}
			
		}
		else // som é do tipo música
		{
			if (IsPlaying()) // se som já estiver tocando, retorna
			{
				return false;
			}
			// insere no vetor de músicas tocando esta classe
			pr_SoundManager->PlayMusic.push_back(this);
			
			// já preenche o buffer com dados
			DWORD dwBytesLocked = 0;
			DWORD dwTotalLocked = 0;
			void  *p_Data       = NULL;

			while (dwTotalLocked < (uiMidBuffer * 2))				
			{
				// tranca todo o buffer e insere som
				p_Buffer->Lock(dwTotalLocked,
				  						 (uiMidBuffer * 2) - dwTotalLocked,
											 &p_Data,
											 &dwBytesLocked,
											 NULL,
											 NULL,
											 0);
				fread((unsigned char *)p_Data,dwBytesLocked,sizeof(unsigned char),p_File);
				uiWavCurPos += dwBytesLocked;
				// destrava o pedaço de buffer trancado
				p_Buffer->Unlock(p_Data,dwBytesLocked,NULL,0);
				// soma ao total travado
				dwTotalLocked += dwBytesLocked;
			}
			// começa a tocar o buffer
			hr = p_Buffer->Play(0,0,DSBPLAY_LOOPING);
			//seta loop
			bMusicLoop = bLoopPam;
		}
	}

	if (hr == DS_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//---------------------------------------------------------------------------- 
// Nome: SeekStartWaveData()
// Desc: Posiciona o File Pointer no inicio da área de dados da wave
//       necessário para streamming (som em loop)
// Pams: nenhum
//---------------------------------------------------------------------------- 
void CNGLSound::SeekToWaveData(void)
{
	if (p_File)
	{
		// reseta o file pointer
		fseek(p_File,0,SEEK_SET);
		// pula o reader e vai direto pra parte de dados
		// apenas variavel temporária (não vai ser usado)
		char temp[42];
		// lê todo o header da wave para poder pular.
		fread(temp,42,sizeof(unsigned char),p_File);
		// reseta a variavel que controla a posicao do cursor na wav (area de dados)
		uiWavCurPos = 0;
	}
}

//---------------------------------------------------------------------------- 
// Nome: ClearSoundBuffer(void)
// Desc: seta o buffer alocado com 0's (silencio)
// Pams: nenhum
//---------------------------------------------------------------------------- 
void CNGLSound::ClearSoundBuffer(void)
{
	if (p_Buffer)
	{
		// estrutura directx que pega informações sobre buffer de som
		DSBCAPS dsbCaps;
		HRESULT hr;
		// variáveis necessárias para lock no buffer
		void *p_StartMemory = NULL;
		DWORD dwBytesLocked = 0;
		DWORD dwTotalLocked = 0;
		
		// seta o buffer
		dsbCaps.dwSize = sizeof(DSBCAPS);
		p_Buffer->GetCaps(&dsbCaps);

		// coloca o cursor do play buffer na posição 0;
		p_Buffer->SetCurrentPosition(0);

		// enquanto o total trancado for menor que o tamanho do buffer, continua
		while (dwTotalLocked < dsbCaps.dwBufferBytes)
		{
			p_Buffer->Lock(
						dwTotalLocked,  // inicio do lock no buffer
						dsbCaps.dwBufferBytes - dwTotalLocked, // o quanto deve trancar
						&p_StartMemory, // memoria trancada
						&dwBytesLocked, // quanto trancou
			    		NULL,
						0,
						DSBLOCK_FROMWRITECURSOR);

			// limpa o buffer
			memset(p_StartMemory,0,sizeof(Byte)*dwBytesLocked);
			// soma ao que já foi trancado
			dwTotalLocked += dwBytesLocked;
			// destrava o buffer
			hr = p_Buffer->Unlock(p_StartMemory,dwBytesLocked,NULL,0);
		}	
	}
}


//---------------------------------------------------------------------------- 
// Nome: DuplicateSound(CNGLSound SoundSrc,CNGLSound SoundDest)
// Desc: Duplica um som, através de um outro já existente. o Ponteiro para
//       o buffer de som apenas sera uma referência para um buffer já criado
// Pams: Ponteiro para buffer Fonte, e ponteiro para buffer Destino
//---------------------------------------------------------------------------- 
bool CNGLSound::DuplicateSound(CNGLSound *p_SoundSrc)
{
	// copia todas as variáveis de uma classe para outra
	uiDataSize		  = p_SoundSrc->GetDataSize();
	usChannels		  = p_SoundSrc->GetNumberOfChannels();
	uiFrequency	  = p_SoundSrc->GetFrequency();
	usBitsPerSamples = p_SoundSrc->GetBitsPerSample();
	uiAvgBytesPerSec = p_SoundSrc->GetAvgBytesPerSec();
	uiMidBuffer      = p_SoundSrc->GetMidOfBuffer();


	// DirectSound faz a duplicação do buffer (reference count)
	if (pr_SoundManager->p_DSound->DuplicateSoundBuffer(p_SoundSrc->p_Buffer,&p_Buffer) == DS_OK)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------- 
// Nome: IsPlaying()
// Desc: Retorna se o som está sendo reproduzido.
// Pams: nenhum
//----------------------------------------------------------------------------
bool CNGLSound::IsPlaying()
{
	if (p_Buffer)
	{
		DWORD dwStatus;

		p_Buffer->GetStatus(&dwStatus);

		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
		{
			return true;
		}
		if ((dwStatus & DSBSTATUS_LOOPING) == DSBSTATUS_LOOPING)
		{
			return true;
		}
	}
	return false;
}