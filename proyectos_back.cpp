//
// Created by alejandra on 5/12/26.
//

#include <iostream>
#include <vector>
using namespace std;
// ---- CÓDIGO MERGESORT GEEKSFORGEEKS ---------------------------------------
// Merges two subarrays of vec.
// First subarray is vec[left..mid]
// Second subarray is vec[mid+1..right]
void merge(vector<int>& vec, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    vector<int> leftVec(n1), rightVec(n2);

    // Copy data to temporary vectors
    for (i = 0; i < n1; i++)
        leftVec[i] = vec[left + i];
    for (j = 0; j < n2; j++)
        rightVec[j] = vec[mid + 1 + j];

    // Merge the temporary vectors back into vec[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (leftVec[i] <= rightVec[j]) {
            vec[k] = leftVec[i];
            i++;
        } else {
            vec[k] = rightVec[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftVec[], if any
    while (i < n1) {
        vec[k] = leftVec[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightVec[], if any
    while (j < n2) {
        vec[k] = rightVec[j];
        j++;
        k++;
    }
}

// The subarray to be sorted is in the index range [left..right]
void mergeSort(vector<int>& vec, int left, int right) {
    if (left < right) {

        // Calculate the midpoint
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        // Merge the sorted halves
        merge(vec, left, mid, right);
    }
}
// ---- FIN CÓDIGO MERGESORT GEEKSFORGEEKS ------------------------------------

class Solucion {
    public:
    Solucion(int pm, vector<int> c, vector<int>b) {
        PM = pm;
        this->c = c;
        size = c.size();
        benef = b;
        colores.resize(size);
        nodos_podados = 0;
        nodos_generados = 0;
    }
    void IniciaComp(int k){
       colores[k] = -1; // valor NULO
    }
    void SigValComp(int k){
        colores[k]++; // Siguiente valor del dominio. -1->0->1->2
    }
    bool TodosGenerados(int k) const{
        return colores[k]==2; // END
    }
    bool Factible(int p_actual);

    //int Solucion::Decision(int k) const;
    //Obtener valor componente k, return X[k]
    void ProcesaSolucion() {
        float coste = 0.0, beneficio = 0.0;
        cout << "{";
        for (int i = 0; i < size; i++) {
            if (colores[i] != 0) {
                cout << " " << (char)(i+'A');
                coste += c[i];
                beneficio += benef[i];
            }
        }
        cout << " }\tcoste: " << coste << " " << "beneficio: " << beneficio << endl;
    }
    // Representa el proceso que se realiza cuando se alcanza una solución.
    //Permite quedarnos con la mejor solución


    void imprimeNodos() {
        cout << endl << "Nodos podados: " << nodos_podados << endl;
        cout << "Nodos generados: " << nodos_generados << endl;
    }

    int getSize() {
        return size;
    }

    private:
        vector<int> colores; // X, aka soluciones posibles
        int PM; // prime minister aka presupuesto maximo
        vector<int> c; // coste
        vector<int> benef; // beneficio
        int nodos_podados, nodos_generados;
        int size;
};

bool Solucion::Factible(int p_actual){
    //El coste (c) de los proyectos seleccionados no sobrepase el presupuesto (PM)
    float ps = 0.0;
    for (int k=0; k<=p_actual; k++) {
        ps += colores[k]*c[k];
    }
    nodos_generados++;
    if (ps>PM) {
        nodos_podados++;
    }
    return (ps<=PM);
}


void seleccion_proyectos(Solucion &s, int k) {
    if (k == s.getSize()) {
        s.ProcesaSolucion();
    }
    else {
        s.IniciaComp(k); // a -1
        s.SigValComp(k); // lo ponemos a 0 (-1++)
        while (!s.TodosGenerados(k)) { // mientras no sea 2 (END)
            if (s.Factible(k)) { // si los costes hasta esa posición son menores que el presupuesto máximo.
                seleccion_proyectos(s, k+1);  //paso a calcular el siguiente hijo de esa rama
            }
           s.SigValComp(k); //cuando sale de generar todos los hijos, pasa al siguiente "estado". Si estaba en 0, a 1, si estaba en 1, acaba.
        }
    }
}

/*
int main () {
    vector<int> proyecto = {2,3,5};
    vector<int> benef = {4,5,6};
    //antes de pasarle el vector hay que ordena
    Solucion S(8,proyecto, benef);
    seleccion_proyectos(S, 0);
    S.imprimeNodos();
}*/
