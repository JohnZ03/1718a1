#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

// SECTION: def for cl file
FILE *fp;
char *source_str;
size_t source_size;

// SECTION: def for openCL
#define CL_TARGET_OPENCL_VERSION 120

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)

cl_command_queue command_queue;

cl_kernel kernel_update_weights_dense_b;
cl_kernel kernel_update_weights_dense_b2;
cl_kernel kernel_update_weights_dense_w;
cl_kernel kernel_update_weights_dense_w2;
cl_kernel kernel_update_weights_conv_b;
cl_kernel kernel_update_weights_conv_w;
cl_kernel kernel_forward_conv;
cl_kernel kernel_sig_layer;

// SECTION: global variables for openCL
cl_platform_id platform_id = NULL;
cl_device_id device_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;

// Mem_obj declare
cl_mem eta_mem_obj;
cl_mem dense_b_mem_obj;
cl_mem db1_mem_obj;
cl_mem dense_b2_mem_obj;
cl_mem db2_mem_obj;
cl_mem dense_w2_mem_obj;
cl_mem dw2_mem_obj;
cl_mem dense_w_mem_obj;
cl_mem dw1_mem_obj;
cl_mem conv_w_mem_obj;
cl_mem dw_conv_mem_obj;
cl_mem conv_b_mem_obj;
cl_mem db_conv_mem_obj;
cl_mem sig_layer_mem_obj;
cl_mem conv_layer_mem_obj;
cl_mem img_mem_obj;
cl_mem max_pooling_mem_obj;
//! TEST
float test[5][28][28];

using namespace std;
const int filter_size = 7;
const float eta = 0.01;
//! TEST
const int batch_size = 200;
// const int batch_size = 200;

unsigned char data_train[60000][784];
unsigned char data_test[10000][784];
unsigned char label_train[60000];
unsigned char label_test[10000];

float conv_w[5][7][7];
float conv_b[5][28][28];
float conv_layer[5][28][28];
float sig_layer[5][28][28];
char max_pooling[5][28][28];
float max_layer[5][14][14];

float dense_input[980];
float dense_w[980][120];
float dense_b[120];
float dense_sum[120];
float dense_sigmoid[120];
float dense_w2[120][10];
float dense_b2[10];
float dense_sum2[10];
float dense_softmax[10];

float dw2[120][10];
float db2[10];
float dw1[980][120];
float db1[120];

float dw_max[5][28][28];
float dw_conv[5][7][7];
float db_conv[5][28][28];

/* ************************************************************ */
/* Helper functions */

// TODO: Kernel
float sigmoid(float x)
{
	if (x > 500)
		x = 500;
	if (x < -500)
		x = -500;
	return 1 / (1 + exp(-x));
}

// TODO: Kernel
float d_sigmoid(float x)
{
	float sig = sigmoid(x);
	return sig * (1 - sig);
}

// TODO: Kernel
float softmax_den(float *x, int len)
{
	float val = 0;
	for (int i = 0; i < len; i++)
	{
		val += exp(x[i]);
	}
	return val;
}

void initialise_weights()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			for (int k = 0; k < 28; k++)
			{
				if (j < 7 && k < 7)
				{
					conv_w[i][j][k] = 2 * float(rand()) / RAND_MAX - 1;
				}
				conv_b[i][j][k] = 2 * float(rand()) / RAND_MAX - 1;
			}
		}
	}

	for (int i = 0; i < 980; i++)
	{
		for (int j = 0; j < 120; j++)
		{
			dense_w[i][j] = 2 * float(rand()) / RAND_MAX - 1;
		}
	}
	for (int i = 0; i < 120; i++)
	{
		dense_b[i] = 2 * float(rand()) / RAND_MAX - 1;
	}

	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			dense_w2[i][j] = 2 * float(rand()) / RAND_MAX - 1;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		dense_b2[i] = 2 * float(rand()) / RAND_MAX - 1;
	}
}

// TODO: Move initialized weights to buffer

/* ************************************************************ */

