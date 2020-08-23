/*!
 * \file TestFichierEnvoi.cpp
 * \author Atma
 * \date 22 Août 2020
 * \brief Programme permettant de tester la classe Fichier
 *       en envoi
 *       Compilation:
 *       g++ -o TestFichierEnvoi TestFichierEnvoi.cpp Fichier.cpp ServeurTCP.cpp
 * \version 1.0
 */

#include <iostream>
#include <cstring>
#include "../Fichier.h"
#include "../ServeurTCP.h"
#include "../Header.h"

int main(void)
{
    ServeurTCP *monServeurTCP = NULL;
    Fichier *monFichier = NULL;
	Header headerFichier;

    try
    {
        monFichier = new Fichier("../testEnvoiFichier.txt", 'r');
        monServeurTCP = new ServeurTCP();

		//On détermine la taille du fichier et on initialise le buffer
        monFichier->determinerTailleFichier();
        monFichier->initialiserBuffer();

		//On lit le fichier et on met son contenu en mémoire
        monFichier->lire();

		//On initialise le header pour le futur fichier
		memcpy(headerFichier.nomFichier, "testEnvoiFichier.txt", strlen("testEnvoiFichier.txt"));
		headerFichier.tailleNomFichier = strlen(headerFichier.nomFichier);
		headerFichier.tailleFichier = monFichier->get_tailleFichier();

        monServeurTCP->ouvrir("127.0.0.1", 55555);
		
        while(1)
        {
            monServeurTCP->connecterUnClient();

			//On envoi d'abord le header au client
			monServeurTCP->emettreData(&headerFichier, sizeof(headerFichier));
            
			//On envoi ensuite le contenu du fichier
			monServeurTCP->emettreData(monFichier->get_buffer(), strlen(monFichier->get_buffer()));

            monServeurTCP->deconnecterUnClient();
        }
        
        monServeurTCP->fermer();
    }

    catch (ServeurTCP::ErreurServeurTCP x)
	{
		std::cout << "Erreur ServeurTCP: ";
		switch (x)
		{
			case ServeurTCP::ErreurServeurTCP::CreationInterface :
				std::cerr << "Création du socket" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::AdresseMalEcrite :
				std::cerr << "Adresse IPv4 mal ecrite" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::AdresseDejaUtilise :
				std::cerr << "Adresse IPv4 deja utilise" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::OuvertureServeur :
				std::cerr << "Ouverture du serveur TCP impossible" << std::endl;
				monServeurTCP->fermer();
				break;

			case ServeurTCP::ErreurServeurTCP::EchecReception :
				std::cerr << "Reception des donnees impossible" << std::endl;
				monServeurTCP->fermer();
				break;

			case ServeurTCP::ErreurServeurTCP::EchecEnvoi :
				std::cerr << "Envoie des donnees impossible" << std::endl;
				break;

			case ServeurTCP::ErreurServeurTCP::EchecAttenteClient :
				std::cerr << "Attente client" << std::endl;
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

    delete monServeurTCP;
    delete monFichier;

    return 0;
}