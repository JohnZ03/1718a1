#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include <cstring>
#include <immintrin.h>

#include <cstring>

using namespace std;
const int filter_size=7;
const double eta=0.01;
__m256d v_eta = _mm256_set1_pd (0.01f);
const int batch_size=200;
// __m256d v_zeros = _mm256_setzero_pd();

unsigned char data_train[60000][784];
unsigned char data_test[10000][784];
unsigned char label_train[60000];
unsigned char label_test[10000];

// double conv_w[5][7][7];
double conv_w[5][8][8]={};
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
// double dense_w2[120][10];
double dense_w2[120][12];
// double dense_b2[10];
double dense_b2[12];
double dense_sum2[10];
double dense_softmax[10];

// double dw2[120][10];
double dw2[120][12]={};
// double db2[10];
double db2[12]={};

double dw1[980][120];
double db1[120];

double dw_max[5][28][28];
// double dw_conv[5][7][7];
double dw_conv[5][8][8]={};
double db_conv[5][28][28];


/* ************************************************************ */
/* Helper functions */
double sigmoid(double x) {
        if (x>500) x=500;
        if (x<-500) x=-500;
        return 1/(1+exp(-x));
}
double d_sigmoid(double x) {
        double sig = sigmoid(x);
        return sig*(1-sig);
}
double softmax_den(double *x, int len) {
        double val =0;
        for (int i=0; i<len; i++) {
                val += exp(x[i]);
        }
        return val;
}

void initialise_weights() {
        for (int i=0; i<5; i++) {
                for (int j=0; j<28; j++) {
                        for (int k=0; k<28; k++) {
                                if (j<7 && k<7) {
                                        conv_w[i][j][k] = 2*double(rand())/RAND_MAX-1;
                                }
                                conv_b[i][j][k] = 2*double(rand())/RAND_MAX-1;
                        }
                }
        }

        for (int i=0; i<980; i++) {
                for (int j=0; j<120; j++) {
                        dense_w[i][j] = 2*double(rand()) / RAND_MAX-1;
                }
        }
        for (int i=0; i<120; i++) {
                dense_b[i] = 2*double(rand()) / RAND_MAX-1;
        }

        for (int i=0; i<120; i++) {
                for (int j=0; j<10; j++) {
                        dense_w2[i][j] = 2*double(rand())/RAND_MAX-1;
                }
        }
        for (int i=0; i<10; i++) {
                dense_b2[i] = 2*double(rand())/RAND_MAX-1;
        }
}
/* ************************************************************ */

