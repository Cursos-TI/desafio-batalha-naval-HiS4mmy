#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_TAB 10

// ---------- Funções de habilidades ----------

void habilidade_cone(int ataque[TAM_TAB][TAM_TAB], int origem_l, int origem_c)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = -i; j <= i; j++)
        {
            int l = origem_l + i;
            int c = origem_c + j;
            if (l >= 0 && l < TAM_TAB && c >= 0 && c < TAM_TAB)
                ataque[l][c] = 1;
        }
    }
}

void habilidade_cruz(int ataque[TAM_TAB][TAM_TAB], int origem_l, int origem_c)
{
    int i;
    for (i = -2; i <= 2; i++)
    {
        int l = origem_l + i;
        int c = origem_c + i;

        // linha horizontal
        if (origem_l >= 0 && origem_l < TAM_TAB && c >= 0 && c < TAM_TAB)
            ataque[origem_l][c] = 1;

        // linha vertical
        if (l >= 0 && l < TAM_TAB && origem_c >= 0 && origem_c < TAM_TAB)
            ataque[l][origem_c] = 1;
    }
}

void habilidade_octaedro(int ataque[TAM_TAB][TAM_TAB], int origem_l, int origem_c)
{
    int i, j;
    for (i = -2; i <= 2; i++)
    {
        for (j = -2; j <= 2; j++)
        {
            if (
                (i == 0 && j >= -2 && j <= 2) ||
                (i == -1 && j >= -1 && j <= 1) ||
                (i == 1 && j >= -1 && j <= 1) ||
                (i == -2 && j == 0) ||
                (i == 2 && j == 0))
            {
                int l = origem_l + i;
                int c = origem_c + j;
                if (l >= 0 && l < TAM_TAB && c >= 0 && c < TAM_TAB)
                    ataque[l][c] = 1;
            }
        }
    }
}

// ---------- Função para mostrar o tabuleiro ----------

void mostrar_tabuleiro(int tabuleiro[TAM_TAB][TAM_TAB], int ataque[TAM_TAB][TAM_TAB])
{
    int i, j;
    printf("\n   ");
    for (i = 0; i < TAM_TAB; i++)
        printf(" %d ", i);
    printf("\n");

    for (i = 0; i < TAM_TAB; i++)
    {
        printf("%2d ", i);
        for (j = 0; j < TAM_TAB; j++)
        {
            if (tabuleiro[i][j] == 3 && ataque[i][j] == 1)
                printf(" * "); // acerto
            else if (tabuleiro[i][j] == 3)
                printf(" 3 "); // navio
            else if (ataque[i][j] == 1)
                printf(" x "); // tiro na água
            else
                printf(" . "); // água
        }
        printf("\n");
    }
}

// ---------- Função para gerar posições aleatórias de navios ----------
void embaralhar_navios(int tabuleiro[TAM_TAB][TAM_TAB])
{
    int i;
    int l, c, sentido;
    int navios_colocados = 0;

    while (navios_colocados < 4)
    {
        sentido = rand() % 4; // 0 = horizontal, 1 = vertical, 2 = diagonal ↘, 3 = diagonal ↗

        l = rand() % (TAM_TAB - 2);
        if (sentido == 3)
            c = 2 + rand() % (TAM_TAB - 2);
        else
            c = rand() % (TAM_TAB - 2);

        int pode_colocar = 1;
        for (i = 0; i < 3; i++)
        {
            int li = l + ((sentido == 1 || sentido == 2 || sentido == 3) ? i : 0);
            int ci;
            if (sentido == 0)
                ci = c + i;
            else if (sentido == 1)
                ci = c;
            else if (sentido == 2)
                ci = c + i;
            else
                ci = c - i;

            if (li < 0 || li >= TAM_TAB || ci < 0 || ci >= TAM_TAB || tabuleiro[li][ci] == 3)
            {
                pode_colocar = 0;
                break;
            }
        }

        if (pode_colocar)
        {
            for (i = 0; i < 3; i++)
            {
                int li = l + ((sentido == 1 || sentido == 2 || sentido == 3) ? i : 0);
                int ci;
                if (sentido == 0)
                    ci = c + i;
                else if (sentido == 1)
                    ci = c;
                else if (sentido == 2)
                    ci = c + i;
                else
                    ci = c - i;

                tabuleiro[li][ci] = 3;
            }
            navios_colocados++;
        }
    }
}

