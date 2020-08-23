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

int main(void)
{
    ServeurTCP *monServeurTCP = NULL;
    Fichier *monFichier = NULL;

	char * tailleFichier;

    try
    {
        
        monFichier = new Fichier("../testEnvoiFichier.txt", 'r');
        monServeurTCP = new ServeurTCP();

        monFichier->determinerTailleFichier();
        monFichier->initialiserBuffer();
        monFichier->copierDansMemoire();

        monServeurTCP->ouvrir("127.0.0.1", 55555);
		
        while(1)
        {
            monServeurTCP->connecterUnClient();

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