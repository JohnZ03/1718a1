#include <iostream>
#include <fstream>
#include <sstream>

#include "device.cpp"

// test related
unsigned char data_test[10000][784];
unsigned char label_test[10000];
int val_len = 600;
int result_counter = 0;
double vector[600][10] = {};
unsigned char imgs_to_give[600][35][32];

// Initial state of the device
// device_ready = 1;
// device_valid = 0;
int device_state[2] = {1, 0};

// 64-bit bus;
double bus;

void read_test_data();
void give_img(unsigned char *vec, unsigned char img[35][32]);
int give_prediction_test(double *vector);

int main()
{
    // read weights
    std::cout << "reading data..." << std::endl;
    std::ifstream fin("weights.txt");
    if (!fin)
    {
        std::cerr << "Unable to open file!";
        exit(-1);
    }
    // double weights[120 + 10 + 120 * 10 + 120 * 980 + 5 * 7 * 7 + 5 * 28 * 28];
    double weights[123095];
    for (int i = 0; i < 123095; i++)
        fin >> weights[i];
    // fixed weight; // quantized weights, 16 bits each

    // read data
    read_test_data();
    for (int i = 0; i < val_len; i++)
    {
        give_img(data_test[i], imgs_to_give[i]);
    }

    //-----------------------------------------------------------------------------
    // also a representation for 64 bits
    double *ptr_weights = weights;

    // send weights
    std::cout << "sending weights..." << std::endl;
    while (ptr_weights != &weights[123095])
    {
        bus = *ptr_weights; // load weights into bus
        ptr_weights++;
        // forward(Mode_select, host_valid, host_ready);
        int *res = forward(1, 1, 0, bus);

        //------not necessary; explicitly show interfaces only; applicable to all below
        device_state[0] = *res;
        res++;
        device_state[1] = *res;
        //----------------------------------------------------
    }
    // image transfer
    bool imgs_finished = 0;
    unsigned char *img_ptr;
    int digit = 0;
    std::cout << "start sending imgs..." << std::endl;
    while (!imgs_finished)
    {
        // device_valid == 1, device is sending data
        if (device_state[1] == 1)
        {
            int *res = forward(2, 0, 1, bus);
            vector[result_counter][digit] = bus; // read result
            digit++;
            if (digit == 10)
            {
                result_counter++;
                std::cout << "image " << result_counter << " finished" << std::endl;

                digit = 0;
            }
            std::cout << "image " << result_counter << " vector digit " << digit << " finished... " << std::endl;

            device_state[0] = *res;
            res++;
            device_state[1] = *res;
        }
        // device_valid == 0, device_ready == 1
        else if (device_state[0] == 1)
        {
            std::cout << "segment sent!" << std::endl;
            double imgs_in_bus = (double)*img_ptr;
            bus = imgs_in_bus; //! load 64 bits into bus; temporary
            img_ptr++;
            if (img_ptr == &imgs_to_give[600][35][32])
                imgs_finished = 1; // finish sending everything

            int *res = forward(2, 1, 0, bus);

            device_state[0] = *res;
            res++;
            device_state[1] = *res;
        }
        else // device_valid == 0, device_ready == 0;
        {
            std::cout << "host is waiting.." << std::endl;
            // bus sends nothing
            int *res = forward(2, 0, 1, bus);
            device_state[0] = *res;
            res++;
            device_state[1] = *res;
        }
    }
    // nothing to send, waits remaining results
    std::cout << "waiting remaining results..." << std::endl;
    while (result_counter < val_len)
    {
        int *res = forward(2, 0, 1, bus);
        device_state[0] = *res;
        res++;
        device_state[1] = *res;
        if (device_state[1] == 1)
        {
            vector[result_counter][digit] = int(bus); // read result
            if (digit == 10)
            {
                result_counter++;
                digit = 0;
            }
        }
    }

    //--------------------------------------
    std::cout << "Start Testing." << std::endl;
    int confusion_mat[10][10];
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            confusion_mat[i][j] = 0;
    }

    int cor = 0;
    for (int i = 0; i < val_len; i++)
    {
        int pre = give_prediction_test(vector[i]);
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

void give_img(unsigned char *vec, unsigned char img[35][32])
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

int give_prediction_test(double *vector)
{
    double max_val = vector[0];
    int max_pos = 0;
    for (int i = 1; i < 10; i++)
    {
        if (vector[i] > max_val)
        {
            max_val = vector[i];
            max_pos = i;
        }
    }

    return max_pos;
}