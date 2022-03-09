module d1_mul_acc (
    input clk,
    input rst_n,
    input [15:0] dense_sum_i,
    input [15:0] dense_w_i,
    input [15:0] dense_input_i,
    output [15:0] dense_output_o
);

// Assign input output sugnals to logics
logic [15:0] dense_sum;     // Sum to be accumulated
logic [15:0] dense_w;       // Weights
logic [15:0] dense_input;   // Input from activaction functions
logic [15:0] dense_output;  // ! Last output needs to be added with dense_b

assign dense_sum = dense_sum_i;
assign dense_w = dense_w_i;
assign dense_input = dense_input_i;
assign dense_output_o = dense_output;

logic [15:0] mul_out;       // Conncects to the output of multiplier

logic [15:0] buffer_reg;    // Buffer register

// Instantiate modules
fix_ari_mul mul0 (.data_in1(dense_input_i), .data_in2(dense_w_i), .data_out_round(mul_out));
fix_ari_add add0 (.data_in1(dense_sum_i), .data_in2(buffer_reg), .data_out_round(dense_output));


// 2 cycles to execute: 1 mul, 1 add
always_ff @( posedge clk or negedge rst_n) begin
    if (rst_n) begin
        buffer_reg <= 16'b0;
    end else begin
        buffer_reg <= mul_out;
    end
end


endmodule