#include <svdpi.h>
#include <math.h>
#include <stdio.h>

int conv_core_model(const int img[49],const int conv_w[49]){
    int conv_layer = 0;
    int k,l;
    for (k = 0; k < 7; k++)
    {
            for (l = 0; l < 7; l++)
            {
                    conv_layer += img[k*7+l] * conv_w[k*7+l];
            }
    }
    return conv_layer;
}
