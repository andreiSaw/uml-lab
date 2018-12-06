#ifndef TRANSITION_TOUR_H
#define TRANSITION_TOUR_H

#include <vector>
#include <string>
#include <fstream>

class FSM;

struct OutStateSucc {
    int nextState;
    int output;
    bool activeFlag;

    explicit OutStateSucc(int nextState = -1, int output = -1, bool activeFlag = false);
    OutStateSucc(const OutStateSucc &otherOutStateSucc);

    OutStateSucc& operator=(const OutStateSucc &otherOutStateSucc);

    bool IsActiveFlag() const;
    void PrintOutStateSucc() const;
};

struct DiffPair {
    std::vector<int> reachNewInputVector;
    int currState;
    int distBFS;
    DiffPair(int currState);
    DiffPair(std::vector<int> reachNewInputVector, int currState, int distBFS);
    DiffPair(const DiffPair &otherDiffPair);
};

// TransitionTour

class TransitionTour {
    std::vector<std::vector<bool>> coveredTransition;
    std::vector<std::vector<int>> matrixOfPath;
    std::vector<int> testTransitionTourSuite;
    std::vector<int> outputTransitionTourSuite;
    std::vector<int> delSeq;
    std::vector<int> reachableStates;

    void CreateMatrixOfPath(const FSM &fsm);
    void CreateSequence(const FSM &fsm, int currState, int currInput);

public:
    TransitionTour(const FSM &fsm);
    friend class FSM;
};

class FSM {
    std::vector<std::vector<OutStateSucc>> stateFSM;
    std::vector<std::vector<int>> ioFSM;
    int numberOfStates;
    int numberOfTransitions;
    int numberOfInputs;
    int numberOfOutputs;
    int numberIOFSMInputs;
    int initialState;

    void ReadHeader(std::ifstream &graphIn);
    void ReadTransitions(std::ifstream &graphIn);

public:
    FSM();
    FSM(const std::string &inputFile);
    FSM(const FSM &otherFSM);
    void CreateTransitionTour(std::vector<int> &transitionTourSuite,
                              std::vector<int> &outputTransitionTourSuite,
                              std::vector<int> &delSeq) const;
    void WriteTransitionTour(const std::string &outputFile,
                             const std::vector<int> &transitionTourSuite, 
                             const std::vector<int> &outputTransitionTourSuite, 
                             const std::vector<int> &delSeq);
    bool FindPath(int endState, std::vector<int> &vectorSeq) const;
    bool IsConnectedFSM();

    void PrintStateFSM() const;
    void PrintIOFSM() const;
    
    friend class TransitionTour;
};

#endif