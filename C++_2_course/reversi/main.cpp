#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <assert.h>

using namespace std;

class Field {
public:
    static const int N = 8;
    static const char BLACK = 'B';
    static const char WHITE = 'W';
    static const char EMPTY = '.';
private:
    char tab[N][N];
    void clear() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                tab[i][j] = EMPTY;
            }
        }
    }
    void setBeginState() {
        set(3, 3, BLACK);
        set(4, 4, BLACK);
        set(3, 4, WHITE);
        set(4, 3, WHITE);
    }
public:


    static char getOtherPlayer(char player) {
        if (player == BLACK) {
            return WHITE;
        }
        return BLACK;
    }

    Field() {
        clear();
        setBeginState();
    }

    char get(int i, int j) const {
        return tab[i][j];
    }

    void set(int i, int j, char player) {
        tab[i][j] = player;
    }

    pair<int, int> getChipsCount() const {
        pair<int, int> res(0, 0);
        for (int i = 0; i < Field::N; ++i) {
            for (int j = 0; j < Field::N; ++j) {
                if (tab[i][j] == BLACK) {
                    res.first++;
                } else if (tab[i][j] == WHITE) {
                    res.second++;
                }
            }
        }
        return res;
    }

    void print() const {
        for (int i = 7; i >= 0; --i) {
            for (int j = 0; j < 8; ++j) {
                cout << tab[i][j];
            }
            cout << endl;
        }
    }
};

struct Gamer {
    virtual bool doMove(Field& filed, char player) const = 0;
    static bool isCellExist(int x, int y) {
        return x >= 0 && y >= 0 && x < Field::N && y < Field::N;
    }
    bool canDoThisMove(Field& field, int x, int y, char player) const {
        if (!isCellExist(x, y) || field.get(x, y) != Field::EMPTY) {
            return false;
        }
        int dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
        int dy[] = {1, 1, 1, 0, -1, -1, -1, 0};
        for (int i = 0; i < 8; ++i) {
            int curX = x + dx[i];
            int curY = y + dy[i];
            if (isCellExist(curX, curY) && field.get(curX, curY) == Field::getOtherPlayer(player)) {
                while (isCellExist(curX, curY) && field.get(curX, curY) == Field::getOtherPlayer(player)) {
                    curX += dx[i];
                    curY += dy[i];
                }
                if (isCellExist(curX, curY) && field.get(curX, curY) == player) {
                    return true;
                }
            }
        }
        return false;
    }
    vector<pair<int, int> > doThisMove(Field& field, int x, int y, char player) const {
        assert(isCellExist(x, y) && field.get(x, y) == Field::EMPTY);
        int dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
        int dy[] = {1, 1, 1, 0, -1, -1, -1, 0};
        vector <pair<int, int> > changed;
        for (int i = 0; i < 8; ++i) {
            int curX = x + dx[i];
            int curY = y + dy[i];
            if (isCellExist(curX, curY) && field.get(curX, curY) == Field::getOtherPlayer(player)) {
                while (isCellExist(curX, curY) && field.get(curX, curY) == Field::getOtherPlayer(player)) {
                    curX += dx[i];
                    curY += dy[i];
                }
                if (isCellExist(curX, curY) && field.get(curX, curY) == player) {
                    curX -= dx[i];
                    curY -= dy[i];
                    while (isCellExist(curX, curY) && field.get(curX, curY) == Field::getOtherPlayer(player)) {
                        field.set(curX, curY, player);
                        changed.push_back(make_pair(curX, curY));
                        curX -= dx[i];
                        curY -= dy[i];
                    }
                }
            }
        }
        field.set(x, y, player);
        changed.push_back(make_pair(x, y));

        return changed;
    }

    void retChanges(Field& field, vector<pair<int, int> >& changed) const {
        for (auto &x : changed) {
            field.set(x.first, x.second, Field::getOtherPlayer(field.get(x.first, x.second)));
        }
        field.set(changed.back().first, changed.back().second, Field::EMPTY);
    }
};

struct Gamer1 : public Gamer {
    int depth = 6;

    struct Situation {
        int black;
        int white;
        int bestX;
        int bestY;
        Situation(int black = 0, int white = 0, int bestX = -1, int bestY = -1) :
                black(black), white(white), bestX(bestX), bestY(bestY)
        {}
        Situation(pair<int, int> p, int bestX = -1, int bestY = -1) :
                black(p.first), white(p.second), bestX(bestX), bestY(bestY)
        {}
    };

    //! не изменяет поле
    Situation dfs(Field& field, char player, int d) const {
        if (d == depth) {
            return Situation(field.getChipsCount());
        }
        Situation best;
        for (int i = 0; i < Field::N; ++i) {
            for (int j = 0; j < Field::N; ++j) {
                if (canDoThisMove(field, i, j, player)) {
                    auto changed = doThisMove(field, i, j, player);
                    Situation next = dfs(field, Field::getOtherPlayer(player), d + 1);
                    if (d % 2 == 0 ^ player == Field::BLACK) {
                        if (next.black > best.black) {
                            best = next;
                            best.bestX = i;
                            best.bestY = j;
                        }
                    } else {
                        if (next.white > best.white) {
                            best = next;
                            best.bestX = i;
                            best.bestY = j;
                        }
                    }
                    retChanges(field, changed);
                }
            }
        }
        if (best.bestX == -1) { //! Игрок не может сделать ход
            return dfs(field, player, d + 1);
        }
        return best;
    }

    //! Сделай "лучший" ход для данного игрока
    bool doMove(Field& field, char player) const {
        Situation move = dfs(field, player, 0);
        if (canDoThisMove(field, move.bestX, move.bestY, player)) {
            doThisMove(field, move.bestX, move.bestY, player);
            return true;
        }
        return false;
    }
};

int main() {
    Field field;
    Gamer1 gamer1;
    field.print();
    char player = Field::BLACK;
    bool predOk = false;
    for (int i = 0; i < 100; ++i) {
        bool ok = gamer1.doMove(field, player);
        player = Field::getOtherPlayer(player);
        field.print();
        cout << endl;
        if (!ok && !predOk) {
            cout << "GAME OVER" << endl;
            break;
        }
        predOk = ok;
    }
    cout << field.getChipsCount().first << " " << field.getChipsCount().second << endl;
    gamer1.doMove(field, player);
    player = Field::getOtherPlayer(player);
    gamer1.doMove(field, player);
    return 0;
}