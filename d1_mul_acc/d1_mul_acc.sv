module d1_mul_acc (
    input dense_sum_i,
    input dense_w_i,
    input dense_input_i,
    output dense_output_o
);

logic [15:0] dense_sum;
logic [15:0] dense_w;
logic [15:0] dense_input;
logic [15:0] dense_output;

assign dense_sum = dense_sum_i;
assign dense_w = dense_w_i;
assign dense_input = dense_input_i;
assign dense_output = dense_output_o;



endmodule