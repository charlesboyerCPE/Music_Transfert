/*!
 * \file ClientTcp.h
 * \author Charles BOYER
 * \date 15 Septembre 2019
 * \brief Contient la d�claration de la classe permettant la
 *		  communication, cot� client, via le protocole TCP
 * \version 1.0
 */

#ifndef CLIENTTCP_H
#define CLIENTTCP_H
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint>


/*!
 * \class ClientTcp
 * \brief Cette classe permet d'emettre ou de recevoir des donn�es,
 *  cot� client, via le protocole TCP
 */
class ClientTcp 
{
  private:
    uint32_t socketDialogue;


  public:
	/*!\struct ErreurClientTcp
	 * \brief Liste des erreurs possible de la classe ClientTcp
	 */
	enum class ErreurClientTcp
	{
		Ouverture,
		AdresseMalEcrite,
		Connexion,
		ErreurEnvoi,
		ErreurReception,
		NonOuvert,
	};
	
	/*!\brief Constructeur par d�faut*/
	ClientTcp();
	
	/*!\brief Destructeur de la classe ClientTcp */
	~ClientTcp();
	
	/*!\brief Etablit la connexion avec le serveur d'adresse "adresse" sur le port "port".*/
    void connecter(std::string adresse, uint16_t port);

	/*!\brief Se d�connecte du serveur.*/
    void clore();

	/*!\brief Emmet au serveur connect� les "nbOctets" premier octets du tableau "data".*/
    ssize_t emettreData(void* data, uint32_t nbOctets);

	/*!\brief Receptionne depuis le serveur "nbOctets" octets et stocke ces octets dans le tableau "data".*/
    ssize_t recevoirData(void* data, uint32_t nbOctets);

};
#endif
