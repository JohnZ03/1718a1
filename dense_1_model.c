// Take pointers input, return pointer to tb
long int* dense_1(const long int* dense_w, const long int* dense_input, const long int* dense_b, long int* dense_sum[120])
{   
    // Dense Layer 1
    for (int i = 0; i < 120; i++)
    {
            dense_sum[i] = 0;
            for (int j = 0; j < 980; j++)
            {
                    dense_sum[i] += dense_w[j*120 + i] * dense_input[j];
            }
            dense_sum[i] += dense_b[i];
    }
}
