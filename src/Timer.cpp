
#include <iostream>
#include <chrono>

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>

using namespace std;
using namespace boost;

#include "negcyc.h"

class cMyVertex
{
};
class cMyEdge
{
public:
    double myWeight;
};

typedef adjacency_list<vecS, vecS, directedS,
        cMyVertex, cMyEdge> graph_t;
typedef erdos_renyi_iterator<minstd_rand, graph_t> ERGen;

void add_edge( int a, int b, double w, graph_t& g )
{
    g[add_edge( a, b, g ).first].myWeight = w;
}

int
main(int argc, char* argv[])
{
    if( argc != 2 )
    {
        cout << "Usage: Timer <node_count>\n";
        exit(1);
    }
    int nodeCount = atoi( argv[1] );

    vector< vector<int> > vnegcycs;
    cNegCyc<graph_t> NC;

    minstd_rand gen;
    graph_t g10(ERGen(gen, nodeCount, 0.05, true), ERGen(), nodeCount);
    auto es = edges(g10);
    for (auto eit = es.first; eit != es.second; ++eit)
        g10[*eit].myWeight = -1;

    auto t1 = std::chrono::high_resolution_clock::now();
    vnegcycs = NC.find( g10 );
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
    cout << vnegcycs.size() << " cycles found in " << duration << " secs\n";
}

