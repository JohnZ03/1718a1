#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

#include <cstring>
// SECTION: def for cl file
FILE *fp1;
char *source_str1;
size_t source_size1;
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
cl_mem delta3_mid_mem_obj;
cl_mem delta3_mem_obj;
cl_mem delta2_mid_mem_obj;
cl_mem delta2_mem_obj;
cl_mem dense_w_mem_obj;
cl_mem dw1_mem_obj;
cl_mem d_sigmoid_dense_input_mem_obj;
cl_mem dense_w2_mem_obj;
cl_mem delta4_mem_obj;
cl_mem d_sigmoid_dense_sum_mem_obj;

cl_command_queue command_queue;

cl_kernel kernel;
cl_kernel kernel_wgx;
cl_kernel kernel_wgx1;
cl_kernel kernel_wgx2;
cl_kernel kernel_wgx3;

// SECTION: global variables for openCL
cl_platform_id platform_id = NULL;
cl_device_id device_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;

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
	for (int i = 0; i < 120; i++)
	{
		dense_b[i] -= eta * db1[i];
		for (int j = 0; j < 10; j++)
		{
			dense_b2[j] -= eta * db2[j];
			dense_w2[i][j] -= eta * dw2[i][j];
		}
		for (int k = 0; k < 980; k++)
		{
			dense_w[k][i] -= eta * dw1[k][i];
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int k = 0; k < 7; k++)
		{
			for (int j = 0; j < 7; j++)
			{
				conv_w[i][k][j] -= eta * dw_conv[i][k][j];
			}
		}
		for (int l = 0; l < 28; l++)
		{
			for (int m = 0; m < 28; m++)
			{
				conv_b[i][l][m] -= eta * db_conv[i][l][m];
			}
		}
	}
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

	for(int i=0; i<120; i++){
		d_sigmoid_dense_sum[i] = d_sigmoid(dense_sum[i]);
	}
	memset(delta3,0,sizeof(delta3));
	float z2_wgx = 0;
	ret = clEnqueueWriteBuffer(command_queue, delta4_mem_obj, CL_TRUE, 0,
							   sizeof(delta4), delta4, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, dense_w2_mem_obj, CL_TRUE, 0,
							   sizeof(dense_w2), dense_w2, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, delta3_mid_mem_obj, CL_TRUE, 0,
							   sizeof(delta3), delta3, 0, NULL, NULL);

	size_t global_item_size_wgx3 = 120; // Process the entire lists
	size_t local_item_size_wgx3 = 20;			 // Process in groups of 10
   	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx2, 1, NULL,
								 &global_item_size_wgx3, &local_item_size_wgx3, 0, NULL, NULL);

	ret = clEnqueueWriteBuffer(command_queue, d_sigmoid_dense_sum_mem_obj, CL_TRUE, 0,
							   sizeof(d_sigmoid_dense_sum), d_sigmoid_dense_sum, 0, NULL, NULL);

   	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx3, 1, NULL,
								 &global_item_size_wgx3, &local_item_size_wgx3, 0, NULL, NULL);

	//ret = clEnqueueReadBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						  sizeof(delta3), delta3, 0, NULL, NULL);
	
	ret = clEnqueueReadBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
							  sizeof(db1), db1, 0, NULL, NULL);

	/*
	float delta3_1[120];
	for (int i = 0; i < 120; i++)
	{
		delta3_1[i] = 0;
		for (int j = 0; j < 10; j++)
		{
			delta3_1[i] += dense_w2[i][j] * delta4[j];
		}
		delta3_1[i] *= d_sigmoid(dense_sum[i]);
		//if (delta3_1[i] != delta3[i])
			printf("delta3_1: origin %f, new %f\n",delta3_1[i],delta3[i]);
	}
    
	for (int i = 0; i < 120; i++)
		db1[i] = delta3[i]; // Bias Weight change

    */
	// Calculate Weight Changes for Dense Layer 1
	// TODO: attempt on OPENCL (top prio)
	/*
	for (int i = 0; i < 980; i++)
	{
		for (int j = 0; j < 120; j++)
		{
			dw1[i][j] = dense_input[i] * delta3[j];
		}
	}
	*/

	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, dense_input_mem_obj, CL_TRUE, 0,
							   sizeof(dense_input), dense_input, 0, NULL, NULL);
	//ret = clEnqueueWriteBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						   sizeof(delta3), delta3, 0, NULL, NULL);

	// Execute the OpenCL kernel on the list
	size_t global_item_size[2] = {980, 120}; // Process the entire lists
	size_t local_item_size[2] = {10, 12};			 // Process in groups of 64 
	// // ! Error code -48, CL_INVALID_KERNEL
	// ! Error code = -54, CL_INVALID_WORK_GROUP_SIZE
	// ! Fixed, requires 2D local_item_size
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL,
								 global_item_size, local_item_size, 0, NULL, NULL);
	// printf("Execute the OpenCL kernel on the list: %d\n", ret);

	// Read the memory buffer C on the device to the local variable C
	// // ! Ret = 0, correct
	// ! Error code = -5, CL_OUT_OF_RESOURCES
	ret = clEnqueueReadBuffer(command_queue, dw1_mem_obj, CL_TRUE, 0,
							  sizeof(dw1), dw1, 0, NULL, NULL);
	// printf("Read the memory buffer C on the device to the local variable C: %d\n", ret);

	//float dw1_1[980][120];
	// Calculate Weight Changes for Dense Layer 1

	// for (int i = 0; i < 980; i++)
	// {
	// 	for (int j = 0; j < 120; j++)
	// 	{
	// 		dw1_1[i][j] = dense_input[i] * delta3[j];
	// 		if (dw1_1[i][j] != dw1[i][j])
	// 		{
	// 			printf("i=%d,j=%d,dw_opencl=%f,dw_origin=%f\n", i, j, dw1[i][j], dw1_1[i][j]);
	// 		} else
	// 		{
	// 			printf("Matched values.\n");
	// 		}
			
	// 	}
	// }

	// Delta2
	// TODO: attempt on OPENCL      Guoxian
	
    float d_sigmoid_dense_input[980];

	for(int i=0; i<980; i++){
		d_sigmoid_dense_input[i] = d_sigmoid(dense_input[i]);
	}
	float delta2[980];
	memset(delta2,0,sizeof(delta2));
	float z1_wgx = 0;
