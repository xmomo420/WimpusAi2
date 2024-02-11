#include "src/Agent.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        int tailleCarte = stoi(argv[1]);
        Agent agent = Agent(tailleCarte);
        agent.jouer();
    }
    return 0;
}
