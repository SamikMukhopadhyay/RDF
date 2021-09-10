#include<stdio.h>
#include<math.h>
#include<stdlib.h>

/* typedef struct{
    float x, y;
}VecR; */
int n_max;

struct molecule{
    int id;
    float x;
    float y;
};

float distance(struct molecule par1, struct molecule par2){
    float dist, x_dist, y_dist;

    x_dist = (par1).x - (par2).x;
    y_dist = (par1).y - (par2).y;

    dist = sqrt(x_dist*x_dist + y_dist*y_dist);

    return dist;
}

float max_distance(int num_part, float* distance){
    int i;
    float maximum = -10;

    for(i=0; i<num_part; i++){
        if(distance[i]>maximum){
            maximum = distance[i];
        }
    }

    return maximum;
}


void grForOneRef(int num_part, int ref_part_id, struct molecule *lattice, float deltaR, float rho, float *probability){

    float n_max_float; // deltaR is The width of each histogram rectangle
    int i, j, n;
    float dist[num_part], dist_part, max_dist; // dist[num_part] contains distance of all particles wrt reference particle


    /** Distance Calculation **/

    for(i=0; i<num_part; i++){
        dist_part = distance(lattice[i], lattice[ref_part_id-1]);
        dist[i] = dist_part;
    }

    /* for(i=0; i<num_part; i++){printf("%d %0.2f \n", lattice[i].id, dist[i]);} */

    max_dist = max_distance(num_part, dist); // Maximum Distance of any particle in the system from the ref particle
    n_max = max_dist/deltaR +1;
    n_max_float = n_max;

    /** Output Initialization **/

    int *hist = (int*)malloc(n_max * sizeof(int));
    float *prob = (int*)malloc(n_max * sizeof(int));
    float *gr = (int*)malloc(n_max * sizeof(int));

    for(i=0; i<n_max; i++){
        hist[i]=0; // number of particles in distance range calculation
        prob[i]=0;
        gr[i]=0;
    }
    hist[0] = -1;

    for(i=0; i<num_part; i++){
        n = dist[i]/deltaR;
        // printf("%d %.2f %d\n", i+1, dist[i], n);
        hist[n] += 1;
    }

    /* for(i=0; i<n_max; i++){printf("%d %d\n", i+1, hist[i]);} */

    for(i=0; i<n_max; i++){
        prob[i] = hist[i]/(float)num_part;
        gr[i] = prob[i]/rho;

        probability[i] += prob[i];
    }



    /* printf("The probability distribution is: \n");
    printf("Bin  Probability \n");
    for(i=0; i<n_max; i++){printf("%d %.4f\n", i+1, prob[i]);} */

}


void grMaster(int num_part, int max_n_max, struct molecule *lattice){
    float *prob = (int*)malloc(max_n_max * sizeof(int));
    int i;
    float token=0;

    for(i=0;i<max_n_max;i++){
        prob[i]=0;
    }

    for(i=0; i<num_part; i++){
            grForOneRef(num_part, i, lattice, 0.5, 1, prob);
    }

    printf("The probability distribution is: \n");
    printf("Bin  Probability \n");
    for(i=0; i<max_n_max; i++){printf(" %.6f\n", prob[i]/num_part);}

}

void main(void){
    /*VecR par1, par2; float dist; par1.x = 0; par1.y = 0; par2.x = 5; par2.y = 12; dist = distance(par1, par2);
    printf("%f", dist);*/

    int num_part = 400, latt_x = 20, latt_y = 20;
    int ref_part_id = 61; // The particle with respect to which the RDF is to be calculated
    struct molecule lattice[num_part];
    float deltaR = 0.5, rho = 1, dist_max_n_max;
    int i, j, max_n_max;



    /** Lattice Initialization **/
    for(i=0; i<latt_x; i++){
        for(j=0; j<latt_y; j++){
            lattice[i*latt_x + j].id = i*latt_x + j + 1;
            lattice[i*latt_x + j].x = i;
            lattice[i*latt_x + j].y = j;
        }
    }

    /* for(i=0; i<num_part; i++){printf("%d %0.2f %0.2f \n", lattice[i].id, lattice[i].x, lattice[i].y);} */

    dist_max_n_max = distance(lattice[0], lattice[num_part-1]);
    max_n_max = (dist_max_n_max/deltaR) +1;

    grMaster(num_part, max_n_max, lattice);

}