/* ************************************************************ */
/* Forward Pass */
void forward_pass(unsigned char img[][32])
{

	// Convolution Operation + Sigmoid Activation
	// for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	// {
	// 	for (int i = 0; i < 28; i++)
	// 	{
	// 		for (int j = 0; j < 28; j++)
	// 		{
	// 			max_pooling[filter_dim][i][j] = 0;

	// 			conv_layer[filter_dim][i][j] = 0;
	// 			sig_layer[filter_dim][i][j] = 0;
	// 			for (int k = 0; k < filter_size; k++)
	// 			{
	// 				for (int l = 0; l < filter_size; l++)
	// 				{
	// 					conv_layer[filter_dim][i][j] += img[i + k + 1][j + l - 2] * conv_w[filter_dim][k][l];
	// 				}
	// 			}
	// 			sig_layer[filter_dim][i][j] = sigmoid(conv_layer[filter_dim][i][j] + conv_b[filter_dim][i][j]);
	// 		}
	// 	}
	// }
	// ! TEST
	// cout << endl
	// 	 << "memory\t" << endl;
	// for (int filter_dim = 0; filter_dim < 1; filter_dim++)
	// {
	// 	for (int i = 13; i < 14; i++)

	// 	{
	// 		for (int j = 0; j < 28; j++)
	// 		{
	// 			cout << conv_layer[filter_dim][j][i] << " ";
	// 		}
	// 		cout << endl;
	// 	}
	// }
	// Conv
	cl_char zeros_char = 0;
	ret = clEnqueueFillBuffer(command_queue, max_pooling_mem_obj, &zeros_char, sizeof(zeros_char), 0,
							  sizeof(max_pooling), 0, NULL, NULL);
	cl_float zeros_float = 0;
	ret = clEnqueueFillBuffer(command_queue, conv_layer_mem_obj, &zeros_float, sizeof(zeros_float), 0,
							  sizeof(conv_layer), 0, NULL, NULL);
	ret = clEnqueueFillBuffer(command_queue, sig_layer_mem_obj, &zeros_float, sizeof(zeros_float), 0,
							  sizeof(sig_layer), 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, conv_w_mem_obj, CL_TRUE, 0,
							   sizeof(conv_w), conv_w, 0, NULL, NULL);
	size_t global_item_size[3] = {5, 28, 28};
	ret = clEnqueueNDRangeKernel(command_queue, kernel_forward_conv, 3, NULL,
								 global_item_size, NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, conv_layer_mem_obj, CL_TRUE, 0,
							  sizeof(conv_layer), conv_layer, 0, NULL, NULL);

	// !TEST
	// Print the result of conv kernel
	// cout << endl
	// 	 << "buffer\t" << endl;
	// for (int filter_dim = 0; filter_dim < 1; filter_dim++)
	// {
	// 	for (int i = 13; i < 14; i++)
	// 	{
	// 		for (int j = 0; j < 28; j++)
	// 		{
	// 			cout << conv_layer[filter_dim][j][i] << " ";
	// 		}
	// 		cout << endl;
	// 	}
	// }
	// cout << endl;
	// cout << endl;
	
	
	// //! TEST
	// // Write buffer with origin conv_layer, and execute sigmoid kernel
	// ret = clEnqueueWriteBuffer(command_queue, conv_layer_mem_obj, CL_TRUE, 0,
	// 						   sizeof(conv_layer), conv_layer, 0, NULL, NULL);
	
	// // Sigmoid
	// ret = clEnqueueWriteBuffer(command_queue, conv_b_mem_obj, CL_TRUE, 0,
	// 						   sizeof(conv_b), conv_b, 0, NULL, NULL);
	// // No change with global_item_size
	// ret = clEnqueueNDRangeKernel(command_queue, kernel_sig_layer, 3, NULL,
	// 							 global_item_size, NULL, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(command_queue, sig_layer_mem_obj, CL_TRUE, 0,
	// 						  sizeof(sig_layer), sig_layer, 0, NULL, NULL);


	
	// Convolution Operation + Sigmoid Activation
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 28; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				// max_pooling[filter_dim][i][j] = 0;

				// test[filter_dim][i][j] = 0;
				sig_layer[filter_dim][i][j] = 0;
				for (int k = 0; k < filter_size; k++)
				{
					for (int l = 0; l < filter_size; l++)
					{
						// test[filter_dim][i][j] += img[i + k + 1][j + l - 2] * conv_w[filter_dim][k][l];
					}
				}
				sig_layer[filter_dim][i][j] = sigmoid(conv_layer[filter_dim][i][j] + conv_b[filter_dim][i][j]);
			}
		}
	}


	// MAX Pooling (max_pooling, max_layer)
	float cur_max = 0;
	int max_i = 0, max_j = 0;
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 28; i += 2)
		{
			for (int j = 0; j < 28; j += 2)
			{
				max_i = i;
				max_j = j;
				cur_max = sig_layer[filter_dim][i][j];
				for (int k = 0; k < 2; k++)
				{
					for (int l = 0; l < 2; l++)
					{
						if (sig_layer[filter_dim][i + k][j + l] > cur_max)
						{
							max_i = i + k;
							max_j = j + l;
							cur_max = sig_layer[filter_dim][max_i][max_j];
						}
					}
				}
				max_pooling[filter_dim][max_i][max_j] = 1;
				max_layer[filter_dim][i / 2][j / 2] = cur_max;
			}
		}
	}

	int k = 0;
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 14; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				dense_input[k] = max_layer[filter_dim][i][j];
				k++;
			}
		}
	}

	// Dense Layer
	// TODO: attempt on OPENCL      Haotian
	for (int i = 0; i < 120; i++)
	{
		dense_sum[i] = 0;
		dense_sigmoid[i] = 0;
		for (int j = 0; j < 980; j++)
		{
			dense_sum[i] += dense_w[j][i] * dense_input[j];
		}
		dense_sum[i] += dense_b[i];
		dense_sigmoid[i] = sigmoid(dense_sum[i]);
	}

	// Dense Layer 2
	// TODO: attempt on OPENCL      Zhuojun
	for (int i = 0; i < 10; i++)
	{
		dense_sum2[i] = 0;
		for (int j = 0; j < 120; j++)
		{
			dense_sum2[i] += dense_w2[j][i] * dense_sigmoid[j];
		}
		dense_sum2[i] += dense_b2[i];
	}

	// Softmax Output
	float den = softmax_den(dense_sum2, 10);
	for (int i = 0; i < 10; i++)
	{
		dense_softmax[i] = exp(dense_sum2[i]) / den;
	}
}

