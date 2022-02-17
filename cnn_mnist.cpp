#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

#include <cstring>

// SECTION: def for cl file
FILE *fp;
char *source_str;
size_t source_size;
FILE *fp_wgx;
char *source_str1_wgx;
size_t source_size1_wgx;

// SECTION: def for openCL
#define CL_TARGET_OPENCL_VERSION 120

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define MAX_SOURCE_SIZE (0x100000)

float delta3[120];
cl_mem dense_input_mem_obj;
cl_mem delta3_mem_obj;
cl_mem dw1_mem_obj;

cl_mem sig_layer_mem_obj;
cl_mem max_pooling_mem_obj;
cl_mem max_layer_mem_obj;

cl_mem delta3_mid_mem_obj;
cl_mem delta2_mid_mem_obj;
cl_mem delta2_mem_obj;
cl_mem dense_w_mem_obj;
cl_mem d_sigmoid_dense_input_mem_obj;
cl_mem dense_w2_mem_obj;
cl_mem delta4_mem_obj;
cl_mem d_sigmoid_dense_sum_mem_obj;
cl_mem dense_sum_mem_obj;
cl_mem dense_b_mem_obj;
cl_mem dense_sigmoid_mem_obj;
cl_mem dense_sum2_mem_obj;
cl_mem dense_b2_mem_obj;
cl_mem dense_softmax_mem_obj;

cl_command_queue command_queue;

cl_kernel kernel;
cl_kernel max_pooling_kernel;
cl_kernel kernel_wgx;
cl_kernel kernel_wgx1;
cl_kernel kernel_wgx2;
cl_kernel kernel_wgx3;
cl_kernel kernel_wgx4;
cl_kernel kernel_wgx5;
cl_kernel kernel_wgx6;
cl_kernel kernel_wgx7;
cl_kernel kernel_wgx8;
cl_kernel kernel_wgx9;

// SECTION: global variables for openCL
cl_platform_id platform_id = NULL;
cl_device_id device_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;

// ! wlf
cl_kernel kernel_update_weights_dense_b;
cl_kernel kernel_update_weights_dense_b2;
cl_kernel kernel_update_weights_dense_w;
cl_kernel kernel_update_weights_dense_w2;
cl_kernel kernel_update_weights_conv_b;
cl_kernel kernel_update_weights_conv_w;
cl_kernel kernel_forward_conv;
cl_kernel kernel_sig_layer;

// ! zht
cl_kernel conv_weight_kernel;
cl_kernel max_layer_back_kernel;

// Mem_obj declare
cl_mem eta_mem_obj;

cl_mem db1_mem_obj;
cl_mem db2_mem_obj;
cl_mem dw2_mem_obj;
cl_mem conv_w_mem_obj;
cl_mem dw_conv_mem_obj;
cl_mem conv_b_mem_obj;
cl_mem db_conv_mem_obj;
cl_mem conv_layer_mem_obj;
cl_mem img_mem_obj;

cl_mem dw_max_mem_obj;

//! TEST
float test[5][28][28];

using namespace std;
const int filter_size = 7;
const float eta = 0.01;
const int batch_size = 200;

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
float sigmoid(float x)
{
	if (x > 500)
		x = 500;
	if (x < -500)
		x = -500;
	return 1 / (1 + exp(-x));
}
float d_sigmoid(float x)
{
	float sig = sigmoid(x);
	return sig * (1 - sig);
}
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
	ret = clEnqueueWriteBuffer(command_queue, conv_b_mem_obj, CL_TRUE, 0,
							   sizeof(conv_b), conv_b, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, conv_w_mem_obj, CL_TRUE, 0,
							   sizeof(conv_w), conv_w, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dense_w_mem_obj, CL_TRUE, 0,
							   sizeof(dense_w), dense_w, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dense_b_mem_obj, CL_TRUE, 0,
							   sizeof(dense_b), conv_w, 0, NULL, NULL);
}
/* ************************************************************ */

