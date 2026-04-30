#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <queue>

typedef int Vertex;
typedef std::pair<Vertex, Vertex> Edge;
typedef std::map<Vertex, std::list<Vertex>> AdjacencyList;

class Graph {
    u_int num_vertices;
    u_int num_edges;

    AdjacencyList adj;
    std::map<Edge, int> costs;
    std::pair<std::map<Vertex, int>, std::map<Vertex, Vertex>> last_dijkstra_result;

  public:
    void add_edge(Vertex u, Vertex v, int cost = 1) {
        adj[u].push_back(v);
        costs[Edge{u, v}] = cost;
    }

    void add_edge_undirected(Vertex u, Vertex v, int cost = 1) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        costs[Edge{u, v}] = cost;
        costs[Edge{v, u}] = cost;
    }

    void remove_vertex(Vertex v) {
        adj.erase(v);
        for (auto &[n, n_adj] : adj) {
            auto it = std::find(n_adj.begin(), n_adj.end(), v);
            if (it != n_adj.end()) {
                n_adj.erase(it);
            }
        }
    }

    u_int get_num_vertices() {
        return adj.size();
    }

    std::vector<Vertex> get_vertices() {
        std::vector<Vertex> vertices;
        for (auto &[v, _] : adj) {
            vertices.push_back(v);
        }
        return vertices;
    }

    /// Runs the dijkstra algorithm on the graph
    std::pair<std::map<Vertex, int>, std::map<Vertex, Vertex>> dijkstra(Vertex start) {
        // min-heap Priority queue consisting of (dist, vertex)
        std::priority_queue<std::pair<int, Vertex>, std::vector<std::pair<int, Vertex>>,
                            std::greater<std::pair<int, Vertex>>>
            pq;
        std::map<Vertex, int> dists;
        std::map<Vertex, Vertex> parents;
        for (const auto &[vertex, vert_adj] : adj) {
            dists[vertex] = INT32_MAX;
            parents[vertex] = INT32_MAX;
        }
        dists[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            std::pair<int, Vertex> top = pq.top();
            pq.pop();
            int dist = top.first;
            Vertex v = top.second;

            // If this distance not the shortest one, skip it
            if (dist > dists[v])
                continue;

            // Explore all neighbors of the current vertex
            for (Vertex &n : adj[v]) {
                int cost = costs[Edge{v, n}];

                // If we found a shorter path to v through u, update it
                if (dists[v] + cost < dists[n]) {
                    dists[n] = dists[v] + cost;
                    pq.emplace(dists[n], n);
                    parents[n] = v;
                }
            }
        }

        return std::pair(dists, parents);
    }

    /// Runs dijkstra and saves the result internally, which is retrievable using
    /// `get_cached_dijkstra_result`
    std::pair<std::map<Vertex, int>, std::map<Vertex, Vertex>> run_caching_dijkstra(Vertex start) {
        last_dijkstra_result = dijkstra(start);
        return last_dijkstra_result;
    }

    std::pair<std::map<Vertex, int>, std::map<Vertex, Vertex>> get_cached_dijkstra_result() {
        return last_dijkstra_result;
    }

    /// @brief Checks if a path exists in the graph
    /// @param path The path to search for
    /// @return true if the path exists, else false
    bool has_path(std::vector<Vertex> &path) {
        auto p_iter = path.begin();
        Vertex last_vertex = *p_iter;
        p_iter++;
        for (p_iter; p_iter != path.end(); p_iter++) {
            Vertex cur = *p_iter;
            // if the last vertex does not have an edge to the current, the path doesnt exist
            if (!std::count(adj[last_vertex].begin(), adj[last_vertex].end(), cur)) {
                return false;
            }
            last_vertex = cur;
        }
        return true;
    }

    /// @brief Tries to find a path using only the vertices provided. It does this by constructing a
    /// new graph using only those vertices and topological sorting that graph.
    /// @param vertices The list of vertices to use.
    /// @return Result of the topological sort (empty if no sorting was found).
    std::vector<Vertex> find_path_using_vertices(std::vector<Vertex> &vertices) {
        Graph new_graph = *this;
        // Construct new_graph to only consist of vertices which are in this update
        for (Vertex v : get_vertices()) {
            if (!std::count(vertices.begin(), vertices.end(), v)) {
                new_graph.remove_vertex(v);
            }
        }
        std::vector<Vertex> top = new_graph.topological_sort();
        return top;
    }

    /// @brief Returns a path from `start` to `end` and the distance of that path.
    /// @param start Start vertex of the pathfinding
    /// @param end End vertex of the pathfinding
    std::pair<std::vector<Vertex>, int> get_path(Vertex start, Vertex end) {
        std::pair<std::map<Vertex, int>, std::map<Vertex, Vertex>> dijkstra_res = dijkstra(start);
        std::map<Vertex, int> dists = dijkstra_res.first;
        std::map<Vertex, Vertex> parents = dijkstra_res.second;

        if (dists[end] == INT32_MAX) {
            return std::pair(std::vector<Vertex>(), 0);
        }

        std::vector<Vertex> path;

        Vertex current = end;
        path.push_back(current);
        for (size_t i = get_num_vertices(); i > 0; --i) {
            current = parents[current];
            path.push_back(current);

            if (current == start) {
                break;
            }
        }
        std::reverse(path.begin(), path.end());
        int path_dist = dists[end];
        return std::pair(path, path_dist);
    }

    // Function to print the adjacency list representation
    // of the graph
    void print() {
        std::cout << "Adjacency list for the Graph: " << std::endl;
        // Iterate over each vertex
        for (auto i : adj) {
            // Print the vertex
            std::cout << i.first << " -> ";
            // Iterate over the connected vertices
            for (auto j : i.second) {
                // Print the connected vertex
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_path(std::vector<Vertex> path) {
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i < path.size() - 1) {
                std::cout << " -> ";
            } else {
                std::cout << std::endl;
            }
        }
    }

    /// @brief Topological sorting using Kahn's algorithm
    /// @return Sorted list of vertices
    std::vector<Vertex> topological_sort() {
        std::vector<Vertex> result;
        std::queue<Vertex> queue;

        std::map<Vertex, int> in_degree;
        for (auto &[v, _] : adj) {
            int incoming_edges = 0;
            for (auto &[v2, v2_adj] : adj) {
                if (v2 == v)
                    continue;
                if (std::count(v2_adj.begin(), v2_adj.end(), v)) {
                    incoming_edges++;
                }
            }

            in_degree[v] = incoming_edges;
            if (incoming_edges == 0) {
                queue.push(v);
            }
        }

        while (!queue.empty()) {
            Vertex v = queue.front();
            queue.pop();
            result.push_back(v);

            for (Vertex &n : adj[v]) {
                in_degree[n] -= 1;
                if (in_degree[n] == 0) {
                    queue.push(n);
                }
            }
        }

        // Graph has a cycle
        if (result.size() != adj.size()) {
            return std::vector<Vertex>();
        }
        return result;
    }
};