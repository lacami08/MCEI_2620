#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Función para el desenvolvimiento angular (unwrap)
void unwrap(vector<double>& theta) {
    for (size_t i = 1; i < theta.size(); ++i) {
        double diff = theta[i] - theta[i - 1];
        while (diff > M_PI) {
            theta[i] -= 2.0 * M_PI;
            diff = theta[i] - theta[i - 1];
        }
        while (diff < -M_PI) {
            theta[i] += 2.0 * M_PI;
            diff = theta[i] - theta[i - 1];
        }
    }
}

int main() {
    ifstream file("trayectoria_robot.csv");
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo trayectoria_robot.csv" << endl;
        return 1;
    }

    string line;
    getline(file, line); // Omitir el encabezado "t,x,y"

    vector<double> t, x, y;
    while (getline(file, line)) {
        stringstream ss(line);
        string valT, valX, valY;
        if (getline(ss, valT, ',') && getline(ss, valX, ',') && getline(ss, valY, ',')) {
            t.push_back(stod(valT));
            x.push_back(stod(valX));
            y.push_back(stod(valY));
        }
    }
    file.close();

    size_t N = t.size();
    double h = t[1] - t[0];

    vector<double> vx(N), vy(N), v(N), theta(N), omega(N);

    // 1. Diferencias centrales para vx e vy
    for (size_t i = 1; i < N - 1; ++i) {
        vx[i] = (x[i + 1] - x[i - 1]) / (2.0 * h);
        vy[i] = (y[i + 1] - y[i - 1]) / (2.0 * h);
    }
    // Extremos (diferencias unilaterales)
    vx[0] = (x[1] - x[0]) / h;
    vx[N - 1] = (x[N - 1] - x[N - 2]) / h;
    vy[0] = (y[1] - y[0]) / h;
    vy[N - 1] = (y[N - 1] - y[N - 2]) / h;

    // 2. Magnitud de v y orientación theta
    for (size_t i = 0; i < N; ++i) {
        v[i] = sqrt(vx[i] * vx[i] + vy[i] * vy[i]);
        theta[i] = atan2(vy[i], vx[i]);
    }

    // 3. Aplicar unwrap a theta
    unwrap(theta);

    // 4. Diferencias centrales para omega
    for (size_t i = 1; i < N - 1; ++i) {
        omega[i] = (theta[i + 1] - theta[i - 1]) / (2.0 * h);
    }
    omega[0] = (theta[1] - theta[0]) / h;
    omega[N - 1] = (theta[N - 1] - theta[N - 2]) / h;

    // Mostrar los primeros 3 datos procesados
    cout << fixed << setprecision(6);
    cout << "--- Resultados procesados en C++ ---" << endl;
    cout << "v[:3]     = [" << v[0] << ", " << v[1] << ", " << v[2] << "]" << endl;
    cout << "theta[:3] = [" << theta[0] << ", " << theta[1] << ", " << theta[2] << "]" << endl;
    cout << "omega[:3] = [" << omega[0] << ", " << omega[1] << ", " << omega[2] << "]" << endl;

    return 0;
}
