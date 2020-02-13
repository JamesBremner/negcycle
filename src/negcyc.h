template<typename graph_t>
class cNegCyc
{
public:

vector< vector<int> > find( graph_t& g )
{
    vector< vector<int> > vnegcycs;
    for( int start = 0; start < (int)num_vertices(g); start++ )
    {
        // check that a cycle can start and finish on this vertex
        if( ! IsCyclePossible( start, g) )
            continue;

        //cout << "start " << start << "\n";

        std::vector<int> pred( num_vertices(g),-1);
        pred[start] = start;
        breadth_first_search(
            g,
            start,
            boost::visitor(
                boost::make_bfs_visitor(
                    boost::record_predecessors(
                        pred.data(),
                        boost::on_tree_edge()))));

        auto es = edges(g);
        for (auto eit = es.first; eit != es.second; ++eit)
        {
            // check for back edge
            if( (int)boost::target(*eit, g) != start )
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
    for( auto& c : negcycs )
    {
        bool same = true;
        for( auto v : path )
        {
            if ( std::find( c.begin(), c.end(), v ) == c.end() )
            {
                same = false;
                break;
            }
        }
        if( same )
            return;
    }

    path.push_back( path[0] );
    negcycs.push_back( path );
}

bool IsCyclePossible( int v, graph_t& g )
{
    if( ! out_degree( v, g) )
        return false;;
    auto es = edges(g);
    for (auto eit = es.first; eit != es.second; ++eit)
        if( (int)boost::target(*eit, g) == v )
            return true;
    return false;
}
};
