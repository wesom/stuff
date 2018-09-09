# https://www.lintcode.com/problem/minimum-risk-path/description
class Graph:

    def __init__(self, vertices):
        self.V = vertices
        self.graph = []

    def addEdge(self, u, v, w):
        self.graph.append([u, v, w])

    # 是否同一树
    def find(self, parent, i):
        if parent[i] == i:
            return i
        return self.find(parent, parent[i])

    # 路径压缩 合并
    def union(self, parent, rank, x, y):
        xroot = self.find(parent, x)
        yroot = self.find(parent, y)

        if rank[xroot] < rank[yroot]:
            parent[xroot] = yroot
        elif rank[xroot] > rank[yroot]:
            parent[yroot] = xroot
        else:
            parent[yroot] = xroot
            rank[xroot] += 1

    def KruskalMST(self, s, des):
        result = []
        i = 0
        e = 0
        self.graph = sorted(self.graph, key=lambda item: item[2])
        parent = []
        rank = []

        for node in range(self.V):
            parent.append(node)
            rank.append(0)

        des = min(des, len(parent) - 1)

        # 并查集
        while e < self.V - 1:
            u, v, w = self.graph[i]
            i = i + 1
            print(u, v, self.V)
            x = self.find(parent, u)
            y = self.find(parent, v)
            if x != y:
                # 合并到一个树下
                e = e + 1
                result.append([u, v, w])
                self.union(parent, rank, x, y)
                if self.find(parent, s) == self.find(parent, des):
                    return w
        return -1

class Solution:
    def getMinRiskValue(self, n, m, x, y, w):
        g = Graph(500)
        for i in range(m):
            g.addEdge(x[i], y[i], w[i])
        return g.KruskalMST(0, n)

def case1():
    print('list graph:')
    n = 5
    m = 7
    x = [0, 0, 1, 2, 3, 3, 4]
    y = [1, 2, 3, 4, 4, 5, 5]
    w = [2, 5, 3, 4, 3, 4, 1]
    s = Solution()
    ret = s.getMinRiskValue(n, m, x, y, w)
    print(ret)

def case2():
    print('list graph:')
    n = 4
    m = 5
    x = [0, 1, 1, 2, 3]
    y = [1, 2, 3, 4, 4]
    w = [3, 2, 4, 2, 1]
    s = Solution()
    ret = s.getMinRiskValue(n, m, x, y, w)
    print(ret)

def case3():
    print('list graph:')
    n = 2
    m = 2
    x = [0, 1]
    y = [1, 2]
    w = [1, 2]
    s = Solution()
    ret = s.getMinRiskValue(n, m, x, y, w)
    print(ret)

if __name__ == '__main__':
    case1()
    case2()
    case3()