#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Funcao para gerar uma matricula aleatoria
void gerarMatricula(char* matricula) {
    for (int i = 0; i < 6; i++) {
        matricula[i] = '0' + (rand() % 10);
    }
    matricula[6] = '\0';
}

// Funcao para gerar uma funcao aleatoria
void gerarFuncao(char* funcao) {
    char funcoes[][30] = {"Analista", "Programador", "Gerente", "Diretor", "Contador", "Secretario", "Assistente", "Consultor", "Estagiario", "Coordenador"};
    
    strcpy(funcao, funcoes[rand() % 10]);
}

// Funcao para gerar um salario aleatorio
float gerarSalario() {
    return 1000.0f + (rand() % 9000);
}

// Funcao para criar um arquivo de banco de dados com 10000 funcionarios
void criarBancoDeDados(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    int sucesso = 0;
    
    if (arquivo != NULL) {
        srand(time(NULL));
        
        for (int i = 0; i < 10000; i++) {
            char matricula[7];
            char nome[50];
            char funcao[30];
            float salario;
            
            gerarMatricula(matricula);
            // Gera nomes sequenciais: Funcionario0001, Funcionario0002, etc.
            sprintf(nome, "Funcionario%04d", i+1);
            gerarFuncao(funcao);
            salario = gerarSalario();
            
            fprintf(arquivo, "%s;%s;%s;%.2f\n", matricula, nome, funcao, salario);
        }
        
        fclose(arquivo);
        printf("Banco de dados criado com sucesso: %s\n", nomeArquivo);
        sucesso = 1;
    }
    
    if (sucesso == 0) {
        printf("Erro ao criar o arquivo de banco de dados.\n");
    }
}

int main() {
    const char* nomeArquivo = "funcionarios.txt";
    
    // Verificar se o arquivo ja existe
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL) {
        fclose(arquivo);
        printf("Usando banco de dados existente: %s\n", nomeArquivo);
    } else {
        // Se nao existir, cria o arquivo de banco de dados
        criarBancoDeDados(nomeArquivo);
    }
    
    return 0;
}