#include <iostream> 
#include <fstream> //para ifstream
#include <vector>
#include <algorithm>
#include <cassert>
#include <random>
#include <numeric>
using namespace std;

#ifndef PROGRAMA //evitamos que se inserte vairas veces el mismo archivo de cabecera
#define PROGRAMA

//definimos un struct que almacenará las matrices de flujo/distancia leidas del fichero
struct Matrices{
    vector<vector<int>> matriz_flujo;
    vector<vector<int>> matriz_distancia;
};

//lectura del fichero de datos del problema
Matrices lectura_datos(string nombre_fichero){

    ifstream fichero(nombre_fichero);
    if (!fichero){
        cout<<"Error al abrir el fichero";
        exit(EXIT_FAILURE);
    }

    //leemos el valor de 'n'
    int n;
    fichero>>n;

    Matrices salida;
    //especificamos el tamaño de las dos matrices del struct
    salida.matriz_flujo.resize(n,vector<int>(n));
    salida.matriz_distancia.resize(n,vector<int>(n));

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            fichero>>salida.matriz_flujo[i][j];
        }
    }
    //lectura matriz de distancias
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
           fichero>>salida.matriz_distancia[i][j];
        }
    }

    //terminamos de trabajar con el fichero...
    fichero.close();

    return salida;

}

/*
Funcion fitness -> evalúa cómo de buena es una solución al problema (su calidad)
En este caso, una solución será mejor cuanto menor sea su valor fitness, es decir, cuanto menor sea el coste total de asignacion
La funcion fitness devolverá el coste total de las asignaciones
Parámetros:
1.Solucion cuya calidad queremos conocer: vector de n componentes
2.Matriz de distancia
3.Matriz de flujo
4. Numero de unidades a asignar entre las localizaciones
*/

int fitness(vector<int> solucion,vector<vector<int>> matriz_flujo,vector<vector<int>> matriz_distancia){
    int coste_total=0;
    int distancia=0;
    int flujo=0;

    int n=matriz_flujo.size();

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            distancia=matriz_distancia[solucion[i]][solucion[j]];
            flujo=matriz_flujo[i][j];
            coste_total+=distancia*flujo;
        }
    }

    return coste_total;

}


/*
Cálculo del fi de cada unidad//di de cada localizacion
fi: entero calculado como la suma del flujo de la unidad i-resto de unidades
di: entero calculado como la suma de la distancia de la localizacion i-resto de localizaciones
la función devolvera un array con dos vectores de tamaño 'n':
1) Flujo total de cada unidad
2) Distancia total de cada localizacion
*/

struct DosVectores{
    //definimos los vectores de potenciales de flujo / distancia
    vector<int> potenciales_flujo;
    vector<int> potenciales_distancia;
    
    DosVectores(int n){
        potenciales_flujo.resize(n);
        potenciales_distancia.resize(n);
    }
    
};

DosVectores potenciales(vector<vector<int>> matriz_flujo,vector<vector<int>> matriz_distancia){

   int n=matriz_flujo.size();
   DosVectores salida(n);

   int flujo;
   int distancia;

   for (int i = 0; i < n; i++) {
        flujo = 0;
        distancia = 0;

        for (int j = 0; j < n; j++) {
            flujo += matriz_flujo[i][j];
            distancia += matriz_distancia[i][j];
        }

        salida.potenciales_flujo[i] = flujo;
        salida.potenciales_distancia[i] = distancia;
    }

    //ya tenemos los potenciales de flujo y distancia, los devolvemos

    return salida;

}


/*
algoritmo Greedy, construimos la solucion paso a paso, tal que la localizacion a la
que se asignará la siguiente unidad 'i' será la mejor a nivel local
*/

bool comparar_flujos(pair<int,int> a,pair<int,int> b){
    //return(a.second>=b.second);
    //recordemos que tiene que definir un ordenamiento estrictamente debil
    //esto implica que debe definir una relacion irreflexiva y antisimetrica
    /*
    -irreflexiva: un elemento no es menor que sí mismo (devuelve false para dos mismos elementos)
    -antisimetrica: si devuelve 'true' para 'x' e 'y', devolverá 'false' para 'y e 'x'
    (dicho de otra forma: si x<=y y y<=x ---> x=y [parejas iguales])
    */
    if(a.second>b.second){
        return true; //primero va el potencial de flujo mayor
    }
    elif(a.second==b.second){
        //mismo potencial, colocamos por ej la pareja tal que el indice de la unidad sea mayor
        return(a.first>b.first);
    }
    else{
        //potencial de 'a' menor estricto que potencial de 'b' -> 'b' va antes que 'a'
        return false;
    }
}

//¿Como se ordenan los potenciales de distancia? --> de menor a mayor
//Si devuelve true (primer potencial de distancia < sgundo potencial de distancia) -> 'a' irá antes que 'b'
bool comparar_distancias(pair<int,int> a,pair<int,int> b){
    if(a.second<b.second){
        return true; //primero va el potencial de distancia menor
    }
    elif(a.second==b.second){
        //mismo potencial, colocamos por ej la pareja tal que el indice de la unidad sea menor
        return(a.first<b.first);
    }
    else{
        //potencial de distancia de 'a' mayor estricto que el de 'b'
        return false;
    }
}

