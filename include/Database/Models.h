#ifndef INCLUDE_DATABASE_MODELS_H
#define INCLUDE_DATABASE_MODELS_H

#include <string>

typedef struct {
    std::string CarteiraAssinada;
    std::string TempoEmprego;
    std::string ValorDesejado;
    std::string Nome;
    std::string Cpf;
    std::string Telefone;
    std::string Tag;
    std::string Produto;
    std::string Processado;
} EntradaClt;

typedef struct {
    std::string InstanceName;
    std::string InstanceNumber;
    bool Status;
    std::string Token;
    int Index;
} Instance;

typedef struct {
    std::string Source;
    std::string SourceName;
    std::string Destino;
    std::string Status;
    std::string Template;
} Message;

typedef struct {
    std::string Name;
    std::string Phone;
    std::string Tag;
} Trafego;

#endif //INCLUDE_DATABASE_MODELS_H
