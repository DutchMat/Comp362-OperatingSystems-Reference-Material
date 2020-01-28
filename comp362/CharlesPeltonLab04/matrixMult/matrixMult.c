#include "matrixMult.h"
// Name: Charles Pelton 
// Lab: Lab04
// Date: 9/24/2018 
void free_matrix(int **a, int **b, int **c, int m, int k){ // free_matrix is not in .h thus needs
// to be interpreted before call.
	for(int row = 0; row < m; row++){ // obviously reverse allocation
		free((a)[row]);
		free((c)[row]);
	}
	for(int row = 0; row < k; row++){
		free((b)[row]);
	}
	free(a);
	free(b);
	free(c); // I don't want data leaks from malloc.
}

int main(int argc, char *argv[])
{
   if (freopen(argv[1], "r", stdin) == 0)
      oops("Cannot open the input file.\n", -1);

   int **a1, **b1, **c1, **a2, **b2, **c2; // matrices
   int m1, k1, n1, m2, k2, n2; // dimensions of the matices m x k and k x m

   allocateAndLoadMatrices(&a1, &b1, &c1, &m1, &k1, &n1);
   allocateAndLoadMatrices(&a2, &b2, &c2, &m2, &k2, &n2);

   // the real magic happens in there

   // TODO done: implemented the magic
   free_tids(multiply(a1, b1, c1, m1, k1, n1), m1); // free tids after threads
   free_tids(multiply(a2, b2, c2, m2, k2, n2), m2);
   //Display results of matrix multiplication

   printf("\nMATRIX A1\n");
   displayMatrix(a1, m1, k1);
   printf("\nMATRIX B1\n");
   displayMatrix(b1, k1, n1);
   printf("\nMATRIX A1 x B1\n");
   displayMatrix(c1, m1, n1);

   printf("\nMATRIX A2\n");
   displayMatrix(a2, m2, k2);
   printf("\nMATRIX B2\n");
   displayMatrix(b2, k2, n2);
   printf("\nMATRIX A2 x B2\n");
   displayMatrix(c2, m2, n2);

   free_matrix(a1, b1, c1, m1, k1); // free
   free_matrix(a2, b2, c2, m2, k2);

   return 0;
}

void *matrixThread(void *param)
{
   // map the parameter onto the structure
   struct matrixCell cell = *(struct matrixCell *)param;

   // TODO DONE: implemented the magic
	(cell.c)[cell.i][cell.j] = 0;
	for(int k = 0; k < cell.k; k++){ // row * col per thread
		(cell.c)[cell.i][cell.j] += (cell.a)[cell.i][k] * (cell.b)[k][cell.j];
	}

}

void allocateAndLoadMatrices(int ***a, int ***b, int ***c, int *m, int *k, int *n)
// takes pointers to two-dimensional matrices, so they can be allocated in here
// and used by the caller
{
   if (scanf("%d %d %d", m, k, n) == 0)
      oops("Cannot read matrix sizes.\n", -2);
   // TODO DONE: implemented the magic
   // a is m x k ASSIGNED
   // b is k x n ASSIGNED
   // c is m x n TBD
 (*a) = malloc((*m) * sizeof(int *)); // a is m
 (*b) = malloc((*k) * sizeof(int *)); // b is k
 (*c) = malloc((*m) * sizeof(int *)); // c is m
	for(int row = 0; row < *m; row++){ 
		(*a)[row] = malloc((*k) * sizeof(int)); // a is m x k
		(*c)[row] = malloc((*n) * sizeof(int)); // c is m x n
	}
	for(int row = 0; row < *k; row++){
		(*b)[row] = malloc((*n) * sizeof(int)); // b is k x n
	}
	loadMatrix(a, *m, *k); // reads
	loadMatrix(b, *k, *n); // reads
}

void loadMatrix(int ***matrix, int m, int n)
{
	// TODO DONE: implemented the magic
	for(int row = 0; row < m; row++){
		for(int col = 0; col < n; col++){
			if (scanf("%d", &(*matrix)[row][col]) == 0) // loads the matrix
      				oops("Matrix does not have all elements.\n", -3);
		}
	}
	
}

pthread_t **multiply(int **a, int **b, int **c, int m, int k, int n)
{
   pthread_t **tids = alloc_tids(m, n);

   // TODO DONE: implemented the magic
	struct matrixCell mCell[m][n]; // creates a struct for each thread, data handled by the function/stack
for(int row = 0; row < m; row++){
	for(int col = 0; col < n; col++){
	mCell[row][col].i = row; mCell[row][col].j = col; mCell[row][col].k = k;
	mCell[row][col].a = a; mCell[row][col].b = b; mCell[row][col].c = c; // sets the data
	pthread_create(&tids[row][col], NULL, matrixThread, &(mCell[row][col]));
	 // creates thread and stacks function.
		}
	}
	join(tids, m, n); // waits for all the threads to finish
   return tids;
}

pthread_t **alloc_tids(int m, int n)
{
   pthread_t **tids;
	// TODO DONE: implemented the magic
	tids = malloc(m * sizeof(pthread_t *));
	for(int row = 0; row < m; row++){
		tids[row] = malloc(n * sizeof(pthread_t));
	}

   return tids;
}

void free_tids(pthread_t **tids, int m)
{
   // TODO DONE: implemented the magic
	for(int row = 0; row < m; row++){
			free(tids[row]); // frees the data! Be free!
	}
	free(tids);
}

void join(pthread_t **tids, int m, int n)
{
   // TODO DONE: implemented the magic
	for(int row = 0; row < m; row++){
		for(int col = 0; col < n; col++){
			pthread_join(tids[row][col], NULL); // waits for the thread it reaches to finish/checks if it has finished, then moves to the next thread.
		}
	}
}

void displayMatrix(int **matrix, int m, int n)
{
   // TODO DONE: implemented the magic
	for(int row = 0; row < m; row++){
		for(int col = 0; col < n; col++){
			printf("%d ", (matrix)[row][col]); // prints number and spaces
		}
		printf("\n"); // a next line
	}
}

