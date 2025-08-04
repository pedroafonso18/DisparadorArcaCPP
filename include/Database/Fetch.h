#ifndef INCLUDE_DATABASE_FETCH_H
#define INCLUDE_DATABASE_FETCH_H
#include <memory>

#include "Database.h"
#include "Models.h"

class Fetch {
private:
    std::unique_ptr<Database> ownedDb;
    Database* db;
public:
    explicit Fetch(Database& db) : db(&db) {}
    explicit Fetch(const std::string& Url) : ownedDb(std::make_unique<Database>(Url)), db(ownedDb.get()) {}
    const EntradaClt* FetchBolsa();
    const std::string FetchTemplates();
    const Instance* FetchInstances();
    const Instance* GetNextInstance();
};

#endif //INCLUDE_DATABASE_FETCH_H