// ---------- Outras funções auxiliares ----------
void resetar_ataque(int ataque[TAM_TAB][TAM_TAB])
{
    for (int i = 0; i < TAM_TAB; i++)
        for (int j = 0; j < TAM_TAB; j++)
            ataque[i][j] = 0;
}

void limpar_buffer_stdin()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

// ---------- Programa Principal ----------
int main()
{
    srand((unsigned)time(NULL));

    int tabuleiro[TAM_TAB][TAM_TAB] = {0};
    int ataque[TAM_TAB][TAM_TAB] = {0};
    int escolha = -1, origem_l = 0, origem_c = 0;
    char continuar = 'N';

    embaralhar_navios(tabuleiro);

    printf("🚢 ~*~*~ BATALHA NAVAL: NIVEL MESTRE ~*~*~ 🚢\n");
    printf("💧 . = Agua | 3 = Navio | * = Acerto | x = Tiro na Agua\n\n");
    mostrar_tabuleiro(tabuleiro, ataque);

    printf("\nPressione ENTER para iniciar...");
    limpar_buffer_stdin();
    getchar();

    do
    {
        printf("\n=== 🎯 MENU DE HABILIDADES ===\n");
        printf("1️⃣  Cone (ataque em formato de cone)\n");
        printf("2️⃣  Cruz (ataque em formato de cruz)\n");
        printf("3️⃣  Octaedro (ataque em forma de diamante)\n");
        printf("0️⃣  Sair do jogo\n");
        printf("==============================\n");
        printf("Escolha sua habilidade: ");

        if (scanf("%d", &escolha) != 1)
        {
            printf("\nEntrada invalida. Digite um numero.\n");
            limpar_buffer_stdin();
            escolha = -1;
            continue;
        }

        limpar_buffer_stdin();
        resetar_ataque(ataque);

        switch (escolha)
        {
        case 1:
            origem_l = rand() % TAM_TAB;
            origem_c = rand() % TAM_TAB;
            habilidade_cone(ataque, origem_l, origem_c);
            printf("\n🌀 Habilidade CONE ativada em (%d, %d)!\n", origem_l, origem_c);
            break;
        case 2:
            origem_l = rand() % TAM_TAB;
            origem_c = rand() % TAM_TAB;
            habilidade_cruz(ataque, origem_l, origem_c);
            printf("\n❌ Habilidade CRUZ ativada em (%d, %d)!\n", origem_l, origem_c);
            break;
        case 3:
            origem_l = rand() % TAM_TAB;
            origem_c = rand() % TAM_TAB;
            habilidade_octaedro(ataque, origem_l, origem_c);
            printf("\n💎 Habilidade OCTAEDRO ativada em (%d, %d)!\n", origem_l, origem_c);
            break;
        case 0:
            printf("\n👋 Encerrando a batalha...\n");
            break;
        default:
            printf("\n⚠️ Opcao invalida!\n");
        }

        if (escolha != 0)
        {
            mostrar_tabuleiro(tabuleiro, ataque);
            printf("\nDeseja usar outra habilidade? (S/N): ");
            continuar = getchar();
            if (continuar == '\n')
                continuar = getchar();
            if (continuar >= 'a' && continuar <= 'z')
                continuar -= ('a' - 'A');
        }
        else
            continuar = 'N';

    } while (continuar == 'S');

    printf("\n🏁 Fim do jogo! Obrigado por jogar.\n");
    return 0;
}
