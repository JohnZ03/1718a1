    kernel void matrix_mul_1(__global float *A, global float *B, __global float *C, int wA, int wB){
            int tx = get_global_id(0);
	            int ty = get_global_id(1);
		            float elementA = A[tx];
			            float elementB = B[ty];
				            float value = elementA * elementB;
					            C[tx*wA + ty] = value;
						    }