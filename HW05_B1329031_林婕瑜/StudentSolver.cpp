#ifndef STUDENTSOLVER_CPP
#define STUDENTSOLVER_CPP

#include "Maze.cpp"


//TODO 1 : Solve by DFS
bool solveByDFS(const Maze& maze, PathNode*& path) {
    int n  = maze.getN();
    int sR = maze.getStartR(), sC = maze.getStartC();
    int eR = maze.getEndR(),   eC = maze.getEndC();
    path = nullptr;

    // 配置輔助陣列
    bool** visited = new bool*[n];
    int**  parentR = new int*[n];
    int**  parentC = new int*[n];
    for (int i = 0; i < n; i++) {
        visited[i] = new bool[n]();
        parentR[i] = new int[n];
        parentC[i] = new int[n];
        for (int j = 0; j < n; j++)
            parentR[i][j] = parentC[i][j] = -1;
    }

    // Stack：用 PathNode linked list 實作（LIFO，push 接在 head 前）
    PathNode* stackHead = new PathNode(sR, sC, nullptr);

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    bool found = false;

    while (stackHead != nullptr) {
        // pop
        PathNode* top = stackHead;
        int r = top->r, c = top->c;
        stackHead = stackHead->next;
        delete top;

        if (visited[r][c]) continue;
        visited[r][c] = true;

        if (r == eR && c == eC) { found = true; break; }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && !visited[nr][nc] && maze.isPassable(nr, nc)) {
                parentR[nr][nc] = r;
                parentC[nr][nc] = c;
                // push
                stackHead = new PathNode(nr, nc, stackHead);
            }
        }
    }

    // 釋放 stack 剩餘節點
    freeList(stackHead);

    if (found) {
        // 從終點回溯，每次 prepend → 最終順序為 起點→...→終點
        PathNode* result = nullptr;
        int r = eR, c = eC;
        while (!(r == sR && c == sC)) {
            result = new PathNode(r, c, result);
            int pr = parentR[r][c], pc = parentC[r][c];
            r = pr; c = pc;
        }
        result = new PathNode(sR, sC, result);
        path = result;
    }

    // 釋放輔助陣列
    for (int i = 0; i < n; i++) {
        delete[] visited[i];
        delete[] parentR[i];
        delete[] parentC[i];
    }
    delete[] visited;
    delete[] parentR;
    delete[] parentC;

    return found;
}


//TODO 2 : Solve by BFS
bool solveByBFS(const Maze& maze, PathNode*& path) {
    int n  = maze.getN();
    int sR = maze.getStartR(), sC = maze.getStartC();
    int eR = maze.getEndR(),   eC = maze.getEndC();
    path = nullptr;

    // 配置輔助陣列
    bool** visited = new bool*[n];
    int**  parentR = new int*[n];
    int**  parentC = new int*[n];
    for (int i = 0; i < n; i++) {
        visited[i] = new bool[n]();
        parentR[i] = new int[n];
        parentC[i] = new int[n];
        for (int j = 0; j < n; j++)
            parentR[i][j] = parentC[i][j] = -1;
    }

    // Queue：用 PathNode linked list 實作（FIFO，enqueue 接在 tail，dequeue 從 head）
    PathNode* qHead = new PathNode(sR, sC, nullptr);
    PathNode* qTail = qHead;
    visited[sR][sC] = true;

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    bool found = false;

    while (qHead != nullptr) {
        // dequeue
        int r = qHead->r, c = qHead->c;
        PathNode* old = qHead;
        qHead = qHead->next;
        if (!qHead) qTail = nullptr;
        delete old;

        if (r == eR && c == eC) { found = true; break; }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && !visited[nr][nc] && maze.isPassable(nr, nc)) {
                visited[nr][nc] = true;
                parentR[nr][nc] = r;
                parentC[nr][nc] = c;
                // enqueue
                PathNode* node = new PathNode(nr, nc, nullptr);
                if (qTail) qTail->next = node;
                else        qHead = node;
                qTail = node;
            }
        }
    }

    // 釋放 queue 剩餘節點
    freeList(qHead);

    if (found) {
        // 從終點回溯，prepend → 起點→...→終點
        PathNode* result = nullptr;
        int r = eR, c = eC;
        while (!(r == sR && c == sC)) {
            result = new PathNode(r, c, result);
            int pr = parentR[r][c], pc = parentC[r][c];
            r = pr; c = pc;
        }
        result = new PathNode(sR, sC, result);
        path = result;
    }

    // 釋放輔助陣列
    for (int i = 0; i < n; i++) {
        delete[] visited[i];
        delete[] parentR[i];
        delete[] parentC[i];
    }
    delete[] visited;
    delete[] parentR;
    delete[] parentC;

    return found;
}

#endif
