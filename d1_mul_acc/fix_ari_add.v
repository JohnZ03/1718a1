module fix_ari_add
#(parameter DATA = 16)
(data_in1,data_in2,data_out);


parameter MIN=16'sb1000_0000_0000_0000;
parameter MAX=16'sb0111_1111_1111_1111;

input  signed [DATA-1:0] data_in1;
input  signed [DATA-1:0] data_in2;
output signed [DATA-1:0] data_out;

wire signed [DATA:0] sum_mid;
reg  signed [DATA-1:0] sum;

assign sum_mid = data_in1 + data_in2;
always@(*) begin
    if(sum_mid < MIN) begin
      sum = (MIN);
 	end
    else if(sum_mid > MAX)
      sum = (MAX);
    else begin
      sum = sum_mid[DATA-1:0];
	end
end
assign data_out = sum;
//assign data_out = data_in1 + data_in2;

endmodule 
