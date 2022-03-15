#include <svdpi.h>
#include <math.h>
#include <stdio.h>

int conv_core_model(const int img[7][7],const int conv_w[7][7]){
    int conv_layer = 0;
    for (int k = 0; k < 7; k++)
    {
            for (int l = 0; l < 7; l++)
            {
                    conv_layer += img[k][l] * conv_w[k][l];
            }
    }
    return conv_layer;
}
