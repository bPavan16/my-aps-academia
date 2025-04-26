
### Core Approaches:

1. **Depth-First Search (DFS) Backtracking**
   - Classic method.
   - Explore all possible paths recursively.
   - Backtrack when hitting dead ends.
   - Can be modified to **track weights/costs** along paths if needed.

2. **Breadth-First Search (BFS) (for all paths)**
   - Less common for _all paths_, but possible using **queues** to store paths.
   - Typically used in unweighted graphs but can be modified for weighted.

3. **Modified Dijkstra (for K shortest paths, not all paths)**
   - Not for **all** paths, but if you're looking for **best few** (shortest, second-shortest, etc.), then K-paths methods apply.

4. **Yen's Algorithm (for K Shortest Loopless Paths)**
   - Finds the **top K shortest** simple paths between two nodes.
   - Not exactly _all_ paths, but useful when graph is huge.

5. **A\* Search (Modified for All Paths)**
   - In theory, you could modify A* to find **all** paths, not just the optimal one.
   - Typically impractical for large graphs.

6. **Dynamic Programming + Memoization (for DAGs specifically)**
   - If the graph is a **DAG (Directed Acyclic Graph)**:
     - You can use DP to cache paths from intermediate nodes to the destination.
     - Way faster than raw DFS.

7. **Johnson's Algorithm**
   - Typically used for finding **all pairs shortest paths** in weighted directed graphs.
   - If you precompute using Johnson's, it helps when you want paths between any arbitrary source-destination quickly.
   - (but **not for listing all** possible paths.)

---

### Special Mentions (Not pure "all paths" but related):
- **Kahn’s algorithm** (topological sorting for DAGs) can help enumerate paths if you use it cleverly.
- **Bellman-Ford Algorithm** — finds shortest paths even with negative weights — but again, **only shortest**, not all.
- **Recursive Enumeration with Path Pruning** — A fancy DFS variant where you prune based on path length, cost, or constraints.

---

### TL;DR
> **If you want _all_ paths from source to destination in a directed weighted graph:**  
> ➔ **DFS + Backtracking** is the _most straightforward and flexible._  
> ➔ **If it's a DAG**, consider **Dynamic Programming** for optimization.

---
Of course! Here’s a **detailed table** comparing the main algorithms/methods for finding **all paths** (or related to finding multiple paths) between a **source and destination** in a **directed weighted graph**:

---

| Algorithm/Method                     | Time Complexity                               | Space Complexity         | Practical Use Case                                                      | Comments                                              |
|:-------------------------------------|:---------------------------------------------|:-------------------------|:------------------------------------------------------------------------|:------------------------------------------------------|
| **DFS + Backtracking**                | O(2^N) (worst case exponential)              | O(N) per path             | Finding **all possible paths**, especially in small or medium graphs   | Simple, flexible; not scalable for huge graphs        |
| **BFS (for all paths)**               | O(2^N) (similar to DFS)                      | O(N) per path             | Exploring all paths **level by level**                                 | Less common for all paths; queue overhead             |
| **Modified Dijkstra (for K paths)**   | O((V + E) log V) per shortest path found      | O(V + E)                  | Finding **shortest path** (only 1 or few)                               | Not for "all" paths, only **best** ones                |
| **Yen's K Shortest Paths Algorithm**  | O(K * (V + E + V log V))                      | O(KV)                     | Finding **K best loopless paths** between two nodes                    | For **top-K paths** problems                          |
| **A\* Search (Modified for All Paths)** | O(2^N) if not pruned                         | O(N) per path             | **Heuristic guided** all paths search; rarely used for all             | Only practical if very aggressive pruning is used     |
| **DP (in DAGs)**                      | O(V + E) (to find number of paths)            | O(V)                     | Fast path counting / enumeration **in DAGs**                           | Only for **acyclic graphs**; very efficient           |
| **Johnson’s Algorithm**               | O(V^2 log V + VE)                            | O(V^2)                    | **All pairs shortest paths**; useful for precomputation                | Not for **all** paths between two nodes               |
| **Recursive Enumeration + Pruning**   | O(2^N) but pruned by conditions              | O(N) per path             | Finding **paths under constraints** (e.g., limited weight, hops)       | Customizable pruning strategies                      |

---

### Quick Observations:
- **All paths** = usually **exponential time** (`O(2^N)`), unavoidable because number of paths can grow crazy fast.
- **DAGs** are your friend: **DP** can massively speed up computations if no cycles exist.
- **Weighted graphs**: you **don't need special handling** unless you care about path weight (like shortest, minimum cost, etc.).
- **Yen’s Algorithm** is perfect if you need "**several good paths**" and not literally all.
- **DFS** is usually the default choice for general all-path finding.

---

### ⚡ Practical Recommendation
| Graph Type            | Best Method          |
|:----------------------|:---------------------|
| Small/Medium Size      | DFS + Backtracking    |
| DAG (no cycles)        | DP + Memoization      |
| Only Few Paths Needed  | Yen's K Shortest Paths |
| Precompute All Pairs   | Johnson’s Algorithm    |

---

