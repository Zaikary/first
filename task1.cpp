#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <queue>
#include <deque>
#include <unordered_set>
#include <set>

struct IGraph {
    virtual ~IGraph() {}
        
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

struct SetGraph : public IGraph
{
public:
    SetGraph(int size)
    : adjacencySets(size)
    {
    }
    
    SetGraph(const IGraph &graph)
    {
        adjacencySets.resize(graph.VerticesCount());
        for (int from = 0; from < graph.VerticesCount(); ++from)
        {
            for(int to : graph.GetNextVertices(from))
            {
                adjacencySets[from].insert(to);
            }
        }
    }
    
    ~SetGraph()
    {
    }
    
    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencySets.size());
        assert(0 <= to && to < adjacencySets.size());
        adjacencySets[from].insert(to);
    }
    
    int VerticesCount() const override
    {
        return (int)adjacencySets.size();
    }
    
    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencySets.size());
        std::vector<int>NextVertices;
        for (int i: adjacencySets[vertex])
        {
            NextVertices.push_back(i);
        }
        return NextVertices;
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencySets.size());
        std::vector<int> prevVertices;
        
        for (int from = 0; from < adjacencySets.size(); ++from)
        {
            if (adjacencySets[from].count(vertex) > 0)
            {
                prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }
    
private:
    std::vector<std::set<int>> adjacencySets;
};

struct MatrixGraph: public IGraph
{
public:
    MatrixGraph(int size)
    : adjacencyMatrix(size, std::vector<bool>(size, false))
    {
    }

    MatrixGraph(const IGraph &graph)
    {
        int size = graph.VerticesCount();
        adjacencyMatrix.resize(size, std::vector<bool>(size,false));
        for (int from = 0; from < size; ++from)
        {
            for (int to : graph.GetNextVertices(from))
            {
            adjacencyMatrix[from][to] = true;                
            }
        }
    }

    ~MatrixGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyMatrix.size());
        assert(0 <= to && to < adjacencyMatrix.size());
        adjacencyMatrix[from][to] = true;
    }
    
    int VerticesCount() const override
    {
        return (int)adjacencyMatrix.size();
    }
    
    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        std::vector<int> nextVertices;
        for(int i = 0; i < adjacencyMatrix.size(); i++)
        {
            if (adjacencyMatrix[vertex][i])
            {
                nextVertices.push_back(i);
            }
        }
        return nextVertices;
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyMatrix.size());
        std::vector<int> prevVertices;
        
        for (int from = 0; from < adjacencyMatrix.size(); ++from)
        {
            if (adjacencyMatrix[from][vertex])
            {
                prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }

private:
    std::vector<std::vector<bool>> adjacencyMatrix;    
};

struct ListGraph: public IGraph
{
public:
    ListGraph(int size)
    : adjacencyLists(size)
    {
    }
    
    ListGraph(const IGraph &graph)
    {
        adjacencyLists.resize(graph.VerticesCount());
        for (int i = 0; i < graph.VerticesCount(); ++i)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }
    
    ~ListGraph()
    {
    }
    
    void AddEdge(int from, int to) override
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }
    
    int VerticesCount() const override
    {
        return (int)adjacencyLists.size();
    }
    
    std::vector<int> GetNextVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;
        
        for (int from = 0; from < adjacencyLists.size(); ++from)
        {
            for (int to: adjacencyLists[from])
            {
                if (to == vertex)
                    prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }
    
private:
    std::vector<std::vector<int>> adjacencyLists;
};

struct ArcGraph: public IGraph
{
public:
    ArcGraph(int size)
    : count(size)
    {
    }
    
    ArcGraph(const IGraph &graph)
    {
        count = graph.VerticesCount();
        for (int from = 0; from < VerticesCount(); ++from)
        {
            for (int to : graph.GetNextVertices(from))
            {
                edges.push_back({from,to});
            }
        }
    }
    
    ~ArcGraph()
    {
    }
    
    void AddEdge(int from, int to) override
    {
        edges.push_back({from,to});
        edges.push_back({to,from});
    }
    
    int VerticesCount() const override
    {
        return count;
    }
    
    std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> next;
        for (const auto& [from,to] : edges)
        {
            if (from == vertex)
            {
                next.push_back(to);
            }
        }
        return next;
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int>PrevVertices;
        for (const auto& [from,to]:edges)
        {
            if(from == vertex)
            {
                PrevVertices.push_back(to);
            }
        }
        return PrevVertices;
    }
    
private:
    int count;
    std::vector<std::pair<int,int>> edges;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;
    
    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();
        
        func(currentVertex);
        
        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);
    
    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            DFS(graph, i, visited, func);
        }
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;
    
    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            topologicalSortInternal(graph, nextVertex, visited, sorted);
        }
    }
    
    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            topologicalSortInternal(graph, i, visited, sorted);
        }
    }
    
    return sorted;
}

int main(int argc, const char * argv[]) {
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);
    
    mainBFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    
    mainDFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    
    for (int vertex: topologicalSort(listGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    
    // Нужно продемонстрировать работу конструктора копирования, проинициализировав
    // графы разных классов друг от друга. Далее, показать, что вершины и ребра
    // успешно скопированы: число вершин осталось прежним, произвести BFS и DFS.
    std::cout << "Use MatrixGraph" << std::endl;
    MatrixGraph matrixGraph(listGraph);
    mainBFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    for (int vertex: topologicalSort(matrixGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    std::cout << "Use ArcGraph" << std::endl;
    ArcGraph arcGraph(matrixGraph);
    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    for (int vertex: topologicalSort(arcGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    std::cout << "Use SetGraph" << std::endl;
    SetGraph setGraph(arcGraph);
    mainBFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    mainDFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;
    for (int vertex: topologicalSort(setGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    return 0;
}

// Вывод:
// 0 1 5 2 3 6 4 
// 0 1 2 3 4 6 5
// 0 1 5 3 6 4 2
// Use MatrixGraph
// 0 1 5 2 3 6 4
// 0 1 2 3 4 6 5
// 0 1 5 3 6 4 2
// Use ArcGraph
// 0 1 5 2 3 6 4
// 0 1 2 3 4 6 5
// 0 1 5 3 6 4 2
// Use SetGraph
// 0 1 5 2 3 6 4
// 0 1 2 3 4 6 5
// 0 1 5 3 6 4 2