__kernel void update_weights_1d(__global float *A, __global float *B,
                                __global float *C) {

  // Get the index of the current element
  int i = get_global_id(0);

  // Do the operation
  C[i] -= *A * B[i];
}

__kernel void update_weights_2d(__global float *A, __global float *B,
                                __global float *C, int w) {
  int i = get_global_id(0);
  int j = get_global_id(1);
  C[i * w + j] -= *A * B[i * w + j];
}

// w1 is the last dimension, w2 is the second last dimension.
__kernel void update_weights_3d(__global float *A, __global float *B,
                                __global float *C, int w1, int w2) {
  int i = get_global_id(0);
  int j = get_global_id(1);
  int k = get_global_id(2);
  C[i * w2 * w1 + j * w1 + k] -= *A * B[i * w2 * w1 + j * w1 + k];
}

__kernel void forward_conv(__global float *img, __global float *conv_w,
                           __global float *conv_layer) {

  int k = get_global_id(0);
  int l = get_global_id(1);
  for (int filter_dim = 0; filter_dim < 5; filter_dim++) {
    for (int i = 0; i < 28; i++)
      for (int j = 0; j < 28; j++)
        conv_layer[filter_dim * 784 + i * 28 + j] +=
            img[(i + k + 1) * 32 + j + l - 2] *
            conv_w[filter_dim * 784 + k * 28 + l];
  }
}

float sigmoid(float x) {
  if (x > 500)
    x = 500;
  if (x < -500)
    x = -500;
  return 1 / (1 + exp(-x));
}
__kernel void sig_layer(__global float *sig_layer, __global float *conv_layer, __global float *conv_b) {
  int filter_dim = get_global_id(0);
  int i = get_global_id(1);
  int j = get_global_id(2);
  sig_layer[filter_dim * 784 + i * 28 + j] =
      sigmoid(conv_layer[filter_dim * 784 + i * 28 + j] + conv_b[filter_dim * 784 + i * 28 + j]);
}
