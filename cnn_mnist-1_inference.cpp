#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include "fixed.h"

using fixed = numeric::fixed<52, 12>;
using weights = numeric::fixed<20, 12>;

#if __cplusplus >= 201402L
#define STATIC_ASSERT14(expr) static_assert((expr), "")
#else
#define STATIC_ASSERT14(expr) assert((expr))
#endif
fixed conv_layer_test[5][28][28];
fixed max_pooling_test[5][28][28];
fixed sig_layer_test[5][28][28];
fixed dense_input_test[980];
fixed max_layer_test[5][14][14];
fixed dense_sum2_test[10];
fixed dense_sum_test[120];
fixed dense_sigmoid_test[120];

weights conv_w_test[5][7][7];
weights conv_b_test[5][28][28];
weights dense_w_test[980][120];
weights dense_b_test[120];
weights dense_w2_test[120][10];
weights dense_b2_test[10];

const int filter_size = 7;
const double eta = 0.01;
const int batch_size = 200;

unsigned char data_train[60000][784];
unsigned char data_test[10000][784];
unsigned char label_train[60000];
unsigned char label_test[10000];

double conv_w[5][7][7];
double conv_b[5][28][28];
double conv_layer[5][28][28];
double sig_layer[5][28][28];
char max_pooling[5][28][28];
double max_layer[5][14][14];

double dense_input[980];
double dense_w[980][120];
double dense_b[120];
double dense_sum[120];
double dense_sigmoid[120];
double dense_w2[120][10];
double dense_b2[10];
double dense_sum2[10];
double dense_softmax[10];

// double dw2[120][10];
// double db2[10];
// double dw1[980][120];
// double db1[120];

// double dw_max[5][28][28];
// double dw_conv[5][7][7];
// double db_conv[5][28][28];

/* ************************************************************ */
/* Helper functions */
fixed sigmoid_test(fixed x)
{
        // if (x>20) x=20;
        // if (x<-20) x=-20;
        // return 1/(1+exp1(-x));
        fixed y;
        if (x > -1 && x <= 1)
                y = 0.2383 * x + 0.5000;
        if (x > -2 && x <= -1)
                y = 0.0467 * x * x + 0.2896 * x + 0.5118;
        if (x > -3 && x <= -2)
                y = 0.0298 * x * x + 0.2202 * x + 0.4400;
        if (x > -4 && x <= -3)
                y = 0.0135 * x * x + 0.1239 * x + 0.2969;
        if (x > -5 && x <= -4)
                y = 0.0054 * x * x + 0.0597 * x + 0.1703;
        if (x > -5.03 && x <= -5)
                y = 0.0066;
        if (x > -5.2 && x <= -5.03)
                y = 0.0060;
        if (x > -5.41 && x <= -5.2)
                y = 0.0050;
        if (x > -5.66 && x <= -5.41)
                y = 0.0040;
        if (x > -6 && x <= -5.66)
                y = 0.0030;
        if (x > -6.53 && x <= -6)
                y = 0.0020;
        if (x > -7.6 && x <= -6.53)
                y = 0.0010;
        if (x <= -7.6)
                y = 0;
        if (x > 1 && x <= 2)
                y = -0.0467 * x * x + 0.2896 * x + 0.4882;
        if (x > 2 && x <= 3)
                y = -0.0298 * x * x + 0.2202 * x + 0.5600;
        if (x > 3 && x <= 4)
                y = -0.0135 * x * x + 0.1239 * x + 0.7030;
        if (x > 4 && x <= 5)
                y = -0.0054 * x * x + 0.0597 * x + 0.8297;
        if (x > 5 && x <= 5.0218)
                y = 0.9930;
        if (x > 5.0218 && x <= 5.1890)
                y = 0.9940;
        if (x > 5.1890 && x <= 5.3890)
                y = 0.9950;
        if (x > 5.3890 && x <= 5.6380)
                y = 0.9960;
        if (x > 5.6380 && x <= 5.9700)
                y = 0.9970;
        if (x > 5.9700 && x <= 6.4700)
                y = 0.9980;
        if (x > 6.4700 && x <= 7.5500)
                y = 0.9990;
        if (x > 7.5500)
                y = 1;
        return y;
}

