/*  Programa: Oito Rainhas
       Autor: Rafael Timbó Matos (RA 106228)
       Turma MC102Y
       Versao: a
       Concluída em: 07/06/2010
       Breve descricao:
          Este programa descobre 8 posicoes para rainhas em um tabuleiro
          de xadrez de modo que nenhuma ameace a outra
*/

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define FALSE 0
#define TRUE -1
#define CEL_TAM 75
#define TABULEIRO_DIMENSAO 600 //75 * 8

#define COR_BRANCO   makecol( 255, 255, 255)
#define COR_PRETO    makecol( 0  , 0  , 0  )

#define RUIM     makecol( 255, 0  , 0  )
#define TESTE    makecol( 0  , 255  , 255)
#define OK       CalculaCor(coluna + linha)

#define DELTA_TEMPO 90

int CalculaCor(int ij)
{
    if ( (ij) % 2 == 0 ) //casas com (i+j) par são brancas
        return COR_BRANCO;
    else return COR_PRETO;
}

void Sair()
{
    if (key[KEY_ESC]) exit(EXIT_SUCCESS);
}

void DesenhaRainha(BITMAP *rainha, int coluna, int linha)
//coloque a rainha na posição[linha][coluna]
{
    draw_sprite(screen, rainha, coluna *  CEL_TAM, linha * CEL_TAM);
}

void RetiraRainha(int coluna, int linha)
//retire a rainha da posição[linha][coluna]
{
    rectfill(screen, coluna * CEL_TAM, linha * CEL_TAM, coluna * CEL_TAM + CEL_TAM, linha * CEL_TAM + CEL_TAM, CalculaCor(linha + coluna) );
}

void Posicao(int cor,int coluna, int linha)
//indica a situacao da rainha da posicao[linha][coluna]
//TESTE => a rainha acabou de ser posta naquela posicao
//OK => A rainha nao esta ameacada e o programa avançará para a proxima linha
//RUIM => A rainha da posicao esta ameaçada ou nao foi possivel posicionar as 8 rainhas com uma rainha na posicao em questao
{
    rectfill(screen, coluna * CEL_TAM, linha * CEL_TAM, coluna * CEL_TAM + CEL_TAM, linha * CEL_TAM + CEL_TAM, cor );
}

void init(BITMAP **rainha)
//Carrega as configurações iniciais necessárias para o jogo
{
    int i, j
    ;
    allegro_init();
    install_keyboard();
    set_color_depth(desktop_color_depth());

    if ( (set_gfx_mode(GFX_AUTODETECT_WINDOWED, TABULEIRO_DIMENSAO, TABULEIRO_DIMENSAO, 0, 0) ) != 0) {
        allegro_message(allegro_error);
        exit(EXIT_FAILURE);
    }
    *rainha = load_bmp("Queen.bmp",NULL);
    set_window_title("Eight Queens Puzzle - Pressione ESC para sair");

    rectfill(screen, 0, 0, TABULEIRO_DIMENSAO, TABULEIRO_DIMENSAO, COR_BRANCO);
    for (i = 0 ; i < 8 ; i++)
        for ( j = 0 ; j < 8 ; j++)
            rectfill(screen, j * CEL_TAM, i * CEL_TAM, j * CEL_TAM + CEL_TAM, i * CEL_TAM + CEL_TAM, CalculaCor(i + j) );
}

void apresenteSolucao(int tabuleiro[8][8],int *total)
{
    allegro_message("Solucao encontrada!! Total: %2d \nPressione ENTER para continuar ou ESC para Sair.", ++(*total));
    //printf("Total De Solucoes: %2d\n", *total);
    while (!key[KEY_ENTER]) Sair();
}

int naoAmeacada(int tabuleiro[8][8], int linha, int coluna)
{
    int i, j, posicaoLegal = TRUE;
    i = linha - 1;
    while (i >= 0 && posicaoLegal)
    {
        posicaoLegal = !tabuleiro[i][coluna];
        i = i - 1;
    }
    i = linha - 1;
    j = coluna + 1;
    while (i >= 0 && j < 8 && posicaoLegal)
    {
        posicaoLegal = !tabuleiro[i][j];
        i = i - 1;
        j = j + 1;
    }
    i = linha - 1;
    j = coluna - 1;
    while (i >= 0 && j >= 0 && posicaoLegal)
    {
        posicaoLegal = !tabuleiro[i][j];
        i = i - 1;
        j = j - 1;
    }
    return posicaoLegal;
}

int coloqueRainha(int tabuleiro[8][8], int linha, BITMAP *rainha, int *total)
{
    int coluna = 0, boaPosicao = FALSE;
    if (linha >= 8) //Foi encontrada uma solucao
    {
        //apresente a solucao e procure a próxima
        apresenteSolucao(tabuleiro, total);
        RetiraRainha(coluna, linha);
        return FALSE;
    }
    else
    {
        while (coluna < 8 && !boaPosicao)
        {
            tabuleiro[linha][coluna] = TRUE;
            Posicao(TESTE,coluna, linha);
            DesenhaRainha(rainha, coluna, linha);
            rest_callback( DELTA_TEMPO/3, Sair);

            if (naoAmeacada(tabuleiro,linha,coluna))
            {
                Posicao(OK,coluna, linha);
                DesenhaRainha(rainha, coluna, linha);
                boaPosicao = coloqueRainha(tabuleiro,linha + 1, rainha, total);
            }
            if (!boaPosicao)
            {
                Posicao(RUIM, coluna, linha);
                DesenhaRainha(rainha, coluna, linha);
                rest_callback( DELTA_TEMPO/2, Sair);
                tabuleiro[linha][coluna] = FALSE;
                RetiraRainha(coluna,linha);
                coluna = coluna + 1;
            }
        }
        return boaPosicao;
    }
}

int main(void)
{
    int i, j, tabuleiro[8][8], totalsolucoes = 0;
    BITMAP *rainha;
    init(&rainha);
    for (i = 0; i < 8; i = i + 1)
        for (j = 0; j < 8; j = j + 1) tabuleiro[i][j] = FALSE;
    if (coloqueRainha(tabuleiro, 0, rainha, &totalsolucoes)) apresenteSolucao(tabuleiro, &totalsolucoes);
    else
    {
        //printf("Nao foram encontradas mais solucoes\n");
        allegro_message("Nao foram encontradas mais solucoes!");
    }
    return 0;
}
END_OF_MAIN();
