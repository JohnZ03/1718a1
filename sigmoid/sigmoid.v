module sigmoid(
    input        clk,
    input        rst_n,
    input        ena,
    input signed [15:0] sigmoid_in,
    output       valid,
    output reg signed [15:0] sigmoid_out
    );

reg[2:0] range1;

reg signed[15:0] sigmoid_in_dly1;
reg signed[15:0] sigmoid_in_dly2;
reg signed[15:0] sigmoid_in_dly3;
reg signed[15:0] sigmoid_in_dly4;
reg signed[15:0] sigmoid_in_dly5;

reg signed[15:0] quadratic_term;
reg signed[15:0] linear_term;
reg signed[15:0] constant_term;

wire signed[30:0] mul_out0_1;
wire signed[15:0] mul_out0_1_round;
wire signed[30:0] mul_out0_2;
wire signed[15:0] mul_out0_2_round;
wire signed[30:0] mul_out1_1;
wire signed[15:0] mul_out1_1_round;

reg  signed[15:0] mul_out0_2_round_dly1;
reg  signed[15:0] mul_out0_2_round_dly2;
reg  signed[15:0] mul_out0_2_round_dly3;

reg [8:0] ena_dly;

reg signed[15:0] constant_term_dly1;
reg signed[15:0] constant_term_dly2;
reg signed[15:0] constant_term_dly3;
reg signed[15:0] constant_term_dly4;
reg signed[15:0] constant_term_dly5;
reg signed[15:0] constant_term_dly6;

//sigmoid_in delay1,2
always @ (posedge clk or negedge rst_n)
    if (!rst_n) begin
        sigmoid_in_dly1 <= 16'sd0;
        sigmoid_in_dly2 <= 16'sd0;
        sigmoid_in_dly3 <= 16'sd0;
        sigmoid_in_dly4 <= 16'sd0;
        sigmoid_in_dly5 <= 16'sd0;
    end
    else begin
        sigmoid_in_dly1 <= sigmoid_in;
      always @ (posedge clk or negedge rst_n)
    if (!rst_n) begin
        sigmoid_in_dly2 <= sigmoid_in_dly1;
        sigmoid_in_dly3 <= sigmoid_in_dly2;
        sigmoid_in_dly4 <= sigmoid_in_dly3;
        sigmoid_in_dly5 <= sigmoid_in_dly4;
    end

//mul_out0_2 delay1,2
always @ (posedge clk or negedge rst_n)
    if (!rst_n) begin
        mul_out0_2_round_dly1 <= 31'sd0;
        mul_out0_2_round_dly2 <= 31'sd0;
        mul_out0_2_round_dly3 <= 31'sd0;
    end
    else begin
        mul_out0_2_round_dly1 <= mul_out0_2_round;
        mul_out0_2_round_dly2 <= mul_out0_2_round_dly1;
        mul_out0_2_round_dly3 <= mul_out0_2_round_dly2;
    end


