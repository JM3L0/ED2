#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char matricula[7]; // 6 digitos + '\0'
    char nome[50];
    char funcao[30];
    float salario;
} Funcionario;

typedef struct
{
    Funcionario tabela[180]; // Tamanho máximo da tabela
    int tamanho;
    int colisoes;
} TabelaHash;

void inicializarTabela(TabelaHash *th, int tamanho)
{
    th->tamanho = tamanho;
    th->colisoes = 0;

    memset(th->tabela, 0, sizeof(th->tabela));
}

int hashingA(char *matricula, int tamanho)
{
    char rotacionado[7];
    strcpy(rotacionado, matricula + 2); // Copia a partir do 3º digito
    rotacionado[4] = matricula[0];      // Adiciona o 1º digito
    rotacionado[5] = matricula[1];      // Adiciona o 2º digito
    rotacionado[6] = '\0';

    char extraido[4];
    extraido[0] = rotacionado[1]; // 2º digito
    extraido[1] = rotacionado[3]; // 4º digito
    extraido[2] = rotacionado[4]; // 5º digito
    extraido[3] = '\0';

    int valor = atoi(extraido);
    return valor % tamanho;
}

int hashingB(char *matricula, int tamanho)
{
    char grupo1[4];
    grupo1[0] = matricula[0]; // 1º digito
    grupo1[1] = matricula[1]; // 2º digito
    grupo1[2] = matricula[5]; // 6º digito
    grupo1[3] = '\0';

    char grupo2[4];
    grupo2[0] = matricula[2]; // 3º digito
    grupo2[1] = matricula[3]; // 4º digito
    grupo2[2] = matricula[4]; // 5º digito
    grupo2[3] = '\0';

    int valor = atoi(grupo1) + atoi(grupo2);
    return valor % tamanho;
}

// Função geral para resolver colisões
int resolverColisao(char *matricula, int posicao, int tamanho, int metodo)
{
    if (metodo == 1)
    {
        // Método A: Soma o primeiro dígito da matrícula
        int incremento = matricula[0] - '0';
        return (posicao + incremento) % tamanho;
    }
    else
    {
        // Método B: Soma o valor formado pelo primeiro e sexto dígito
        char digitos[3];
        digitos[0] = matricula[0];
        digitos[1] = matricula[5];
        digitos[2] = '\0';

        int incremento = atoi(digitos);
        return (posicao + incremento) % tamanho;
    }
}

// Função para inserir um funcionário na tabela hash usando o método A
void inserirA(TabelaHash *th, Funcionario func)
{
    int posicao = hashingA(func.matricula, th->tamanho);
    int posicaoOriginal = posicao;
    int inserido = 0;

    // Se a posição estiver vazia, insere diretamente
    if (th->tabela[posicao].matricula[0] == '\0')
    {
        th->tabela[posicao] = func;
        inserido = 1;
    }

    if (inserido == 0)
    {
        // Ocorreu colisão
        th->colisoes++;

        // Tenta resolver com a estratégia de colisão
        int tentativas = 0;
        while (tentativas < th->tamanho && inserido == 0)
        {
            posicao = resolverColisao(func.matricula, posicao, th->tamanho, 1);

            // Se encontrou posição vazia, insere
            if (th->tabela[posicao].matricula[0] == '\0')
            {
                th->tabela[posicao] = func;
                inserido = 1;
            }

            // Próxima tentativa
            tentativas++;
        }

        // Se não conseguiu inserir, substitui a primeira posição encontrada
        if (inserido == 0)
        {
            th->tabela[posicaoOriginal] = func;
        }
    }
}

// Função para inserir um funcionário na tabela hash usando o método B
void inserirB(TabelaHash *th, Funcionario func)
{
    int posicao = hashingB(func.matricula, th->tamanho);
    int posicaoOriginal = posicao;
    int inserido = 0;

    // Se a posição estiver vazia, insere diretamente
    if (th->tabela[posicao].matricula[0] == '\0')
    {
        th->tabela[posicao] = func;
        inserido = 1;
    }

    if (inserido == 0)
    {
        // Ocorreu colisão
        th->colisoes++;

        // Tenta resolver com a estratégia de colisão
        int tentativas = 0;
        while (tentativas < th->tamanho && inserido == 0)
        {
            posicao = resolverColisao(func.matricula, posicao, th->tamanho, 2);

            // Se encontrou posição vazia, insere
            if (th->tabela[posicao].matricula[0] == '\0')
            {
                th->tabela[posicao] = func;
                inserido = 1;
            }

            // Próxima tentativa
            tentativas++;
        }

        // Se não conseguiu inserir, substitui a primeira posição encontrada
        if (inserido == 0)
        {
            th->tabela[posicaoOriginal] = func;
        }
    }
}

// Função para carregar os funcionários do arquivo
int carregarFuncionarios(const char *nomeArquivo, Funcionario *funcionarios, int maxFuncionarios)
{
    int count = 0;
    FILE *arquivo = fopen(nomeArquivo, "r");
    
    if (arquivo != NULL)
    {
        while (count < maxFuncionarios && fscanf(arquivo, "%6s %49[^\n] %29[^\n] %f", funcionarios[count].matricula, funcionarios[count].nome, funcionarios[count].funcao, &funcionarios[count].salario) == 4)
        {
            count++;
        }
        fclose(arquivo);
    }
    else
    {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
    }
    
    return count;
}

