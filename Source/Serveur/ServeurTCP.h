/*!
 * \file ServeurTCP.h
 * \author Atma
 * \date 21 Août 2020
 * \file ServeurTCP.h
 * \brief Contient la déclaration de la classe permettant
 *        la communication, coté serveur, via le protocole TCP
 * \version 1.0
 */

#ifndef _SERVEURTCP_H
#define _SERVEURTCP_H

#include <string>

/*!
 * \class ServeurTCP
 * \brief Cette classe permet d'emettre ou de recevoir des données,
 *  coté serveur, via le protocole TCP
 */
class ServeurTCP 
{
	private:
    
		//Socket permettant d'attendre des clients TCP.
		int32_t socketAttenteClient;
		int32_t socketDialogueClient;

	public:

		/*!\struct ErreurClientTcp
		 * \brief Liste des erreurs possible de la classe ClientTcp
		 */
		enum class ErreurServeurTCP
		{
			CreationInterface,
			AdresseMalEcrite,
			AdresseDejaUtilise,
			OuvertureServeur,
			EchecReception,
			EchecEnvoi, 
			EchecAttenteClient,
		};

		/*!\brief Constructeur par défaut*/
		ServeurTCP();

		/*!\brief Destructeur de la classe ServeurTCP */
		~ServeurTCP();

		/*!\briefFerme le serveur TCP. Coupe si necessaire la communication en cours avec un client. */
		void fermer();

		/*!\brief Ouvre le serveur TCP sur l'interface aillant pour adresse ou non "adresse" avec comme port "port". Le nombre de place pour la file d'attente est "nbPlaces". */
		void ouvrir(std::string adresse, uint16_t port, int32_t nbPlaces = 10);

		/*!\brief Recoit des données du client connecté. Receptionne "tailleData" octets. Les octets recus sont stockés dans le tableau "data". */
		ssize_t recevoirData(void* data, uint32_t tailleData);

		/*!\brief Emet au client connecté les "nbOctets" premiers octets du tableau "data". */
		ssize_t emettreData(void* data, uint32_t nbOctets);

		/*!\brief Accepte un client. Méthode bloquante. */
		void connecterUnClient();

		/*!\brief Déconnecte le client connecté. */
		void deconnecterUnClient();
};

#endif
