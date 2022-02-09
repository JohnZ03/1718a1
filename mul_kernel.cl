__kernel void mul_kernel(__global float *A, __global float *B, __global float *C) {
    
    // Get the index of the current element
    int i = get_global_id(0);
    int j = get_global_id(1);

    // Do the operation
    C[i*120 + j] = A[i] * B[j];
}
