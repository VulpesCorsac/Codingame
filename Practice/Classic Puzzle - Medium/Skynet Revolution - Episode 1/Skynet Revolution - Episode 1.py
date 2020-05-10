import sys

cuts = 0
nodes_amount, links_amount, exits_amount = [int(i) for i in input().split()]
exits = []
graph = {}
last_skynet = -1


def findLinkToCut(skynet):
    for exit in exits:
        if exit in graph[skynet]:
            return [skynet, exit]
    # if need best move:
    if cuts == 0:
        three_connected = []
        for edge in graph[max_exit]:
            if len(graph[edge]) == 3:
                three_connected.append(edge)
        for edge in three_connected:
            for edge1 in graph[edge]:
                if len(graph[edge1]) == 4:
                    print('Disconnect cycle', file=sys.stderr)
                    return [edge, edge1]
    else:
        for next_node in graph[skynet]:
            if next_node != last_skynet:
                print('Win', file=sys.stderr)
                return [skynet, next_node]
    # if no best move:
    for exit in exits:
        if len(graph[exit]) > 0:
            print('Overkill', file=sys.stderr)
            return [exit, graph[exit][0]]
    return None


def cutLink(c1, c2):
    graph[c1].remove(c2)
    graph[c2].remove(c1)


for _ in range(links_amount):
    n1, n2 = [int(j) for j in input().split()]
    graph.setdefault(n1, []).append(n2)
    graph.setdefault(n2, []).append(n1)
for _ in range(exits_amount):
    exits.append(int(input()))

max_exit = None
for exit in exits:
    if max_exit == None or len(graph[exit]) > len(graph[max_exit]):
        max_exit = exit

while True:
    skynet = int(input())
    c1, c2 = findLinkToCut(skynet)
    cutLink(c1, c2)
    cuts += 1
    last_skynet = skynet
    print("{0} {1}".format(c1, c2))