#include <svdpi.h>
#include <math.h>
#include <stdio.h>

int sigmoid_model(int x) {
    int y;
    double x1=x;
    if(x1>-256&&x1<=256)               y =                               floor(7809*x1/32768) + 128;
    else if(x1>-512&&x1<=-256)         y = floor(floor(1530*x1/256)*x1/32768) + floor(9490*x1/32768) + 131;
    else if(x1>-768&&x1<=-512)         y = floor(floor(976 *x1/256)*x1/32768) + floor(7216*x1/32768) + 113;
    else if(x1>-1024&&x1<=-768)        y = floor(floor(442 *x1/256)*x1/32768) + floor(4060*x1/32768) + 76;

    else if(x1>-1280&&x1<=-1024)        y = floor(floor(177 *x1/256)*x1/32768) + floor(1956*x1/32768) + 44;

    else if(x1>1280  && x1<1380)       y = 254;
    else if(x1>=1380 && x1<1536)       y = 255;
    else if(x1>=1536)                  y = 256;

    else if(x1<512&&x1>=256)           y = floor(floor(-1530*x1/256)*x1/32768) + floor(9490*x1/32768) + 125;
    else if(x1<768&&x1>=512)           y = floor(floor(-976 *x1/256)*x1/32768) + floor(7216*x1/32768) + 143;
    else if(x1<1024&&x1>=768)          y = floor(floor(-442 *x1/256)*x1/32768) + floor(4060*x1/32768) + 180;
    else if(x1<=1280&&x1>=1024)         y = floor(floor(-177 *x1/256)*x1/32768) + floor(1956*x1/32768) + 212;

    else if(x1<=-1280  && x1>-1385)   y = 2;
    else if(x1<-1380 && x1>-1536)     y = 1;
    else if(x1<=-1536)                y = 0;
    return y;
}


void conv_top_model(const int img[35][32], const int conv_w[5][7][7],const int conv_b[5][28][28],int sig_layer[5][28][28]){
    int conv_layer[5][28][28];
    int filter_dim;
    int i,j,k,l;
    for (filter_dim = 0; filter_dim < 5; filter_dim++)
    {
            for (i = 0; i < 28; i++)
            {
                    for (j = 0; j < 28; j++)
                    {
                            conv_layer[filter_dim][i][j] = 0;
                            sig_layer[filter_dim][i][j] = 0;
                            for (k = 0; k < filter_size; k++)
                            {
                                    for (l = 0; l < filter_size; l++)
                                    {
                                            conv_layer[filter_dim][i][j] += img[i + k + 1][j + l - 2] * conv_w[filter_dim][k][l];
                                    }
                            }
                            sig_layer[filter_dim][i][j] = sigmoid_model(conv_layer[filter_dim][i][j] + floor((double)conv_b[filter_dim][i][j])/256);
                    }
            }
    }


