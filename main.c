#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 20
#define INF_DIST 99999

/* Global Data Structures */
int costMatrix[MAX_NODES][MAX_NODES];
int distance[MAX_NODES];
int predecessor[MAX_NODES];
char locationNames[MAX_NODES][50];
int totalLocations = 0;
int selectedSource = -1;

/* Function to find the unvisited vertex with minimum tentative distance */
int findMinDistanceNode(int dist[], int visited[], int n) {
    int minVal = INF_DIST;
    int minIndex = -1;

    for (int v = 0; v < n; v++) {
        if (!visited[v] && dist[v] < minVal) {
            minVal = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

/* Recursive function to reconstruct and print the shortest path */
void displayRoutePath(int destNode) {
    if (predecessor[destNode] == -1) {
        printf("%s", locationNames[destNode]);
        return;
    }
    displayRoutePath(predecessor[destNode]);
    printf(" -> %s", locationNames[destNode]);
}

/* Input function to build the campus road network */
void inputCampusGraph() {
    int totalRoads;
    int u, v, weight;

    printf("\nEnter total number of campus locations (max %d): ", MAX_NODES);
    scanf("%d", &totalLocations);

    while (totalLocations <= 1 || totalLocations > MAX_NODES) {
        printf("Invalid count! Please enter a number between 2 and %d: ", MAX_NODES);
        scanf("%d", &totalLocations);
    }

    /* Input names for all locations */
    for (int i = 0; i < totalLocations; i++) {
        printf("Enter name of Location %d: ", i + 1);
        scanf(" %49[^\n]", locationNames[i]);
    }

    /* Initialize adjacency matrix */
    for (int i = 0; i < totalLocations; i++) {
        for (int j = 0; j < totalLocations; j++) {
            if (i == j)
                costMatrix[i][j] = 0;
            else
                costMatrix[i][j] = INF_DIST;
        }
    }

    printf("\nEnter total number of roads connecting these locations: ");
    scanf("%d", &totalRoads);

    printf("\nEnter road connections in the format: <Source_No> <Destination_No> <Distance>\n");
    for (int i = 0; i < totalRoads; i++) {
        printf("Road %d: ", i + 1);
        scanf("%d %d %d", &u, &v, &weight);

        if (u < 1 || u > totalLocations || v < 1 || v > totalLocations || weight < 0) {
            printf("Invalid entry! Node indices must be between 1 and %d with non-negative weight. Try again.\n", totalLocations);
            i--;
            continue;
        }

        /* Undirected campus road network */
        costMatrix[u - 1][v - 1] = weight;
        costMatrix[v - 1][u - 1] = weight;
    }

    printf("\n[Success] Campus road network successfully created!\n");
}

/* Display the Adjacency/Cost Matrix */
void printCostMatrix() {
    printf("\n---------------- Campus Adjacency Matrix ----------------\n\n\t");
    for (int i = 0; i < totalLocations; i++) {
        printf("[%d]\t", i + 1);
    }
    printf("\n");

    for (int i = 0; i < totalLocations; i++) {
        printf("[%d]\t", i + 1);
        for (int j = 0; j < totalLocations; j++) {
            if (costMatrix[i][j] == INF_DIST)
                printf("INF\t");
            else
                printf("%d\t", costMatrix[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------------------------------------\n");
}

/* Dijkstra's Shortest Path Algorithm */
void executeDijkstra(int src) {
    int visited[MAX_NODES] = {0};

    /* Initialization step */
    for (int i = 0; i < totalLocations; i++) {
        distance[i] = INF_DIST;
        predecessor[i] = -1;
    }

    distance[src] = 0;

    /* Main algorithm loop */
    for (int step = 0; step < totalLocations; step++) {
        int u = findMinDistanceNode(distance, visited, totalLocations);

        /* All remaining reachable nodes processed */
        if (u == -1 || distance[u] == INF_DIST)
            break;

        visited[u] = 1;

        /* Relaxation step for adjacent neighbors */
        for (int v = 0; v < totalLocations; v++) {
            if (!visited[v] && costMatrix[u][v] != INF_DIST) {
                if (distance[u] + costMatrix[u][v] < distance[v]) {
                    distance[v] = distance[u] + costMatrix[u][v];
                    predecessor[v] = u;
                }
            }
        }
    }
}

/* Display full route paths with total distance */
void displayShortestPaths() {
    printf("\nStarting Point: %s\n", locationNames[selectedSource]);
    printf("====================================================================================\n");
    printf("%-25s | %-12s | %s\n", "Destination", "Distance", "Shortest Path");
    printf("====================================================================================\n");

    for (int i = 0; i < totalLocations; i++) {
        if (i == selectedSource)
            continue;

        if (distance[i] == INF_DIST) {
            printf("%-25s | %-12s | No Route Available\n", locationNames[i], "Unreachable");
        } else {
            printf("%-25s | %-12d | ", locationNames[i], distance[i]);
            displayRoutePath(i);
            printf("\n");
        }
    }
    printf("====================================================================================\n");
}

/* Display summary distance table */
void displayDistanceSummary() {
    printf("\nShortest Distances from %s:\n", locationNames[selectedSource]);
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < totalLocations; i++) {
        if (distance[i] == INF_DIST)
            printf("%-25s : Unreachable\n", locationNames[i]);
        else
            printf("%-25s : %d units\n", locationNames[i], distance[i]);
    }
    printf("-----------------------------------------------------\n");
}

/* Main driver program */
int main() {
    int userChoice;
    int isGraphReady = 0;
    int isSourceConfigured = 0;

    do {
        printf("\n=====================================================\n");
        printf("         CAMPUS SHORTEST ROUTE FINDER\n");
        printf("              (DIJKSTRA'S ALGORITHM)\n");
        printf("=====================================================\n");
        printf("1. Enter Campus Graph\n");
        printf("2. Display Adjacency Matrix\n");
        printf("3. Select Source Location\n");
        printf("4. Find Shortest Distance\n");
        printf("5. Display Shortest Paths\n");
        printf("6. Display Distance from Source to All Locations\n");
        printf("7. Exit\n");
        printf("-----------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        switch (userChoice) {
            case 1:
                inputCampusGraph();
                isGraphReady = 1;
                isSourceConfigured = 0;
                break;

            case 2:
                if (!isGraphReady) {
                    printf("\n[Error] Please configure the campus graph first.\n");
                } else {
                    printCostMatrix();
                }
                break;

            case 3:
                if (!isGraphReady) {
                    printf("\n[Error] Please configure the campus graph first.\n");
                    break;
                }

                printf("\nAvailable Campus Locations:\n");
                for (int i = 0; i < totalLocations; i++) {
                    printf("  %d. %s\n", i + 1, locationNames[i]);
                }

                printf("Choose source location number: ");
                scanf("%d", &selectedSource);

                if (selectedSource < 1 || selectedSource > totalLocations) {
                    printf("\n[Error] Invalid location selected.\n");
                    selectedSource = -1;
                    isSourceConfigured = 0;
                } else {
                    selectedSource--; // Convert to 0-based indexing
                    isSourceConfigured = 1;
                    printf("\n[Active Source] Selected: %s\n", locationNames[selectedSource]);
                }
                break;

            case 4:
                if (!isGraphReady) {
                    printf("\n[Error] Please configure the campus graph first.\n");
                } else if (!isSourceConfigured) {
                    printf("\n[Error] Please select a starting source location first.\n");
                } else {
                    executeDijkstra(selectedSource);
                    printf("\n[Done] Dijkstra's Algorithm executed successfully.\n");
                }
                break;

            case 5:
                if (!isGraphReady) {
                    printf("\n[Error] Please configure the campus graph first.\n");
                } else if (!isSourceConfigured) {
                    printf("\n[Error] Please select a starting source location first.\n");
                } else {
                    executeDijkstra(selectedSource);
                    displayShortestPaths();
                }
                break;

            case 6:
                if (!isGraphReady) {
                    printf("\n[Error] Please configure the campus graph first.\n");
                } else if (!isSourceConfigured) {
                    printf("\n[Error] Please select a starting source location first.\n");
                } else {
                    executeDijkstra(selectedSource);
                    displayDistanceSummary();
                }
                break;

            case 7:
                printf("\nExiting Campus Shortest Route Finder. Goodbye!\n");
                break;

            default:
                printf("\nInvalid menu selection! Please enter a number between 1 and 7.\n");
        }
    } while (userChoice != 7);

    return 0;
}
