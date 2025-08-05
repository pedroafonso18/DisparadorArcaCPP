#include "../../include/Database/Insert.h"

#include <iostream>

void Insert::InsertLog(Message *msg) const {
    std::clog << "[Insert::InsertLog] Inserting log for destino: " << msg->Destino << ", status: " << msg->Status << std::endl;
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("INSERT INTO logs_disparo (source, source_name, destino, status, template) VALUES ($1, $2, $3, $4, $5)"), pqxx::params(msg->Source, msg->SourceName, msg->Destino, msg->Status, msg->Template));
        std::clog << "[Insert::InsertLog][SUCCESS] Log inserted for destino: " << msg->Destino << ", status: " << msg->Status << std::endl;
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "[Insert::InsertLog][DB ERROR] " << e.what() << " for destino: " << msg->Destino << ", status: " << msg->Status << std::endl;
        return;
    }
    transaction.commit();
    std::clog << "[Insert::InsertLog] Log inserted for destino: " << msg->Destino << std::endl;
}

void Insert::InsertTrafegoLog(Trafego *trafego) const {
    std::clog << "[Insert::InsertTrafegoLog] Inserting trafego log for phone: " << trafego->Phone << std::endl;
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("INSERT INTO logstrafegohuggy (name, phone, tag, timestamp) VALUES ($1, $2, $3, NOW())"), pqxx::params(trafego->Name, trafego->Phone, trafego->Tag));
        if (res.affected_rows() == 0) {
            std::clog << "[Insert::InsertTrafegoLog][ERROR] No rows inserted for phone: " << trafego->Phone << std::endl;
        } else {
            std::clog << "[Insert::InsertTrafegoLog][SUCCESS] Trafego log inserted for phone: " << trafego->Phone << std::endl;
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "[Insert::InsertTrafegoLog][DB ERROR] " << e.what() << " for phone: " << trafego->Phone << std::endl;
        return;
    }
    transaction.commit();
    std::clog << "[Insert::InsertTrafegoLog] Trafego log inserted for phone: " << trafego->Phone << std::endl;
}

void Insert::UpdateProcessado(const std::string &telefone) const {
    std::clog << "[Insert::UpdateProcessado] Updating processado for telefone: " << telefone << std::endl;
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec(pqxx::zview("UPDATE entrada_clt SET processado = true WHERE telefone = $1"), pqxx::params(telefone));
        if (!res.affected_rows() == 0) {
            std::clog << "[Insert::UpdateProcessado][ERROR] No rows updated for telefone: " << telefone << std::endl;
        } else {
            std::clog << "[Insert::UpdateProcessado][SUCCESS] Updated processado for telefone: " << telefone << std::endl;
        }
    }catch (pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "[Insert::UpdateProcessado][DB ERROR] " << e.what() << " for telefone: " << telefone << std::endl;
        return;
    }
    transaction.commit();
    std::clog << "[Insert::UpdateProcessado] Processado updated for telefone: " << telefone << std::endl;
}

void Insert::UpdateTabela(const std::string &telefone) const {
    std::clog << "[Insert::UpdateTabela] Updating tabela for telefone: " << telefone << std::endl;
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
    std::clog << "[Insert::UpdateTabela] Tabela updated for telefone: " << telefone << std::endl;
}

void Insert::SetBanned(const std::string &telefone) const {
    std::clog << "[Insert::SetBanned] Setting banned for telefone: " << telefone << std::endl;
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
    std::clog << "[Insert::SetBanned] Banned set for telefone: " << telefone << std::endl;
}

void Insert::InsertInstance(const Instance_t& instance) const {
    std::clog << "[Insert::InsertInstance] Checking if instance exists: " << instance.InstanceNumber << std::endl;
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return;
    }
    pqxx::work transaction(*c);
    try {
        pqxx::result res = transaction.exec_params(
            "SELECT 1 FROM parametros_disparos WHERE connection_number = $1 LIMIT 1", instance.InstanceNumber
        );
        if (res.empty()) {
            std::clog << "[Insert::InsertInstance] Instance not found, inserting: " << instance.InstanceNumber << std::endl;
            transaction.exec_params(
                "INSERT INTO parametros_disparos (connection_name, connection_number, is_active, is_banned, daily_limit, sent_today) VALUES ($1, $2, true, false, 1000, 0)",
                instance.InstanceName, instance.InstanceNumber
            );
            transaction.commit();
            std::clog << "[Insert::InsertInstance] Instance inserted: " << instance.InstanceNumber << std::endl;
        } else {
            std::clog << "[Insert::InsertInstance] Instance already exists: " << instance.InstanceNumber << std::endl;
            transaction.commit();
        }
    } catch (const pqxx::sql_error& e) {
        transaction.abort();
        std::clog << "Database error: " << e.what() << '\n';
    }
}
