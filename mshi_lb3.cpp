#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
using namespace std;

// Передавання точок кластеру
struct fp {
    double x;
    double y;
    int c;
    fp(double a, double b) : x(a), y(b), c(-1) {}
};

// Розрахунок Евклідової відстані між двома точками
double dp(const fp& p1, const fp& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Функція для виконання алгоритму кластеризації K-середніх
void kp(vector<fp>& fps, int k) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, fps.size() - 1);
    // Ініціалізація центрів кластерів
    vector<fp> centr;
    for (int i = 0; i < k; i++) {
        int idx = dis(gen);
        centr.push_back(fps[idx]);
    }
    int t = 0;
    while (t == 0) {
        // Призначення точок до найближчих центрів кластерів
        for (fp& p : fps) {
            double md = numeric_limits<double>::max();
            for (int i = 0; i < k; i++) {
                double d = dp(p, centr[i]);
                if (d < md) {
                    md = d;
                    p.c = i;
                }
            }
        }
        // Оновлення центрів кластерів
        vector<fp> ncentr(k, fp(0, 0));
        vector<int> cc(k, 0);
        for (const fp& p : fps) {
            ncentr[p.c].x += p.x;
            ncentr[p.c].y += p.y;
            cc[p.c]++;
        }
        t++;
        for (int i = 0; i < k; i++) {
            if (cc[i] > 0) {
                ncentr[i].x /= cc[i];
                ncentr[i].y /= cc[i];
                if (ncentr[i].x != centr[i].x || ncentr[i].y != centr[i].y) {
                    t = 0;
                }
            }
        }
        centr = ncentr;
    }
}

// Функція для виконання алгоритму кластеризації C-середніх
void cp(vector<fp>& fps, int k) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    // Ініціалізація вагових коефіцієнтів кластерів
    vector<double> w(k);
    double sum = 0.0;
    for (int i = 0; i < k; i++) {
        w[i] = dis(gen);
        sum += w[i];
    }
    for (int i = 0; i < k; i++) {
        w[i] /= sum;
    }
    int t = 0;
    while (t == 0) {
        // Призначення точок до кластерів з використанням вагових коефіцієнтів
        for (fp& p : fps) {
            double md = numeric_limits<double>::max();
            for (int i = 0; i < k; i++) {
                double d = dp(p, fp(w[i], w[i]));
                if (d < md) {
                    md = d;
                    p.c = i;
                }
            }
        }
        // Оновлення вагових коефіцієнтів кластерів
        vector<double> nw(k, 0.0);
        vector<int> cc(k, 0);
        for (const fp& p : fps) {
            nw[p.c] += p.x;
            cc[p.c]++;
        }
        t++;
        for (int i = 0; i < k; i++) {
            if (cc[i] > 0) {
                nw[i] /= cc[i];
                if (nw[i] != w[i]) {
                    t = 0;
                }
            }
        }
        w = nw;
    }
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    const int N = 1000;// Кількість N значень послідовності
    const int k = 3; // Кількість кластерів

    //Генерація послідовності з N значень
    vector<fp> p_k;
    vector<fp> p_c;
    for (int i = 0; i < N; i++) {
        double x = dis(gen);
        double y = dis(gen);
        p_k.push_back(fp(x, y));
        p_c.push_back(fp(x, y));
    }

    //Вивід послідовності у текстовий файл
    //ofstream pFile("C:\\Users\\user\\Desktop\\p.txt");
    //for (const fp& p : p_k) {
    //    pFile << p.x << " " << p.y << endl;
    //}
    //pFile.close();

    kp(p_k, k);
    cp(p_c, k);

    // Обчислення середньо-зваженого розміру кластерів
    vector<int> ksize(k, 0);
    vector<int> csize(k, 0);
    for (const fp& p : p_k) {
        ksize[p.c]++;
    }
    for (const fp& p : p_c) {
        csize[p.c]++;
    }
    double ksz = 0.0;
    double csz = 0.0;
    for (int i = 0; i < k; i++) {
        ksz += ksize[i] * ksize[i];
        csz += csize[i] * csize[i];
    }

    cout << " serednio-zvagene K-sehedn: " << ksz << endl;
    cout << " serednio-zvagene C-sehedn: " << csz << endl;

    //Вивід кластеризованих послідовностей у текстові файли
    //ofstream kFile("C:\\Users\\user\\Desktop\\k.txt");
    //for (const fp& p : p_k) {
    //    kFile << p.x << " " << p.y << " " << p.c << endl;
    //}
    //kFile.close();
    //ofstream cFile("C:\\Users\\user\\Desktop\\c.txt");
    //for (const fp& p : p_c) {
    //    cFile << p.x << " " << p.y << " " << p.c << endl;
    //}
    //cFile.close();

    return 0;
}