void update_weights()
{
	// TODO: attempt on OPENCL      Lingfeng
	// for (int i = 0; i < 120; i++)
	// {
	// 	dense_b[i] -= eta * db1[i];
	// 	for (int j = 0; j < 10; j++)
	// 	{
	// 		dense_b2[j] -= eta * db2[j];
	// 		dense_w2[i][j] -= eta * dw2[i][j];
	// 	}
	// 	for (int k = 0; k < 980; k++)
	// 	{
	// 		dense_w[k][i] -= eta * dw1[k][i];
	// 	}
	// }

	// for (int i = 0; i < 5; i++)
	// {
	// 	for (int k = 0; k < 7; k++)
	// 	{
	// 		for (int j = 0; j < 7; j++)
	// 		{
	// 			conv_w[i][k][j] -= eta * dw_conv[i][k][j];
	// 		}
	// 	}
	// 	for (int l = 0; l < 28; l++)
	// 	{
	// 		for (int m = 0; m < 28; m++)
	// 		{
	// 			conv_b[i][l][m] -= eta * db_conv[i][l][m];
	// 		}
	// 	}
	// }

	ret = clEnqueueWriteBuffer(command_queue, eta_mem_obj, CL_TRUE, 0,
							   sizeof(eta), &eta, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, db1_mem_obj, CL_TRUE, 0,
							   sizeof(db1), db1, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, db2_mem_obj, CL_TRUE, 0,
							   sizeof(db2), db2, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dw2_mem_obj, CL_TRUE, 0,
							   sizeof(dw2), dw2, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dw1_mem_obj, CL_TRUE, 0,
							   sizeof(dw1), dw1, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dw_conv_mem_obj, CL_TRUE, 0,
							   sizeof(dw_conv), dw_conv, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, db_conv_mem_obj, CL_TRUE, 0,
							   sizeof(db_conv), db_conv, 0, NULL, NULL);

	size_t global_item_size[3] = {120};
	ret = clEnqueueNDRangeKernel(command_queue, kernel_update_weights_dense_b, 1, NULL,
								 global_item_size, NULL, 0, NULL, NULL);

	global_item_size[0] = 10;
	ret = clEnqueueNDRangeKernel(command_queue, kernel_update_weights_dense_b2, 1, NULL,
								 global_item_size, NULL, 0, NULL, NULL);

	global_item_size[0] = 120;
	global_item_size[1] = 10;
	ret = clEnqueueNDRangeKernel(command_queue, kernel_update_weights_dense_w2, 2, NULL,
								 global_item_size, NULL, 0, NULL, NULL);

	global_item_size[0] = 980;
	global_item_size[1] = 120;
	ret = clEnqueueNDRangeKernel(command_queue, kernel_update_weights_dense_w, 2, NULL,
								 global_item_size, NULL, 0, NULL, NULL);

	global_item_size[0] = 5;
	global_item_size[1] = 7;
	global_item_size[2] = 7;
	ret = clEnqueueNDRangeKernel(command_queue, kernel_update_weights_conv_w, 3, NULL,
								 global_item_size, NULL, 0, NULL, NULL);

	global_item_size[0] = 5;
	global_item_size[1] = 28;
	global_item_size[2] = 28;
	ret = clEnqueueNDRangeKernel(command_queue, kernel_update_weights_conv_b, 3, NULL,
								 global_item_size, NULL, 0, NULL, NULL);

	ret = clEnqueueReadBuffer(command_queue, dense_b_mem_obj, CL_TRUE, 0,
							  sizeof(dense_b), dense_b, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, dense_b2_mem_obj, CL_TRUE, 0,
							  sizeof(dense_b2), dense_b2, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, dense_w2_mem_obj, CL_TRUE, 0,
							  sizeof(dense_w2), dense_w2, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, dense_w_mem_obj, CL_TRUE, 0,
							  sizeof(dense_w), dense_w, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, conv_w_mem_obj, CL_TRUE, 0,
							  sizeof(conv_w), conv_w, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, conv_b_mem_obj, CL_TRUE, 0,
							  sizeof(conv_b), conv_b, 0, NULL, NULL);
}
/* ************************************************************ */