// Função para calcular a taxa de ocupação da tabela
float calcularTaxaOcupacao(TabelaHash *th)
{
    int ocupados = 0;
    for (int i = 0; i < th->tamanho; i++)
    {
        if (th->tabela[i].matricula[0] != '\0')
        {
            ocupados++;
        }
    }
    return (float)ocupados / th->tamanho * 100;
}

void buscarA(TabelaHash *th, char *matricula)
{
    int posicao = hashingA(matricula, th->tamanho);
    Funcionario *resultado = NULL;
    int encontrado = 0;

    // Se a posição não está vazia, verifica se é o funcionário procurado
    if (th->tabela[posicao].matricula[0] != '\0')
    {
        // Se encontrou na posição hash, retorna
        if (strcmp(th->tabela[posicao].matricula, matricula) == 0)
        {
            resultado = &th->tabela[posicao];
            encontrado = 1;
        }
    }
    else
    {
        encontrado = 1; // Posição vazia, não precisa procurar mais
    }

    if (encontrado == 0)
    {
        // Tenta as posições alternativas
        int tentativas = 0;

        while (tentativas < th->tamanho && encontrado == 0)
        {
            posicao = resolverColisao(matricula, posicao, th->tamanho, 1);

            // Se a posição está vazia, não encontrou
            if (th->tabela[posicao].matricula[0] == '\0')
            {
                encontrado = 1; // Termina a busca
            }
            else
            {
                // Se encontrou, guarda o resultado
                if (strcmp(th->tabela[posicao].matricula, matricula) == 0)
                {
                    resultado = &th->tabela[posicao];
                    encontrado = 1;
                }
            }

            tentativas++;
        }
    }

    if (resultado != NULL)
    {
        printf("Funcionário encontrado: %s - %s - %s - %.2f\n", resultado->matricula, resultado->nome, resultado->funcao, resultado->salario);
    }
    else
    {
        printf("Funcionário com matrícula %s não encontrado.\n", matricula);
    }
}

// Função para testar o desempenho do método A
void testarMetodoA(int tamanho, const char *nomeArquivo)
{
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    int sucesso = 0;
    
    if (numFuncionarios > 0)
    {
        // Mede o tempo de inserção
        clock_t inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++)
        {
            inserirA(&tabela, funcionarios[i]);
        }
        
        clock_t fim = clock();
        double tempo_insercao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        // Mede o tempo de busca (busca todos os funcionários)
        inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++)
        {
            buscarA(&tabela, funcionarios[i].matricula);
        }
        
        fim = clock();
        double tempo_busca = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        printf("Metodo A (tamanho %d):\n", tamanho);
        printf("Numero de colisoes: %d\n", tabela.colisoes);
        printf("Tempo de insercao: %.6f segundos\n", tempo_insercao);
        printf("Tempo de busca: %.6f segundos\n", tempo_busca);
        float taxa_ocupacao = calcularTaxaOcupacao(&tabela);
        printf("Taxa de ocupacao: %.2f%%\n\n", taxa_ocupacao);
        
        sucesso = 1;
    }
}

void testarMetodoB(int tamanho, const char *nomeArquivo)
{
    TabelaHash tabela;
    inicializarTabela(&tabela, tamanho);
    
    Funcionario funcionarios[10000];
    int numFuncionarios = carregarFuncionarios(nomeArquivo, funcionarios, 10000);
    int sucesso = 0;
    
    if (numFuncionarios > 0)
    {
        // Mede o tempo de inserção
        clock_t inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++)
        {
            inserirB(&tabela, funcionarios[i]);
        }
        
        clock_t fim = clock();
        double tempo_insercao = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        // Mede o tempo de busca (busca todos os funcionários)
        inicio = clock();
        
        for (int i = 0; i < numFuncionarios; i++)
        {
            buscarA(&tabela, funcionarios[i].matricula);
        }
        
        fim = clock();
        double tempo_busca = (double)(fim - inicio) / CLOCKS_PER_SEC;
        
        printf("Metodo B (tamanho %d):\n", tamanho);
        printf("Numero de colisoes: %d\n", tabela.colisoes);
        printf("Tempo de insercao: %.6f segundos\n", tempo_insercao);
        printf("Tempo de busca: %.6f segundos\n", tempo_busca);
        float taxa_ocupacao = calcularTaxaOcupacao(&tabela);
        printf("Taxa de ocupacao: %.2f%%\n\n", taxa_ocupacao);
        
        sucesso = 1;
    }
}

int main()
{
    const char *nomeArquivo = "funcionarios.txt";
    int tamanho = 180; // Tamanho da tabela hash
    int tamanho2 = 121; // Tamanho da tabela hash alternativa

    printf("Usando banco de dados existente: %s\n", nomeArquivo);
    
    printf("== Tamanho da tabela: %d ==\n", tamanho);
    testarMetodoA(tamanho, nomeArquivo);
    testarMetodoB(tamanho, nomeArquivo);

    printf("== Tamanho da tabela: %d ==\n", tamanho);
    testarMetodoA(tamanho, nomeArquivo);
    testarMetodoB(tamanho, nomeArquivo);

    return 0;
}