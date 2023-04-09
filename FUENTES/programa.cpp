#include <iostream>
#include "../includes/programa.hpp"
#include <time.h>

//lectura del fichero con los datos del problema a resolver

int main(int argc,char *argv[]){

    //tomamos la semilla pasada como parametro, y a partir de la cual se generará un generador 
    unsigned int semilla=0;
    if(argc==1){
        //le hemos pasado una determinada semilla
        cout<<"RECUERDE QUE DEBE PASAR COMO PARÁMETRO LA SEMILLA A UTILIZAR...\n";
        exit(-1);
    }

    semilla=atoi(argv[1]);
    //cout<<"Semilla utilizada ---> "<<semilla<<"\n";

    

    //-------------- LECTURA DE FICHEROS ---------------
    
    vector<string> archivos;
    archivos.push_back("../BIN/chr22a.dat");
    archivos.push_back("../BIN/chr22b.dat");
    archivos.push_back("../BIN/chr25a.dat");
    archivos.push_back("../BIN/esc128.dat"); //repasar
    archivos.push_back("../BIN/had20.dat");
    archivos.push_back("../BIN/lipa60b.dat");
    archivos.push_back("../BIN/lipa80b.dat");
    archivos.push_back("../BIN/nug28.dat");
    archivos.push_back("../BIN/sko81.dat");
    archivos.push_back("../BIN/sko90.dat");
    archivos.push_back("../BIN/sko100a.dat");
    archivos.push_back("../BIN/sko100f.dat");
    archivos.push_back("../BIN/tai100a.dat");
    archivos.push_back("../BIN/tai100b.dat");
    archivos.push_back("../BIN/tai150b.dat");
    archivos.push_back("../BIN/tai256c.dat"); //repasar
    archivos.push_back("../BIN/tho40.dat");
    archivos.push_back("../BIN/tho150.dat");
    archivos.push_back("../BIN/wil50.dat");
    archivos.push_back("../BIN/wil100.dat");
    
    Matrices salida;
    vector<int> solucion;
    clock_t inicio,fin;
    double tiempo_total;
    for(int i=0;i<archivos.size();i++){
        //para cada archivo...
        //1. LEEMOS LOS DATOS
        salida=lectura_datos(archivos[i]);
        //2. EJECUTAMOS GREEDY
        inicio=clock();
        solucion=algoritmo_greedy(salida.matriz_flujo,salida.matriz_distancia);
        fin=clock();
        tiempo_total=((fin - inicio) / (double) CLOCKS_PER_SEC)* 1000;

        cout<<"\n// Nombre del archivo de datos --> "<<archivos[i]<<" //\n";
        cout<<"\nUSANDO ALGORITMO GREEDY...";
        cout<<"\n-Coste de la solucion obtenida:"<<fitness(solucion,salida.matriz_flujo,salida.matriz_distancia);
        cout<<"\n-Tiempo necesario para obtener la solucion (milisegundos):"<<tiempo_total;

        //3. EJECUTAMOS BUSQUEDA LOCAL PRIMERO EL MEJOR
        inicio=clock();
        solucion=busqueda_local(salida.matriz_flujo,salida.matriz_distancia,semilla);
        fin=clock();
        tiempo_total=((fin - inicio) / (double) CLOCKS_PER_SEC)* 1000;

        cout<<"\n\nUSANDO ALGORITMO DE BUSQUEDA LOCAL PRIMERO EL MEJOR...";
        cout<<"\n-Coste de la solucion obtenida:"<<fitness(solucion,salida.matriz_flujo,salida.matriz_distancia);
        cout<<"\n-Tiempo necesario para obtener la solucion (milisegudos):"<<tiempo_total<<"\n";

    }
    
    
    
    
    //PRUEBAS REALIZADAS PARA COMPROBAR EL CORRECTO FUNCIONAMIENTO...
    /*
    Matrices salida=lectura_datos("../BIN/chr22a.dat");

    int n=salida.matriz_flujo.size();
    
    //----------VISUALIZAR POR PANTALLA LOS DATOS LEIDOS DEL FICHERO------------
    
    cout<<"Mostrando la informacion leida del fichero de datos...\n";
    cout<<"Numero de unidades es "<<n<<"\n";
    
    cout<<"----------------------";
    cout<<"MATRIZ DE FLUJO...\n\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<salida.matriz_flujo[i][j]<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";

    cout<<"----------------------";
    cout<<"MATRIZ DE DISTANCIAS...\n\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<salida.matriz_distancia[i][j]<<" ";
        }
        cout<<"\n";
    }
    
   
   //----------ALGORITMO GREEDY-------------------
   
   clock_t inicio2=clock();
   vector<int> solucion=algoritmo_greedy(salida.matriz_flujo,salida.matriz_distancia);
   clock_t fin2=clock();
    
    cout<<"Vector SOLUCION obtenido con el algoritmo GREEDY...";
    for(int i=0;i<n;i++){
        //suponemos que las 'n' unidades son las siguientes: 0,1,2...(n-1)
        cout<<"\n("<<i<<","<<solucion[i]<<")";
    }
    
    cout<<"\nCalidad de la solucion:"<<fitness(solucion,salida.matriz_flujo,salida.matriz_distancia);
    double tiempo_total2=((fin2 - inicio2) / (double) CLOCKS_PER_SEC)* 1000;
    cout<<"\nTiempo de CPU necesario (en microsegundos) --> "<<tiempo_total2<<"\n";
    
   //-----------BUSQUEDA LOCAL PRIMERO EL MEJOR-------------
   
   vector<int> solucion_final;
   //calculamos el tiempo 
   clock_t inicio=clock();
   solucion_final=busqueda_local(salida.matriz_flujo,salida.matriz_distancia,semilla);
   clock_t fin=clock();
   double tiempo_total=((fin - inicio) / (double) CLOCKS_PER_SEC)* 1000;
   //la diferencia de ambas será el tiempo de CPU necesario para ejecutar la busqueda
   //local, expresado mediante el numero de ticks del reloj del procesador
   //CLOCK_PER_SEC: numero de ticks por segundo
   //Dividiendo nºticks total/CLOCK_PER_SECONDS --> tiempo en segundos
   
   cout<<"\n\nSOLUCION FINAL:\t";
    for (auto n : solucion_final) {
            cout << n << " ";
        }
   
    cout<<"\n\nCalidad solucion final...-->"<<fitness(solucion_final,salida.matriz_flujo,salida.matriz_distancia)<<"\n";
    cout<<"\nTiempo de CPU necesario (en microsegundos) --> "<<tiempo_total<<"\n";
    */
    
    
    return 0;
}