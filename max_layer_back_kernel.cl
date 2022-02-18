__kernel void max_layer_back_kernel(__global float *max_pooling,
                                    __global char *delta2,
                                    __global float *dw_max) {
  // Get the index of the current element
  int filter_dim = get_global_id(0);
  int i = get_global_id(1) * 2;
  int j = get_global_id(2) * 2;

  int x = get_global_id(1);
  int y = get_global_id(2);

  for (int l = 0; l < 2; l++) {
    for (int m = 0; m < 2; m++) {
      if (max_pooling[filter_dim*784 + (i+l)*28 + j + m] == 1) {
          dw_max[filter_dim*784 + i*28 + j] = delta2[filter_dim*196 + x*14 + y];
      } else {
          dw_max[filter_dim*784 + i*28 + j] = 0;
      }
    }
  }
}