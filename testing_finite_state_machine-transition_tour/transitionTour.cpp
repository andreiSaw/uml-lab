#include "transitionTour.h"

#include <iostream>
#include <queue>
#include <cmath>
#include <string>
#include <cstdlib>

// OutStateSucc

OutStateSucc::OutStateSucc(int nextState, int output, bool activeFlag)
    : nextState(nextState)
    , output(output)
    , activeFlag(activeFlag) {}

OutStateSucc::OutStateSucc(const OutStateSucc &otherOutStateSucc)
    : nextState(otherOutStateSucc.nextState)
    , output(otherOutStateSucc.output)
    , activeFlag(otherOutStateSucc.activeFlag) {}

OutStateSucc& OutStateSucc::operator=(const OutStateSucc &otherOutStateSucc) {
    nextState = otherOutStateSucc.nextState;
    output = otherOutStateSucc.output;
    activeFlag = otherOutStateSucc.activeFlag;
    return *this;
}

bool OutStateSucc::IsActiveFlag() const {
    return activeFlag;
}

void OutStateSucc::PrintOutStateSucc() const {
    if (activeFlag == true) {
        std::cout << "(" << nextState << " / "
        << output << ")" << std::endl;
    }
}

// DiffPair

DiffPair::DiffPair(int currState)
    : currState(currState)
    , distBFS(0) {}

DiffPair::DiffPair(std::vector<int> reachNewInputVector, int currState, int distBFS)
    : reachNewInputVector(reachNewInputVector)
    , currState(currState)
    , distBFS(distBFS) {}

DiffPair::DiffPair(const DiffPair &otherDiffPair)
    : reachNewInputVector(otherDiffPair.reachNewInputVector)
    , currState(otherDiffPair.currState)
    , distBFS(otherDiffPair.distBFS) {}


// FSM declaration

// FSM is complete and connective
TransitionTour::TransitionTour(const FSM &fsm)
        : matrixOfPath(fsm.numberOfStates) {
    std::vector<bool> passedTran(fsm.numberOfInputs, false);
    coveredTransition.resize(fsm.numberOfStates, passedTran);
    CreateMatrixOfPath(fsm);
    /*
    for (const auto &currState: reachableStates) {
        std::cout << currState << ' ';
    }
    std::cout << std::endl;
    */
    if (reachableStates.size() == fsm.numberOfStates) {
        delSeq.push_back(0);
        for (const auto &currState: reachableStates) {
            for (int currInput = 0; currInput < coveredTransition.at(currState).size(); ++currInput) {
                if (coveredTransition.at(currState).at(currInput) == false) {
                    testTransitionTourSuite.insert(testTransitionTourSuite.end(),
                                                   matrixOfPath.at(currState).begin(),
                                                   matrixOfPath.at(currState).end());
                    CreateSequence(fsm, currState, currInput);
                    delSeq.push_back(testTransitionTourSuite.size());
                } 
            }
        }
    }
    else {
        std::cout << "FSM is not connected" << std::endl;
        exit(0);
    }
    for (int i = 0; i < delSeq.size() - 1; ++i) {
        int currState = fsm.initialState;
        for (int j = delSeq[i]; j < delSeq[i + 1]; ++j) {
            int out = fsm.stateFSM[currState][testTransitionTourSuite[j]].output;
            currState = fsm.stateFSM[currState][testTransitionTourSuite[j]].nextState;
            outputTransitionTourSuite.push_back(out);
        }
    }
}

void TransitionTour::CreateMatrixOfPath(const FSM &fsm) {
    for (int currState = 0; currState < fsm.numberOfStates; ++currState) {
        if (currState != fsm.initialState) {
            if (fsm.FindPath(currState, matrixOfPath.at(currState))) {
                reachableStates.push_back(currState);
            }
        }
        else {
            reachableStates.push_back(fsm.initialState);
        }
    }
}

void TransitionTour::CreateSequence(const FSM &fsm, int currState, int currInput) {
    while (true) {
        for (currInput = 0; (currInput < coveredTransition.at(currState).size()) && 
                            (coveredTransition.at(currState).at(currInput) == true); ++currInput);
        if (currInput == coveredTransition.at(currState).size()) {
            return;
        }
        testTransitionTourSuite.push_back(currInput);
        coveredTransition.at(currState).at(currInput) = true;
        currState = fsm.stateFSM.at(currState).at(currInput).nextState;
    }
}


// FSM

FSM::FSM() {}

FSM::FSM(const std::string &inputFile) {
    std::ifstream fsmIn(inputFile);
    ReadHeader(fsmIn);
    ReadTransitions(fsmIn);
}

FSM::FSM(const FSM &otherFSM) {
    stateFSM = otherFSM.stateFSM;
    ioFSM = otherFSM.ioFSM;
    numberOfStates = otherFSM.numberOfStates;
    numberOfInputs = otherFSM.numberOfInputs;
    numberOfOutputs = otherFSM.numberOfOutputs;
    initialState = otherFSM.initialState;
    numberOfTransitions = otherFSM.numberOfTransitions;
    numberIOFSMInputs = numberOfInputs * numberOfOutputs;
}

