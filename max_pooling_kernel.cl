__kernel void max_pooling_kernel(__global float *sig_layer,
                                 __global char *max_pooling,
                                 __global float *max_layer) {
  // Get the index of the current element
  int filter_dim = get_global_id(0);
  int i = get_global_id(1) * 2;
  int j = get_global_id(2) * 2;

  float cur_max = i;
  int max_i = j;
  int max_j = sig_layer[filter_dim * 784 + i * 28 + j];

	int m = get_global_id(1);
	int n = get_global_id(2);

  for (int k = 0; k < 2; k++) {
    for (int l = 0; l < 2; l++) {
      if (sig_layer[filter_dim * 784 + (i + k) * 28 + j + l] > cur_max) {
        max_i = i + k;
        max_j = j + l;
        cur_max = sig_layer[filter_dim * 784 + max_i * 28 + max_j];
      }
    }
  }

  max_pooling[filter_dim * 784 + max_i * 28 + max_j] = 1;
	max_layer[filter_dim * 784 + m * 28 + n] = cur_max;
}