//	ret = clEnqueueFillBuffer(command_queue, delta2_mid_mem_obj, &z1_wgx, sizeof(z1_wgx),
//			0, sizeof(delta2), 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, delta2_mid_mem_obj, CL_TRUE, 0,
							   sizeof(delta2), delta2, 0, NULL, NULL);
	//printf("WriteBuffer: %d\n", ret);
	ret = clEnqueueWriteBuffer(command_queue, dense_w_mem_obj, CL_TRUE, 0,
							   sizeof(dense_w), dense_w, 0, NULL, NULL);
	//printf("WriteBuffer: %d\n", ret);
	//ret = clEnqueueWriteBuffer(command_queue, delta3_mem_obj, CL_TRUE, 0,
	//						   sizeof(delta3), delta3, 0, NULL, NULL);
	//printf("WriteBuffer: %d\n", ret);

	size_t global_item_size_wgx1 = 980; // Process the entire lists
	size_t local_item_size_wgx1 = 20;			 // Process in groups of 20
   	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx, 1, NULL,
								 &global_item_size_wgx1, &local_item_size_wgx1, 0, NULL, NULL);
	//printf("NDRangeKernel: %d\n", ret);
	//ret = clEnqueueReadBuffer(command_queue, delta2_mid_mem_obj, CL_TRUE, 0,
	//						  sizeof(delta2), delta2, 0, NULL, NULL);
	//printf("ReadBuffer: %d\n", ret);
/*	
	for(int i=0; i<980; i++){
		delta2[i] *= d_sigmoid(dense_input[i]);
	}
*/	


	ret = clEnqueueWriteBuffer(command_queue, d_sigmoid_dense_input_mem_obj, CL_TRUE, 0,
							   sizeof(d_sigmoid_dense_input), d_sigmoid_dense_input, 0, NULL, NULL);
	//printf("WriteBuffer: %d\n", ret);

   	ret = clEnqueueNDRangeKernel(command_queue, kernel_wgx1, 1, NULL,
								 &global_item_size_wgx1, &local_item_size_wgx1, 0, NULL, NULL);
	//printf("NDRangeKernel: %d\n", ret);

	ret = clEnqueueReadBuffer(command_queue, delta2_mem_obj, CL_TRUE, 0,
							  sizeof(delta2), delta2, 0, NULL, NULL);
	//printf("ReadBuffer: %d\n", ret);
