import heapq

def read_matrix(filename):
    with open(filename, "r") as f:
        return [
            list(map(int, line.strip().split(",")))
            for line in f
        ]

def min_path_sum(matrix):
    n = len(matrix)
    directions = [(1,0), (-1,0), (0,1), (0,-1)]

    dist = [[float('inf')] * n for _ in range(n)]
    dist[0][0] = matrix[0][0]

    pq = [(matrix[0][0], 0, 0)]

    while pq:
        cost, x, y = heapq.heappop(pq)

        if (x, y) == (n - 1, n - 1):
            return cost

        if cost > dist[x][y]:
            continue

        for dx, dy in directions:
            nx, ny = x + dx, y + dy

            if 0 <= nx < n and 0 <= ny < n:
                new_cost = cost + matrix[nx][ny]

                if new_cost < dist[nx][ny]:
                    dist[nx][ny] = new_cost
                    heapq.heappush(pq, (new_cost, nx, ny))

    return dist[n - 1][n - 1]

matrix = read_matrix("matrix.txt")

result = min_path_sum(matrix)

print("Minimum path sum:", result)