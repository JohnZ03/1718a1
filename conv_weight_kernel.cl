__kernel void max_layer_back_kernel(__global float *dw_max, __global char *img,
                                    __global float *dw_conv) {
  // Get the index of the current element
  int i = get_global_id(0);
  int j = get_global_id(1);

  for (int filter_dim = 0; filter_dim < 5; filter_dim++) {
    float cur_val = dw_max[filter_dim * 784 + i * 28 + j];
    for (int k = 0; k < 5; k++) {
      for (int l = 0; l < 5; l++) {
        dw_conv[filter_dim * 49 + k * 7 + l] +=
            img[(i + k + 1) * 32 + j + l - 2] * cur_val;
      }
    }
  }
}