vector<int> algoritmo_greedy(vector<vector<int>> matriz_flujo,vector<vector<int>> matriz_distancia){
    //primero calculamos los potenciales para cada unidad y distancia...
    DosVectores matrices=potenciales(matriz_flujo,matriz_distancia);
    vector<int> potenciales_flujo=matrices.potenciales_flujo;
    vector<int> potenciales_distancia=matrices.potenciales_distancia;
    /*
    cout<<"Imprimo el vector de potenciales de distancia... de tamanio"<<potenciales_distancia.size()<<"\n";
    for(int i=0;i<potenciales_distancia.size();i++){
        cout<<potenciales_distancia[i]<<"-";
    }
    */
    
    //ordenamos primeramente los vectores de potenciales
    //si ordenamos directamente los potenciales de flujo, perdemos la referencia de la unidad con ese flujo
    //por tanto definimos una pareja unidad-fi correspondiente
    int n=potenciales_flujo.size();

    pair<int,int> pareja;
    vector<pair<int,int>> flujos;
    vector<pair<int,int>> distancias;
    //recorremos 
    for(int i=0;i<n;i++){
        //unidad-potencial de flujo
        pareja.first=i;
        pareja.second=potenciales_flujo[i];
        flujos.push_back(pareja);
        //localizacion-potencial de distancia
        pareja.second=potenciales_distancia[i];
        distancias.push_back(pareja);
    }
    /*
    cout<<"\nImprimo parejas (unidad,potencial flujo), de tamanio "<<flujos.size()<<"\n";
    for(int i=0;i<n;i++){
        cout<<flujos[i].first<<"-"<<flujos[i].second<<"\n";
    }
    */
    
    /*
    //imprimo para comprobar
    cout<<"\nPAREJAS UNIDAD-POTENCIAL DE FLUJO...\n";
    for(int i=0;i<n;i++){
        cout<<flujos[i].first<<"-"<<flujos[i].second<<"\n";
    }

    cout<<"\nPAREJAS UNIDAD-POTENCIAL DE FLUJO...\n";
    for(int i=0;i<n;i++){
        cout<<distancias[i].first<<"-"<<distancias[i].second<<"\n";
    }
    */
    
    //procedemos a ordenar
    
    sort(flujos.begin(),flujos.end(),comparar_flujos);
    sort(distancias.begin(),distancias.end(),comparar_distancias);
    
    
    //construimos la solucion paso a paso...
    vector<int> solucion;
    solucion.resize(n);

    
    for(int i=0;i<n;i++){
        //tomamos la unidad con mayor potencial de flujo y le asignamos la distancia con menor potencial
        //flujos[i].first con distancias[i].first 
        solucion[flujos[i].first]=distancias[i].first;
    }

    /*
    cout<<"\nSOLUCION OBTENIDA:\n";
    for(int i=0;i<n;i++){
        cout<<"(unidad "<<i<<" ,localizacion "<<solucion[i]<<")\n";
    }
    */
    
    return solucion;
}

//procedemos a implementar la busqueda local, haciendo uso de las siguientes funciones

//funcion que nos devuelve la diferencia de coste entre la nueva solucion vecina y la solucion actual
//si el valor devuelto es negativo -> hemos encontrado una solucion de mayor calidad
//podemos hacer Coste(solucion_nueva)-Coste(solucion-actual), pero en este caso lo que haremos
//será realizar el menor numero de operaciones posibles factorizando...
//recibe como entrada la solucion actual, y las unidades (entre 0 y n-1) cuyas localizaciones queremos intercambiar
int diferencia_coste(vector<int> solucion_actual,int r,int s,vector<vector<int>> matriz_flujo,vector<vector<int>> matriz_distancia){
    int n=solucion_actual.size();
    //los indices de las unidades deben estar en [0,n-1]
    assert(r>=0 && r<n && r!=s);
    assert(s>=0 && s<n);

    int diferencia=0;
    for(int i=0;i<n;i++){
        if(i!=r && i!=s){
            diferencia+=matriz_flujo[r][i]*(matriz_distancia[solucion_actual[s]][solucion_actual[i]]-matriz_distancia[solucion_actual[r]][solucion_actual[i]])+
                        matriz_flujo[s][i]*(matriz_distancia[solucion_actual[r]][solucion_actual[i]]-matriz_distancia[solucion_actual[s]][solucion_actual[i]])+
                        matriz_flujo[i][r]*(matriz_distancia[solucion_actual[i]][solucion_actual[s]]-matriz_distancia[solucion_actual[i]][solucion_actual[r]])+
                        matriz_flujo[i][s]*(matriz_distancia[solucion_actual[i]][solucion_actual[r]]-matriz_distancia[solucion_actual[i]][solucion_actual[s]]);
        }
    }

    return diferencia;
}

