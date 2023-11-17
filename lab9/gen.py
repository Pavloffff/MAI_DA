import random

def generate_test(num_vertices, num_edges):
    edges = set()
    
    max_edges = num_vertices * (num_vertices - 1) // 2
    num_edges = min(num_edges, max_edges)

    while len(edges) < num_edges:
        u = random.randint(1, num_vertices)
        v = random.randint(1, num_vertices)

        if u != v and (u, v) not in edges and (v, u) not in edges:
            edges.add((u, v))

    return num_vertices, edges

num_vertices, num_edges = map(int, input().split())
n, edges = generate_test(num_vertices, num_edges)

print(n, len(edges))
for edge in edges:
    print(*edge)
