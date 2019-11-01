#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
using namespace std;

typedef int individu[8];
typedef individu populasi[50];
typedef int arr_parent[50];
typedef individu pop_gabungan[100];
typedef double arr_fitness[100];

//fungsi-fungsi
void buatKromosom(populasi pops){
    int i,x;
    for (i = 0; i<50; i++){
        for (x = 0;x<8;x++){
            pops[i][x] = rand()%10;
        }
    }
}
double x1(individu indv){
    return -3+((3-(-3))/(9*(pow(10,-1)+pow(10,-2)+pow(10,-3)+pow(10,-4))))*(indv[0]*pow(10,-1)+indv[1]*pow(10,-2)+indv[2]*pow(10,-3)+indv[3]*pow(10,-4));
}
double x2(individu indv){
    return -2+((2-(-2))/(9*(pow(10,-1)+pow(10,-2)+pow(10,-3)+pow(10,-4))))*(indv[4]*pow(10,-1)+indv[5]*pow(10,-2)+indv[6]*pow(10,-3)+indv[7]*pow(10,-4));
}
double fungsi(double x1, double x2){
    return (4-2.1*pow(x1,2)+pow(x1,4)/3)*pow(x1,2)+x1*x2+(-4+4*x2*x2)*pow(x2,2);
}
double fitness(double x1, double x2){
    return (1/(fungsi(x1,x2)+3));
}
double totalFitness(populasi pops){
    int i; double temp = 0;
    for (i = 0; i<50; i++){
        temp = temp + fitness(x1(pops[i]),x2(pops[i]));
    }
    return temp;
}
double probabilitasIndv(double fitnss, populasi pops){
    return (fitnss/totalFitness(pops));
}
double totalProbIndv(populasi pops){
    int i; double temp = 0;
    for (i=0;i<50;i++){
        temp = temp + probabilitasIndv(fitness(x1(pops[i]),x2(pops[i])),pops);
    }
    return temp;
}
double probabilitasBaru(populasi pops, int i){
    return probabilitasIndv(fitness(x1(pops[i]),x2(pops[i])),pops);
}
float choosingParent(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
void outputKromosom(individu indv){
    int gen;
    for (gen=0;gen<8;gen++){
        cout<<indv[gen]<<' ';
    }
}
//encode kromosom
void encodeKromosom(populasi population){
    int a,b;
    for (a = 0;a<50;a++){
        cout<<"indiv "<<a+1<<". ";
        for (b = 0;b<8;b++){
            cout<<population[a][b]<<' ';
        }
        cout<<",  fitness : "<<fitness(x1(population[a]),x2(population[a]))<<", Probabilitas : "<<probabilitasBaru(population,a)<<endl;
    }
}
//menentukan fitness
void perhitunganFitness(populasi population){
    int indiv,b;
    for(indiv=0;indiv<50;indiv++){
        double x = fitness(x1(population[indiv]),x2(population[indiv]));
    }
}
//menentukan parent
void menentukanParent(populasi population,int &parent1){
    float p1 = choosingParent();
    int indv = 0; double delta =  0;
    while (delta < p1){
        delta = delta + probabilitasBaru(population, indv);
        indv++;
    }
    parent1 = indv-1;
}
//crossover
void crossover(individu parent1,individu parent2, individu &child1, individu &child2){
    int place = rand()%7+1;
    cout<<"tempat persilangan : "<<place<<endl;
    int gen;
    individu x, y;
    for (gen=0;gen<place;gen++){
        child1[gen] = parent1[gen];
        child2[gen] = parent2[gen];
    }
    for (gen=place;gen<8;gen++){
        child1[gen] = parent2[gen];
        child2[gen] = parent1[gen];
    }

}
//mutasi
void mutasi(individu &child){
    int place = rand()%8+1;
    cout<<"gen yang dimutasi : "<<place<<endl;
    int replace = rand()%10;
    cout<<"digantikan oleh   : "<<replace<<endl;
    child[place-1] = replace;
}
void prosesMutasi(populasi &childs){
    int child; float n;
    for (child=0;child<50;child++){
        n = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        if (n < 0.1){
            mutasi(childs[child]);
            cout<<"Child ke-"<<child+1<<", hasil mutasi : "; outputKromosom(childs[child]); cout<<endl;
        }
    }
}
//Pergantian Generasi -- Steady state
void gabungParentChild(pop_gabungan &generation, populasi parent, populasi child){
    int i,j,k;
    individu x;
    j = 0;
    for (i=0;i<50;i++){
        for (k=0;k<8;k++){
            generation[j][k] = parent[i][k];
        }
        j++;
    }
    for (i=0;i<50;i++){
        for (k=0;k<8;k++){
            generation[j][k] = child[i][k];
        }
        j++;
    }
}
void shortt(arr_fitness &arfit, pop_gabungan &generasi){
    int a,b,c,n;
    double temp;
    individu x;
    for (a=0;a<100-1;a++){
        b = a;
        for (c=a+1;c<100;c++){
            if(arfit[b]<arfit[c]){
                b = c;
            }
        }
        temp = arfit[b];
        for (n=0;n<8;n++){
            x[n] = generasi[b][n];
        }
        arfit[b] = arfit[a];
        for (n=0;n<8;n++){
            generasi[b][n] = generasi[a][n];
        }
        arfit[a] = temp;
        for (n=0;n<8;n++){
            generasi[a][n] = x[n];
        }
    }
    for (a=0;a<100;a++){
        cout<<a+1<<". "; outputKromosom(generasi[a]); cout<<", fitness: "<<arfit[a]<<endl;
    }
}
void regenerasi(pop_gabungan generasi, populasi &generasiBaru){
    int a,b;
    for(a=0;a<50;a++){
        for(b=0;b<8;b++){
            generasiBaru[a][b] = generasi[a][b];
        }
    }
    for(a=0;a<50;a++){
        cout<<"individu "<<a+1<<" : "; outputKromosom(generasiBaru[a]); cout<<endl;
    }
}
// final state
bool isSame(populasi pops){
    bool status = true;
    int i,j,k;
    k = 0;
    for (j=1;j<50;j++) {
        for(i=0;i<8;i++){
            if(pops[k][i] == pops[j][i]){
                status = true;
            } else {
                status = false;
                break;
            }
        }
    }
    return status;
}
int main()
{
    srand(time(0));
    int a,b,i,x;
    x = 0;
    populasi pop1, childs;
    arr_parent parents;
    arr_fitness arfit;
    pop_gabungan generation;
    buatKromosom(pop1);
    while(!isSame(pop1)){
        x++;
        cout<<endl<<"============================GENERASI "<<x+1<<"===================================="<<endl;
        encodeKromosom(pop1);
        for (a=0;a<50;a++){
            menentukanParent(pop1,parents[a]);
        }
        for (a=0;a<50;a++){
            cout<<"Parent "<<a+1<<" : individu "<<parents[a]+1<<", = "; outputKromosom(pop1[parents[a]]); cout<<endl;
        }
        for (a=0;a<50;a++){
            crossover(pop1[parents[a]],pop1[parents[a+1]],childs[a],childs[a+1]);
            a++;
        }

        for (a=0;a<50;a++){
            cout<<"Child "<<a+1<<" : "; outputKromosom(childs[a]); cout<<endl;
        }
        prosesMutasi(childs);
        gabungParentChild(generation,pop1,childs);
        for(a=0;a<100;a++){
            arfit[a] = fitness(x1(generation[a]),x2(generation[a]));
        }
        cout<<"--------------------------HASIL PENGURUTAN----------------------------"<<endl;
        shortt(arfit,generation);
        cout<<"============================GENERASI BARU============================="<<endl;
        regenerasi(generation,pop1);
    }
    cout<<"x1          : "<<x1(pop1[0])<<endl;
    cout<<"x2          : "<<x2(pop1[0])<<endl;
    cout<<"hasil akhir : "<<fungsi(x1(pop1[0]),x2(pop1[0]));
    return 0;
}
