#include "../../include/Database/Insert.h"

#include <iostream>

void Insert::InsertLog(Message *msg) const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("INSERT INTO logs_disparo (source, source_name, destino, status, template) VALUES ($1, $2, $3, $4, $5)"), pqxx::params(msg->Source, msg->SourceName, msg->Destino, msg->Status, msg->Template));
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when inserting Logs.\n";
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "Database error: " << std::string(e.what()) << '\n';
    }

    transaction.commit();
}

void Insert::InsertTrafegoLog(Trafego *trafego) const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("INSERT INTO logstrafegohuggy (name, phone, tag, timestamp) VALUES ($1, $2, $3, NOW())"), pqxx::params(trafego->Name, trafego->Phone, trafego->Tag));
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when inserting Trafego Logs.\n";
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "Database error: " << std::string(e.what()) << '\n';
    }

    transaction.commit();
}

void Insert::UpdateProcessado(const std::string &telefone) const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("UPDATE entrada_clt SET processado = true WHERE telefone = $1"), pqxx::params(telefone));
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when updating Processado.\n";
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "Database error: " << std::string(e.what()) << '\n';
    }

    transaction.commit();
}

void Insert::UpdateTabela(const std::string &telefone) const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("UPDATE parametros_disparos SET sent_today = sent_today + 1, last_message_time = NOW() WHERE connection_number = $1 "), pqxx::params(telefone));
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when updating Table.\n";
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "Database error: " << std::string(e.what()) << '\n';
    }
    transaction.commit();
}

void Insert::SetBanned(const std::string &telefone) const {
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("UPDATE parametros_disparos SET is_banned = true, is_active = false WHERE connection_number = $1 "), pqxx::params(telefone));
        if (!res.affected_rows() == 0) {
            std::clog << "ERROR: error when updating ban.\n";
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "Database error: " << std::string(e.what()) << '\n';
    }
    transaction.commit();
}
