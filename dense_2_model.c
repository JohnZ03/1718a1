long int* dense_sum2[10];

// Take pointers input, return pointer to tb
long int* dense_1(long int* dense_w2, long int* dense2_input, long int* dense_b2)
{   
    // Dense Layer 2
    for (int i = 0; i < 10; i++)
    {
        dense_sum2[i] = 0;
        for (int j = 0; j < 120; j++)
        {
                dense_sum2[i] += dense_w2[j*10 + i] * dense2_input[j];
        }
        dense_sum2[i] += dense_b2[i];
    }

    return dense_sum2;
}