double sigmoid(double x)
{
        if (x > 500)
                x = 500;
        if (x < -500)
                x = -500;
        return 1 / (1 + exp(-x));
}
double d_sigmoid(double x)
{
        double sig = sigmoid(x);
        return sig * (1 - sig);
}
double softmax_den(double *x, int len)
{
        double val = 0;
        for (int i = 0; i < len; i++)
        {
                val += exp(x[i]);
        }
        return val;
}

/* ************************************************************ */
/* Forward Pass */
void forward_pass_test(unsigned char img[][32])
{
        fixed imgfix[35][32];
        for (int i = 0; i < 35; i++)
                for (int j = 0; j < 32; j++)
                        imgfix[i][j] = img[i][j];

        // Convolution Operation + Sigmoid Activation
        for (int filter_dim = 0; filter_dim < 5; filter_dim++)
        {
                for (int i = 0; i < 28; i++)
                {
                        for (int j = 0; j < 28; j++)
                        {
                                max_pooling_test[filter_dim][i][j] = 0;

                                conv_layer_test[filter_dim][i][j] = 0;
                                sig_layer_test[filter_dim][i][j] = 0;
                                for (int k = 0; k < filter_size; k++)
                                {
                                        for (int l = 0; l < filter_size; l++)
                                        {
                                                conv_layer_test[filter_dim][i][j] += imgfix[i + k + 1][j + l - 2] * conv_w_test[filter_dim][k][l];
                                        }
                                }
                                sig_layer_test[filter_dim][i][j] = sigmoid_test(conv_layer_test[filter_dim][i][j] + conv_b_test[filter_dim][i][j]);
                        }
                }
        }

        // MAX Pooling (max_pooling, max_layer)
        fixed cur_max = 0;
        int max_i = 0, max_j = 0;
        for (int filter_dim = 0; filter_dim < 5; filter_dim++)
        {
                for (int i = 0; i < 28; i += 2)
                {
                        for (int j = 0; j < 28; j += 2)
                        {
                                max_i = i;
                                max_j = j;
                                cur_max = sig_layer_test[filter_dim][i][j];
                                for (int k = 0; k < 2; k++)
                                {
                                        for (int l = 0; l < 2; l++)
                                        {
                                                if (sig_layer_test[filter_dim][i + k][j + l] > cur_max)
                                                {
                                                        max_i = i + k;
                                                        max_j = j + l;
                                                        cur_max = sig_layer_test[filter_dim][max_i][max_j];
                                                }
                                        }
                                }
                                max_pooling_test[filter_dim][max_i][max_j] = 1;
                                max_layer_test[filter_dim][i / 2][j / 2] = cur_max;
                        }
                }
        }

        int k = 0;
        for (int filter_dim = 0; filter_dim < 5; filter_dim++)
        {
                for (int i = 0; i < 14; i++)
                {
                        for (int j = 0; j < 14; j++)
                        {
                                dense_input_test[k] = max_layer_test[filter_dim][i][j];
                                k++;
                        }
                }
        }

        // Dense Layer
        for (int i = 0; i < 120; i++)
        {
                dense_sum_test[i] = 0;
                dense_sigmoid_test[i] = 0;
                for (int j = 0; j < 980; j++)
                {
                        dense_sum_test[i] += dense_w_test[j][i] * dense_input_test[j];
                }
                dense_sum_test[i] = dense_sum_test[i] + dense_b_test[i];
                dense_sigmoid_test[i] = sigmoid_test(dense_sum_test[i]);
        }

        // Dense Layer 2
        for (int i = 0; i < 10; i++)
        {
                dense_sum2_test[i] = 0;
                for (int j = 0; j < 120; j++)
                {
                        dense_sum2_test[i] += dense_w2_test[j][i] * dense_sigmoid_test[j];
                }
                dense_sum2_test[i] = dense_sum2_test[i] + dense_b2_test[i];
        }

        // // Softmax Output
        // double den = softmax_den(dense_sum2, 10);
        // for (int i = 0; i < 10; i++)
        // {
        //         dense_softmax[i] = exp(dense_sum2[i]) / den;
        // }
}

