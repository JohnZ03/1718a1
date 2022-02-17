__kernel void conv_weight_kernel(__global float *dw_max, __global char *img,
                                 __global float *dw_conv) {
  // Get the index of the current element
  int filter_dim = get_global_id(0);
  int k = get_global_id(1);
  int l = get_global_id(2);

  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      float cur_val = dw_max[filter_dim * 784 + i * 28 + j];
      dw_conv[filter_dim * 49 + k * 7 + l] +=
          img[(i + k + 1) * 32 + j + l - 2] * cur_val;
    }
  }
}