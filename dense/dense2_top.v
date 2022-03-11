module dense2_top(
    input                clk,
    input                rst_n,
    input                ena,
    input                frame_start_in,
    input                frame_end_in,
    input  signed [15:0] dense_sigmoid,
    output wire          valid,
    output wire  [159:0] dense_sum2_out
);

wire [9:0] valid_10;
assign valid=&valid_10;

wire [7:0]    rom0_addr;
wire [159:0]  rom0_data;

genvar i;
generate 
    for(i=0;i<10;i=i+1) begin:gen
        dense_multiply_accumulator mul_acc_inst(
            .clk      (clk), 
            .rst_n    (rst_n),   
            .ena      (ena),
            .frame_start_in   (frame_start_in),    
            .frame_end_in     (frame_end_in),
            .dense_input      (dense_sigmoid ),   
            .valid            (valid_10[i]),
            .dense_sum_out    (dense_sum2_out[i*16+15:i*16]),
            .rom0_addr        (rom0_addr),
            .rom0_data        (rom0_data[i*16+15:i*16])
        );
    end
endgenerate

single_port_rom  
#(
    .DATA_WIDTH (160),
    .ADDR_WIDTH (8)
    )
rom_inst
(
    .clk  (clk),
    .ena  (ena),
    .addr (rom0_addr),
    .q    (rom0_data)
    );
endmodule


