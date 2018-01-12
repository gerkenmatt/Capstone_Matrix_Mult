
#include "matrix_mult.h"
#include "uart_com.h"

/*matrix mult variables*/
int first_matrix[side][side];
int second_matrix[side][side];
unsigned long results_matrix[side][side];
unsigned long golden_matrix[side][side];

unsigned long int ind = 0;
int local_errors = 0;
int in_block = 0;
int seed_value = -1;

void init_matrices()
{
	int i = 0;
	int j = 0;

	//seed the random number generator
	//the method is designed to reset SEUs in the matrices, using the current seed value
	//that way each test starts error free
	if (seed_value == -1)
	{
		srand(ind);
		seed_value = ind;
	}
	else
	{
		srand(seed_value);
	}

	//fill the matrices
	for ( i = 0; i < side; i++ )
	{
		for (j = 0; j < side; j++)
		{
			first_matrix[i][j] = rand();
			second_matrix[i][j] = rand();
		}
	}
}

void matrix_multiply(int f_matrix[][side], int s_matrix[][side], unsigned long r_matrix[][side])
{
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned long sum = 0;

	//MM
	for ( i = 0 ; i < side ; i++ )
	{
		for ( j = 0 ; j < side ; j++ )
		{
			for ( k = 0 ; k < side ; k++ )
			{
				sum = sum + f_matrix[i][k]*s_matrix[k][j];
			}
			r_matrix[i][j] = sum;
			sum = 0;
		}
	}
}

int checker(unsigned long golden_matrix[][side], unsigned long results_matrix[][side])
{
	int first_error = 0;
	int num_of_errors = 0;
	int i = 0;
	int j = 0;

	for(i=0; i<side; i++)
	{
		for (j = 0; j < side; j++)
		{
			if (golden_matrix[i][j] != results_matrix[i][j])
			{
				//checker found an error, print results to screen
				if (!first_error)
				{
					if (!in_block && robust_printing)
					{
						xprintf(" - i: %n\n\r\n", ind);
						xprintf("   E: {%i_%i: [%x, %x],", i, j, golden_matrix[i][j], results_matrix[i][j]);
						first_error = 1;
						in_block = 1;
					}
					else if (in_block && robust_printing)
					{
						xprintf("   E: {%i_%i: [%x, %x],", i, j, golden_matrix[i][j], results_matrix[i][j]);
						first_error = 1;
					}
				}
				else
				{
					if (robust_printing)
						xprintf("%i_%i: [%x, %x],", i, j, golden_matrix[i][j], results_matrix[i][j]);
				}
				num_of_errors++;
			}
		}
	}

	if (first_error)
	{
		xprintf("}\n\r\n");
		first_error = 0;
	}

	if (!robust_printing && (num_of_errors > 0))
	{
		if (!in_block)
		{
			xprintf(" - i: %n\n\r\n", ind);
			xprintf("   E: %i\n\r\n", num_of_errors);
			in_block = 1;
		}
		else
		{
			xprintf("   E: %i\n\r\n", num_of_errors);
		}
	}

	return num_of_errors;
}

void matrix_multiply_test()
{

	//initialize variables
	int total_errors = 0;

	init_matrices();
	//setup golden values
	matrix_multiply(first_matrix, second_matrix, golden_matrix);

	while (1)
	{
		matrix_multiply(first_matrix, second_matrix, results_matrix);
		local_errors = checker(golden_matrix, results_matrix);

		//if there is an error, fix the input matrics
		//golden is recomputed so that the code doesn't
		//have to figure out if the error was in the results
		//or golden matrix
		if (local_errors > 0)
		{
			init_matrices();
			matrix_multiply(first_matrix, second_matrix, golden_matrix);
		}

		//acking to see if alive, as well as changing input values
		if (ind % change_rate == 0)
		{
			xprintf("# %n, %i\n\r\n", ind, total_errors);
			seed_value = -1;
			init_matrices();
			//have to recompute the golden
			matrix_multiply(first_matrix, second_matrix, golden_matrix);
		}

		//reset vars and such
		ind++;
		total_errors += local_errors;
		local_errors = 0;
		in_block = 0;
	}

}
