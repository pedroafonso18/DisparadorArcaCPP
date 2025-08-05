#include "../../include/Database/Fetch.h"

#include <iostream>

const std::vector<EntradaClt> Fetch::FetchBolsa() {
    std::vector<EntradaClt> vec;
    auto c = db->getConnection();
    if (!c->is_open()) {
        std::clog << "ERROR: db connection is not open.\n";
        return vec;
    }

    pqxx::work transaction(*c);
    const std::string query = R"(SELECT DISTINCT ON (telefone)
		carteira_assinada, tempo_emprego, valor_desejado, nome, cpf, telefone, tag, produto, processado
		FROM entrada_clt
		WHERE processado = FALSE AND produto = 'bolsa-familia'
		AND tag != 'base'
		ORDER BY telefone, random()
		LIMIT 45;)";
    try {
        auto result = transaction.exec(pqxx::zview(query));
    	if (!result.empty()) {
    		for (auto row: result) {
    			EntradaClt e;
    			e.CarteiraAssinada = row[0].as<std::string>();
    			e.TempoEmprego = row[1].as<std::string>();
    			e.ValorDesejado = row[2].as<std::string>();
    			e.Nome = row[3].as<std::string>();
    			e.Cpf = row[4].as<std::string>();
    			e.Telefone = row[5].as<std::string>();
    			e.Tag = row[6].as<std::string>();
    			e.Produto = row[7].as<std::string>();
    			e.Processado = row[8].as<std::string>();

    			vec.push_back(e);
    		}
    		return vec;
    	}

    	auto res2 = transaction.exec(pqxx::zview(R"(SELECT DISTINCT ON (telefone)
			carteira_assinada, tempo_emprego, valor_desejado, nome, cpf, telefone, tag, produto, processado
			FROM entrada_clt
			WHERE processado = FALSE AND produto = 'bolsa-familia'
			AND tag = 'base'
			AND status_cref->'costumer_data'->>'erro' = 'false'
			AND status_cref->'costumer_data'->'objeto'->>'permiteCaptura' = 'true'
			AND status_cref->'costumer_data'->'objeto'->>'mensagem' = 'Prosseguir com simulação'
			ORDER BY telefone, random()
			LIMIT 45;)"));

    	if (!res2.empty()) {
    		for (auto row: result) {
    			EntradaClt e;
    			e.CarteiraAssinada = row[0].as<std::string>();
    			e.TempoEmprego = row[1].as<std::string>();
    			e.ValorDesejado = row[2].as<std::string>();
    			e.Nome = row[3].as<std::string>();
    			e.Cpf = row[4].as<std::string>();
    			e.Telefone = row[5].as<std::string>();
    			e.Tag = row[6].as<std::string>();
    			e.Produto = row[7].as<std::string>();
    			e.Processado = row[8].as<std::string>();

    			vec.push_back(e);
    		}
    		return vec;
    	}

    	return vec;

    } catch (pqxx::sql_error const &e) {
    	transaction.abort();
		return vec;
    }
}


std::string Fetch::FetchTemplates() const {
	auto c = db->getConnection();
	if (!c->is_open()) {
		std::clog << "ERROR: db connection is not open.\n";
		return "";
	}
	pqxx::work transaction(*c);
	const std::string query = R"(SELECT template_texto FROM templates_anas WHERE is_active = true LIMIT 1;)";
	try {
		const auto result = transaction.exec(pqxx::zview(query));
		transaction.commit();
		return result[0][0].as<std::string>();
	} catch (const pqxx::sql_error& e) {
		std::clog << "SQL error: " << e.what() << "\n";
		return "";
	} catch (const std::exception& e) {
		std::clog << "Error: " << e.what() << "\n";
		return "";
	}
}


const std::vector<Instance> Fetch::FetchInstances() {
	auto c = db->getConnection();
	std::vector<Instance> vec;
	if (!c->is_open()) {
		std::clog << "ERROR: db connection is not open.\n";
		return vec;
	}
	pqxx::work transaction(*c);
	const std::string query = "SELECT connection_name, connection_number, connection_token, daily_limit, sent_today FROM parametros_disparos WHERE is_active = true AND is_banned = false AND sent_today < daily_limit";
	try {
		auto res = transaction.exec(pqxx::zview(query));
		for (auto row: res) {
			Instance inst;
			inst.InstanceName = row[0].as<std::string>();
			inst.InstanceNumber = row[1].as<std::string>();
			inst.Token = row[2].as<std::string>();
			inst.DailyLimit = row[3].as<int>();
			inst.SentToday = row[4].as<int>();
			vec.push_back(inst);
		}
		return vec;
	} catch (const pqxx::sql_error& e) {
		std::clog << "SQL error: " << e.what() << "\n";
		return vec;
	} catch (const std::exception& e) {
		std::clog << "Error: " << e.what() << "\n";
		return vec;
	}
}

