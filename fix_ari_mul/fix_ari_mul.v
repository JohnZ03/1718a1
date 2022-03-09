module fix_ari_mul(data_in1,data_in2,data_out,data_out_round,clk,rst_n);

parameter DATA=16;
parameter EX_SI=DATA-1;
parameter SIGN=1;
parameter INTE=7;
parameter POIN=8;

input [DATA-1:0] data_in1;
input [DATA-1:0] data_in2;
output [EX_SI*2:0] data_out;
output [DATA-1:0] data_out_round;
input clk;
input rst_n;

reg [EX_SI*2-1:0] data_in1_r,data_in2_r;

always@(posedge clk or negedge rst_n)begin
  if(!rst_n)begin
     data_in1_r <= 'b0;
     data_in2_r <= 'b0;
   end
   else begin
     data_in1_r <= {{EX_SI{data_in1[DATA-1]}},data_in1[DATA-2:0]};
     data_in2_r <= {{EX_SI{data_in1[DATA-1]}},data_in1[DATA-2:0]};
   end
 end

reg [EX_SI*2-1:0] shift_r [EX_SI*2-1:0];
integer i,j,k;
always@(posedge clk or negedge rst_n)
  if(!rst_n)
    for(i = 0; i < EX_SI*2; i=i+1)
      shift_r[i] <= 'b0;
    else
      for(k = 0; k < EX_SI*2; k=k+1)
        if(data_in2_r[k] == 1)
          shift_r[k] <= (data_in1_r << k);
        else
          shift_r[k] <= 'b0;

reg [EX_SI*2-1:0] sum;
always@(posedge clk or negedge rst_n)
  if(!rst_n)
    sum <= 'b0;
  else begin
    for(i = 0; i < EX_SI*2; i=i+1)
      sum <= sum[i];
  end
    
assign data_out[EX_SI*2-1:0] = sum;
assign data_out[EX_SI*2] = (data_in1[DATA-1] ^ data_in2[DATA-1]) ? 1 : 0;
assign data_out_round = {data_out[EX_SI*2],data_out[2*POIN+INTE-1:2*POIN],data_out[2*POIN-1:POIN]};
endmodule
