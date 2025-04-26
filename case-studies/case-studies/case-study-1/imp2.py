import random
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# --- Build Graph ---
G = nx.DiGraph()
# Add stations with waiting passengers
stations = ["Train Station A", "Train Station B", "Airport Terminal"]
for station in stations:
    G.add_node(station, type='station', passengers_waiting=random.randint(500,2000))
# Add junctions
junctions = ["Junction 1", "Junction 2", "Junction 3", "Junction 4"]
for junction in junctions:
    G.add_node(junction, type='junction', passengers_waiting=0)
# Add venue
destination = 'Venue'
G.add_node(destination, type='venue', passengers_arrived=0)

# Add edges with attributes
roads = [
    ("Train Station A", "Junction 1", {"capacity":500, "flow":0, "night_allowed":True}),
    ("Train Station A", "Junction 2", {"capacity":300, "flow":0, "night_allowed":False}),
    ("Train Station B", "Junction 2", {"capacity":400, "flow":0, "night_allowed":True}),
    ("Train Station B", "Junction 3", {"capacity":350, "flow":0, "night_allowed":True}),
    ("Airport Terminal", "Junction 3", {"capacity":600, "flow":0, "night_allowed":False}),
    ("Airport Terminal", "Junction 4", {"capacity":450, "flow":0, "night_allowed":True}),
    ("Junction 1", "Junction 2", {"capacity":300, "flow":0, "night_allowed":True}),
    ("Junction 2", "Junction 3", {"capacity":250, "flow":0, "night_allowed":True}),
    ("Junction 3", "Junction 4", {"capacity":350, "flow":0, "night_allowed":False}),
    ("Junction 1", destination, {"capacity":400, "flow":0, "night_allowed":False}),
    ("Junction 2", destination, {"capacity":350, "flow":0, "night_allowed":True}),
    ("Junction 3", destination, {"capacity":450, "flow":0, "night_allowed":False}),
    ("Junction 4", destination, {"capacity":500, "flow":0, "night_allowed":True})
]
for u, v, attrs in roads:
    G.add_edge(u, v, **attrs)

# --- Prepare Buses ---
buses = []
def shortest_path(src):
    return nx.shortest_path(G, src, destination)
for station in stations:
    path = shortest_path(station)
    buses.append({"id": station, "path": path, "pos": 0})

# --- Simulation Function ---
def simulate(steps):
    for step in range(1, steps + 1):
        print(f"-- Step {step} --")
        # reset flows
        for u, v in G.edges():
            G[u][v]['flow'] = 0
        # move buses
        for bus in buses:
            if bus['pos'] >= len(bus['path']) - 1:
                print(f"Bus {bus['id']}: resting at {destination}")
                continue
            u = bus['path'][bus['pos']]
            v = bus['path'][bus['pos'] + 1]
            if G[u][v]['flow'] < G[u][v]['capacity']:
                G[u][v]['flow'] += 1
                bus['pos'] += 1
                print(f"Bus {bus['id']}: {u} → {v}")
            else:
                print(f"Bus {bus['id']}: waiting at {u} (capacity full)")

# --- Run Console Simulation ---
simulate(10)

# --- Visualization ---
pos = {
    destination: (0, 0),
    "Junction 1": (1, 2),
    "Junction 2": (2, 1),
    "Junction 3": (3, 0),
    "Junction 4": (4, -1)
}
# Place stations around
pos["Train Station A"] = (-1, 2)
pos["Train Station B"] = (-1, 0)
pos["Airport Terminal"] = (-1, -2)

fig = plt.figure(figsize=(8, 6))
def update(frame):
    plt.clf()
    nx.draw(G, pos, with_labels=True, node_size=600, arrowsize=20)
    # reset flows for animation
    for u, v in G.edges():
        G[u][v]['flow'] = 0
    # move buses up to current frame
    for bus in buses:
        bus['pos'] = 0
        for _ in range(frame):
            if bus['pos'] >= len(bus['path']) - 1:
                break
            u = bus['path'][bus['pos']]
            v = bus['path'][bus['pos'] + 1]
            if G[u][v]['flow'] < G[u][v]['capacity']:
                G[u][v]['flow'] += 1
                bus['pos'] += 1
            else:
                break
        x, y = pos[bus['path'][bus['pos']]]
        plt.text(x, y + 0.2, f"oooo", fontsize=14)
    plt.title(f"Simulation Step {frame}")

anim = FuncAnimation(fig, update, frames=11, interval=1000, repeat=False)
plt.show()
