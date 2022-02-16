__kernel void multi_add(__global float *A, __global float *B, __global float *C, int wB) {
    
    int i = get_global_id(0);
	float value = 0;
	for(int j=0; j<wB ;j++){
		value += A[i*wB+j] * B[j];
	}
    C[i] = value;

}

__kernel void multi_add2(__global float *A, __global float *B, __global float *C,__global float *D, int wA, int wB) {
    
    int i = get_global_id(0);
	float value = 0;
	for(int j=0; j<wA ;j++){
		value += A[j*wB+i] * B[j];
	}
	value += D[i];
    C[i] = value;
}

__kernel void vector_multi(__global float *A, __global float *B, __global float *C) {
    
    // Get the index of the current element
    int i = get_global_id(0);

    // Do the operation
    C[i] = A[i] * B[i];
}

__kernel void d_sigmoid(__global float *A, __global float *B) {
    
    int i = get_global_id(0);
	float x=A[i];
	float sigmoid_x;
	if(A[i] >  500) x =  500;
	if(A[i] < -500) x = -500;
    sigmoid_x = 1 / (1 + exp(-x));
	B[i] = sigmoid_x * (1 - sigmoid_x);
}

__kernel void sigmoid(__global float *A, __global float *B) {
    
    int i = get_global_id(0);
	float x=A[i];
	if(A[i] >  500) x =  500;
	if(A[i] < -500) x = -500;
    B[i] = 1 / (1 + exp(-x));
}

__kernel void softmax_den(__global float *A, __global float *B, int len){
	float value = 0;
	for ( int j=0; j<len; j++){
		value += exp(A[j]);
	}
	int i = get_global_id(0);
	B[i] = exp(A[i])/value;
}	

