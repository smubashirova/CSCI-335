#include "Graph.hpp"
#include <fstream>
#include <sstream>

namespace VertexCover {

Graph readFromFile(const std::string& filename) {
    //Create empty graph object
    Graph graph;
    //Open the file
    std::ifstream fin(filename);
    
    // Check if file opened successfully
    if (!fin.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line; // Reads each line
    //Read the file line by line
    while (std::getline(fin, line)) {
        // Find the positions of airport codes
        // Format: "Departure (HH:MM) - XXX | Destination (HH:MM) - YYY"
        size_t dep_pos = line.find("- ") + 2;
        size_t dep_end = line.find(' ', dep_pos);
        std::string departure = line.substr(dep_pos, dep_end - dep_pos);
        
        // Find destination airport code
        size_t dest_pos = line.find("- ", dep_end) + 2;
        size_t dest_end = line.find(' ', dest_pos);
        std::string destination = line.substr(dest_pos, dest_end - dest_pos);
        
        // Add both directions since the graph is undirected
        graph[departure].insert(destination);
        graph[destination].insert(departure);
    }
    
    return graph;
}

std::unordered_set<Vertex> cover_graph(Graph g) {
    // Initialize empty vertex cover
    std::unordered_set<Vertex> cover;
    
    // Continue until all edges are covered
    while (true) {
        // Find the vertex with maximum degree
        Vertex max_degree_vertex;
        size_t max_degree = 0;
        bool found = false; // Flag to check if we found any vertices
        
        // Iterate through all vertices in the graph
        for (const auto& [vertex, neighbors] : g) {
            // Check if this vertex has more edges than current max
            if (neighbors.size() > max_degree) {
                max_degree = neighbors.size();
                max_degree_vertex = vertex;
                found = true; // We found at least one vertex
            }
        }
        
        // If no edges left, we're done
        if (!found) {
            break;
        }
        
        // Add the vertex to the cover
        cover.insert(max_degree_vertex);
        
        // Remove all edges connected to this vertex
        auto neighbors = g[max_degree_vertex]; // Make a copy
        for (const auto& neighbor : neighbors) {
            g[neighbor].erase(max_degree_vertex); // Remove edge from neighbor's list
        }
        
        // Remove the vertex itself from the graph
        g.erase(max_degree_vertex);
    }
    
    return cover;// Return the final vertex cover
}

} // namespace VertexCover