/* ************************************************************ */
/* Backward Pass */
void backward_pass(float *y_hat, int *y, unsigned char img[][32])
{
	float delta4[10];
	for (int i = 0; i < 10; i++)
	{
		delta4[i] = y_hat[i] - y[i]; // Derivative of Softmax + Cross entropy
		db2[i] = delta4[i];			 // Bias Changes
	}

	// Calculate Weight Changes for Dense Layer 2
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			dw2[i][j] = dense_sigmoid[i] * delta4[j];
		}
	}

	// Delta 3
	float delta3[120];
	for (int i = 0; i < 120; i++)
	{
		delta3[i] = 0;
		for (int j = 0; j < 10; j++)
		{
			delta3[i] += dense_w2[i][j] * delta4[j];
		}
		delta3[i] *= d_sigmoid(dense_sum[i]);
	}
	for (int i = 0; i < 120; i++)
		db1[i] = delta3[i]; // Bias Weight change

	// Calculate Weight Changes for Dense Layer 1
	// TODO: attempt on OPENCL (top prio)

	for (int i = 0; i < 980; i++)
	{
		for (int j = 0; j < 120; j++)
		{
			dw1[i][j] = dense_input[i] * delta3[j];
		}
	}

	// Delta2
	// TODO: attempt on OPENCL      Guoxian
	float delta2[980];
	for (int i = 0; i < 980; i++)
	{
		delta2[i] = 0;
		for (int j = 0; j < 120; j++)
		{
			delta2[i] += dense_w[i][j] * delta3[j];
		}
		delta2[i] *= d_sigmoid(dense_input[i]);
	}

	// Calc back-propagated max layer dw_max
	int k = 0;
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 28; i += 2)
		{
			for (int j = 0; j < 28; j += 2)
			{
				for (int l = 0; l < 2; l++)
				{
					for (int m = 0; m < 2; m++)
					{
						if (max_pooling[filter_dim][i + l][j + m] == 1)
							dw_max[filter_dim][i][j] = delta2[k];
						else
							dw_max[filter_dim][i][j] = 0;
					}
				}
				k++;
			}
		}
	}
	// Calc Conv Bias Changes
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 28; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				db_conv[filter_dim][i][j] = dw_max[filter_dim][i][j];
			}
		}
	}

	// Set Conv Layer Weight changes to 0
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				dw_conv[filter_dim][i][j] = 0;
			}
		}
	}

	// Calculate Weight Changes for Conv Layer
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 26; i++)
		{
			for (int j = 0; j < 26; j++)
			{
				float cur_val = dw_max[filter_dim][i][j];
				for (int k = 0; k < 5; k++)
				{
					for (int l = 0; l < 5; l++)
					{
						dw_conv[filter_dim][k][l] += img[i + k + 1][j + l - 2] * cur_val;
					}
				}
			}
		}
	}
}
/* ************************************************************ */

