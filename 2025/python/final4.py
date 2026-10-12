import sys
import networkx as nx
from typing import List, Tuple, Optional


# ─────────────────────────── Алгоритм Краскала ───────────────────────────────

def find(parent: List[int], x: int) -> int:
    """
    Находит корень для вершины x

    Параметры:
        parent - список родителей вершин
        x      - вершина, чей корень нужно найти

    Возвращает: корень вершины x
    """
    while parent[x] != x:
        parent[x] = parent[parent[x]]
        x = parent[x]
    return x


def kruskal_mst(n: int, edges: List[Tuple[int, int, float]]) -> Optional[Tuple[List, float]]:
    """
    Строит минимальное остовное дерево алгоритмом Краскала

    Параметры:
        n     — число вершин (нумерация с 0)
        edges — список рёбер (u, v, w)

    Возвращает:
        (mst_edges, total_weight) - рёбра МОД и суммарный вес
        None                      - если граф несвязный

    Алгоритм: сортируем рёбра по весу, жадно добавляем ребро, если оно не создаёт цикл. O(E log E)
    """
    parent = list(range(n))

    def union(x, y):
        rx, ry = find(parent, x), find(parent, y)
        if rx == ry:
            return False
        parent[rx] = ry
        return True

    result = []
    for u, v, w in sorted(edges, key=lambda e: e[2]):
        if union(u, v):
            result.append((u, v, w))
            if len(result) == n - 1: break

    if len(result) < n - 1: return None

    return result, sum(w for _, _, w in result)


# ──────────────────────────────── networkx ───────────────────────────────────

def nx_mst(n: int, edges: List[Tuple[int, int, float]]) -> Optional[Tuple[List, float]]:
    G = nx.Graph()
    G.add_nodes_from(range(n))
    G.add_weighted_edges_from(edges)
    if not nx.is_connected(G): return None
    T = nx.minimum_spanning_tree(G)
    mst_edges = [(u, v, d["weight"]) for u, v, d in T.edges(data=True)]
    return sorted(mst_edges), T.size(weight="weight")


# ──────────────────────────────── Чтение ─────────────────────────────────────

def read_input(filename: str) -> List[Tuple[int, List]]: #NOSONAR
    try:
        with open(filename, encoding="utf-8") as file: it = iter(file.read().split())
    except FileNotFoundError:
        print(f"Ошибка: файл '{filename}' не найден.")
        sys.exit(1)

    def next_token() -> str:
        try: return next(it)
        except StopIteration: raise ValueError("Неожиданный конец файла.")

    def read_int() -> int:
        value = next_token()
        try: return int(value)
        except ValueError: raise ValueError(f"Ожидалось целое число, получено: '{value}'")

    def read_weight() -> float:
        value = next_token()
        try: weight = float(value)
        except ValueError: raise ValueError(f"Ожидалось вещественное число, получено: '{value}'")
        if weight < 0: raise ValueError(f"Вес не может быть отрицательным: {weight}")
        return weight

    graphs = []
    for _ in range(read_int()):
        n, k = read_int(), read_int()
        if n < 1: raise ValueError(f"Число вершин должно быть >= 1, получено: {n}")
        if k < 0: raise ValueError(f"Число рёбер не может быть отрицательным: {k}")

        edges = []
        for _ in range(k):
            u, v, w = read_int(), read_int(), read_weight()
            if not (1 <= u <= n and 1 <= v <= n): raise ValueError(f"Вершины {u}, {v} вне диапазона [1, {n}]")
            edges.append((u - 1, v - 1, w))
        graphs.append((n, edges))

    return graphs


# ──────────────────────────────── Вывод ──────────────────────────────────────

W = 52

def fmt(w: float) -> str: return f"{w:.4f}".rstrip("0").rstrip(".")

def print_edges(edges: List, indent: str = "   ") -> None:
    for u, v, w in edges: print(f"{indent}{u+1} <-> {v+1}   {fmt(w)}")

def print_mst_result(label: str, result: Optional[Tuple]) -> None:
    print(f"\n   [ {label} ]")
    if result is None:
        print("   Граф несвязный — МОД не существует")
        return
    mst, total = result
    print_edges(mst)
    print(f"   {'-' * 22}")
    print(f"   Вес МОД : {fmt(total)}")


# ──────────────────────────────── Main ───────────────────────────────────────

def main():
    filename = sys.argv[1] if len(sys.argv) > 1 else "input.txt"

    try: graphs = read_input(filename)
    except ValueError as e:
        print(f"Ошибка входных данных: {e}")
        sys.exit(1)

    for i, (n, edges) in enumerate(graphs, 1):
        print("\n" + "=" * W)
        print(f"  ГРАФ {i}   ({n} вершин, {len(edges)} рёбер)".center(W))
        print("-" * W)

        print("\n   [ Исходные рёбра ]")
        print_edges(edges)

        print("\n" + ("." * (W - 5)).center(W))
        result = kruskal_mst(n, edges)
        print_mst_result("Краскал", result)

        print("\n" + ("." * (W - 5)).center(W))
        lib_result = nx_mst(n, edges)
        print_mst_result("networkx", lib_result)

        if result is not None and lib_result is not None:
            match = abs(result[1] - lib_result[1]) < 1e-9
            print(f"\n   Веса совпадают: {'+' if match else '-'}")

        print("\n"+"=" * W)

    print()

if __name__ == "__main__": main()