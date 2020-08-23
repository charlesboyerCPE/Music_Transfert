/*!
 * \file Fichier.cpp
 * \author Atma
 * \date 22 Août 2020
 * \brief Contient la déclaration de la classe Fichier
 * \version 1.0
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "Fichier.h"

Fichier::Fichier(char * cheminFichier, std::string mode)
{
	pointeurFichier = NULL;
	contenuFichier = {NULL};
	tailleFichier = 0;

	if (mode == "r")
	{
		pointeurFichier = fopen(cheminFichier, "r");
		if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierLecture;
	} 
	else if (mode == "w")
	{
		pointeurFichier = fopen(cheminFichier, "w");
		if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierEcriture;
	}
	else if (mode == "rb")
	{
		pointeurFichier = fopen(cheminFichier, "rb");
		if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierEcriture;
	}
	else if (mode == "wb")
	{
		pointeurFichier = fopen(cheminFichier, "rb");
		if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichierEcriture;
	}

}

Fichier::~Fichier()
{
	fclose(pointeurFichier);
	free(contenuFichier);
}

void Fichier::initialisation()
{
	fseek(pointeurFichier, 0, SEEK_END);
	tailleFichier = ftell(pointeurFichier);
	rewind (pointeurFichier);

	contenuFichier = (char*) malloc (sizeof(char) * tailleFichier);
	if (contenuFichier == NULL) throw ErreurFichier::AllocationMemoire;
}

void Fichier::initialisation(long tailleFichier)
{
	this->tailleFichier = tailleFichier;

	contenuFichier = (char*) malloc (sizeof(char) * this->tailleFichier);
	if (contenuFichier == NULL) throw ErreurFichier::AllocationMemoire;
}

void Fichier::lire()
{
	size_t resultat;

	resultat = fread(contenuFichier, 1, tailleFichier, pointeurFichier);
	if (resultat != tailleFichier) throw ErreurFichier::CopieMemoire;
}

void Fichier::sauvegarder()
{
	size_t resultat;

	contenuFichier[tailleFichier] = '\0';
	resultat = fwrite(contenuFichier, sizeof(char), strlen(contenuFichier), pointeurFichier);
	if (resultat != strlen(contenuFichier)) throw ErreurFichier::CopieFichier;
}

char* Fichier::get_contenuFichier()
{
	return contenuFichier;
}

long Fichier::get_tailleFichier()
{
	return tailleFichier;
}
