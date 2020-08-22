/*!
 * \file Fichier.h
 * \author Atma
 * \date 22 Août 2020
 * \brief Contient les prototypes de la classe Fichier
 * \version 1.0
 */

 #ifndef FICHIER_H
 #define FICHIER_H

class Fichier
{
    private:
        FILE *pointeurFichier;
	    long tailleMusique;
	    char *buffer;
	    size_t resultat;

    public:

        /*!\struct ErreurFichier
        * \brief Liste des erreurs possible de la classe Fichier
        */
        enum class ErreurFichier
        {
            OuvertureFichier,
            FichierDejaOuvert,
            TailleFichierIncorrect,
            
        };

        Fichier();

        ~Fichier();

        void ouvrirLecture(std::string cheminFichier);

        void ouvrirEcriture(std::string cheminFichier)
};

 #endif