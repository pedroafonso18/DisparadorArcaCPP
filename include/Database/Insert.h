#ifndef INCLUDE_DATABASE_INSERT_H
#define INCLUDE_DATABASE_INSERT_H

#include "Database.h"
#include "Models.h"
#include "../Api/Models.h"

class Insert {
private:
    std::unique_ptr<Database> ownedDb;
    Database *db;
public:
    explicit Insert(Database& db) : db(&db) {}
    explicit Insert(const std::string& Url) : ownedDb(std::make_unique<Database>(Url)), db(ownedDb.get()) {}
    void InsertLog(Message* msg) const;
    void InsertTrafegoLog(Trafego* trafego) const;
    void UpdateProcessado(const std::string& telefone) const;
    void UpdateTabela(const std::string& telefone) const;
    void SetBanned(const std::string& telefone) const;
    void InsertInstance(const Instance_t& instance) const;
};

#endif //INCLUDE_DATABASE_INSERT_H