/* ************************************************************ */
/* Forward Pass */
void forward_pass(unsigned char img[][32]) {
// img[5~32] [2~29] of img [35][32] are valid! Refer to give_img()

        // Convolution Operation + Sigmoid Activation
        // TODO: 5 Lingfeng
        // for (int filter_dim=0; filter_dim<5; filter_dim++) {
        //         for (int i=0; i<28; i++) {
        //                 for (int j=0; j<28; j++) {
        //                         max_pooling[filter_dim][i][j] = 0;
        //                         conv_layer[filter_dim][i][j] = 0;
        //                         sig_layer[filter_dim][i][j] = 0;
        //                         for (int k=0; k<filter_size; k++) {
        //                                 for (int l=0; l<filter_size; l++) {
        //                                         conv_layer[filter_dim][i][j] += img[i+k+1][j+l-2]*conv_w[filter_dim][k][l];
        //                                 }
        //                         }
        //                         sig_layer[filter_dim][i][j] = sigmoid(conv_layer[filter_dim][i][j] + conv_b[filter_dim][i][j]);
        //                 }
        //         }
        // }

        memset(&max_pooling, 0, 3920);
        // // for(int filter_dim = 0; filter_dim < 5; filter_dim++) {
        // //         for(int i = 0; i < 28; i++) {
        // //                 for(int j = 0; j < 28; j += 32) {
        // //                         _mm256_store_pd(&max_pooling[filter_dim][i][j], v_zeros);
        // //                 }
        // //         }
        // // }

        double img_double[35][32];
        for(int i = 0; i < 35; i++)
                for(int j = 0; j < 32; j++)
                        img_double[i][j] = (double) img [i][j];
        for(int filter_dim = 0; filter_dim < 5; filter_dim++) {
                for(int i = 0; i < 28; i++) {
                        double conv_sum[28][4];
                        for(int j = 0; j < 28; j++) {
                                __m256d v_conv_sum = _mm256_setzero_pd();
                                for(int k = 0; k < filter_size; k++) {
                                        for(int l = 0; l < filter_size; l += 4) {
                                                __m256d v_conv_w = _mm256_load_pd(&conv_w[filter_dim][k][l]);
                                                __m256d v_img = _mm256_load_pd(&img_double[i+k+1][j+l-2]);
                                                v_conv_sum = _mm256_fmadd_pd(v_conv_w, v_img, v_conv_sum);
                                        }
                                }
                                _mm256_store_pd(&conv_sum[j][0], v_conv_sum);
                                for (int k = 1; k < 4; k++)
                                        conv_sum[j][0] += conv_sum[j][k];
                                
                        }
                        double sum[28];
                        for (int k = 0; k < 28; k++)
                                sum[k] = conv_sum[k][0];
                        for (int j = 0; j < 28; j += 4) {
                                __m256d v_sum = _mm256_load_pd(&sum[j]);
                                _mm256_store_pd(&conv_layer[filter_dim][i][j], v_sum);
                        }
                        
                        
                }
        }
        for(int filter_dim = 0; filter_dim < 5; filter_dim++) {
                for(int i = 0; i < 28; i++) {
                        for(int j = 0; j < 28; j += 4) {
                                __m256d v_conv_b = _mm256_load_pd(&conv_b[filter_dim][i][j]);
                                __m256d v_conv_layer = _mm256_load_pd(&conv_layer[filter_dim][i][j]);
                                __m256d v_sig_layer = _mm256_add_pd(v_conv_b, v_conv_layer);
                                _mm256_store_pd(&sig_layer[filter_dim][i][j], v_sig_layer);
                                for(int k = 0; k < 4; k++) {
                                        sig_layer[filter_dim][i][j+k] = sigmoid(sig_layer[filter_dim][i][j+k]);
                                }
                                
                        }
                }
        }

        // MAX Pooling (max_pooling, max_layer)
        // TODO: 5 Haotian
        double cur_max =0;
        int max_i=0, max_j=0;
        for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<28; i+=2) {
                        for (int j=0; j<28; j+=2) {
                                max_i=i;
                                max_j=j;
                                cur_max=sig_layer[filter_dim][i][j];
                                for (int k=0; k<2; k++) {
                                        for (int l=0; l<2; l++) {
                                                if (sig_layer[filter_dim][i+k][j+l] > cur_max) {
                                                        max_i = i+k;
                                                        max_j = j+l;
                                                        cur_max = sig_layer[filter_dim][max_i][max_j];
                                                }
                                        }
                                }
                                max_pooling[filter_dim][max_i][max_j] = 1;
                                max_layer[filter_dim][i/2][j/2] = cur_max;
                        }
                }
        }

        // TODO: 3 Lingfeng
        // int k=0;
        // for (int filter_dim=0;filter_dim<5;filter_dim++) {
        //         for (int i=0;i<14;i++) {
        //                 for (int j=0;j<14;j++) {
        //                         dense_input[k] = max_layer[filter_dim][i][j];
        //                         k++;
        //                 }
        //         }
        // }
        double *k = (double *) max_layer;
        for(int i = 0; i < 246; i += 4) {
                __m256d v_dense_input = _mm256_load_pd(k);
                _mm256_store_pd(&dense_input[i], v_dense_input);
                k += 4;
        }

        // Dense Layer
        // TODO: 3 Haotian
        for (int i=0; i<120; i++) {
                dense_sum[i] = 0;
                dense_sigmoid[i] = 0;
                for (int j=0; j<980; j++) {
                        dense_sum[i] += dense_w[j][i] * dense_input[j];
                }
                dense_sum[i] += dense_b[i];
                dense_sigmoid[i] = sigmoid(dense_sum[i]);
        }

        // Dense Layer 2
        // TODO: 2 Haotian
        for (int i=0; i<10; i++) {
                dense_sum2[i]=0;
                for (int j=0; j<120; j++) {
                        dense_sum2[i] += dense_w2[j][i] * dense_sigmoid[j];
                }
                dense_sum2[i] += dense_b2[i];
        }

        // Softmax Output
        // TODO: 1 Haotian
        double den = softmax_den(dense_sum2, 10);
        for (int i=0; i<10; i++) {
                dense_softmax[i] = exp(dense_sum2[i])/den;
        }
}

