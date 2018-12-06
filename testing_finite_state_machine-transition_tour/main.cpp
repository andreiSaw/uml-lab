#include "transitionTour.h"

#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    if (argc == 3) {
        FSM fsm(argv[1]);
        std::vector<int> transitionTourSuite;
        std::vector<int> outputTransitionTourSuite;
        std::vector<int> delSeq;
        fsm.CreateTransitionTour(transitionTourSuite, outputTransitionTourSuite, delSeq);
        fsm.WriteTransitionTour(argv[2], transitionTourSuite, outputTransitionTourSuite, delSeq);
    }
    else {
        std::cout << "argc != 3" << std::endl;
    }
    return 0;
}