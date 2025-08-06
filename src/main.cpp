#include <iostream>
#include <thread>
#include <random>

#include "../include/Config.h"
#include "../include/Utils.h"
#include "../include/Database/Fetch.h"
#include "../include/Validator.h"
#include "../include/Api/Api.h"
#include "../include/Database/Insert.h"

[[noreturn]] int main() {
    std::clog << "[main] Starting application" << std::endl;
    const Config cfg;
    const auto& env = cfg.GetEnv();
    std::clog << "[main] Entering main loop" << std::endl;
    while (true) {
        if (!Validator::ValidateTime()) {
            std::clog << "Not in working time, waiting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }
        std::clog << "[main] Validating connections" << std::endl;
        Validator::ValidateConns();
        std::clog << "[main] Fetching main_db" << std::endl;
        auto main_db = Fetch(env.DbUrl);
        std::clog << "[main] Fetching leads" << std::endl;
        auto leads = main_db.FetchBolsa();
        std::clog << "[main] Fetching template" << std::endl;
        const auto template_t = main_db.FetchTemplates();
        std::clog << "[main] Fetching v3_db" << std::endl;
        auto v3_db = Fetch(env.DbUrlV3);
        std::clog << "[main] Fetching instances" << std::endl;
        auto instances = v3_db.FetchInstances();
        std::clog << "[main] Fetching API connections" << std::endl;
        auto api_conns = Api::GetConnections(env.LoginArca, env.PasswordArca);
        auto v3_db_ins = Insert(env.DbUrlV3);
        for (const auto& api_conn : api_conns) {
            v3_db_ins.InsertInstance(api_conn);
        }
        std::clog << "[main] Preparing Insert objects" << std::endl;
        auto main_db_ins = Insert(env.DbUrl);
        size_t instanceIndex = 0;
        size_t totalInstances = instances.size();
        for (auto it = leads.begin(); it != leads.end(); ++it) {
            std::clog << "[main] Processing lead: " << it->Telefone << std::endl;
            main_db_ins.UpdateProcessado(it->Telefone);
            auto phone = Utils::FormatPhone(it->Telefone);
            const auto& instance = instances[instanceIndex];
            std::clog << "[main] Using instance: " << instance.InstanceNumber << std::endl;
            v3_db_ins.UpdateTabela(instance.InstanceNumber);
            instanceIndex = (instanceIndex + 1) % totalInstances;
            Request req = {
                .ApiKey = instance.Token,
                .Number = phone,
                .Body = template_t
            };
            std::clog << "[main] Sending message via API" << std::endl;
            auto status = Api::SendMessageApi(&req);
            auto msg = Message {
                .Source = instance.InstanceNumber,
                .SourceName = instance.InstanceName,
                .Destino = phone,
                .Status = status ? "sucesso" : "erro",
                .Template = template_t
            };
            std::clog << "[main] Inserting log into v3_db_ins" << std::endl;
            v3_db_ins.InsertLog(&msg);
            auto tra = Insert(env.DbUrlTrafego);
            if (status) {
                std::clog << "[main] Message sent successfully, inserting trafego log" << std::endl;
                auto tr = Trafego {
                    .Name = it->Nome,
                    .Phone = phone,
                    .Tag = it->Tag
                };
                tra.InsertTrafegoLog(&tr);
            } else {
                std::clog << "[main] Message failed to send" << std::endl;
            }
            std::clog << "[main] Sleeping ~10 seconds (+/-10%) before next lead" << std::endl;
            {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0.9, 1.1);
                double factor = dis(gen);
                int sleep_ms = static_cast<int>(10000 * factor);
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
            }
        }
        std::clog << "[main] Sleeping ~13 minutes (+/-10%) before next main loop iteration" << std::endl;
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.9, 1.1);
            double factor = dis(gen);
            int sleep_ms = static_cast<int>(13 * 60 * 1000 * factor);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        }

    }
}