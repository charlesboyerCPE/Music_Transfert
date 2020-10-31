/*!
 * \file ServeurMusique.h
 * \author Atma
 * \date 30 Octobre 2020
 * \file ServeurMusique.h
 * \brief Contient la déclaration de la classe ServeurMusique
 * \version 1.0
 */

#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>

class ServeurMusique
{
    private:

    public:
        ServeurMusique();
        
        ~ServeurMusique();

        std::vector<std::string> analyserDossier();

        void envoyerFichier(int numeroFichier);

        void envoyerFichier();
};