/*
	float delta2_1[980];
	
	for (int i = 0; i < 980; i++)
	{
		delta2_1[i] = 0;
		for (int j = 0; j < 120; j++)
		{
			delta2_1[i] += dense_w[i][j] * delta3[j];
		}
		delta2_1[i] *= d_sigmoid(dense_input[i]);
		if(delta2_1[i]!=delta2[i])	
			printf("delta2_1: origin %f, new %f\n",delta2_1[i],delta2[i]);
	}
*/
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
	// SECTION Load the kernel source code into the array source_str1
	fp1 = fopen("mul_kernel.cl", "r");
	if (!fp1)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str1 = (char *)malloc(MAX_SOURCE_SIZE);
	source_size1 = fread(source_str1, 1, MAX_SOURCE_SIZE, fp1);
	fclose(fp1);
	
	fp_wgx = fopen("multi_add_kernel.cl", "r");
	//fp_wgx = fopen("mul_kernel.cl", "r");
	if (!fp_wgx)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str1_wgx = (char *)malloc(MAX_SOURCE_SIZE);
	source_size1_wgx = fread(source_str1_wgx, 1, MAX_SOURCE_SIZE, fp_wgx);
	fclose(fp_wgx);

	// SECTION: openCL setup
	// Get platform and device information
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1,
						 &device_id, &ret_num_devices);

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	// Create a command queue
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	// Create memory buffers on the device for each vector
	dense_input_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
							   sizeof(dense_input), NULL, &ret);
	delta3_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
							   120 * sizeof(float), NULL, &ret);
	dw1_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
							   980 * 120 * sizeof(float), NULL, &ret);

	// ! Error = -38, CL_INVALID_MEM_OBJECT if not added
	delta2_mid_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
							   980 * sizeof(float), NULL, &ret);
	delta2_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
							   980 * sizeof(float), NULL, &ret);
	dense_w_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
							   sizeof(dense_w), NULL, &ret);
    d_sigmoid_dense_input_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
							   980 * sizeof(float), NULL, &ret);

	dense_w2_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
							   1200 * sizeof(float), NULL, &ret);
	delta4_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
							   10 * sizeof(float), NULL, &ret);
	d_sigmoid_dense_sum_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
							   120 * sizeof(float), NULL, &ret);
	delta3_mid_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
							   120 * sizeof(float), NULL, &ret);
	// Create a program from the kernel source
	// ! Ret = 0, correct
	cl_program program = clCreateProgramWithSource(context, 1,
												   (const char **)&source_str1, (const size_t *)&source_size1, &ret);
	cl_program program_wgx = clCreateProgramWithSource(context, 1,
												   (const char **)&source_str1_wgx, (const size_t *)&source_size1_wgx, &ret);

	// Build the program
	// ! Error = -11, CL_BUILD_PROGRAM _FAILURE
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	printf("Build the program: %d\n", ret);
	ret = clBuildProgram(program_wgx, 1, &device_id, NULL, NULL, NULL);
	printf("Build the program: %d\n", ret);

	// Create the OpenCL kernel
	kernel = clCreateKernel(program, "mul_kernel", &ret);
	//printf("Create the OpenCL kernel: %d\n", ret);
	kernel_wgx = clCreateKernel(program_wgx, "multi_add", &ret);
	//printf("Create the OpenCL kernel multi_add: %d\n", ret);
	kernel_wgx1 = clCreateKernel(program_wgx, "vector_multi", &ret);
	//printf("Create the OpenCL kernel vector_multi: %d\n", ret);
	kernel_wgx2 = clCreateKernel(program_wgx, "multi_add", &ret);
	printf("Create the OpenCL kernel multi_add: %d\n", ret);
	kernel_wgx3 = clCreateKernel(program_wgx, "vector_multi", &ret);
	printf("Create the OpenCL kernel vector_multi: %d\n", ret);

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
	ret = clReleaseMemObject(dense_input_mem_obj);
	ret = clReleaseMemObject(delta3_mem_obj);
	ret = clReleaseMemObject(dw1_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	return 0;
}