void update_weights() {
		for (int j=0; j<12; j++) {
                        // Only 10 operations, round to 12
                        dense_b2[j] -= eta*db2[j];
                }	
		for(int i = 0; i < 120; i += 4) {
			// Load 256-bits (composed of 4 packed double-precision (64-bit) floating-point elements) 
			// from memory into dst. mem_addr must be aligned on a 32-byte boundary or a general-protection 
			// exception may be generated.
			
			// If crashed, use __mm256_loadu_pd the unaligned counterparts!
			__m256d v_db1 = _mm256_load_pd (&db1[i]);
			__m256d v_dense_b = _mm256_load_pd (&dense_b[i]);
			
			// Multiply packed double-precision (64-bit) floating-point elements in a and b,
			// add the negated intermediate result to packed elements in c, and store the results in dst.
			v_dense_b = _mm256_fnmadd_pd (v_db1, v_eta, v_dense_b);
			// Store 256-bits (composed of 4 packed double-precision (64-bit) floating-point elements) 
			// from a into memory. mem_addr must be aligned on a 32-byte boundary or a general-protection 
			// exception may be generated.
			_mm256_store_pd(&dense_b[i], v_dense_b);
		}
		for(int i = 0; i < 120; i++) {
			// Expand j to 12
			for (int j = 0; j < 12; j += 4) {
				__m256d v_dense_w2 = _mm256_load_pd (&dense_w2[i][j]);
				__m256d v_dw2 = _mm256_load_pd (&dw2[i][j]);
				v_dense_w2 = _mm256_fnmadd_pd (v_dw2, v_eta, v_dense_w2);
				_mm256_store_pd(&dense_w2[i][j], v_dense_w2);
			}
		}
		for(int i = 0; i < 120; i += 4) {
			for (int k = 0; k < 980; k++) {
				__m256d v_dense_w = _mm256_load_pd (&dense_w[k][i]);
				__m256d v_dw1 = _mm256_load_pd (&dw1[k][i]);
				v_dense_w = _mm256_fnmadd_pd (v_dw1, v_eta, v_dense_w);
				_mm256_store_pd(&dense_w[k][i], v_dense_w);
                        }
		}
			
		
        // for (int i=0; i<120; i++) {
                // dense_b[i] -= eta*db1[i];
                // for (int j=0; j<10; j++) {
                        // dense_b2[j] -= eta*db2[j];
                        // dense_w2[i][j] -= eta*dw2[i][j];
                // }
                // for (int k=0; k<980; k++) {
                        // dense_w[k][i] -= eta*dw1[k][i];
                // }
        // }

        // TODO: 4 Lingfeng
        // for (int i=0; i<5; i++) {
        //         for (int k=0; k<7; k++) {
        //                 for (int j=0; j<7; j++) {
        //                         conv_w[i][k][j] -= eta*dw_conv[i][k][j];
        //                 }
        //         }
        //         for (int l=0; l<28; l++) {
        //                 for (int m=0; m<28; m++) {
        //                         conv_b[i][l][m] -= eta*db_conv[i][l][m];
        //                 }
        //         }
        // }
        for(int i = 0; i < 5; i++) {
                for(int k = 0; k < 7; k++) {
                        for(int j = 0; j < 7; j += 4) {
                                __m256d v_dw_conv = _mm256_load_pd(&dw_conv[i][k][j]);
                                __m256d v_conv_w = _mm256_load_pd(&conv_w[i][k][j]);
                                v_conv_w = _mm256_fnmadd_pd (v_dw_conv, v_eta, v_conv_w);
                                _mm256_store_pd(&conv_w[i][k][j], v_conv_w);
                        }
                }
                for(int l = 0; l < 28; l++) {
                        for(int m = 0; m < 28; m += 4) {
                                __m256d v_db_conv = _mm256_load_pd(&db_conv[i][l][m]);
                                __m256d v_conv_b = _mm256_load_pd(&conv_b[i][l][m]);
                                v_conv_b = _mm256_fnmadd_pd (v_db_conv, v_eta, v_conv_b);
                                _mm256_store_pd(&conv_b[i][l][m], v_conv_b);
                        }
                }
        }
}
/* ************************************************************ */

