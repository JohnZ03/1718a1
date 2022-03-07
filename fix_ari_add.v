module fix_ari_add(data_in1,data_in2,data_out);

parameter DATA=29;
parameter SIGN=1;
parameter INTE=12;
parameter POIN=16;
parameter MIN=29'sb10000000000000000000000000001;
parameter MAX=29'sb01111111111111111111111111111;

input signed [DATA-1:0] data_in1;
input signed [DATA-1:0] data_in2;
output signed [DATA-1:0] data_out;

wire signed [DATA:0] sum;

assign sum = data_in1 + data_in2;

always@(*)
if(sum < MIN)
  sum = MIN;
else if(sum > MAX)
  sum = MAX;

assign data_out = sum;

endmodule 
