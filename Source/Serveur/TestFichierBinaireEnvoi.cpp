/*!
 * \file TestFichierBinaireEnvoi.cpp
 * \author Atma
 * \date 24 Août 2020
 * \brief Programme permettant de tester la classe Fichier
 *       avec des fichiers binaires en envoi
 *       Compilation:
 *       g++ -o TestFichierBinaireEnvoi TestFichierEnvoi.cpp ../Fichier.cpp ../ServeurTCP.cpp
 * \version 1.0
 */

#include <iostream>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>

#include "ServeurTCP.h"
#include "../HeaderFichier.h"
#include "../Header.h"

#define CHEMIN_MUSIQUE "Musique"

//Retourne un vecteur contenant tous les noms de fichier présent dans le dossier, à l'adresse chemin
std::vector<std::string> obtenirFichierDansDossier(std::string chemin)
{
	std::vector<std::string> nomsFichier;

	for(const auto & entry : std::filesystem::directory_iterator(chemin))
	{
		std::cout << "Fichier trouver: " << entry.path().filename() << std::endl;
		nomsFichier.push_back(entry.path());
	}

	std::cout << "\nNombre de fichier présent dans " << CHEMIN_MUSIQUE << ": " << nomsFichier.size() << std::endl << std::endl;
	return nomsFichier;
}

int main(void)
{
    ServeurTCP *monServeurTCP = NULL;
	HeaderFichier headerFichier;
	Header header;

	std::ifstream fichierEnvoi;

	int tailleAlire = 5000;
	int tailleLu = 0;
	char bufferFichier[tailleAlire];
	
	std::uintmax_t tailleFichier;

	std::vector<std::string> nomsFichier;

    try
    {
        monServeurTCP = new ServeurTCP();
		
		//On analyse le dossier Musique pour obtenir le nom de tous les fichiers
		nomsFichier = obtenirFichierDansDossier(CHEMIN_MUSIQUE);
		
		//On ouvre le serveur et on connecte un client
		monServeurTCP->ouvrir("127.0.0.1", 55555);
		monServeurTCP->connecterUnClient();

		//On envoi le nombre de fichier
		header.nombreFichier = nomsFichier.size();
		
		monServeurTCP->emettreData(&header, sizeof(header));

		for (int i = 0; i < nomsFichier.size(); i++)
		{
			//On ouvre le fichier en lecture
			fichierEnvoi.open(nomsFichier[i], std::ios::binary);

			//On récupère la taille du fichier
			tailleFichier = std::filesystem::file_size(nomsFichier[i]);

			//On initialise le header pour le futur fichier
			strcpy(headerFichier.nomFichier, nomsFichier[i].c_str());
			headerFichier.tailleNomFichier = strlen(headerFichier.nomFichier);
			headerFichier.tailleFichier = tailleFichier;

			//Afficher le contenu du header
			std::cout << "Nom: " << headerFichier.nomFichier << std::endl;
			std::cout << "Taille du nom: " << headerFichier.tailleNomFichier << std::endl;
			std::cout << "Taille: " << headerFichier.tailleFichier << std::endl << std::endl;

			//On envoi d'abord le header au client
			monServeurTCP->emettreData(&headerFichier, sizeof(headerFichier));

			//On envoi ensuite le contenu du fichier petit à petit
			while(tailleLu < tailleFichier)
			{
				if((tailleLu + tailleAlire) > tailleFichier)
				{
					tailleAlire = tailleFichier - tailleLu;
				}
				
				//On copie une partie du fichier dans le buffer
				fichierEnvoi.read(bufferFichier, tailleAlire);

				//On envoi le buffer petit à petit
				monServeurTCP->emettreData(bufferFichier, tailleAlire);
				tailleLu += tailleAlire;
			}

			//On ferme le fichier en cours
			std::cout << "Envoi de " << headerFichier.nomFichier << std::endl;	
			fichierEnvoi.close();
		}

		monServeurTCP->deconnecterUnClient();
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


    delete monServeurTCP;

    return 0;
}