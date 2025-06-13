import random
import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from collections import defaultdict, deque
import time

class ShuttleFestivalSimulator:
    def __init__(self):
        self.graph = nx.DiGraph()
        self.stations = []  # Entry points (stations/airports)
        self.junctions = []  # Intermediate points
        self.venue = "Venue"  # Destination
        self.time_of_day = "Day"  # Day or Night
        self.score = 0
        self.day = 1
        self.max_days = 3
        self.achievements = set()
        self.people_transported = 0
        
    def create_default_network(self):
        """Create a default transportation network with stations, junctions, and venue"""
        # Add stations (entry points)
        self.stations = ["Train Station A", "Train Station B", "Airport Terminal"]
        for station in self.stations:
            self.graph.add_node(station, type="station", passengers_waiting=random.randint(500, 2000))
            
        # Add junctions
        self.junctions = ["Junction 1", "Junction 2", "Junction 3", "Junction 4"]
        for junction in self.junctions:
            self.graph.add_node(junction, type="junction", passengers_waiting=0)
            
        # Add venue (destination)
        self.graph.add_node(self.venue, type="venue", passengers_arrived=0)
        
        # Add edges (roads) with capacity constraints
        roads = [
            # From stations to junctions
            ("Train Station A", "Junction 1", {"capacity": 500, "flow": 0, "night_allowed": True}),
            ("Train Station A", "Junction 2", {"capacity": 300, "flow": 0, "night_allowed": False}),
            ("Train Station B", "Junction 2", {"capacity": 400, "flow": 0, "night_allowed": True}),
            ("Train Station B", "Junction 3", {"capacity": 350, "flow": 0, "night_allowed": True}),
            ("Airport Terminal", "Junction 3", {"capacity": 600, "flow": 0, "night_allowed": False}),
            ("Airport Terminal", "Junction 4", {"capacity": 450, "flow": 0, "night_allowed": True}),
            
            # Between junctions
            ("Junction 1", "Junction 2", {"capacity": 300, "flow": 0, "night_allowed": True}),
            ("Junction 2", "Junction 3", {"capacity": 250, "flow": 0, "night_allowed": True}),
            ("Junction 3", "Junction 4", {"capacity": 350, "flow": 0, "night_allowed": False}),
            
            # From junctions to venue
            ("Junction 1", "Venue", {"capacity": 400, "flow": 0, "night_allowed": False}),
            ("Junction 2", "Venue", {"capacity": 350, "flow": 0, "night_allowed": True}),
            ("Junction 3", "Venue", {"capacity": 450, "flow": 0, "night_allowed": False}),
            ("Junction 4", "Venue", {"capacity": 500, "flow": 0, "night_allowed": True})
        ]
        
        self.graph.add_edges_from(roads)
        
    def display_network(self):
        """Visualize the transportation network with edge capacities and flows"""
        plt.figure(figsize=(12, 8))
        
        # Define positions for nodes
        pos = {
            "Train Station A": (0, 4),
            "Train Station B": (0, 2),
            "Airport Terminal": (0, 0),
            "Junction 1": (3, 4),
            "Junction 2": (3, 2),
            "Junction 3": (3, 0),
            "Junction 4": (3, -2),
            "Venue": (6, 2)
        }
        
        # Draw nodes with different colors based on type
        node_colors = []
        for node in self.graph.nodes():
            if "station" in self.graph.nodes[node]["type"]:
                node_colors.append("skyblue")
            elif node == self.venue:
                node_colors.append("green")
            else:
                node_colors.append("orange")
        
        nx.draw_networkx_nodes(self.graph, pos, node_color=node_colors, node_size=800)
        nx.draw_networkx_labels(self.graph, pos)
        
        # Draw edges with capacity and flow info
        edge_labels = {}
        for u, v, data in self.graph.edges(data=True):
            night_symbol = "✓" if data['night_allowed'] else "✗"
            edge_labels[(u, v)] = f"Flow: {data['flow']}/{data['capacity']}\nNight: {night_symbol}"
        
        # Different colors for night-restricted roads
        edge_colors = ["red" if not data["night_allowed"] else "black" 
                        for _, _, data in self.graph.edges(data=True)]
        
        nx.draw_networkx_edges(self.graph, pos, edge_color=edge_colors, width=2, 
                               arrowsize=15, connectionstyle="arc3,rad=0.1")
        
        nx.draw_networkx_edge_labels(self.graph, pos, edge_labels=edge_labels, font_size=8)
        
        # Add title with time of day and stats
        total_passengers = sum(self.graph.nodes[station]["passengers_waiting"] for station in self.stations)
        arrived = self.graph.nodes[self.venue]["passengers_arrived"]
        plt.title(f"Day {self.day} - {self.time_of_day} - Waiting: {total_passengers} - Arrived: {arrived}")
        
        plt.axis('off')
        plt.tight_layout()
        plt.show()

    def reset_flows(self):
        """Reset all flows to 0"""
        for u, v, data in self.graph.edges(data=True):
            data["flow"] = 0

    def find_augmenting_path(self, source, sink):
        """Find an augmenting path from source to sink using BFS"""
        # Create a queue for BFS
        queue = deque([(source, [source], float('inf'))])
        visited = set([source])
        
        while queue:
            node, path, residual = queue.popleft()
            
            # If we reached the sink, return the path and residual capacity
            if node == sink:
                return path, residual
            
            # Explore neighbors
            for neighbor in self.graph.neighbors(node):
                edge_data = self.graph[node][neighbor]
                
                # Skip edges that can't be used at night
                if self.time_of_day == "Night" and not edge_data["night_allowed"]:
                    continue
                
                # Calculate residual capacity
                capacity = edge_data["capacity"]
                flow = edge_data["flow"]
                residual_capacity = capacity - flow
                
                # If there's residual capacity and we haven't visited this node
                if residual_capacity > 0 and neighbor not in visited:
                    new_residual = min(residual, residual_capacity)
                    queue.append((neighbor, path + [neighbor], new_residual))
                    visited.add(neighbor)
        
        # No augmenting path found
        return None, 0

    def ford_fulkerson(self, source_list, sink):
        """Calculate maximum flow from multiple sources to sink using Ford-Fulkerson algorithm"""
        # Add a super source that connects to all actual sources
        super_source = "SuperSource"
        self.graph.add_node(super_source, type="source", passengers_waiting=0)
        
        # Connect super source to all entry points
        for source in source_list:
            source_passengers = self.graph.nodes[source]["passengers_waiting"]
            self.graph.add_edge(super_source, source, capacity=source_passengers, flow=0, night_allowed=True)
        
        # Reset all existing flows
        self.reset_flows()
        
        # Run Ford-Fulkerson algorithm
        total_flow = 0
        while True:
            path, residual = self.find_augmenting_path(super_source, sink)
            if not path or residual <= 0:
                break
                
            # Augment flow along the path
            for i in range(len(path) - 1):
                u, v = path[i], path[i + 1]
                self.graph[u][v]["flow"] += residual
                
            total_flow += residual
        
        # Remove super source and its edges
        self.graph.remove_node(super_source)
        
        # Transfer passengers based on calculated flows
        for station in self.stations:
            outflow = sum(self.graph[station][v]["flow"] for v in self.graph.neighbors(station))
            self.graph.nodes[station]["passengers_waiting"] -= outflow
        
        # Update venue count
        venue_inflow = sum(self.graph[u][self.venue]["flow"] for u in self.graph.predecessors(self.venue))
        self.graph.nodes[self.venue]["passengers_arrived"] += venue_inflow
        self.people_transported += venue_inflow
        
        return total_flow

    def analyze_bottlenecks(self):
        """Identify bottleneck roads in the network"""
        bottlenecks = []
        for u, v, data in self.graph.edges(data=True):
            if data["flow"] == data["capacity"]:
                bottlenecks.append((u, v, data["capacity"]))
        
        if bottlenecks:
            print("\n📊 BOTTLENECK ANALYSIS 📊")
            print("The following roads are operating at maximum capacity:")
            for u, v, capacity in bottlenecks:
                print(f"  - {u} to {v}: {capacity} people/hour")
            print("\nThese roads are limiting the overall flow of people to the venue.")
            print("Consider increasing their capacity or finding alternative routes.")
        else:
            print("\nNo bottlenecks detected in the current configuration.")
        
        # Check for unused capacity
        unused_capacity = []
        for u, v, data in self.graph.edges(data=True):
            if data["flow"] == 0 and ((self.time_of_day == "Day") or 
                                      (self.time_of_day == "Night" and data["night_allowed"])):
                unused_capacity.append((u, v, data["capacity"]))
        
        if unused_capacity:
            print("\n💡 UNUSED CAPACITY 💡")
            print("The following roads have unused capacity:")
            for u, v, capacity in unused_capacity:
                print(f"  - {u} to {v}: {capacity} people/hour available")
            print("\nConsider redirecting traffic to these roads.")

    def modify_road(self):
        """Allow the user to modify a road in the network"""
        print("\n🛠️ ROAD MODIFICATION 🛠️")
        print("Select a road to modify:")
        
        # List all roads
        roads = list(self.graph.edges(data=True))
        for i, (u, v, data) in enumerate(roads):
            print(f"{i+1}. {u} to {v} (Capacity: {data['capacity']}, Night allowed: {data['night_allowed']})")
        
        try:
            choice = int(input("\nEnter road number (or 0 to cancel): "))
            if choice == 0:
                return
            
            if 1 <= choice <= len(roads):
                u, v, data = roads[choice-1]
                print(f"\nModifying road: {u} to {v}")
                print("1. Increase capacity")
                print("2. Decrease capacity")
                print("3. Toggle night operation")
                
                action = int(input("\nChoose an action (1-3): "))
                
                if action == 1:
                    increase = int(input("Enter capacity increase amount: "))
                    self.graph[u][v]["capacity"] += increase
                    print(f"Capacity increased to {self.graph[u][v]['capacity']}")
                    self.score += 50
                    
                elif action == 2:
                    decrease = int(input("Enter capacity decrease amount: "))
                    new_capacity = max(0, self.graph[u][v]["capacity"] - decrease)
                    self.graph[u][v]["capacity"] = new_capacity
                    print(f"Capacity decreased to {new_capacity}")
                    self.score += 30
                    
                elif action == 3:
                    self.graph[u][v]["night_allowed"] = not self.graph[u][v]["night_allowed"]
                    status = "allowed" if self.graph[u][v]["night_allowed"] else "not allowed"
                    print(f"Night operation is now {status}")
                    self.score += 100
                    
                else:
                    print("Invalid action choice.")
            else:
                print("Invalid road number.")
        except ValueError:
            print("Please enter a valid number.")

    def add_new_road(self):
        """Allow the user to add a new road to the network"""
        print("\n🛣️ ADD NEW ROAD 🛣️")
        
        # List all nodes
        nodes = list(self.graph.nodes())
        print("Available nodes:")
        for i, node in enumerate(nodes):
            print(f"{i+1}. {node}")
        
        try:
            from_idx = int(input("\nSelect starting point (enter number): "))
            to_idx = int(input("Select ending point (enter number): "))
            
            if 1 <= from_idx <= len(nodes) and 1 <= to_idx <= len(nodes):
                from_node = nodes[from_idx-1]
                to_node = nodes[to_idx-1]
                
                # Check if this road already exists
                if self.graph.has_edge(from_node, to_node):
                    print("This road already exists. Try modifying it instead.")
                    return
                
                capacity = int(input("Enter road capacity (people/hour): "))
                night_allowed = input("Allow night operation? (y/n): ").lower() == 'y'
                
                self.graph.add_edge(from_node, to_node, 
                                   capacity=capacity, 
                                   flow=0, 
                                   night_allowed=night_allowed)
                
                print(f"New road added from {from_node} to {to_node}")
                self.score += 200
                self.achievements.add("Road Planner")
                
            else:
                print("Invalid node selection.")
        except ValueError:
            print("Please enter valid numbers.")

    def change_time(self):
        """Toggle between day and night"""
        self.time_of_day = "Night" if self.time_of_day == "Day" else "Day"
        print(f"\n🕒 Time changed to {self.time_of_day}")

    def next_day(self):
        """Advance to the next day"""
        if self.day < self.max_days:
            self.day += 1
            self.time_of_day = "Day"
            
            # Increase passenger numbers for the final day
            if self.day == 3:
                for station in self.stations:
                    self.graph.nodes[station]["passengers_waiting"] *= 2
                print("\n⚠️ FINAL DAY: Crowd has doubled! ⚠️")
                self.achievements.add("Final Countdown")
            else:
                # Refresh passengers for regular days
                for station in self.stations:
                    self.graph.nodes[station]["passengers_waiting"] += random.randint(500, 1500)
            
            print(f"\n🌞 Day {self.day} begins!")
        else:
            print("\n🎮 GAME OVER! The festival has ended.")
            self.show_final_results()
            return True
        return False

    def add_random_event(self):
        """Add a random event to challenge the player"""
        events = [
            self.event_road_closure,
            self.event_station_surge,
            self.event_capacity_reduction,
            self.event_vip_arrival
        ]
        random.choice(events)()

    def event_road_closure(self):
        """Close a random road"""
        valid_edges = [(u, v) for u, v, data in self.graph.edges(data=True) 
                      if u != self.venue and v != self.venue]  # Don't close roads directly to venue
        
        if valid_edges:
            u, v = random.choice(valid_edges)
            old_capacity = self.graph[u][v]["capacity"]
            self.graph[u][v]["capacity"] = 0
            print(f"\n⚠️ EVENT: Road from {u} to {v} is closed due to an accident! (Previous capacity: {old_capacity})")
            self.achievements.add("Road Manager")

    def event_station_surge(self):
        """Unexpected surge at a random station"""
        if self.stations:
            station = random.choice(self.stations)
            surge = random.randint(500, 1000)
            self.graph.nodes[station]["passengers_waiting"] += surge
            print(f"\n⚠️ EVENT: Unexpected crowd surge at {station}! {surge} additional passengers waiting.")
            self.achievements.add("Crowd Controller")

    def event_capacity_reduction(self):
        """Reduce capacity on several roads due to weather"""
        affected_roads = []
        for u, v, data in self.graph.edges(data=True):
            if random.random() < 0.3:  # 30% chance for each road
                reduction = random.randint(50, 150)
                new_capacity = max(data["capacity"] - reduction, 0)
                reduction_actual = data["capacity"] - new_capacity
                if reduction_actual > 0:
                    self.graph[u][v]["capacity"] = new_capacity
                    affected_roads.append((u, v, reduction_actual))
        
        if affected_roads:
            print("\n⚠️ EVENT: Bad weather! Several roads have reduced capacity:")
            for u, v, reduction in affected_roads:
                print(f"  - {u} to {v}: -{reduction} capacity")
            self.achievements.add("Weather Manager")
        else:
            print("\n⚠️ EVENT: Weather warning issued, but no significant impact.")

    def event_vip_arrival(self):
        """VIP arrival requires priority transport"""
        vip_station = random.choice(self.stations)
        print(f"\n⚠️ EVENT: VIP arrival at {vip_station}! You need to ensure they reach the venue.")
        
        # Run a special simulation just for VIPs
        vip_count = random.randint(50, 100)
        self.graph.nodes[vip_station]["passengers_waiting"] += vip_count
        
        print(f"  - {vip_count} VIPs added to {vip_station}")
        print("  - Ensure they reach the venue in your next simulation!")
        self.achievements.add("VIP Handler")

    def show_final_results(self):
        """Display final results and achievements"""
        print("\n🏆 FESTIVAL SHUTTLE SIMULATION RESULTS 🏆")
        print(f"Total score: {self.score}")
        print(f"Total people transported to venue: {self.people_transported}")
        
        # Calculate efficiency
        total_possible = 0
        for station in self.stations:
            # This is a rough estimate of total possible passengers
            total_possible += self.graph.nodes[station]["passengers_waiting"] + \
                             self.graph.nodes[self.venue]["passengers_arrived"]
        
        if total_possible > 0:
            efficiency = (self.people_transported / total_possible) * 100
            print(f"Transport efficiency: {efficiency:.1f}%")
        
        # Award final achievements
        if self.people_transported > 10000:
            self.achievements.add("Transport Tycoon")
        if self.score > 1000:
            self.achievements.add("Master Planner")
        
        print("\n🌟 ACHIEVEMENTS UNLOCKED 🌟")
        if self.achievements:
            for achievement in self.achievements:
                print(f"  - {achievement}")
        else:
            print("  No achievements unlocked")

    def run_simulation(self):
        """Run one iteration of the simulation"""
        print(f"\n🚌 RUNNING SIMULATION FOR DAY {self.day} - {self.time_of_day} 🚌")
        
        # Calculate maximum flow
        flow = self.ford_fulkerson(self.stations, self.venue)
        print(f"\nMaximum flow: {flow} people transported to venue")
        
        # Update score
        self.score += flow // 10
        
        # Check achievements
        if flow > 1000:
            self.achievements.add("Mass Transit Expert")
        if flow == 0:
            self.achievements.add("Traffic Jam")
        
        # Analyze the network
        self.analyze_bottlenecks()

    def display_menu(self):
        """Display the main menu"""
        print("\n🎮 FESTIVAL SHUTTLE SIMULATOR 🎮")
        print(f"Day {self.day}/{self.max_days} - {self.time_of_day}")
        print(f"Score: {self.score}")
        print("\nActions:")
        print("1. Run simulation")
        print("2. View network map")
        print("3. Modify existing road")
        print("4. Add new road")
        print("5. Change time (day/night)")
        print("6. Analyze bottlenecks")
        print("7. Add random event (challenge)")
        print("8. Advance to next day")
        print("9. Exit")
        
        choice = input("\nSelect an option (1-9): ")
        return choice

    def main_loop(self):
        """Main game loop"""
        self.create_default_network()
        
        print("\n🎵 WELCOME TO THE FESTIVAL SHUTTLE SIMULATOR 🎵")
        print("You are the logistics manager for a 3-day music festival.")
        print("Your goal is to transport as many people as possible to the venue.")
        print("Manage your shuttle routes wisely!")
        
        game_over = False
        while not game_over:
            choice = self.display_menu()
            
            if choice == '1':
                self.run_simulation()
            elif choice == '2':
                self.display_network()
            elif choice == '3':
                self.modify_road()
            elif choice == '4':
                self.add_new_road()
            elif choice == '5':
                self.change_time()
            elif choice == '6':
                self.analyze_bottlenecks()
            elif choice == '7':
                self.add_random_event()
            elif choice == '8':
                game_over = self.next_day()
            elif choice == '9':
                print("\nThank you for playing!")
                game_over = True
            else:
                print("Invalid choice. Please try again.")

if __name__ == "__main__":
    simulator = ShuttleFestivalSimulator()
    simulator.main_loop()
