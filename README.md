# EV Charging Network

[![ci](https://github.com/yashbagia1708/ev-charging-network/actions/workflows/ci.yml/badge.svg)](https://github.com/yashbagia1708/ev-charging-network/actions/workflows/ci.yml)

A C++ console program that models 24 EV charging stations across Sydney as a
weighted graph and answers routing and cost questions over it: the nearest
open charger, the cheapest one once travel is counted, and the cheapest
place to stop between two suburbs.

Built for a university Data Structures and Algorithms unit. The point of it
is the data structures and the graph algorithms, written by hand rather than
pulled from a library.

## What it does

```
Menu:
1. Print location information
2. Print adjacency matrix
3. Change station availability
4. Generate minimum spanning tree
5. List available charging stations by price
6. Find the nearest available charging station
7. Find the cheapest charging station (considering travel costs)
8. Find path with minimal total cost
9. Find the best charging path (multiple charging)
```

| Option | How it works |
|---|---|
| 4. Minimum spanning tree | Prim's algorithm from Parramatta: the shortest power line that connects every station (221.5 km) |
| 5. Stations by price | Filters open stations and sorts them with a custom comparator |
| 6. Nearest station | Dijkstra from your location, then the closest open station |
| 7. Cheapest station | Dijkstra, then charging cost plus a return trip at $0.08/km |
| 8. Cheapest stop on a trip | For every open station: origin → station → destination distance (two Dijkstra runs) plus its charging cost |
| 9. Best charging path | Over 30 kWh: the cheapest detour through a free station. Up to 30 kWh: charge at the nearest open station and drive on |

Example, option 8 (Penrith to Central, 10 kWh):

```
The cheapest charging station is Blacktown.
Charging cost = $0.00
Travel cost = $6.576
Total cost = $6.576
Travel path: Penrith, Blacktown, Central
```

## Data structures and algorithms

- **Weighted graph:** adjacency lists for neighbours and a matrix of
  distances, loaded from `Distance.txt` (0 means no direct road).
- **`std::map<int, Station>`:** station details by index, loaded from
  `Locations.txt` (name, open or closed, price per kWh).
- **Priority queue (min-heap):** drives both Dijkstra and Prim.

| Algorithm | Used for | Complexity |
|---|---|---|
| Dijkstra | shortest distances from a station | O((V + E) log V) |
| Prim | minimum spanning tree (power line) | O(V²), since each step scans the matrix row |
| Sort with comparator | stations by price | O(n log n) |

Option 8 runs Dijkstra once from the origin and once from each open station,
so it's O(V · (V + E) log V). That's fine for 24 stations.

## Running it

Needs a C++17 compiler.

```bash
make          # builds ./evcharging
make run      # starts the menu
make test     # scripted smoke test, the same one CI runs
```

Run it from the repo root: the program reads `Locations.txt` and
`Distance.txt` from the current directory. Station names are typed exactly as
listed by option 1 (e.g. `Penrith`, `Central`).

## Limitations

- Station data and distances are fixed text files; changes made with option 3
  only last for the session.
- The spanning tree always starts from Parramatta (index 5).
- Distances are treated as kilometres, and the $0.08/km travel rate is hard
  coded.
- Option 9's two rules (free station over 30 kWh, nearest station otherwise)
  come from the assignment brief; they aren't a general multi-stop planner.

## Changes since the original submission

Fixed after the original upload (see the pull requests for detail):

- Option 8 printed a travel cost from the wrong Dijkstra run, so the cost it
  showed didn't match the one it chose by. Its travel path also left out the
  destination.
- Option 9 labelled charging as free without checking the station was free,
  and in the up-to-30 kWh case it didn't apply the $0.08/km rate.
