#include <iostream>
#include "../lib/cadical/src/cadical.hpp"
#include <vector>

using namespace std;

    class Agent {

        /********************************************************
        ************ Structure de données utilitaires ***********
        *********************************************************/

    private:

        struct Point {
            int rangee;
            int colonne;
        };

        struct Case {
            bool visitee = false;
            bool danger = false;
        };

        /********************************************************
        ************** Attributs d'instance privés **************
        *********************************************************/

        vector<vector<Case>> carte;
        Point pointCourant;
        bool termine = false;
        bool orTrouve = false;
        bool entrainReculer = false;
        vector<Point> casesSansDanger = vector<Point>();
        vector<char> chemin = vector<char>();

        // Constantes
        const vector<char> ACTIONS_POSSIBLES{'h', 'b', 'd', 'g'};

    public:

        /**
         * Constructeur
         * @param tailleCarte
         */
        Agent(const int &tailleCarte);

        /********************************************************
        ***** Déclarations de fonctions d'instance publiques ****
        *********************************************************/

        void jouer();

    private:

        /*********************************************************
        ****** Déclarations de fonctions d'instance privées ******
        **********************************************************/

        void stockerDonnees(const bool &odeur,const bool &brise);

        char meilleureAction();

        bool outOfBounds(const Point &position);

        bool positionValide(const Point &position);

        Case & caseAuPoint(const Point &point);

        void seDeplacer(const char &action);

        Point mettreAJourPoint(const char &action);

        void reculer(const char &derniereAction);

        bool aucuneActionSur();

        char actionOpposee(const char &action);

        bool deplacementValide(const char &action);

        vector<Agent::Point> casesAdjacentes(const Point &point);

        /*********************************************************
        ****** Déclarations de fonctions de classes privées ******
        **********************************************************/

        static bool pointMemePosition(const Point &point, const Point &autrePoint);

        static bool contient(const vector<Point> &cases,const Point &uneCase);

    };




