template<typename graph_t>
class cNegCyc
{
public:

    /** Find all negative cycles in graph
        @param[in] g the graph
        @return vector of paths that are negative cycles, each path is a vector of vertex indices visited on the path
    */
    vector< vector<int> > find( graph_t& g )
    {
        // vector of paths that are negative cycles
        vector< vector<int> > vnegcycs;

        // loop over vertices
        for( int start = 0; start < (int)num_vertices(g); start++ )
        {
            // check that a cycle can start and finish on this vertex
            if( ! IsCyclePossible( start, g) )
                continue;

            //cout << "start " << start << "\n";

            // breadth first search of paths starting at this vertex
            std::vector<int> pred( num_vertices(g),-1);
            pred[start] = start;
            breadth_first_search(
                g,
                start,
                visitor(
                    make_bfs_visitor(
                        record_predecessors(
                            pred.data(),
                            on_tree_edge()))));

            // loop over edges
            auto es = edges(g);
            for (auto eit = es.first; eit != es.second; ++eit)
            {
                // check for back edge - an edge leading into this vertex that is reachable by starting at this vertex
                if( (int)target(*eit, g) != start )
                    continue;
                if( pred[source(*eit, g)] == -1)
                    continue;

                // total edge weights in cycle
                double pw = 0;
                vector<int> path;
                int v = source(*eit, g);
                path.push_back( v );
                while( 1 )
                {
                    pw += g[edge(pred[v],v,g).first].myWeight;
                    v = pred[v];
                    path.push_back( v );
                    if( v == start )
                    {
                        pw += g[edge(source(*eit, g),v,g).first].myWeight;
                        break;
                    }
                }

                // check that sum of path weights is negative
                if( pw >= 0 )
                    continue;

                // save if unique cycle
                AddNew( vnegcycs, path );

            }
        }
        return vnegcycs;
    }

private:
    void AddNew(  vector< vector<int> >& negcycs, vector<int>& path )
    {
        // loop over negative cycles previously found
        for( auto& c : negcycs )
        {
            bool same = true;

            // loop over vertices in new cycle
            for( auto v : path )
            {
                // check if vertex present in previously found cycle
                if ( std::find( c.begin(), c.end(), v ) == c.end() )
                {
                    // vertex found, must be a new cycle
                    same = false;
                    break;
                }
            }
            if( same )
                return;
        }

        // add the path
        path.push_back( path[0] );
        negcycs.push_back( path );
    }
    /** true if a cycle is possible starting at a vertex
        @param[in] v index of vertex
        @param[in] g the graph
        A cycle is only possible if there is at least one in edge
        AND one out edge on the vertex.
    */
    bool IsCyclePossible( int v, graph_t& g )
    {
        // count out edges
        if( ! out_degree( v, g) )
            return false;;

        // count in edges
        auto es = edges(g);
        for (auto eit = es.first; eit != es.second; ++eit)
            if( (int)boost::target(*eit, g) == v )
                return true;
        return false;
    }

};