void read_train_data()
{
	ifstream csvread;
	csvread.open("/cad2/ece1718s/mnist_train.csv", ios::in);
	if (csvread)
	{
		string s;
		int data_pt = 0;
		while (getline(csvread, s))
		{
			stringstream ss(s);
			int pxl = 0;
			while (ss.good())
			{
				string substr;
				getline(ss, substr, ',');
				if (pxl == 0)
				{
					label_train[data_pt] = stoi(substr);
				}
				else
				{
					data_train[data_pt][pxl - 1] = stoi(substr);
				}
				pxl++;
			}
			data_pt++;
		}
		csvread.close();
	}
	else
	{
		cerr << "Unable to read train data!" << endl;
		exit(EXIT_FAILURE);
	}
}
void read_test_data()
{
	ifstream csvread;
	csvread.open("/cad2/ece1718s/mnist_test.csv", ios::in);
	if (csvread)
	{
		string s;
		int data_pt = 0;
		while (getline(csvread, s))
		{
			stringstream ss(s);
			int pxl = 0;
			while (ss.good())
			{
				string substr;
				getline(ss, substr, ',');
				if (pxl == 0)
				{
					label_test[data_pt] = stoi(substr);
				}
				else
				{
					data_test[data_pt][pxl - 1] = stoi(substr);
				}
				pxl++;
			}
			data_pt++;
		}
		csvread.close();
	}
	else
	{
		cerr << "Unable to read test data!" << endl;
		exit(EXIT_FAILURE);
	}
}

void give_img(unsigned char *vec, unsigned char img[][32])
{
	int k = 0;
	for (int i = 0; i < 35; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (i < 5 || j < 2 || i > 32 || j > 29)
			{
				img[i][j] = 0;
			}
			else
			{
				img[i][j] = vec[k++];
			}
		}
	}
}

void give_y(int y, int *vector_y)
{
	for (int i = 0; i < 10; i++)
		vector_y[i] = 0;
	vector_y[y] = 1;
}
int give_prediction()
{
	float max_val = dense_softmax[0];
	int max_pos = 0;
	for (int i = 1; i < 10; i++)
	{
		if (dense_softmax[i] > max_val)
		{
			max_val = dense_softmax[i];
			max_pos = i;
		}
	}

	return max_pos;
}

