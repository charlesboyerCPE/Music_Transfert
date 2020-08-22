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

#include "Fichier.h"

Fichier::Fichier()
{
    *pointeurFichier = NULL;
    long tailleFichier = NULL;
	char *buffer = {NULL};
	size_t resultat = NULL;
}

Fichier::~Fichier()
{
    fclose(pointeurFichier);
}

void ouvrirLecture(std::string cheminFichier)
{
    if (pointeurFichier != NULL) throw ErreurFichier::FichierDejaOuvert;

	pointeurFichier = fopen(cheminFichier, "r");
	if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichier;
}

void ouvrirEcriture(std::string cheminFichier)
{
    if (pointeurFichier != NULL) throw ErreurFichier::FichierDejaOuvert;
    
	pointeurFichier = fopen(cheminFichier, "ww");
	if (pointeurFichier == NULL) throw ErreurFichier::OuvertureFichier;
}

void obtenirTailleFichier()
{
	fseek(pointeurFichier, 0, SEEK_END);
	tailleFichier = ftell(pointeurFichier);
	rewind (pointeurFichier);

	if(tailleFichier == NULL) throw ErreurFichier::TailleFichierIncorrect;
}