void FSM::ReadHeader(std::ifstream &graphIn) {
    std::string str;
    int F;
    graphIn >> str;
    graphIn >> F;
    if (F != 0) {
        std::cout << "please enter deterministic FSM" << std::endl;
    }
    graphIn >> str;
    graphIn >> numberOfStates;
    graphIn >> str;
    graphIn >> numberOfInputs;
    graphIn >> str;
    graphIn >> numberOfOutputs;
    graphIn >> str;
    graphIn >> initialState;
    graphIn >> str;
    graphIn >> numberOfTransitions;
    numberIOFSMInputs = numberOfInputs * numberOfOutputs;
    for (int i = 0; i < numberOfStates; ++i) {
        std::vector<OutStateSucc> stateInput(numberOfInputs);
        stateFSM.push_back(stateInput);
    }
    for (int i = 0; i < numberIOFSMInputs; ++i) {
        std::vector<int> stateVector(numberOfStates, -1);
        ioFSM.push_back(stateVector);
    }
}

void FSM::ReadTransitions(std::ifstream &graphIn) {
    int currentState, input, output, nextState;
    int currReadTransitions = 0;
    for (int i = 0; i < numberOfTransitions; ++i) {
        graphIn >> currentState;
        graphIn >> input;
        graphIn >> nextState;
        graphIn >> output;
        if (!stateFSM[currentState][input].IsActiveFlag()) {
            stateFSM[currentState][input] = OutStateSucc(nextState, output, true);
            ioFSM[input * numberOfOutputs + output][currentState] = nextState;
            ++currReadTransitions;
        }
        else {
            std::cout << "non-deterministic" << std::endl;
            exit(0);
        }
    }
    if (currReadTransitions != numberOfStates * numberOfInputs) {
        std::cout << "FSM is partion" << std::endl;
        exit(0);
    }
}

void FSM::CreateTransitionTour(std::vector<int> &transitionTourSuite, 
                               std::vector<int> &outputTransitionTourSuite,
                               std::vector<int> &delSeq) const {
    TransitionTour TT(*this);
    std::vector<int> testOutput;
    transitionTourSuite = TT.testTransitionTourSuite;
    outputTransitionTourSuite = TT.outputTransitionTourSuite;
    delSeq = TT.delSeq;
}

void FSM::WriteTransitionTour(const std::string &outputFile,
                              const std::vector<int> &transitionTourSuite, 
                              const std::vector<int> &outputTransitionTourSuite, 
                              const std::vector<int> &delSeq) {
    std::ofstream fsmOut(outputFile);
    for (int i = 0; i < delSeq.size() - 1; ++i) {
        for (int j = delSeq[i]; j < delSeq[i + 1]; ++j) {
            fsmOut << transitionTourSuite[j] << "/" << outputTransitionTourSuite[j] << ' ';
        }
        fsmOut << std::endl;
    }
}

bool FSM::IsConnectedFSM() {
    std::vector<bool> reachedStates(numberOfStates, false);
    std::queue<int> queueOfStates;
    int currPassed = 0;
    queueOfStates.push(initialState);
    reachedStates[initialState] = true;
    ++currPassed;
    while ((!queueOfStates.empty()) && (currPassed <= numberOfStates)) {
        int currState = queueOfStates.front();
        queueOfStates.pop();
        for (int currInput = 0; currInput < numberOfInputs; ++currInput) {
            int nextState = stateFSM[currState][currInput].nextState;
            if (reachedStates[nextState] == false) {
                queueOfStates.push(nextState);
                reachedStates[nextState] = true;
                ++currPassed;
            }
        }
    }
    if (currPassed == numberOfStates) {
        return true;
    }
    return false;
}

bool FSM::FindPath(int endState, std::vector<int> &vectorSeq) const {
    if (initialState == endState) {
        return true;
    }
    std::queue<DiffPair> queueOfState;
    std::vector<bool> passedState(numberOfStates, false);
    queueOfState.push(DiffPair(initialState));
    passedState[initialState] = true;
    while (!queueOfState.empty()) {
        DiffPair diffPair = queueOfState.front();
        queueOfState.pop();
        for (int i = 0; i < numberOfInputs; ++i) {
            int nextState = stateFSM[diffPair.currState][i].nextState;
            if (nextState == endState) {
                diffPair.reachNewInputVector.push_back(i);
                vectorSeq = diffPair.reachNewInputVector;
                return true;
            }
            if (!passedState[nextState]) {
                DiffPair newDiffPair(diffPair);
                newDiffPair.reachNewInputVector.push_back(i);
                newDiffPair.currState = nextState;
                queueOfState.push(newDiffPair);
                passedState[nextState] = true;
            }
        }
    }
    //std::cout << "None" << std::endl;
    return false;
}

void FSM::PrintStateFSM() const {
    std::cout << "stateFSM" << std::endl;
    for (int i = 0; i < stateFSM.size(); ++i) {
        std::cout << "state_" << i << ": " << std::endl;
        for (int j = 0; j < stateFSM[i].size(); ++j) {
            stateFSM[i][j].PrintOutStateSucc();
        }
    }
}
