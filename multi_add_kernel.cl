__kernel void multi_add(__global float *A, __global float *B, __global float *C, int wB) {
    
    int i = get_global_id(0);
	float value = 0;
	for(int j=0; j<wB ;j++){
		value += A[i*wB+j] * B[j];
	}
    C[i] = value;

}