/* ************************************************************ */
/* Backward Pass */
void backward_pass(double *y_hat, int *y, unsigned char img[][32]) {
        double delta4[10];
        // TODO: 1 Zhuojun
        for (int i=0; i<10; i++) {
                delta4[i] = y_hat[i] - y[i]; // Derivative of Softmax + Cross entropy
                db2[i] = delta4[i]; // Bias Changes
        }

        // Calculate Weight Changes for Dense Layer 2
        // TODO: 2 Zhuojun
        for (int i=0; i<120; i++) {
                for (int j=0; j<10; j++) {
                        dw2[i][j] = dense_sigmoid[i]*delta4[j];
                }
        }

        // Delta 3
        // TODO: 3 Zhuojun
        double delta3[120];
        for (int i=0; i<120; i++) {
                delta3[i] = 0;
                for (int j=0; j<10; j++) {
                        delta3[i] += dense_w2[i][j]*delta4[j];
                }
                delta3[i] *= d_sigmoid(dense_sum[i]);
        }
        // TODO: 1 Zhuojun
        for (int i=0; i<120; i++) db1[i] = delta3[i]; // Bias Weight change

        // Calculate Weight Changes for Dense Layer 1
        // TODO: 2 Zhuojun
        for (int i=0; i<980; i++) {
                for (int j=0; j<120; j++) {
                        dw1[i][j] = dense_input[i]*delta3[j];
                }
        }

        // Delta2 Guoxian
        // TODO: 3 Guoxian

	    /*	
        double delta2[980];
        for (int i=0; i<980; i++) {
                delta2[i] = 0;
                for (int j=0; j<120; j++) {
                        delta2[i] += dense_w[i][j]*delta3[j];
                }
                delta2[i] *= d_sigmoid(dense_input[i]);
        }
        
        */
        //simd---------------------------------------------
		double delta2[980];
        for (int i=0; i<980; i++) {
                //delta2[i] = 0;
			    __m256d v_delta2_i = _mm256_set1_pd(0.00f);
                for (int j=0; j<120; j+=4) {
			            __m256d v_dense_w = _mm256_load_pd (&dense_w[i][j]);
			            __m256d v_delta3  = _mm256_load_pd (&delta3[j]);
					    v_delta2_i = _mm256_fmadd_pd(v_dense_w,v_delta3,v_delta2_i);
                }
				__m256d s= _mm256_hadd_pd(v_delta2_i,v_delta2_i);
				delta2[i] = ((double*)&s)[0] + ((double*)&s)[2];
                //delta2[i] *= d_sigmoid(dense_input[i]);
        }

        double d_sigmoid_dense_input[980];

		for (int i=0; i<980; i++){
            d_sigmoid_dense_input[i] = d_sigmoid(dense_input[i]);
		}

		for (int i=0; i<980; i+=4){
            __m256d v_delta2 = _mm256_load_pd (&delta2[i]);
			__m256d v_d_sigmoid_dense_input = _mm256_load_pd(&d_sigmoid_dense_input[i]);
			v_delta2 = _mm256_fmadd_pd(v_d_sigmoid_dense_input, v_delta2,_mm256_set1_pd(0.00f));
			_mm256_store_pd(&delta2[i], v_delta2);
	    }
        //simd---------------------------------------------
        

        // Calc back-propagated max layer dw_max
        // TODO: 5 Zhuojun
        int k=0;
        for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<28; i+= 2) {
                        for (int j=0; j<28; j+= 2) {
                                for (int l=0; l<2; l++) {
                                        for (int m=0; m<2; m++) {
                                                if (max_pooling[filter_dim][i+l][j+m] == 1) dw_max[filter_dim][i][j] = delta2[k];
                                                else dw_max[filter_dim][i][j] = 0;
                                        }
                                }
                                k++;
                        }
                }
        }
        // Calc Conv Bias Changes
        // TODO: 2 Guoxian
		/*
       for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<28; i++) {
                        for (int j=0; j<28; j++) {
                                db_conv[filter_dim][i][j] = dw_max[filter_dim][i][j];
                        }
                }
        }
	   */
        for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<28; i++) {
                        for (int j=0; j<28; j++) {
								memcpy(db_conv[filter_dim][i],dw_max[filter_dim][i],28);
                        }
                }
        }
 

        // Set Conv Layer Weight changes to 0
        // TODO: 2 Guoxian
        for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<5; i++) {
                        for (int j=0; j<5; j++) {
                                dw_conv[filter_dim][i][j] = 0;
                        }
                }
        }


	// 	for (int filter_dim=0; filter_dim<5; filter_dim++) {
        //         for (int i=0; i<5; i++) {
	// 	     		memset(dw_conv[filter_dim][i],0,5);
	// 			}
        // }


        // Calculate Weight Changes for Conv Layer
        // TODO: 5 Guoxian


	    
        for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<26; i++) {
                        for (int j=0; j<26; j++) {
                                double cur_val = dw_max[filter_dim][i][j];
                                for (int k=0; k<5; k++) {
                                        for (int l=0; l<5; l++) {
                                                dw_conv[filter_dim][k][l] += img[i+k+1][j+l-2] * cur_val;
                                        }
                                }
                        }
                }
        }
	   	
		/*
       //simd ------------------------------------------------------- 
		for (int filter_dim=0; filter_dim<5; filter_dim++) {
                for (int i=0; i<26; i++) {
                        for (int j=0; j<26; j++) {
                                double cur_val[4];
								for(int m=0;m<4;m++){
									cur_val[m] = dw_max[filter_dim][i][j];
								}
                                for (int k=0; k<5; k++) {
									
									double img_temp[4];
									for(int l=0;l<4;l++){
										img_temp[l] = img[i+k+1][j+l-2];
									}

									double dw_temp1[4];
		                            __m256d v_dw_conv  = _mm256_load_pd(&dw_conv[filter_dim][k][0]); 
									__m256d v_img_temp = _mm256_load_pd(&img_temp[0]);
									__m256d v_cur_val  = _mm256_load_pd(&cur_val[0]);
                                    v_dw_conv = _mm256_fmadd_pd(v_img_temp, v_cur_val,v_dw_conv);
									_mm256_store_pd(&dw_temp1[0],v_dw_conv);
								    _mm256_store_pd(&dw_conv[filter_dim][k][0],v_dw_conv);
									
                                    //dw_conv[filter_dim][k][0] += img_temp[0] * cur_val[0];  //l=0
                                    //dw_conv[filter_dim][k][1] += img_temp[1] * cur_val[1];  //l=1
                                    //dw_conv[filter_dim][k][2] += img_temp[2] * cur_val[2];  //l=2
                                    //dw_conv[filter_dim][k][3] += img_temp[3] * cur_val[3];  //l=3
									
                                    dw_conv[filter_dim][k][4] += img[i+k+1][j+2] * cur_val[0];  //l=4
									
                                }

                        }
                }
        }
       //simd -------------------------------------------------------
       */
}
/* ************************************************************ */