/* ************************************************************ */
/* Forward Pass */
void forward_pass(unsigned char img[][32])
{
	// Convolution Operation + Sigmoid Activation
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{
		for (int i = 0; i < 28; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				max_pooling[filter_dim][i][j] = 0;

				conv_layer[filter_dim][i][j] = 0;
				sig_layer[filter_dim][i][j] = 0;
				for (int k = 0; k < filter_size; k++)
				{
					for (int l = 0; l < filter_size; l++)
					{
						conv_layer[filter_dim][i][j] += img[i + k + 1][j + l - 2] * conv_w[filter_dim][k][l];
					}
				}
				// sig_layer[filter_dim][i][j] = sigmoid(conv_layer[filter_dim][i][j] + conv_b[filter_dim][i][j]);
			}
		}
	}
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
	// ret = clEnqueueWriteBuffer(command_queue, conv_w_mem_obj, CL_TRUE, 0,
	// 						   sizeof(conv_w), conv_w, 0, NULL, NULL);
	size_t global_item_size[3] = {5, 28, 28};
	ret = clEnqueueNDRangeKernel(command_queue, kernel_forward_conv, 3, NULL,
								 global_item_size, NULL, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(command_queue, conv_layer_mem_obj, CL_TRUE, 0,
	//   sizeof(conv_layer), conv_layer, 0, NULL, NULL);

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

	//! TEST
	// Write buffer with origin conv_layer, and execute sigmoid kernel
	ret = clEnqueueWriteBuffer(command_queue, conv_layer_mem_obj, CL_TRUE, 0,
							   sizeof(conv_layer), conv_layer, 0, NULL, NULL);

	// Sigmoid
	// ret = clEnqueueWriteBuffer(command_queue, conv_b_mem_obj, CL_TRUE, 0,
	// 						   sizeof(conv_b), conv_b, 0, NULL, NULL);
	// No change with global_item_size
	ret = clEnqueueNDRangeKernel(command_queue, kernel_sig_layer, 3, NULL,
								 global_item_size, NULL, 0, NULL, NULL);
	//ret = clEnqueueReadBuffer(command_queue, sig_layer_mem_obj, CL_TRUE, 0,
	//						  sizeof(sig_layer), sig_layer, 0, NULL, NULL);

	// // Convolution Operation + Sigmoid Activation
	// for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	// {
	// 	for (int i = 0; i < 28; i++)
	// 	{
	// 		for (int j = 0; j < 28; j++)
	// 		{
	// 			// max_pooling[filter_dim][i][j] = 0;

	// 			// test[filter_dim][i][j] = 0;
	// 			sig_layer[filter_dim][i][j] = 0;
	// 			for (int k = 0; k < filter_size; k++)
	// 			{
	// 				for (int l = 0; l < filter_size; l++)
	// 				{
	// 					// test[filter_dim][i][j] += img[i + k + 1][j + l - 2] * conv_w[filter_dim][k][l];
	// 				}
	// 			}
	// 			sig_layer[filter_dim][i][j] = sigmoid(conv_layer[filter_dim][i][j] + conv_b[filter_dim][i][j]);
	// 		}
	// 	}
	// }

	// MAX Pooling (max_pooling, max_layer)
	// Copy the lists A and B to their respective memory buffers
	//ret = clEnqueueWriteBuffer(command_queue, sig_layer_mem_obj, CL_TRUE, 0,
	//						   sizeof(sig_layer), sig_layer, 0, NULL, NULL);

	// Execute the OpenCL kernel on the list
	size_t global_item_size_zht[3] = {5, 14, 14}; // Process the entire lists
	ret = clEnqueueNDRangeKernel(command_queue, max_pooling_kernel, 3, NULL,
								 global_item_size_zht, NULL, 0, NULL, NULL);

	// Read the memory buffer C on the device to the local variable C
	//ret = clEnqueueReadBuffer(command_queue, max_pooling_mem_obj, CL_TRUE, 0,
	//						  sizeof(max_pooling), max_pooling, 0, NULL, NULL);
	//ret = clEnqueueReadBuffer(command_queue, max_layer_mem_obj, CL_TRUE, 0,
	//						  sizeof(max_layer), max_layer, 0, NULL, NULL);

	// ret = clEnqueueReadBuffer(command_queue, max_layer_mem_obj, CL_TRUE, 0,
	//							sizeof(dense_input), dense_input, 0, NULL, NULL);
	ret = clEnqueueCopyBuffer(command_queue, max_layer_mem_obj, dense_input_mem_obj, 0,
							  0, sizeof(dense_input), 0, NULL, NULL);
	/*
	int k = 0;
	for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	{/
		for (int i = 0; i < 14; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				dense_input[k] = max_layer[filter_dim][i][j];
				k++;
			}
		}
	}*/

	// Dense Layer
	// ret = clEnqueueWriteBuffer(command_queue, dense_w_mem_obj, CL_TRUE, 0,
	// 						   sizeof(dense_w), dense_w, 0, NULL, NULL);
	// ret = clEnqueueWriteBuffer(command_queue, dense_input_mem_obj, CL_TRUE, 0,
	//						   sizeof(dense_input), dense_input, 0, NULL, NULL);
	// ret = clEnqueueWriteBuffer(command_queue, dense_b_mem_obj, CL_TRUE, 0,
	// 						   sizeof(dense_b), dense_b, 0, NULL, NULL);

	size_t global_item_size_wgx6 = 120; // Process the entire lists
	size_t local_item_size_wgx6 = 10;	// Process in groups of 20
	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx6, 1, NULL,
								 &global_item_size_wgx6, &local_item_size_wgx6, 0, NULL, NULL);

	// ret = clEnqueueReadBuffer(command_queue, dense_sum_mem_obj, CL_TRUE, 0,
	//						  sizeof(dense_sum), dense_sum, 0, NULL, NULL);

	size_t global_item_size_wgx7 = 120; // Process the entire lists
	size_t local_item_size_wgx7 = 10;	// Process in groups of 20
	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx7, 1, NULL,
								 &global_item_size_wgx7, &local_item_size_wgx7, 0, NULL, NULL);

	// ret = clEnqueueReadBuffer(command_queue, dense_sigmoid_mem_obj, CL_TRUE, 0,
	//						  sizeof(dense_sigmoid), dense_sigmoid, 0, NULL, NULL);

	/*
		float dense_sum_1[120];
		float dense_sigmoid_1[120];
		for (int i = 0; i < 120; i++)
		{
			dense_sum_1[i] = 0;
			dense_sigmoid_1[i] = 0;
			for (int j = 0; j < 980; j++)
			{
				dense_sum_1[i] += dense_w[j][i] * dense_input[j];
			}
			dense_sum_1[i] += dense_b[i];
			if(dense_sum_1[i]-dense_sum[i]>0.00001 ||dense_sum_1[i]-dense_sum[i]<-0.00001)
				printf("o=%f, n=%f, dense_b=%f, dense_w=%f\n",dense_sum_1[i],dense_sum[i],dense_b[i],dense_w[0][i]);
			dense_sigmoid_1[i] = sigmoid(dense_sum_1[i]);
			if(dense_sigmoid[i]-dense_sigmoid_1[i]>0.00001 ||dense_sigmoid[i]-dense_sigmoid_1[i]<-0.00001)
				printf("o_s=%f, n_s=%f, dense_b=%f, dense_w=%f\n",dense_sigmoid_1[i],dense_sigmoid[i],dense_b[i],dense_w[0][i]);
		}
	*/

	// Dense Layer 2
	// TODO: attempt on OPENCL      Zhuojun

	ret = clEnqueueWriteBuffer(command_queue, dense_w2_mem_obj, CL_TRUE, 0,
							   sizeof(dense_w2), dense_w2, 0, NULL, NULL);
	// ret = clEnqueueWriteBuffer(command_queue, dense_sigmoid_mem_obj, CL_TRUE, 0,
	//						   sizeof(dense_sigmoid), dense_sigmoid, 0, NULL, NULL);
	//ret = clEnqueueWriteBuffer(command_queue, dense_b2_mem_obj, CL_TRUE, 0,
	//						   sizeof(dense_b2), dense_b2, 0, NULL, NULL);

	size_t global_item_size_wgx8 = 10; // Process the entire lists
	size_t local_item_size_wgx8 = 10;  // Process in groups of 20
	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx8, 1, NULL,
								 &global_item_size_wgx8, &local_item_size_wgx8, 0, NULL, NULL);

	ret = clEnqueueReadBuffer(command_queue, dense_sum2_mem_obj, CL_TRUE, 0,
							  sizeof(dense_sum2), dense_sum2, 0, NULL, NULL);
	/*
	float dense_sum2_1[10];
	for (int i = 0; i < 10; i++)
	{
		dense_sum2_1[i] = 0;
		for (int j = 0; j < 120; j++)
		{
			dense_sum2_1[i] += dense_w2[j][i] * dense_sigmoid[j];
		}
		dense_sum2_1[i] += dense_b2[i];
		if(dense_sum2_1[i]-dense_sum2[i]>0.00001 ||dense_sum2_1[i]-dense_sum2[i]<-0.00001)
			printf("o=%f, n=%f, dense_b=%f, dense_w=%f\n",dense_sum2_1[i],dense_sum2[i],dense_b[i],dense_w[0][i]);
	}
*/
	// Softmax Output
	//

	size_t global_item_size_wgx9 = 10; // Process the entire lists
	size_t local_item_size_wgx9 = 10;  // Process in groups of 20
	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx9, 1, NULL,
								 &global_item_size_wgx9, &local_item_size_wgx9, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, dense_softmax_mem_obj, CL_TRUE, 0,
							  sizeof(dense_softmax), dense_softmax, 0, NULL, NULL);

	float dense_softmax_1[10];

	float den = softmax_den(dense_sum2, 10);
	/*
	for (int i = 0; i < 10; i++)
	{
		dense_softmax_1[i] = exp(dense_sum2[i]) / den;
		if(dense_softmax_1[i]-dense_softmax[i]>0.00001 ||dense_softmax_1[i]-dense_softmax[i]<-0.00001)
			printf("o=%f, n=%f`\n",dense_softmax_1[i],dense_softmax[i]);
	}*/
}

