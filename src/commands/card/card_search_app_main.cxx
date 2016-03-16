/**
 * Copyright (C) 2015 Virgil Security Inc.
 *
 * Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     (1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *     (2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *     (3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <string>
#include <stdexcept>

#include <tclap/CmdLine.h>

#include <virgil/sdk/ServicesHub.h>
#include <virgil/sdk/io/Marshaller.h>

#include <cli/version.h>
#include <cli/config.h>
#include <cli/pair.h>
#include <cli/util.h>

namespace vsdk = virgil::sdk;
namespace vcrypto = virgil::crypto;
namespace vcli = virgil::cli;

#ifdef SPLIT_CLI
#define MAIN main
#else
#define MAIN card_search_app_main
#endif

int MAIN(int argc, char** argv) {
    try {
        std::string description = "Search for application Cards.\n";

        std::vector<std::string> examples;
        examples.push_back("Search for application Cards:\n"
                           "virgil card-search-app -c <app_name>\n");

        examples.push_back("Get all application cards:\n"
                           "virgil card-search-app -c \"*\"\n");

        std::string descriptionMessage = virgil::cli::getDescriptionMessage(description, examples);

        // Parse arguments.
        TCLAP::CmdLine cmd(descriptionMessage, ' ', virgil::cli_version());

        TCLAP::ValueArg<std::string> outArg("o", "out", "Folder in which will be saved a Virgil Cards", false, "",
                                            "arg");

        TCLAP::ValueArg<std::string> applicationNameArg(
            "c", "application-name", "Application name, name = '*' - get all Cards\n", true, "", "arg");

        cmd.add(applicationNameArg);
        cmd.add(outArg);
        cmd.parse(argc, argv);

        vsdk::ServicesHub servicesHub(VIRGIL_ACCESS_TOKEN);
        std::string appName = "com.virgilsecurity." + applicationNameArg.getValue();

        std::vector<vsdk::models::CardModel> appCards = servicesHub.card().searchApp(appName);
        if (appCards.empty()) {
            std::cout << "Cards by name: " << applicationNameArg.getValue() << " haven't been found." << std::endl;
            return EXIT_FAILURE;
        }

        std::string pathTofolder = outArg.getValue();
        if (pathTofolder.empty()) {
            for (auto&& appCard : appCards) {
                std::string appCardStr = vsdk::io::Marshaller<vsdk::models::CardModel>::toJson<4>(appCard);
                vcli::writeBytes(pathTofolder, appCardStr);
            }
        } else {
            for (auto&& appCard : appCards) {
                std::string identity = appCard.getCardIdentity().getValue();
                std::string cardId = appCard.getId();

                std::string fileName = identity + "-id-" + appCard.getId() + ".vcard";
                std::string appCardStr = vsdk::io::Marshaller<vsdk::models::CardModel>::toJson<4>(appCard);
                vcli::writeBytes(pathTofolder + "/" + fileName, appCardStr);
            }
        }

        std::cout << "По заданному application name:" << applicationNameArg.getValue() << " получено "
                  << appCards.size() << " Карт." << std::endl;

    } catch (TCLAP::ArgException& exception) {
        std::cerr << "card-search-app. Error: " << exception.error() << " for arg " << exception.argId() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception& exception) {
        std::cerr << "card-search-app. Error: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}