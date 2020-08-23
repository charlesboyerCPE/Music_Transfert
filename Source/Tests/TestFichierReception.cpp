/*!
 * \file TestFichierReception.cpp
 * \author Atma
 * \date 22 Août 2020
 * \brief Programme permettant de tester la classe Fichier
        en reception
        Compilation:
        g++ -o TestFichierReception TestFichierEnvoi.cpp Fichier.cpp ClientTcp.cpp
 * \version 1.0
 */

#include <iostream>
#include "../Fichier.h"
#include "../ClientTcp.h"
#include "../Header.h"

int main(void)
{
    ClientTcp *monClient = NULL;
    Fichier *monFichier = NULL;
	Header headerRecu;

    try
    {
        monClient = new ClientTcp();

        monClient->connecter("127.0.0.1", 55555);
		
		//On recoit le header que le serveur a envoyer
		monClient->recevoirData(&headerRecu, sizeof(headerRecu));

		//On créer le fichier avec les informations du header
		monFichier = new Fichier(headerRecu.nomFichier, "w");

		//On saisi la taille du fichier et n initialise le buffer
		monFichier->initialisation(headerRecu.tailleFichier);

		//On stocke les données envoyées dans le buffer
		monClient->recevoirData(monFichier->get_contenuFichier(), headerRecu.tailleFichier);

		monClient->clore();

        monFichier->sauvegarder();
    }

    catch(ClientTcp::ErreurClientTcp x)
    {
        std::cout << "Erreur ClientTcp: ";
		switch (x)
		{
			case ClientTcp::ErreurClientTcp::Ouverture :
				std::cerr << "Création du socket" << std::endl;
				break;

			case ClientTcp::ErreurClientTcp::AdresseMalEcrite :
				std::cerr << "Adresse IPv4 mal ecrite" << std::endl;
				break;

			case ClientTcp::ErreurClientTcp::Connexion :
				std::cerr << "Connexion impossible" << std::endl;
				break;

			case ClientTcp::ErreurClientTcp::ErreurEnvoi :
				std::cerr << "Emission des donnees impossible" << std::endl;
				monClient->clore();
				break;

			case ClientTcp::ErreurClientTcp::ErreurReception :
				std::cerr << "Reception des donnees impossible" << std::endl;
				monClient->clore();
				break;

			case ClientTcp::ErreurClientTcp::NonOuvert :
				std::cerr << "Socket non Ouvert" << std::endl;
				break;
		}
    }

    catch(Fichier::ErreurFichier y)
    {
        std::cout << "Erreur Fichier: ";
		switch (y)
		{
			case Fichier::ErreurFichier::OuvertureFichierLecture :
				std::cerr << "Ouverture du fichier en lecture" << std::endl;
				break;

			case Fichier::ErreurFichier::OuvertureFichierEcriture :
				std::cerr << "Ouverture du fichier en ecriture" << std::endl;
				break;

			case Fichier::ErreurFichier::TailleFichierIncorrect :
				std::cerr << "Taille du fichier incorrect" << std::endl;
				break;

			case Fichier::ErreurFichier::AllocationMemoire :
				std::cerr << "Allocation de la mémoire impossible" << std::endl;
				break;

			case Fichier::ErreurFichier::CopieMemoire :
				std::cerr << "Copie dans la mémoire impossible" << std::endl;
				break;

            case Fichier::ErreurFichier::CopieFichier :
				std::cerr << "Copie dans le fichier impossible" << std::endl;
				break;
		}
    }

    delete monClient;
    delete monFichier;

    return 0;
}