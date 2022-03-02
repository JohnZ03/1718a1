extern int device_state[2];

// layers
double conv_layer_test[5][28][28];
double max_pooling_test[5][28][28];
double sig_layer_test[5][28][28];
double dense_input_test[980];
double max_layer_test[5][14][14];
double dense_sum2_test[10];
double dense_sum_test[120];
double dense_sigmoid_test[120];

// weights
double conv_w_test[5][7][7];
double conv_b_test[5][28][28];
double dense_w_test[980][120];
double dense_b_test[120];
double dense_w2_test[120][10];
double dense_b2_test[10];

unsigned char img_buffer[35][32];
bool img_buffer_full = 0;
unsigned char *img_ptr = (unsigned char *)img_buffer;

const int filter_size = 7;

double weights_buffer[123095];
double *weights_ptr = weights_buffer;
bool weights_assigned = 0; //! temparory soln

bool finished_computing = 0;
int res_idx = 0;
bool result_last = 0;

double sigmoid_test(double x)
{
    double y;
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

void conv_sig()
{
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
                        conv_layer_test[filter_dim][i][j] += img_buffer[i + k + 1][j + l - 2] * conv_w_test[filter_dim][k][l];
                    }
                }
                sig_layer_test[filter_dim][i][j] = sigmoid_test(conv_layer_test[filter_dim][i][j] + conv_b_test[filter_dim][i][j]);
            }
        }
    }
}

void forward_pass_test(unsigned char img[][32])
{

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
                        conv_layer_test[filter_dim][i][j] += img[i + k + 1][j + l - 2] * conv_w_test[filter_dim][k][l];
                    }
                }
                sig_layer_test[filter_dim][i][j] = sigmoid_test(conv_layer_test[filter_dim][i][j] + conv_b_test[filter_dim][i][j]);
            }
        }
    }

    // MAX Pooling (max_pooling, max_layer)
    double cur_max = 0;
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

int *forward(int Mode_select, int host_valid, int host_ready, double &bus)
{
    // host is sending weights
    if (Mode_select == 1 && host_valid == 1 && host_ready == 0)
    {
        // Load weights
        *weights_ptr = bus;
        weights_ptr++;

        // device_ready, device_valid;
        device_state[0] = 1;
        device_state[1] = 0;
        return device_state;
    }

    // weights tranfer finished
    if (Mode_select == 2)
    {
        //! assign weights to variables, temparory soln only----------------------
        // std::cout << "assigning weights..." << std::endl;

        if (!weights_assigned)
        {
            // weights_buffer[120 + 10 + 120 * 10 + 120 * 980 + 5 * 7 * 7 + 5 * 28 * 28];
            weights_ptr = weights_buffer;
            while (weights_ptr != &weights_buffer[123095])
            {
                for (int i = 0; i < 120; i++)
                {
                    dense_b_test[i] = *weights_ptr;
                    weights_ptr++;
                }
                for (int j = 0; j < 10; j++)
                {
                    dense_b2_test[j] = *weights_ptr;
                    weights_ptr++;
                }
                for (int i = 0; i < 120; i++)
                    for (int j = 0; j < 10; j++)
                    {
                        dense_w2_test[i][j] = *weights_ptr;
                        weights_ptr++;
                    }
                for (int i = 0; i < 120; i++)
                    for (int k = 0; k < 980; k++)
                    {
                        dense_w_test[k][i] = *weights_ptr;
                        weights_ptr++;
                    }
                for (int i = 0; i < 5; i++)
                    for (int k = 0; k < 7; k++)
                        for (int j = 0; j < 7; j++)
                        {
                            conv_w_test[i][k][j] = *weights_ptr;
                            weights_ptr++;
                        }
                for (int i = 0; i < 5; i++)
                    for (int l = 0; l < 28; l++)
                        for (int m = 0; m < 28; m++)
                        {
                            conv_b_test[i][l][m] = *weights_ptr;
                            weights_ptr++;
                        }
            }
            weights_assigned = 1;
            std::cout << "assigning weights finished..." << std::endl
                      << std::endl;

        } //!---------------------------------------------------------------

        // host is not sending
        if (host_valid == 0 && host_ready == 1)
        {
            // finished computing and send data to host
            if (finished_computing)
            {
                // std::cout << "preparing sending results" << std::endl;

                if (host_valid == 0 && host_ready == 1 && result_last)
                {
                    std::cout << "sending results 9; sending finished" << std::endl;
                    result_last = 0;
                    finished_computing = 0; //! temporary
                    bus = dense_sum2_test[res_idx];
                    device_state[0] = 1;
                    device_state[1] = 0;
                    return device_state;
                }
                else if (host_valid == 0 && host_ready == 1 && !result_last)
                {
                    std::cout << "sending results " << res_idx << std::endl;

                    bus = dense_sum2_test[res_idx];
                    res_idx++;
                    if (res_idx == 9)
                    {
                        res_idx = 0;
                        result_last = 1;
                    }
                    device_state[0] = 0;
                    device_state[1] = 1;
                    return device_state;
                }
            }
            else
            {
                //! continue computing
                device_state[0] = 1;
                device_state[1] = 0;
                return device_state;
            }
        }
        else //!(host_valid == 0 && host_ready == 1)
        {
            if (host_valid == 1)
            {

                if (!img_buffer_full)
                {
                    *img_ptr = (unsigned char)bus; //! temporary
                    // std::cout << "segment received!" << std::endl;

                    if (img_ptr == &img_buffer[35][32])
                    {
                        img_ptr = (unsigned char *)img_buffer;
                        img_buffer_full = 1;
                        std::cout << "buffer full" << std::endl;
                        device_state[0] = 0;
                        device_state[1] = 0;
                        return device_state;
                    }
                    else
                    {
                        img_ptr++;
                        device_state[0] = 1;
                        device_state[1] = 0;
                        return device_state;
                    }
                }
                else // device_ready set 0
                {

                    std::cout << "start computing" << std::endl;
                    forward_pass_test(img_buffer); //! computation shouldn't be finished in 1 cycle
                    finished_computing = 1;

                    img_buffer_full = 0;

                    device_state[0] = 0;
                    device_state[1] = 1;
                    return device_state;
                }
            }
            else //! host_valid == 0, host_ready == 0 ?
            {
                device_state[0] = 1;
                device_state[1] = 0;
                return device_state;
            }
        }
    }
}