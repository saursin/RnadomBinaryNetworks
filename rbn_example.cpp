#include "rbn.h"
#include <iostream>
#include <map>
#include <fstream>

int main()
{
    srand(time(0));

    const unsigned N = 20;  // no of nodes
    const unsigned M = 3;   // connections per node
    long long unsigned max_generations = 1000;

    // Truth Table
    std::map<unsigned, bool> tt;
    tt.insert(std::make_pair(0b000, false));
    tt.insert(std::make_pair(0b001, false));
    tt.insert(std::make_pair(0b010, true));
    tt.insert(std::make_pair(0b011, true));
    tt.insert(std::make_pair(0b100, true));
    tt.insert(std::make_pair(0b101, false));
    tt.insert(std::make_pair(0b110, true));
    tt.insert(std::make_pair(0b111, false));

    RBN rbn(N, M);
    rbn.connectRandomly();
    rbn.initialize();

    std::vector<std::string> log;
    while(rbn.generation < max_generations)
    {       
        log.push_back(rbn.getSignature('0', ' '));
        rbn.eval(tt);
    }


    std::ofstream f("run.log");
    for (unsigned i=0; i<N; i++)
    {
        for (unsigned j=0; j<log.size(); j++)
        {
            f << log[j][i];
        }
        f << "\n";
    }
    f.close();

    std::cout << "Done! see run.log" << std::endl;   
    return 0;
}