//bucle interno de la BL que se encarga de explorar el vecindario
/*
EL proceso a seguir es el siguiente:
1. Se generan las soluciones vecinas a la actual UNA A UNA hasta que:
    1.1 Se consiga una solucion vecina mejor que la actual -> seguimos iterando
        con esta nueva solucion vecina
    1.2 Generamos el vecindario por completo sin encontrar nada mejor -> FIN

Pseudocodigo:
Bucle externo (cada iteracion equivale a generar el vecindario de cada una de las diversas soluciones que vayamos considerando hasta que generemos el vecindario por completo)
Bucle interno (genera el vecindario de la determinada solucion actual que estamos considerando, frenando si conseguimos una solucion vecina mejor o recorrer todo el vecindario sin exito)
Bucle externo, en resumen, itera los entornos de las diferentes soluciones actuales que vayamos considerando
Bucle interno, en resumen, se focaliza en una determinada solucion actual, y genera su vecindario
*/
vector<int> busqueda_vecindario(vector<bool> &dlb,vector<int> solucion_actual,vector<vector<int>> matriz_flujo,vector<vector<int>> matriz_distancia){
    int n=dlb.size();
    bool mejora=false;

    //fijando la solucion actual, generamos su vecindario, generando primero las soluciones vecinas
    //intercambiando la localizacion asignada a una determinada unidad 'i' con el resto...
    //Si ninguna de las soluciones vecinas obtenidas es mejor, se desactiva dicha unidad
    for(int i=0;i<n;i++){
        if(!dlb[i]){
        //soluciones vecinas que podemos generar utilizando la unidad 'i'
            mejora=false;
            for(int j=0;j<n;j++){
                //intercambiamos la localizacion de la unidad 'i' con la del resto de unidades
                if(j==i) continue;
                //intercambiamos localizaciones siempre y cuando produzca una mejora
                if(diferencia_coste(solucion_actual,i,j,matriz_flujo,matriz_distancia)<0){
                    //solucion genera es mejor...
                    swap(solucion_actual[i],solucion_actual[j]);
                    dlb[i]=false;
                    dlb[j]=false;
                    mejora=true;
                }
            }
            if(!mejora){
                //la unidad 'i' no nos ha permitido generar soluciones vecinas de mayor calidad, no nos sirve
                dlb[i]=true;
            }
        }
    }
    //devolvemos la última solucion vecina generada en el vecindario de la solucion inicial considerada...
    return solucion_actual;
}


//GENERADOR DE UN NUMERO ALEATORIO EN [0,n-1]
int myrandom(int i) {
    return rand()%i;
}

//FUNCION QUE DEVUELVE UNA SOLUCION INICIAL
/*
Usaremos las siguientes funciones:
- srand(), para inicializar la semilla que usa la funcion rand()
- rand()%n para generar numeros aleatorios en [0,n)
*/

vector<int> generar_solucion_aleatoria(unsigned int semilla,int tamanio){
    //creamos un vector de n componentes que almacenen por defecto '0'
    vector<int> solucion_inicial(tamanio);
    //usamos la funcion iota para que almacene los enteros de 0 a tamanio-1
    iota(solucion_inicial.begin(),solucion_inicial.end(),0);
    /*
    cout<<"\nSOLUCION INICIAL SIN PERMUTAR --> ";
    for (auto n : solucion_inicial) {
            cout << n << " ";
    }
    */

    //inicializamos la semilla del generador de numeros aleatorios
    srand(semilla);
    //permutamos usando la funcion random_shuffle()
    random_shuffle(solucion_inicial.begin(),solucion_inicial.end(),myrandom);
    /*
    cout<<"\nSOLUCION INICIAL TRAS PERMUTAR --> ";
    for (auto n : solucion_inicial) {
            cout << n << " ";
    }
    */

    return solucion_inicial;
}

//bucle externo de la busqueda local
vector<int> busqueda_local(vector<vector<int>> matriz_flujo,vector<vector<int>> matriz_distancia,unsigned int semilla){
    //generamos una solucion inicial de la cual partir...
    vector<int> solucion_actual=generar_solucion_aleatoria(semilla,matriz_flujo.size());

    vector<bool> dlb(matriz_flujo.size(),false);
    vector<int> solucion_obtenida;
    bool seguir_iterando=false;

    //bucle externo...
    do{
        //la busqueda interna modifica la solucion_actual siempre y cuando se consiga una mejor solucion
        //por lo que si devuelve un vector solucion identico a la solucion_actual pasada como parametro,
        //quiere decir que no se ha encontrado una mejor solucion vecina en dicho vecindario, y finaliza la bsuqueda local
        solucion_obtenida=busqueda_vecindario(dlb,solucion_actual,matriz_flujo,matriz_distancia);
        seguir_iterando=(solucion_obtenida!=solucion_actual); 
        //actualizamos la solucion actual
        solucion_actual=solucion_obtenida;
    }while(seguir_iterando); //mientras que la solucion vecina sea de mayor calidad

    return solucion_actual;
}


#endif