void read_train_data() {
        ifstream csvread;
        csvread.open("/cad2/ece1718s/mnist_train.csv", ios::in);
        if(csvread) {
                string s;
                int data_pt = 0;
                while(getline(csvread, s)) {
                        stringstream ss(s);
                        int pxl = 0;
                        while( ss.good() ) {
                                string substr;
                                getline(ss, substr,',');
                                if (pxl == 0) {
                                        label_train[data_pt] = stoi(substr);
                                } else {
                                        data_train[data_pt][pxl-1] = stoi(substr);
                                }
                                pxl++;
                        }
                        data_pt++;
                }
                csvread.close();
        }
        else{
                cerr << "Unable to read train data!" << endl;
        exit (EXIT_FAILURE);
        }
}
void read_test_data() {
        ifstream csvread;
        csvread.open("/cad2/ece1718s/mnist_test.csv", ios::in);
        if(csvread) {
                string s;
                int data_pt = 0;
                while(getline(csvread, s)) {
                        stringstream ss(s);
                        int pxl = 0;
                        while( ss.good() ) {
                                string substr;
                                getline(ss, substr,',');
                                if (pxl == 0) {
                                        label_test[data_pt] = stoi(substr);
                                } else {
                                        data_test[data_pt][pxl-1] = stoi(substr);
                                }
                                pxl++;
                        }
                        data_pt++;
                }
                csvread.close();
        }
        else{
                cerr << "Unable to read test data!" << endl;
        exit (EXIT_FAILURE);
        }
}

