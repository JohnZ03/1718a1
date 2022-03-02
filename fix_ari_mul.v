module fix_ari_mul(data_in1,data_in2,data_out,clk,rst_n);

parameter data=15;
parameter sign=1;
parameter inte=6;
parameter poin=8;

input [data-1:0] data_in1;
input [data-1:0] data_in2;
output [data-1:0] data_out;
input clk;
input rst_n;

reg [data*2-1:0] data_in1_r,data_in2_r;

always@(posedge clk or negedge rst_n)begin
  if(!rst_n)begin
     data_in1_r <= 'b0;
     data_in2_r <= 'b0;
   end
   else begin
     data_in1_r <= {{15{data_in1[data-1]}},dat_in1};
     data_in2_r <= {{15{data_in1[data-1]}},dat_in1};
   end
 end

reg [data*2-1:0] shift_r [data*2-1:0];
integer i,j,k;
always@(posedge clk or negedge rst_n)
  if(!rst_n)
    for(i = 0; i < data*2; i=i+1)
      shift_r[i] <= 'b0;
    else
      for(k = 0; k < data*2; k=k+1)
        if(data_in2_r[k] == 1)
          shift_r[k] <= (data_in1_r << k);
        else
          shift_r[k] <= 'b0;

reg [data*2-1:0] sum;
always@(posedge clk or negedge rst_n)
  if(!rst_n)
    sum <= 'b0;
  else begin
    for(i = 0; i < 64; i=i+1)
      sum <= sum[i];
  end


assign data_out = sum;

endmodule