//first comparator
always @ (posedge clk or negedge rst_n)
    if (!rst_n)
        range1 <= 3'h0;
    else begin
        if(sigmoid_in > 16'sb0000010100000000) // >5
            range1 <= 3'h1;
        else if(sigmoid_in > 16'sd0000000100000000) // 1 to 5
            range1 <= 3'h2;
        else if(sigmoid_in > 16'sd1111111100000000) // -1 to 1
            range1 <= 3'h3;
        else if(sigmoid_in > 16'sd1111101100000000) // -5 to -1
            range1 <= 3'h4;
        else                                        // <-5
            range1 <= 3'h5;
    end

//second comparator
always @ (posedge clk or negedge rst_n)
    if (!rst_n) begin
        quadratic_term <= 16'sd0;
        linear_term    <= 16'sd0;
        constant_term  <= 16'sd0;
    end
    else begin
        case(range1)
            3'h1: 
                //5      to 5.3890  1280 to 1380 -> 16'd254  
                //5.3890 to 6       1380 to 1536 -> 16'h255
                //6      to inf     >1536        -> 16'h256
                quadratic_term <= 16'sd0;
                linear_term    <= 16'sd0;
                if(sigmoid_in_dly1 < 16'sd1380) 
                    constant_term  <= 16'sd254;
                else if(sigmoid_in_dly1 < 16'sd1380)
                    constant_term  <= 16'sd255;
                else
                    constant_term  <= 16'sd256;
            3'h2:
                if(sigmoid_in_dly1 < 16'sd512) begin
                    quadratic_term <= 16'sb1111101000000110;
                    linear_term    <= 16'sd9490; 
                    constant_term  <= 16'sd125; 
                end

                else if(sigmoid_in_dly1 < 16'sd768) begin
                    quadratic_term <= 16'sb1111110000110000;
                    linear_term    <= 16'sd7216; 
                    constant_term  <= 16'sd143; 
                end

                else if(sigmoid_in_dly1 < 16'sd1024) begin
                    quadratic_term <= 16'sb1111111111111000; 
                    linear_term    <= 16'sd4060; 
                    constant_term  <= 16'sd180; 
                end

                else begin
                    quadratic_term <= 16'sb1111111101001111;
                    linear_term    <= 16'sd1956; 
                    constant_term  <= 16'sd212; 
                end

            3'h3:
                    quadratic_term <= 16'sb0;
                    linear_term    <= 16'sd7809; 
                    constant_term  <= 16'sd128; 

            3'h4:
                if(sigmoid_in_dly1 < 16'sd512) begin
                    quadratic_term <= 16'sd1530;
                    linear_term    <= 16'sd9490; 
                    constant_term  <= 16'sd131; 
                end

                else if(sigmoid_in_dly1 < 16'sd768) begin
                    quadratic_term <= 16'sd976;
                    linear_term    <= 16'sd7216; 
                    constant_term  <= 16'sd113; 
                end

                else if(sigmoid_in_dly1 < 16'sd1024) begin
                    quadratic_term <= 16'sd442; 
                    linear_term    <= 16'sd4060; 
                    constant_term  <= 16'sd76; 
                end

                else begin
                    quadratic_term <= 16'sd117;
                    linear_term    <= 16'sd1956; 
                    constant_term  <= 16'sd44; 
                end
 
            3'h5: 
                //5      to 5.3890  1280 to 1380 -> 16'd254  
                //5.3890 to 6       1380 to 1536 -> 16'h255
                //6      to inf     >1536        -> 16'h256
                quadratic_term <= 16'sd0;
                linear_term    <= 16'sd0;
                if(sigmoid_in_dly1 < 16'sd1380) 
                    constant_term  <= 16'sd254;
                else if(sigmoid_in_dly1 < 16'sd1380)
                    constant_term  <= 16'sd255;
                else
                    constant_term  <= 16'sd256;
            default:
                quadratic_term <= 16'sd0;
                linear_term    <= 16'sd0;
                constant_term  <= 16'sd0;

        endcase
    end

always @ (posedge clk or negedge rst_n)
    if (!rst_n) begin
       constant_term_dly1 <= 16'sd0;
       constant_term_dly2 <= 16'sd0;
       constant_term_dly3 <= 16'sd0;
       constant_term_dly4 <= 16'sd0;
       constant_term_dly5 <= 16'sd0;
       constant_term_dly6 <= 16'sd0;
    end
    else begin
       constant_term_dly1 <= constant_term;
       constant_term_dly2 <= constant_term_dly1;
       constant_term_dly3 <= constant_term_dly2;
       constant_term_dly4 <= constant_term_dly3;
       constant_term_dly5 <= constant_term_dly4;
       constant_term_dly6 <= constant_term_dly5;
    end

// note: fix point multiply <1,7,8> * <1,0,15>
// f(x) = a * x * x + b * x + c

// a * x <1,7,8> * <1,0,15> = <1,0,15>
fix_ari_mul mul0_1(
    .clk  (clk),
    .rst_n(rst_n),
    .data_in1(quadratic_term),
    .data_in2(sigmoid_in_dly2),
    .data_out(mul_out0_1)
    );

// b * x <1,7,8> * <1,0,15> = <1,7,8>
fix_ari_mul mul0_2(
    .clk  (clk),
    .rst_n(rst_n),
    .data_in1(linear_term),
    .data_in2(sigmoid_in_dly2),
    .data_out(mul_out0_2)
    );

// (a * x) * x <1,7,8> * <1,0,15> = <1,7,8>
fix_ari_mul mul1_1(
    .clk  (clk),
    .rst_n(rst_n),
    .data_in1(mul_out0_1_round),
    .data_in2(sigmoid_in_dly5),
    .data_out(mul_out1_1)
    );

//round multply results

assign mul_out0_1_round = {mul_out0_1[30],mul_out0_1[21:8]};
assign mul_out1_1_round = mul_out1_1[30:15];
assign mul_out0_2_round = mul_out0_2[30:15];

always @ (posedge clk or negedge rst_n)
    if (!rst_n) begin
        sigmoid_out <= 16'sd0;
    else
        sigmoid_out <= mul_out_1_1_round + mul_out_0_2_round_dly3 + constant_term_dly6;

//It takes 9 cycles for data to go through sigmoid
//ena delay 
always @ (posedge clk or negedge rst_n)
    if (!rst_n) 
        ena_dly <= 9'h0;
    else
        ena_dly <= {ena_dly[7:0],ena};
assign valid = ena_dly[8];


