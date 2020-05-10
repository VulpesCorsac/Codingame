class Node:
    def __init__(self, input_connection):
        self.connections = [input_connection]
        self.distance = -1

node_count, edge_count, exit_count = map(int, input().split())
nodes_critical = []
nodes_exit = []
graph = {}

for _ in range(edge_count):
    node1, node2 = map(int, input().split())
    
    if node1 in graph:
        graph[node1].connections.append(node2)
    else:
        graph.update({node1: Node(node2)})
        
    if node2 in graph:
        graph[node2].connections.append(node1)
    else:
        graph.update({node2: Node(node1)})

exits = [int(input()) for _ in range(exit_count)]

for exit in exits:
    for edge in graph[exit].connections:
        if edge not in nodes_critical:
            if edge in nodes_exit:
                nodes_critical.append(edge)
            else:
                nodes_exit.append(edge)

while nodes_exit:
    skynet = int(input())
    
    if skynet in nodes_exit:
        remove_node = skynet
    elif nodes_critical:
        for node in graph.keys() - exits:
            graph[node].distance = None
    
        queue = [skynet]
        graph[skynet].distance = 1
        remove_distance = node_count**2

        for node in queue:
            if node in nodes_exit:
                graph[node].distance -= 1            
            for edge in graph[node].connections:
                if not graph[edge].distance:
                    queue.append(edge)
                    graph[edge].distance = graph[node].distance + 1

        for node in nodes_critical:
            if remove_distance > graph[node].distance:
                remove_distance = graph[node].distance
                remove_node = node
    else:
        remove_node = nodes_exit[0]
    
    for node in graph[remove_node].connections:
        if node in exits:            
            graph[remove_node].connections.remove(node)
            graph[node].connections.remove(remove_node)
            
            if remove_node in nodes_critical:
                connections_to_exits = 0
                for exit in exits:
                    if remove_node in graph[exit].connections:
                        connections_to_exits += 1
                if connections_to_exits < 2:
                    nodes_critical.remove(remove_node)
            else:
                nodes_exit.remove(remove_node)
            
            print(remove_node, node)
            break