void give_img(unsigned char* vec , unsigned char img[][32]) {
        int k=0;
        for (int i=0; i<35; i++) {
                for (int j=0; j<32; j++) {
                        if (i<5 || j<2 || i>32 || j>29) {
                                img[i][j] = 0;
                        } else {
                                img[i][j] = vec[k++];
                        }
                }
        }
}

void give_y(int y, int *vector_y) {
        for (int i=0; i<10; i++) vector_y[i] =0;
        vector_y[y]=1;
}
int give_prediction() {
        double max_val = dense_softmax[0];
        int max_pos = 0;
        for (int i=1; i<10; i++) {
                if (dense_softmax[i] > max_val) {
                        max_val = dense_softmax[i];
                        max_pos = i;
                }
        }

        return max_pos;
}

int main() {
        read_test_data();
        read_train_data();
        initialise_weights();

        int epoch = 500;
        int num = 0;
        cout << "Start Training." << endl;
        for (int i=0; i<epoch; i++) {
                cout << "Epoch " << i << " done." << endl;
                for (int j=0; j<batch_size; j++) {
                        num = rand()%60000;
                        unsigned char img[35][32];
                        int vector_y[10];
                        give_y(label_train[num], vector_y);
                        give_img(data_train[num], img);
                        forward_pass(img);
                        backward_pass(dense_softmax, vector_y, img);
                        update_weights();
                }
        }

        int val_len = 600;
        int cor=0;
        int confusion_mat[10][10];
        for (int i=0; i<10; i++){
                for (int j=0; j<10; j++) confusion_mat[i][j] = 0;
        }

        cout << "Start Testing." << endl;
        for (int i=0; i<val_len; i++) {
                unsigned char img[35][32];
                give_img(data_test[i], img);
                forward_pass(img);
                int pre = give_prediction();
                confusion_mat[label_test[i]][pre]++;
                if (pre == label_test[i]) cor++;
        }
        float accu = double(cor)/val_len;
        cout << "Accuracy: " << accu << endl;

        cout << "   0 1 2 3 4 5 6 7 8 9" << endl;
        for (int i=0; i<10; i++){
                cout << i << ": ";
                for (int j=0; j<10; j++) {
                        cout << confusion_mat[i][j] << " ";
                }
                cout << endl;
        }

        return 0;
}
