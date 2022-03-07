#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

#include <stdlib.h>
#include <time.h>

inline unsigned randInt(unsigned lower, unsigned upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

inline bool randBool() 
{
    return rand() % 2;
}

// Set a bit in number
inline void bitset(unsigned &inp, unsigned k, bool state) 
{
    if (state)
        inp = (inp | (1 << k));     // set kth bit
    else
        inp = inp & ~(1 << k);      // clear kth bit
}


// Node class
class Node
{
    private:
    bool _state;
    unsigned _CPN;      // connections per node
    std::vector<Node *> _connections;

    public:
    Node(bool init_state, unsigned connections_per_node)
        : _state(init_state), _CPN(connections_per_node)
    {
        _connections.reserve(_CPN);
    }


    void defineConnections(std::vector<Node *> init_connections)
    {
        assert(init_connections.size() == _CPN);
        for (unsigned i=0; i<_CPN; i++)    // copy connections
            _connections[i] = &(*init_connections[i]);
    }


    bool getState() { return _state; }
    void setState(bool state) { _state = state; }


    bool getNextState(std::map<unsigned, bool> truthTable)
    {
        // construct key
        unsigned key = 0;
        for (unsigned i=0; i<_CPN; i++)
        {
            key = key << 1;
            bitset(key, 0, _connections[i]->getState());
        }
        // std::cout << key << std::endl;

        // Get value from truth table & update state
        auto x = truthTable.find(key);
        if(x == truthTable.end())
        {
            std::cerr << "Error! TruthTable doesnt have a valid value for key: " << key << std::endl;
            return false;
        }
        return x->second;
    }
};


// Random Boolean Network class
class RBN
{
    private:
    std::vector<Node> nodes;
    unsigned _CPN;

    public:
    long long unsigned generation = 0;

    RBN(unsigned num_nodes, unsigned connections_per_node)
    :   _CPN(connections_per_node)
    {
        // Generate N randomly initialized nodes
        for (unsigned i=0; i<num_nodes; i++)
            nodes.push_back(Node(false, connections_per_node));
    }

    // initialize Randomly
    void initialize()
    {
        for (unsigned i=0; i<nodes.size(); i++)
            nodes[i].setState(randBool());
    }

    // initilaize with provided vector
    void initialize(std::vector<bool> init_vec)
    {
        for (unsigned i=0; i<nodes.size(); i++)
            nodes[i].setState(init_vec[i]);
    }

    void connectRandomly()
    {
        // Set random connections
        for (unsigned i=0; i<nodes.size(); i++)
        {
            std::vector<Node *> c;
            for (unsigned j=0; j<_CPN; j++) // pushback N random node pointers
            {
                Node * n = &nodes[randInt(0, nodes.size()-1)];
                c.push_back(n);
            }
            nodes[i].defineConnections(c);
        }
    }

    void eval(std::map<unsigned, bool> truthTable)
    {
        // Get next states for all nodes
        std::vector<bool> next_states(nodes.size());
        for (unsigned i=0; i<nodes.size(); i++)
            next_states[i] = nodes[i].getNextState(truthTable);
        
        // Update all
        for (unsigned i=0; i<nodes.size(); i++)
            nodes[i].setState(next_states[i]);
        generation++;
    }

    std::string getSignature(char zero_marker = '0', char one_marker = '@')
    {
        std::string s;
        for (unsigned i=0; i<nodes.size(); i++)
            s.push_back(nodes[i].getState() ? one_marker : zero_marker);
        return s;
    }
};