void update_weights()
{
	// TODO: attempt on OPENCL      Lingfeng
	// for (int i = 0; i < 120; i++)
	// {
	// 	dense_b[i] -= eta * db1[i];
	// 	for (int j = 0; j < 10; j++)
	// 	{
	// 		dense_b2[j] -= eta *db2[j];
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
	//ret = clEnqueueWriteBuffer(command_queue, db1_mem_obj, CL_TRUE, 0,
	//						   sizeof(db1), db1, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, db2_mem_obj, CL_TRUE, 0,
							   sizeof(db2), db2, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dw2_mem_obj, CL_TRUE, 0,
							   sizeof(dw2), dw2, 0, NULL, NULL);
	//ret = clEnqueueWriteBuffer(command_queue, dw1_mem_obj, CL_TRUE, 0,
	//						   sizeof(dw1), dw1, 0, NULL, NULL);
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

	// ret = clEnqueueReadBuffer(command_queue, dense_b_mem_obj, CL_TRUE, 0,
	// 						  sizeof(dense_b), dense_b, 0, NULL, NULL);
	//ret = clEnqueueReadBuffer(command_queue, dense_b2_mem_obj, CL_TRUE, 0,
	//						  sizeof(dense_b2), dense_b2, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, dense_w2_mem_obj, CL_TRUE, 0,
							  sizeof(dense_w2), dense_w2, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(command_queue, dense_w_mem_obj, CL_TRUE, 0,
							//   sizeof(dense_w), dense_w, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(command_queue, conv_w_mem_obj, CL_TRUE, 0,
							//   sizeof(conv_w), conv_w, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(command_queue, conv_b_mem_obj, CL_TRUE, 0,
							//   sizeof(conv_b), conv_b, 0, NULL, NULL);
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

	float d_sigmoid_dense_sum[120];
	// ret = clEnqueueWriteBuffer(command_queue, dense_sum_mem_obj, CL_TRUE, 0,
	//						   sizeof(dense_sum), dense_sum, 0, NULL, NULL);

	size_t global_item_size_wgx4 = 120; // Process the entire lists
	size_t local_item_size_wgx4 = 10;	// Process in groups of 10

	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx4, 1, NULL,
								 &global_item_size_wgx4, &local_item_size_wgx4, 0, NULL, NULL);

	// ret = clEnqueueReadBuffer(command_queue, d_sigmoid_dense_sum_mem_obj, CL_TRUE, 0,
	//						  sizeof(d_sigmoid_dense_sum), d_sigmoid_dense_sum, 0, NULL, NULL);
	/*
		for (int i = 0; i < 120; i++)
		{
			d_sigmoid_dense_sum[i] = d_sigmoid(dense_sum[i]);
		}*/

	memset(delta3, 0, sizeof(delta3));
	float z2_wgx = 0;
	ret = clEnqueueWriteBuffer(command_queue, delta4_mem_obj, CL_TRUE, 0,
							   sizeof(delta4), delta4, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dense_w2_mem_obj, CL_TRUE, 0,
							   sizeof(dense_w2), dense_w2, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, delta3_mid_mem_obj, CL_TRUE, 0,
							   sizeof(delta3), delta3, 0, NULL, NULL);

	size_t global_item_size_wgx3 = 120; // Process the entire lists
	size_t local_item_size_wgx3 = 20;	// Process in groups of 10
	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx2, 1, NULL,
								 &global_item_size_wgx3, &local_item_size_wgx3, 0, NULL, NULL);

	// ret = clEnqueueWriteBuffer(command_queue, d_sigmoid_dense_sum_mem_obj, CL_TRUE, 0,
	//						   sizeof(d_sigmoid_dense_sum), d_sigmoid_dense_sum, 0, NULL, NULL);

	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx3, 1, NULL,
								 &global_item_size_wgx3, &local_item_size_wgx3, 0, NULL, NULL);

	// ret = clEnqueueReadBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						  sizeof(delta3), delta3, 0, NULL, NULL);

	ret = clEnqueueCopyBuffer(command_queue, delta3_mem_obj, db1_mem_obj, 0,
							  0, sizeof(db1), 0, NULL, NULL);
	//ret = clEnqueueReadBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						  sizeof(db1), db1, 0, NULL, NULL);

	// Calculate Weight Changes for Dense Layer 1
	// Copy the lists A and B to their respective memory buffers
	// ret = clEnqueueWriteBuffer(command_queue, dense_input_mem_obj, CL_TRUE, 0,
	//						   sizeof(dense_input), dense_input, 0, NULL, NULL);
	// ret = clEnqueueWriteBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						   sizeof(delta3), delta3, 0, NULL, NULL);

	// Execute the OpenCL kernel on the list
	size_t global_item_size[2] = {980, 120}; // Process the entire lists
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
								 global_item_size, NULL, 0, NULL, NULL);
	// printf("Execute the OpenCL kernel on the list: %d\n", ret);

	// Read the memory buffer C on the device to the local variable C
	//ret = clEnqueueReadBuffer(command_queue, dw1_mem_obj, CL_TRUE, 0,
	//						  sizeof(dw1), dw1, 0, NULL, NULL);

	// Delta2
	// TODO: attempt on OPENCL      Guoxian

	float d_sigmoid_dense_input[980];

	// ret = clEnqueueWriteBuffer(command_queue, dense_input_mem_obj, CL_TRUE, 0,
	//						   sizeof(dense_input), dense_input, 0, NULL, NULL);

	size_t global_item_size_wgx5 = 980; // Process the entire lists
	size_t local_item_size_wgx5 = 20;	// Process in groups of 10

	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx5, 1, NULL,
								 &global_item_size_wgx5, &local_item_size_wgx5, 0, NULL, NULL);

	// ret = clEnqueueReadBuffer(command_queue, d_sigmoid_dense_input_mem_obj, CL_TRUE, 0,
	//						  sizeof(d_sigmoid_dense_input), d_sigmoid_dense_input, 0, NULL, NULL);

	float delta2[980];
	memset(delta2, 0, sizeof(delta2));
	// float z1_wgx = 0;
	//	ret = clEnqueueFillBuffer(command_queue, delta2_mid_mem_obj, &z1_wgx, sizeof(z1_wgx),
	//			0, sizeof(delta2), 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, delta2_mid_mem_obj, CL_TRUE, 0,
							   sizeof(delta2), delta2, 0, NULL, NULL);
	// ret = clEnqueueWriteBuffer(command_queue, dense_w_mem_obj, CL_TRUE, 0,
							//    sizeof(dense_w), dense_w, 0, NULL, NULL);
	// ret = clEnqueueWriteBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						   sizeof(delta3), delta3, 0, NULL, NULL);

	size_t global_item_size_wgx1 = 980; // Process the entire lists
	size_t local_item_size_wgx1 = 20;	// Process in groups of 20
	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx, 1, NULL,
								 &global_item_size_wgx1, &local_item_size_wgx1, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(command_queue, delta2_mid_mem_obj, CL_TRUE, 0,
	//						  sizeof(delta2), delta2, 0, NULL, NULL);
	// printf("ReadBuffer: %d\n", ret);
	/*
		for(int i=0; i<980; i++){
			delta2[i] *= d_sigmoid(dense_input[i]);
		}
	*/

	// ret = clEnqueueWriteBuffer(command_queue, d_sigmoid_dense_input_mem_obj, CL_TRUE, 0,
	//						   sizeof(d_sigmoid_dense_input), d_sigmoid_dense_input, 0, NULL, NULL);
	//  printf("WriteBuffer: %d\n", ret);

	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx1, 1, NULL,
								 &global_item_size_wgx1, &local_item_size_wgx1, 0, NULL, NULL);
	// printf("NDRangeKernel: %d\n", ret);

	//ret = clEnqueueReadBuffer(command_queue, delta2_mem_obj, CL_TRUE, 0,
	//						  sizeof(delta2), delta2, 0, NULL, NULL);

	// TODO: Haotian
	// Calc back-propagated max layer dw_max
	// int k = 0;
	// for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	// {
	// 	for (int i = 0; i < 28; i += 2)
	// 	{
	// 		for (int j = 0; j < 28; j += 2)
	// 		{
	// 			for (int l = 0; l < 2; l++)
	// 			{
	// 				for (int m = 0; m < 2; m++)
	// 				{
	// 					if (max_pooling[filter_dim][i + l][j + m] == 1)
	// 						dw_max[filter_dim][i][j] = delta2[k];
	// 					else
	// 						dw_max[filter_dim][i][j] = 0;
	// 				}
	// 			}
	// 			k++;
	// 		}
	// 	}
	// }
	//ret = clEnqueueWriteBuffer(command_queue, delta2_mem_obj, CL_TRUE, 0,
	//						   sizeof(delta2), delta2, 0, NULL, NULL);

	//ret = clEnqueueWriteBuffer(command_queue, max_pooling_mem_obj, CL_TRUE, 0,
	//						   sizeof(max_pooling), max_pooling, 0, NULL, NULL);

	size_t global_item_size_zht1[3] = {5, 14, 14}; // Process the entire lists
	ret = clEnqueueNDRangeKernel(command_queue, max_layer_back_kernel, 3, NULL,
								 global_item_size_zht1, NULL, 0, NULL, NULL);

	ret = clEnqueueReadBuffer(command_queue, dw_max_mem_obj, CL_TRUE, 0,
							  sizeof(dw_max), dw_max, 0, NULL, NULL);

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

	// TODO: Haotian
	// Calculate Weight Changes for Conv Layer
	// for (int filter_dim = 0; filter_dim < 5; filter_dim++)
	// {
	// 	for (int i = 0; i < 26; i++)
	// 	{
	// 		for (int j = 0; j < 26; j++)
	// 		{
	// 			float cur_val = dw_max[filter_dim][i][j];
	// 			for (int k = 0; k < 5; k++)
	// 			{
	// 				for (int l = 0; l < 5; l++)
	// 				{
	// 					dw_conv[filter_dim][k][l] += img[i + k + 1][j + l - 2] * cur_val;
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	ret = clEnqueueWriteBuffer(command_queue, dw_max_mem_obj, CL_TRUE, 0,
							   sizeof(dw_max), dw_max, 0, NULL, NULL);

	ret = clEnqueueWriteBuffer(command_queue, img_mem_obj, CL_TRUE, 0,
							   35 * 32 * sizeof(unsigned char), img, 0, NULL, NULL);

	size_t global_item_size_zht2[3] = {26, 26}; // Process the entire lists
	ret = clEnqueueNDRangeKernel(command_queue, conv_weight_kernel, 2, NULL,
								 global_item_size_zht2, NULL, 0, NULL, NULL);

	ret = clEnqueueReadBuffer(command_queue, dw_conv_mem_obj, CL_TRUE, 0,
							  sizeof(dw_conv), dw_conv, 0, NULL, NULL);
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
	// ! OPENCL Device Info
	// SECTION: openCL setup
	// Get platform and device information
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1,
						 &device_id, &ret_num_devices);

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// Create a command queue
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// ! WLF fpopen
	fp = fopen("kernel_lw.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
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

	// ! kernel zht
	// SECTION: Load the kernel source code into the array source_str
	fp = fopen("mul_2d.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	fp_wgx = fopen("multi_add_kernel.cl", "r");
	// fp_wgx = fopen("mul_kernel.cl", "r");
	if (!fp_wgx)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str1_wgx = (char *)malloc(MAX_SOURCE_SIZE);
	source_size1_wgx = fread(source_str1_wgx, 1, MAX_SOURCE_SIZE, fp_wgx);
	fclose(fp_wgx);

	// Create memory buffers on the device for each vector
	dense_input_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
										 sizeof(dense_input), NULL, &ret);
	delta3_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
									sizeof(delta3), NULL, &ret);

	delta2_mid_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
										980 * sizeof(float), NULL, &ret);
	delta2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
									980 * sizeof(float), NULL, &ret);
	d_sigmoid_dense_input_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
												   980 * sizeof(float), NULL, &ret);

	delta4_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
									10 * sizeof(float), NULL, &ret);
	dense_sum_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
									   120 * sizeof(float), NULL, &ret);
	d_sigmoid_dense_sum_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
												 120 * sizeof(float), NULL, &ret);
	delta3_mid_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
										120 * sizeof(float), NULL, &ret);

	dense_sigmoid_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
										   sizeof(dense_sigmoid), NULL, &ret);

	dense_sum2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
										sizeof(dense_sum2), NULL, &ret);

	dense_softmax_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
										   10 * sizeof(float), NULL, &ret);

	// Create a program from the kernel source
	program = clCreateProgramWithSource(context, 1,
										(const char **)&source_str, (const size_t *)&source_size, &ret);

	cl_program program_wgx = clCreateProgramWithSource(context, 1,
													   (const char **)&source_str1_wgx, (const size_t *)&source_size1_wgx, &ret);

	// Build the program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	printf("Build the program: %d\n", ret);
	ret = clBuildProgram(program_wgx, 1, &device_id, NULL, NULL, NULL);

	// Create the OpenCL kernel
	kernel = clCreateKernel(program, "mul_2d", &ret);
	printf("Create the OpenCL kernel: %d\n", ret);
	kernel_wgx = clCreateKernel(program_wgx, "multi_add", &ret);
	printf("Create the OpenCL kernel multi_add: %d\n", ret);
	kernel_wgx1 = clCreateKernel(program_wgx, "vector_multi", &ret);
	printf("Create the OpenCL kernel vector_multi: %d\n", ret);
	kernel_wgx2 = clCreateKernel(program_wgx, "multi_add", &ret);
	printf("Create the OpenCL kernel multi_add: %d\n", ret);
	kernel_wgx3 = clCreateKernel(program_wgx, "vector_multi", &ret);
	printf("Create the OpenCL kernel vector_multi: %d\n", ret);
	kernel_wgx4 = clCreateKernel(program_wgx, "d_sigmoid", &ret);
	printf("Create the OpenCL kernel d_sigmoid: %d\n", ret);
	kernel_wgx5 = clCreateKernel(program_wgx, "d_sigmoid", &ret);
	printf("Create the OpenCL kernel d_sigmoid: %d\n", ret);
	kernel_wgx6 = clCreateKernel(program_wgx, "multi_add2", &ret);
	printf("Create the OpenCL kernel multi_add: %d\n", ret);
	kernel_wgx7 = clCreateKernel(program_wgx, "sigmoid", &ret);
	printf("Create the OpenCL kernel sigmoid: %d\n", ret);
	kernel_wgx8 = clCreateKernel(program_wgx, "multi_add2", &ret);
	printf("Create the OpenCL kernel d_sigmoid: %d\n", ret);
	kernel_wgx9 = clCreateKernel(program_wgx, "softmax_den", &ret);
	printf("Create the OpenCL kernel softmax_den: %d\n", ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&dense_input_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&delta3_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&dw1_mem_obj);

	cl_int kernel_wgx_size = 120;
	ret = clSetKernelArg(kernel_wgx, 0, sizeof(cl_mem), (void *)&dense_w_mem_obj);
	ret = clSetKernelArg(kernel_wgx, 1, sizeof(cl_mem), (void *)&delta3_mem_obj);
	ret = clSetKernelArg(kernel_wgx, 2, sizeof(cl_mem), (void *)&delta2_mid_mem_obj);
	ret = clSetKernelArg(kernel_wgx, 3, sizeof(cl_int), (void *)&kernel_wgx_size);

	ret = clSetKernelArg(kernel_wgx1, 0, sizeof(cl_mem), (void *)&delta2_mid_mem_obj);
	ret = clSetKernelArg(kernel_wgx1, 1, sizeof(cl_mem), (void *)&d_sigmoid_dense_input_mem_obj);
	ret = clSetKernelArg(kernel_wgx1, 2, sizeof(cl_mem), (void *)&delta2_mem_obj);

	cl_int kernel_wgx_size1 = 10;
	ret = clSetKernelArg(kernel_wgx2, 0, sizeof(cl_mem), (void *)&dense_w2_mem_obj);
	ret = clSetKernelArg(kernel_wgx2, 1, sizeof(cl_mem), (void *)&delta4_mem_obj);
	ret = clSetKernelArg(kernel_wgx2, 2, sizeof(cl_mem), (void *)&delta3_mid_mem_obj);
	ret = clSetKernelArg(kernel_wgx2, 3, sizeof(cl_int), (void *)&kernel_wgx_size1);
	printf("Set kernel Arg: %d\n", ret);
	ret = clSetKernelArg(kernel_wgx3, 0, sizeof(cl_mem), (void *)&delta3_mid_mem_obj);
	ret = clSetKernelArg(kernel_wgx3, 1, sizeof(cl_mem), (void *)&d_sigmoid_dense_sum_mem_obj);
	ret = clSetKernelArg(kernel_wgx3, 2, sizeof(cl_mem), (void *)&delta3_mem_obj);
	printf("Set kernel Arg: %d\n", ret);

	ret = clSetKernelArg(kernel_wgx4, 0, sizeof(cl_mem), (void *)&dense_sum_mem_obj);
	ret = clSetKernelArg(kernel_wgx4, 1, sizeof(cl_mem), (void *)&d_sigmoid_dense_sum_mem_obj);

	ret = clSetKernelArg(kernel_wgx5, 0, sizeof(cl_mem), (void *)&dense_input_mem_obj);
	ret = clSetKernelArg(kernel_wgx5, 1, sizeof(cl_mem), (void *)&d_sigmoid_dense_input_mem_obj);

	cl_int kernel_wgx_size6_1 = 980;
	cl_int kernel_wgx_size6_2 = 120;
	ret = clSetKernelArg(kernel_wgx6, 0, sizeof(cl_mem), (void *)&dense_w_mem_obj);
	ret = clSetKernelArg(kernel_wgx6, 1, sizeof(cl_mem), (void *)&dense_input_mem_obj);
	ret = clSetKernelArg(kernel_wgx6, 2, sizeof(cl_mem), (void *)&dense_sum_mem_obj);
	ret = clSetKernelArg(kernel_wgx6, 3, sizeof(cl_mem), (void *)&dense_b_mem_obj);
	ret = clSetKernelArg(kernel_wgx6, 4, sizeof(cl_int), (void *)&kernel_wgx_size6_1);
	ret = clSetKernelArg(kernel_wgx6, 5, sizeof(cl_int), (void *)&kernel_wgx_size6_2);

	ret = clSetKernelArg(kernel_wgx7, 0, sizeof(cl_mem), (void *)&dense_sum_mem_obj);
	ret = clSetKernelArg(kernel_wgx7, 1, sizeof(cl_mem), (void *)&dense_sigmoid_mem_obj);
	printf("Set kernel Arg: %d\n", ret);

	cl_int kernel_wgx_size8_1 = 120;
	cl_int kernel_wgx_size8_2 = 10;
	ret = clSetKernelArg(kernel_wgx8, 0, sizeof(cl_mem), (void *)&dense_w2_mem_obj);
	ret = clSetKernelArg(kernel_wgx8, 1, sizeof(cl_mem), (void *)&dense_sigmoid_mem_obj);
	ret = clSetKernelArg(kernel_wgx8, 2, sizeof(cl_mem), (void *)&dense_sum2_mem_obj);
	ret = clSetKernelArg(kernel_wgx8, 3, sizeof(cl_mem), (void *)&dense_b2_mem_obj);
	ret = clSetKernelArg(kernel_wgx8, 4, sizeof(cl_int), (void *)&kernel_wgx_size8_1);
	ret = clSetKernelArg(kernel_wgx8, 5, sizeof(cl_int), (void *)&kernel_wgx_size8_2);

	cl_int kernel_wgx_size9 = 10;
	ret = clSetKernelArg(kernel_wgx9, 0, sizeof(cl_mem), (void *)&dense_sum2_mem_obj);
	ret = clSetKernelArg(kernel_wgx9, 1, sizeof(cl_mem), (void *)&dense_softmax_mem_obj);
	ret = clSetKernelArg(kernel_wgx9, 2, sizeof(cl_int), (void *)&kernel_wgx_size9);
	printf("Set kernel Arg: %d\n", ret);

	// ! Max pooling setup (forward pass)
	fp = fopen("max_pooling_kernel.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// Create memory buffers on the device for each vector
	max_layer_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
									   sizeof(max_layer), NULL, &ret);

	// Create a program from the kernel source
	cl_program max_pooling_program = clCreateProgramWithSource(context, 1,
															   (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
	ret = clBuildProgram(max_pooling_program, 1, &device_id, NULL, NULL, NULL);
	printf("Build the program: %d\n", ret);

	// Create the OpenCL kernel
	max_pooling_kernel = clCreateKernel(max_pooling_program, "max_pooling_kernel", &ret);
	printf("Create the OpenCL kernel: %d\n", ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(max_pooling_kernel, 0, sizeof(cl_mem), (void *)&sig_layer_mem_obj);
	ret = clSetKernelArg(max_pooling_kernel, 1, sizeof(cl_mem), (void *)&max_pooling_mem_obj);
	ret = clSetKernelArg(max_pooling_kernel, 2, sizeof(cl_mem), (void *)&max_layer_mem_obj);

	// ! conv weight kernel
	fp = fopen("conv_weight_kernel.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// Create memory buffers on the device for each vector
	dw_max_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
									sizeof(dw_max), NULL, &ret);

	// Create a program from the kernel source
	cl_program conv_weight_kernel_program = clCreateProgramWithSource(context, 1,
																	  (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
	ret = clBuildProgram(conv_weight_kernel_program, 1, &device_id, NULL, NULL, NULL);
	printf("Build the program: %d\n", ret);

	// Create the OpenCL kernel
	conv_weight_kernel = clCreateKernel(conv_weight_kernel_program, "conv_weight_kernel", &ret);
	printf("Create the OpenCL kernel: %d\n", ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(conv_weight_kernel, 0, sizeof(cl_mem), (void *)&dw_max_mem_obj);
	ret = clSetKernelArg(conv_weight_kernel, 1, sizeof(cl_mem), (void *)&img_mem_obj);
	ret = clSetKernelArg(conv_weight_kernel, 2, sizeof(cl_mem), (void *)&dw_conv_mem_obj);

	// ! max_layer_back_kernel
	fp = fopen("max_layer_back_kernel.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// Create a program from the kernel source
	cl_program max_layer_back_kernel_progarm = clCreateProgramWithSource(context, 1,
																		 (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
	ret = clBuildProgram(max_layer_back_kernel_progarm, 1, &device_id, NULL, NULL, NULL);
	printf("Build the program: %d\n", ret);

	// Create the OpenCL kernel
	max_layer_back_kernel = clCreateKernel(max_layer_back_kernel_progarm, "max_layer_back_kernel", &ret);
	printf("Create the OpenCL kernel: %d\n", ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(max_layer_back_kernel, 0, sizeof(cl_mem), (void *)&max_pooling_mem_obj);
	ret = clSetKernelArg(max_layer_back_kernel, 1, sizeof(cl_mem), (void *)&delta2_mem_obj);
	ret = clSetKernelArg(max_layer_back_kernel, 2, sizeof(cl_mem), (void *)&dw_max_mem_obj);

	// ! Load Dataset
	read_test_data();
	read_train_data();
	initialise_weights();

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
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseProgram(program_wgx);

	ret = clReleaseMemObject(dense_input_mem_obj);
	ret = clReleaseMemObject(delta3_mid_mem_obj);
	ret = clReleaseMemObject(delta3_mem_obj);
	ret = clReleaseMemObject(delta2_mid_mem_obj);
	ret = clReleaseMemObject(delta2_mem_obj);
	ret = clReleaseMemObject(dense_w_mem_obj);
	ret = clReleaseMemObject(dw1_mem_obj);
	ret = clReleaseMemObject(d_sigmoid_dense_input_mem_obj);
	ret = clReleaseMemObject(dense_w2_mem_obj);
	ret = clReleaseMemObject(delta4_mem_obj);
	ret = clReleaseMemObject(d_sigmoid_dense_sum_mem_obj);

	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	return 0;
}
