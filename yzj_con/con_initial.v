module con(wei, bias, ima, out_reg, clk, rst_n,enable,valid);

parameter DATA = 16;//original wei width
parameter EXT = 17;//extension width
parameter NUM = 49;//how many multi
parameter IMA = 8;//image width

input enable;
input [DATA*NUM-1:0] wei;
input [IMA*NUM-1:0] ima;
input [DATA-1:0] bias;
input clk;
input rst_n;
output [38:0] out_reg;
output valid;

wire [EXT*NUM-1:0] ima_0;//image after extension
wire [EXT*NUM-1:0] wei_0;//weight after extension

reg en_r1;
reg en_r2;
reg en_r3;
reg en_r4;
reg en_r5;
reg en_r6;
reg en_r7;

always@(posedge clk or negedge rst_n)
  if(!rst_n)begin
    en_r1 <= 0;
    en_r2 <= 0;
    en_r3 <= 0;
    en_r4 <= 0;
    en_r5 <= 0;
    en_r6 <= 0;
    en_r7 <= 0;
  end
    else begin
    en_r1 <= enable;
    en_r2 <= en_r1;
    en_r3 <= en_r2;
    en_r4 <= en_r3;
    en_r5 <= en_r4;
    en_r6 <= en_r5;
    en_r7 <= en_r6;
  end

assign valid <= en_r7;

//image extend
genvar i;
generate
for(i = 1;i < 49;i = i + 1)
begin: ima_extenstion
  assign ima_0[i*17-1:(i-1)*17] = {1'b0,ima[i*8-1:(i-1)*8],{8{1'b0}}};
end
endgenerate

//wei extend
genvar k;
generate
for(k = 1;k < 49;k = k + 1)
begin: wei_extenstion
  assign wei_0[i*17-1:(i-1)*17] = {wei[DATA-1],wei};
end
endgenerate

//mul instantiate
wire [(EXT*2-1)*NUM-1:0] out_w;
wire rou_r1;
reg [(EXT*2-1)*NUM-1:0] out_r1;

genvar j;
generate
for(j = 1;j < 49;j = j + 1)
begin:mul
  fix_air_mul #(
    .DATA(17),
    .EX_SI(16),
    .SIGN(1),
    .INTE(8),
    .POIN(8)
  )
  u_fix_air_mul(
    .data_in1(wei_0[j*17-1:(i-1)*17]),
    .data_in2(ima_0[j*17-1:(j-1)*17]),
    .data_out(out_w[j*33-1:(j-1)*33]),
    .data_round(rou_r1),
    .clk(clk),
    .rst_n(rst_n)
  );
end
endgenerate

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    out_r1 <= 0;
  else
    out_r1 <= out_w;


  //fisrt 25 add
  reg signed [34*25-1:0] out_r2;
  reg r2_done;
  integer a;

  always@(posedge clk or negedge rst_n)
    if(!rst_n)begin
      out_r2 <= 0;
    end
    else begin
      for(a=1;a<25;a=a+1)begin
        out_r2[34*a-1:34*(a-1)] <= $signed(out_r1[33*a*2-1:33*(a*2-1)]) + $signed(out_r1[33*(a*2-1)-1:33*(a*2-2)]);
      end
      out_r2[34*25-1:34*24] <= $signed(out_r1[33*NUM-1:33*(NUM-1)]) + $signed(bias);
    end

    //second 13 add
    reg signed [35*13-1:0] out_r3;
    integer b;

    always@(posedge clk or negedge rst_n)
      if(!rst_n)
        out_r3 <= 0;
      else begin
        for(b=1;b<13;b=b+1)begin
          out_r3[35*b-1:35*(b-1)] <= $signed(out_r2[34*b*2-1:34*(b*2-1)]) + $signed(out_r2[34*(b*2-1)-1:34*(b*2-2)]);
        end
        out_r3[35*13-1:35*12] <= $signed(out_r2[34*25-1:34*24]);
      end

      //third 7 add
      reg signed [36*7-1:0] out_r4;
      integer c;

      always@(posedge clk or negedge rst_n)
        if(!rst_n)
          out_r4 <= 0;
        else begin
          for(c=1;c<7;c=c+1)begin
            out_r4[36*c-1:36*(c-1)] <= $signed(out_r3[35*c*2-1:35*(c*2-1)]) + $signed(out_r3[35*(c*2-1)-1:35*(c*2-2)]);
          end
          out_r4[36*7-1:36*6] <= $signed(out_r3[35*13-1:35*12]);
        end

        //forth 4 add
        reg signed [37*4-1:0] out_r5;
        integer d;

        always@(posedge clk or negedge rst_n)
          if(!rst_n)
            out_r5 <= 0;
          else begin
            for(d=1;d<4;d=d+1)begin
              out_r5[37*d-1:37*(d-1)] <= $signed(out_r4[36*d*2-1:36*(d*2-1)]) + $signed(out_r4[36*(d*2-1)-1:36*(d*2-2)]);
            end
            out_r5[37*4-1:37*3] <= $signed(out_r4[36*7-1:36*6]);
          end

          //fifth 2 add
          reg signed [38*2-1:0] out_r6;

          always@(posedge clk or negedge rst_n)
            if(!rst_n)
              out_r6 <= 0;
            else begin
              out_r6[37:0] <= $signed(out_r5[36:0]) + $signed(out_r5[73:37]);
              out_r6[75:38] <= $signed(out_r5[110:74]) + $signed(out_r5[147:111]);
            end

            //final out
            reg signed [38:0] out_reg;

            always@(posedge clk or negedge rst_n)
              if(!rst_n)
                out_reg <= 0;
              else 
                out_reg <= $signed(out_r6[75:38]) + $signed(out_r6[37:0]);

endmodule
