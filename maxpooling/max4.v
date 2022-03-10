module max4(
    input[15:0] i_0,
    input[15:0] i_1,
    input[15:0] i_2,
    input[15:0] i_3,
    output[15:0] o_0
	);

wire[15:0] max1 = (i_0 > i_1) ? i_0 : i_1;
wire[15:0] max2 = (i_2 > i_3) ? i_2 : i_3;
assign o_0 = (max1 > max2) ? max1 : max2;
endmodule
