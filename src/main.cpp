#include <iostream>
#include <thread>

#include "../include/Config.h"
#include "../include/Utils.h"
#include "../include/Database/Fetch.h"
#include "../include/Validator.h"
#include "../include/Api/Api.h"
#include "../include/Database/Insert.h"

[[noreturn]] int main() {
    const Config cfg;
    const auto& env = cfg.GetEnv();

    while (true) {
        if (!Validator::ValidateTime()) {
            std::clog << "Not in working time, waiting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        Validator::ValidateConns();

        auto main_db = Fetch(env.DbUrl);

        auto leads = main_db.FetchBolsa();

        const auto template_t = main_db.FetchTemplates();

        auto v3_db = Fetch(env.DbUrlV3);

        auto instances = v3_db.FetchInstances();

        auto main_db_ins = Insert(env.DbUrl);
        size_t instanceIndex = 0;
        size_t totalInstances = instances.size();
        auto v3_db_ins = Insert(env.DbUrlV3);


        for (auto it = leads.begin(); it != leads.end(); ++it) {
            main_db_ins.UpdateProcessado(it->Telefone);
            auto phone = Utils::FormatPhone(it->Telefone);
            const auto& instance = instances[instanceIndex];
            instanceIndex = (instanceIndex + 1) % totalInstances;

            Request req = {
                .ApiKey = instance.Token,
                .Number = phone,
                .Body = template_t
            };
            auto status = Api::SendMessageApi(&req);

            auto msg = Message {
                .Source = instance.InstanceNumber,
                .SourceName = instance.InstanceName,
                .Destino = phone,
                .Status = status ? "sucesso" : "erro",
                .Template = template_t
            };

            v3_db_ins.InsertLog(&msg);

            auto tra = Insert(env.DbUrlTrafego);

            if (status) {

                Trafego tr = Trafego {
                    .Name = it->Nome,
                    .Phone = phone,
                    .Tag = it->Tag
                };
                tra.InsertTrafegoLog(&tr);
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));


        }
        std::this_thread::sleep_for(std::chrono::seconds(5));

    }
}