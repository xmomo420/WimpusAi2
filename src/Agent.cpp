#include "Agent.h"

/**
 * Constructeur
 * @param tailleCarte
 */
Agent::Agent(const int &tailleCarte) {
    carte = vector<vector<Case>>();
    vector<Case> ligne = vector<Case>();
    for (int i = 0; i < tailleCarte; ++i) {
        for (int j = 0; j < tailleCarte; ++j)
            ligne.push_back(Case{});
        carte.push_back(ligne);
        ligne.clear();
    }
    pointCourant = Point{tailleCarte - 1, 0};
    caseAuPoint(pointCourant).visitee = true;
}

bool Agent::outOfBounds(const Point &position) {
    return position.rangee >= carte.size() || position.colonne >= carte[0].size() ||
            position.rangee < 0 || position.colonne < 0;
}

void Agent::stockerDonnees(const bool &odeur,const bool &brise) {
    vector<Point> cases = casesAdjacentes(pointCourant);
    bool danger = odeur || brise;
    for (Point &uneCase : cases)
        caseAuPoint(uneCase).danger = danger
                && !contient(casesSansDanger,uneCase);
}

Agent::Case & Agent::caseAuPoint(const Point &point) {
    return carte[point.rangee][point.colonne];
}

void Agent::seDeplacer(const char &action) {
    pointCourant = mettreAJourPoint(action);
    caseAuPoint(pointCourant).visitee = true;
    if (!entrainReculer && !orTrouve)
        chemin.push_back(action);
    cout << action << endl;
}

Agent::Point Agent::mettreAJourPoint(const char &action) {
    switch (action) {
        case 'h':
            return Point{pointCourant.rangee - 1, pointCourant.colonne};
        case 'b':
            return Point{pointCourant.rangee + 1, pointCourant.colonne};
        case 'd':
            return Point{pointCourant.rangee, pointCourant.colonne + 1};
        case 'g':
            return Point{pointCourant.rangee, pointCourant.colonne - 1};
        default:
            cerr << "Action invalide : mettreAJourPoint()"  << endl;
            exit(EXIT_FAILURE);
    }
}

bool Agent::positionValide(const Point &position) {
    if (!outOfBounds(position)) {
        Case uneCase = caseAuPoint(position);
        return (contient(casesSansDanger, position) || !uneCase.danger) && !uneCase.visitee;
    } else
        return false;
}

void Agent::reculer(const char &derniereAction) {
    char action = actionOpposee(derniereAction);
    seDeplacer(action);
    chemin.pop_back();
}

bool Agent::aucuneActionSur() {
    bool retour = true;
    int i = 0;
    while (retour && i < (int)ACTIONS_POSSIBLES.size())
        retour = !deplacementValide(ACTIONS_POSSIBLES[i++]);
    return retour;
}

char Agent::meilleureAction() {
    char meilleure = 'X';
    for (char action : ACTIONS_POSSIBLES)
        if (deplacementValide(action)) {
            meilleure = action;
            Point pointFutur = mettreAJourPoint(meilleure);
            if (!contient(casesSansDanger, pointFutur))
                casesSansDanger.push_back(pointFutur);
        }
    return meilleure;
}

char Agent::actionOpposee(const char &action) {
    switch (action) {
        case 'h':
            return 'b';
        case 'b':
            return 'h';
        case 'd':
            return 'g';
        case 'g':
            return 'd';
        default:
            cerr << "Erreur, action invalide : actionOpposee()";
            exit(EXIT_FAILURE);
    }
}

bool Agent::deplacementValide(const char &action) {
    return positionValide(mettreAJourPoint(action));
}

vector<Agent::Point> Agent::casesAdjacentes(const Point &point) {
    vector<Agent::Point> cases = {};
    Agent::Point pointAdjacent{};
    pointAdjacent = Point{point.rangee - 1, point.colonne}; // En haut
    if (positionValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    pointAdjacent = Point{point.rangee + 1, point.colonne}; // En bas
    if (positionValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    pointAdjacent = Point{point.rangee, point.colonne - 1}; // À gauche
    if (positionValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    pointAdjacent = Point{point.rangee, point.colonne + 1}; // À droite
    if (positionValide(pointAdjacent))
        cases.push_back(pointAdjacent);
    return cases;
}

bool Agent::pointMemePosition(const Point &point, const Point &autrePoint) {
    return point.rangee == autrePoint.rangee && point.colonne == autrePoint.colonne;
}

bool Agent::contient(const vector<Point> &cases, const Point &uneCase) {
    bool contains = false;
    for (int i = 0; i < (int)cases.size() && !contains; ++i)
        contains = pointMemePosition(cases[i], uneCase);
    return contains;
}

void Agent::jouer() {
    while (!termine) {
        bool odeur, brise, lueur, choc;
        std::cin >> odeur >> brise >> lueur >> choc; // Sensors
        char action;
        if (entrainReculer) { // Si on doit reculer
            if (!chemin.empty() && aucuneActionSur()) {
                cerr << "Action : " << actionOpposee(chemin.back()) << endl;
                reculer(chemin.back()); // Reculer
            } else if (chemin.empty() && aucuneActionSur()) { // Case départ et bloqué
                cerr << "Action : q" << endl;
                cout << 'q' << endl; // Quitter
                termine = true;
            } else { // L'agent n'est plus bloqué
                entrainReculer = false;
                action = meilleureAction();
                cerr << "Action : " << action << endl;
                seDeplacer(action);
            }
        } else if (lueur && !orTrouve) { // Or trouvé
            orTrouve = true; // Indiquer que l'or a été trouvé
            cerr << "Action : s" << endl;
            cout << 's' << endl; // Saisir
        } else if (orTrouve) { // Il faut reculer
            if (pointMemePosition(pointCourant, Point{(int)carte.size() - 1, 0})) {
                cerr << "Action : q" << endl;
                cout << 'q' << endl; // Quitter
                termine = true;
            } else {
                cerr << "Action : " << actionOpposee(chemin.back()) << endl;
                reculer(chemin.back());
            }
        } else {
            stockerDonnees(odeur, brise); // On analyse les sensors
            action = meilleureAction();
            if (action == 'X') { // Agent bloqué, doit reculer
                if (!chemin.empty()) { // Chemin pas vide
                    entrainReculer = true;
                    cerr << "Action : " << actionOpposee(chemin.back()) << endl;
                    reculer(chemin.back()); // Reculer
                } else { // Chemin vide
                    cerr << "Action : q" << endl;
                    cout << 'q' << endl;
                    termine = true;
                }
            } else { // L'agent n'est pas bloqué
                cerr << "Action : " << action << endl;
                seDeplacer(action);
            }
        }
    }
}
