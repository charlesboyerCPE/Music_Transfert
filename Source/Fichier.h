/*!
 * \file Fichier.h
 * \author Atma
 * \date 22 Août 2020
 * \brief Contient les prototypes de la classe Fichier
 * \version 1.0
 */

 #ifndef FICHIER_H
 #define FICHIER_H

#include <stdio.h>
#include <string>

class Fichier
{
    private:
        FILE * pointeurFichier;
	    char *buffer;
        long tailleFichier;

    public:

        /*!\struct ErreurFichier
        * \brief Liste des erreurs possible de la classe Fichier
        */
        enum class ErreurFichier
        {
            OuvertureFichierLecture,
            OuvertureFichierEcriture,
            TailleFichierIncorrect,
            AllocationMemoire,
            CopieMemoire,
            CopieFichier,
        };

        Fichier(std::string cheminFichier, int mode);

        ~Fichier();

        void obtenirTailleFichier();

        void initialiserBuffer();

        void copierDansMemoire(); 

        void copierDansFichier();

        char* get_buffer();
};

 #endif