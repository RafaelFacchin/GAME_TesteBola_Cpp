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
#include "NGLbrick.h"

//---------------------------------------------------------------------------- 
// Nome: CNGLBrick(void)
// Desc: construtor padrão
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLBrick::CNGLBrick(void)
{
	iImage = 0;
	iFrame = 0;
}

//---------------------------------------------------------------------------- 
// Nome: ~CNGLBrick(void)
// Desc: destrutor padrão
// Pams: nenhum
//---------------------------------------------------------------------------- 
CNGLBrick::~CNGLBrick(void)
{
}

//---------------------------------------------------------------------------- 
// Nome: Release(void)
// Desc: finaliza o brick 
// Pams: nenhum
//---------------------------------------------------------------------------- 
bool CNGLBrick::Release(void)
{
	return true;
}
