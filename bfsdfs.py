import concurrent.futures
from collections import deque,defaultdict

def bfs(graph,start):
    visited = set()
    q = deque([start])
    result = []

    while q:
        node = q.popleft()
        if node not in visited:
            result.append(node)
            visited.add(node)
            q.extend(graph[node] - visited)
    return result

def dfs(graph,node,visited=None,result=None):
    if visited is None:
        visited = set()
    if result is None:
        result = []
    if node not in visited:
        result.append(node)
        visited.add(node)
        for neighbor in (graph[node] - visited):
            dfs(graph,neighbor,visited,result)
    return result

if __name__ == "__main__":
    graph = defaultdict(set)
    for _ in range(int(input("Number of edges: "))):
        u, v = input("Edge (u v): ").split()
        graph[u].add(v)
        graph[v].add(u)

    start = input("Starting node: ")

    with concurrent.futures.ThreadPoolExecutor() as executor:
        bfs_result, dfs_result = executor.map(lambda f: f(graph, start), [bfs, dfs])

    print("\nBFS Traversal:", ' -> '.join(bfs_result))
    print("DFS Traversal:", ' -> '.join(dfs_result))
