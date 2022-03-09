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
     data_in2_r <= {{EX_SI{data_in2[DATA-1]}},data_in2[DATA-2:0]};
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
/*  else begin
    for(i = 0; i < EX_SI*2; i=i+1)
      sum <= sum + shift_r[i];
  end */
 else
   sum <= shift_r[0]+shift_r[1]+shift_r[2]+shift_r[3]+shift_r[4]+shift_r[5]+shift_r[6]+shift_r[7]+shift_r[8]+shift_r[9]+shift_r[10]+shift_r[11]+shift_r[12]+shift_r[13]+shift_r[14]+shift_r[15]+shift_r[16]+shift_r[17]+shift_r[18]+shift_r[19]+shift_r[20]+shift_r[21]+shift_r[22]+shift_r[23]+shift_r[24]+shift_r[25]+shift_r[26]+shift_r[27]+shift_r[28]+shift_r[29];
    
assign data_out[EX_SI*2-1:0] = sum;
assign data_out[EX_SI*2] = (data_in1[DATA-1] ^ data_in2[DATA-1]) ? 1 : 0;
assign data_out_round = {data_out[EX_SI*2],data_out[2*POIN+INTE-1:2*POIN],data_out[2*POIN-1:POIN]};
endmodule
