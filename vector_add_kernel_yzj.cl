__kernel void vector_add_yzj(__global float *A, __global float *B, __global float *C) {
    
    // Get the index of the current element
    int i = get_global_id(0);
    int j = get_global_id(1);

    // Do the operation
    C[i*10 + j] = A[i] * B[j];
}
// __kernel void vector_add(__global int *A, __global int *B, __global int *C) {
    
//     // Get the index of the current element
//     int i = get_global_id(0);

//     // Do the operation
//     C[i] = A[i] + B[i];
// }