void read_weights()
{
        std::ifstream fin("weights.txt");
        if (!fin)
                std::cerr << "Unable to open file!";
        for (int i = 0; i < 120; i++)
        {
                fin >> dense_b[i];
                dense_b_test[i] = dense_b[i];
        }

        for (int j = 0; j < 10; j++)
        {
                fin >> dense_b2[j];
                dense_b2_test[j] = dense_b2[j];
        }

        for (int i = 0; i < 120; i++)
                for (int j = 0; j < 10; j++)
                {
                        fin >> dense_w2[i][j];
                        dense_w2_test[i][j] = dense_w2[i][j];
                }

        for (int i = 0; i < 120; i++)
                for (int k = 0; k < 980; k++)
                {
                        fin >> dense_w[k][i];
                        dense_w_test[k][i] = dense_w[k][i];
                }

        for (int i = 0; i < 5; i++)
                for (int k = 0; k < 7; k++)
                        for (int j = 0; j < 7; j++)
                        {
                                fin >> conv_w[i][k][j];
                                conv_w_test[i][k][j] = conv_w[i][k][j];
                        }

        for (int i = 0; i < 5; i++)
                for (int l = 0; l < 28; l++)
                        for (int m = 0; m < 28; m++)
                        {
                                fin >> conv_b[i][l][m];
                                conv_b_test[i][l][m] = conv_b[i][l][m];
                        }
}

void read_test_data()
{
        std::ifstream csvread;
        csvread.open("/cad2/ece1718s/mnist_test.csv", std::ios::in);
        if (csvread)
        {
                std::string s;
                int data_pt = 0;
                while (getline(csvread, s))
                {
                        std::stringstream ss(s);
                        int pxl = 0;
                        while (ss.good())
                        {
                                std::string substr;
                                getline(ss, substr, ',');
                                if (pxl == 0)
                                {
                                        label_test[data_pt] = stoi(substr);
                                }
                                else
                                {
                                        data_test[data_pt][pxl - 1] = stoi(substr);
                                }
                                pxl++;
                        }
                        data_pt++;
                }
                csvread.close();
        }
        else
        {
                std::cerr << "Unable to read test data!" << std::endl;
                exit(EXIT_FAILURE);
        }
}

void give_img(unsigned char *vec, unsigned char img[][32])
{
        int k = 0;
        for (int i = 0; i < 35; i++)
        {
                for (int j = 0; j < 32; j++)
                {
                        if (i < 5 || j < 2 || i > 32 || j > 29)
                        {
                                img[i][j] = 0;
                        }
                        else
                        {
                                img[i][j] = vec[k++];
                        }
                }
        }
}

void give_y(int y, int *vector_y)
{
        for (int i = 0; i < 10; i++)
                vector_y[i] = 0;
        vector_y[y] = 1;
}
int give_prediction_test()
{
        fixed max_val = dense_sum2_test[0];
        int max_pos = 0;
        for (int i = 1; i < 10; i++)
        {
                if (dense_sum2_test[i] > max_val)
                {
                        max_val = dense_sum2_test[i];
                        max_pos = i;
                }
        }

        return max_pos;
}

int main()
{
        read_test_data();
        read_weights();
        int val_len = 600;
        int cor = 0;
        int confusion_mat[10][10];
        for (int i = 0; i < 10; i++)
        {
                for (int j = 0; j < 10; j++)
                        confusion_mat[i][j] = 0;
        }

        std::cout << "Start Testing." << std::endl;
        for (int i = 0; i < val_len; i++)
        {
                unsigned char img[35][32];
                give_img(data_test[i], img);
                forward_pass_test(img);
                int pre = give_prediction_test();
                confusion_mat[label_test[i]][pre]++;
                if (pre == label_test[i])
                        cor++;
        }
        float accu = double(cor) / val_len;
        std::cout << "Accuracy: " << accu << std::endl;

        std::cout << "   0 1 2 3 4 5 6 7 8 9" << std::endl;
        for (int i = 0; i < 10; i++)
        {
                std::cout << i << ": ";
                for (int j = 0; j < 10; j++)
                {
                        std::cout << confusion_mat[i][j] << " ";
                }
                std::cout << std::endl;
        }

        return 0;
}