int main()
{
	// SECTION Load the kernel source code into the array source_str
	fp = fopen("kernel_lw.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// SECTION: openCL setup
	// Get platform and device information
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1,
						 &device_id, &ret_num_devices);

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	// Create a command queue
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// Create Buffers
	eta_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float), NULL, &ret);
	dense_b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dense_b), NULL, &ret);
	db1_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(db1), NULL, &ret);
	dense_b2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dense_b2), NULL, &ret);
	db2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(db2), NULL, &ret);
	dense_w2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dense_w2), NULL, &ret);
	dw2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dw2), NULL, &ret);
	dense_w_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dense_w), NULL, &ret);
	dw1_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dw1), NULL, &ret);
	conv_w_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(conv_w), NULL, &ret);
	dw_conv_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(dw_conv), NULL, &ret);
	conv_b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(conv_b), NULL, &ret);
	db_conv_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(db_conv), NULL, &ret);
	sig_layer_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(sig_layer), NULL, &ret);
	conv_layer_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(conv_layer), NULL, &ret);
	max_pooling_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(max_pooling), NULL, &ret);
	img_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 35 * 32 * sizeof(unsigned char), NULL, &ret);

	cl_program program = clCreateProgramWithSource(context, 1,
												   (const char **)&source_str, (const size_t *)&source_size, &ret);

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Create kernels
	kernel_update_weights_dense_b = clCreateKernel(program, "update_weights_1d", &ret);
	ret = clSetKernelArg(kernel_update_weights_dense_b, 0, sizeof(cl_mem), (void *)&eta_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_b, 1, sizeof(cl_mem), (void *)&db1_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_b, 2, sizeof(cl_mem), (void *)&dense_b_mem_obj);

	kernel_update_weights_dense_b2 = clCreateKernel(program, "update_weights_1d", &ret);
	ret = clSetKernelArg(kernel_update_weights_dense_b2, 0, sizeof(cl_mem), (void *)&eta_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_b2, 1, sizeof(cl_mem), (void *)&db2_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_b2, 2, sizeof(cl_mem), (void *)&dense_b2_mem_obj);

	kernel_update_weights_dense_w2 = clCreateKernel(program, "update_weights_2d", &ret);
	ret = clSetKernelArg(kernel_update_weights_dense_w2, 0, sizeof(cl_mem), (void *)&eta_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_w2, 1, sizeof(cl_mem), (void *)&dw2_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_w2, 2, sizeof(cl_mem), (void *)&dense_w2_mem_obj);
	int w1 = 10;
	ret = clSetKernelArg(kernel_update_weights_dense_w2, 3, sizeof(int), &w1);

	kernel_update_weights_dense_w = clCreateKernel(program, "update_weights_2d", &ret);
	ret = clSetKernelArg(kernel_update_weights_dense_w, 0, sizeof(cl_mem), (void *)&eta_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_w, 1, sizeof(cl_mem), (void *)&dw1_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_dense_w, 2, sizeof(cl_mem), (void *)&dense_w_mem_obj);
	w1 = 120;
	ret = clSetKernelArg(kernel_update_weights_dense_w, 3, sizeof(int), &w1);

	kernel_update_weights_conv_w = clCreateKernel(program, "update_weights_3d", &ret);
	ret = clSetKernelArg(kernel_update_weights_conv_w, 0, sizeof(cl_mem), (void *)&eta_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_conv_w, 1, sizeof(cl_mem), (void *)&dw_conv_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_conv_w, 2, sizeof(cl_mem), (void *)&conv_w_mem_obj);
	w1 = 7;
	int w2 = 7;
	ret = clSetKernelArg(kernel_update_weights_conv_w, 3, sizeof(int), &w1);
	ret = clSetKernelArg(kernel_update_weights_conv_w, 4, sizeof(int), &w2);

	kernel_update_weights_conv_b = clCreateKernel(program, "update_weights_3d", &ret);
	ret = clSetKernelArg(kernel_update_weights_conv_b, 0, sizeof(cl_mem), (void *)&eta_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_conv_b, 1, sizeof(cl_mem), (void *)&db_conv_mem_obj);
	ret = clSetKernelArg(kernel_update_weights_conv_b, 2, sizeof(cl_mem), (void *)&conv_b_mem_obj);
	w1 = 28;
	w2 = 28;
	ret = clSetKernelArg(kernel_update_weights_conv_b, 3, sizeof(int), &w1);
	ret = clSetKernelArg(kernel_update_weights_conv_b, 4, sizeof(int), &w2);

	kernel_forward_conv = clCreateKernel(program, "forward_conv", &ret);
	ret = clSetKernelArg(kernel_forward_conv, 0, sizeof(cl_mem), (void *)&img_mem_obj);
	ret = clSetKernelArg(kernel_forward_conv, 1, sizeof(cl_mem), (void *)&conv_w_mem_obj);
	ret = clSetKernelArg(kernel_forward_conv, 2, sizeof(cl_mem), (void *)&conv_layer_mem_obj);

	kernel_sig_layer = clCreateKernel(program, "sig_layer", &ret);
	ret = clSetKernelArg(kernel_sig_layer, 0, sizeof(cl_mem), (void *)&sig_layer_mem_obj);
	ret = clSetKernelArg(kernel_sig_layer, 1, sizeof(cl_mem), (void *)&conv_layer_mem_obj);
	ret = clSetKernelArg(kernel_sig_layer, 2, sizeof(cl_mem), (void *)&conv_b_mem_obj);

	// ! Load Dataset
	read_test_data();
	read_train_data();
	initialise_weights();
	//! TEST
	int epoch = 500;
	int num = 0;
	cout << "Start Training." << endl;
	for (int i = 0; i < epoch; i++)
	{
		cout << "Epoch " << i << " done." << endl;
		for (int j = 0; j < batch_size; j++)
		{
			num = rand() % 60000;
			unsigned char img[35][32];
			int vector_y[10];
			give_y(label_train[num], vector_y);
			give_img(data_train[num], img);
			ret = clEnqueueWriteBuffer(command_queue, img_mem_obj, CL_TRUE, 0,
									   sizeof(img), img, 0, NULL, NULL);
			forward_pass(img);
			backward_pass(dense_softmax, vector_y, img);
			update_weights();
		}
	}

	int val_len = 600;
	int cor = 0;
	int confusion_mat[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			confusion_mat[i][j] = 0;
	}

	//! TEST
	cout << "Start Testing." << endl;
	for (int i = 0; i < val_len; i++)
	{
		unsigned char img[35][32];
		give_img(data_test[i], img);
		forward_pass(img);
		int pre = give_prediction();
		confusion_mat[label_test[i]][pre]++;
		if (pre == label_test[i])
			cor++;
	}
	float accu = float(cor) / val_len;
	cout << "Accuracy: " << accu << endl;

	cout << "   0 1 2 3 4 5 6 7 8 9" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << i << ": ";
		for (int j = 0; j < 10; j++)
		{
			cout << confusion_mat[i][j] << " ";
		}
		cout << endl;
	}

	// SECTION: OpenCL Clean up
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);

	ret = clReleaseKernel(kernel_update_weights_dense_b);
	ret = clReleaseKernel(kernel_update_weights_dense_b2);
	ret = clReleaseKernel(kernel_update_weights_dense_w);
	ret = clReleaseKernel(kernel_update_weights_dense_w2);
	ret = clReleaseKernel(kernel_update_weights_conv_b);
	ret = clReleaseKernel(kernel_update_weights_conv_w);

	ret = clReleaseProgram(program);

	ret = clReleaseMemObject(eta_mem_obj);
	ret = clReleaseMemObject(dense_b_mem_obj);
	ret = clReleaseMemObject(db1_mem_obj);
	ret = clReleaseMemObject(db2_mem_obj);
	ret = clReleaseMemObject(dense_w2_mem_obj);
	ret = clReleaseMemObject(dw2_mem_obj);
	ret = clReleaseMemObject(dense_w_mem_obj);
	ret = clReleaseMemObject(dw1_mem_obj);
	ret = clReleaseMemObject(conv_w_mem_obj);
	ret = clReleaseMemObject(dw_conv_mem_obj);
	ret = clReleaseMemObject(conv_b_mem_obj);
	ret = clReleaseMemObject(db_conv_mem_obj);

	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	return 0;
}
