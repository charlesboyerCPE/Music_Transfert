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
        FILE *pointeurFichier;
	    char *contenuFichier;
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

        Fichier(char* cheminFichier, std::string mode);

        ~Fichier();

        void initialisation();

        void initialisation(long tailleFichier);

        void lire(); 

        void sauvegarder();

        char* get_contenuFichier();

        long get_tailleFichier();
};

 #endif