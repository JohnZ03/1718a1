module con_top(ima, clk , rst_n, frame_start_in,line_start_in,frame_end_in,frame_start_out,frame_end_out,line_start_out,out_valid);
);

parameter IMA = 8;

input [IMA-1:0] ima;
input frame_start_in,frame_end_in,line_start_in;
output frame_start_out,frame_end_out,line_start_out;
input clk,rst_n,out_valid;

parameter SENDING_IMAGE = 1'b1;
parameter IDLE          = 1'b0;

//enable conv core
reg enable;

//fsm signal
reg state;
reg[1:0] state_dly;
reg next_state;

reg [IMA-1:0]  pix_r [32*14-1:0];
reg [32*7-1:0] frame_start_r;
reg [49*8-1:0] ima_to_conv;

reg            frame_start_in_extend;
reg [8:0]      wri_cnt;

//this fsm is used to determine when value sent to conv core is enabled
always@(posedge clk or negedge rst_n)
  if(!rst_n) begin
    state <= IDLE;
    state_dly <= 2'b0;
  end
  else begin 
    state <= next_state;
    state_dly <= {state_dly[0],state};
  end

always@(*)
  case(state)
    IDLE: 
    begin
      if(frame_start)
        next_state = SENDING_IMAGE;
      else
        next_state = IDLE;
    end
    SENDING_IMAGE:
    begin
      if(frame_end&(!frame_start))
        next_state = IDLE;
      else 
        next_state = SENDING_IMAGE;
    end
    default:
      next_state = IDLE;
    endcase

//conv core should not be enable when transfering the first 7 line
always@(posedge clk or negedge rst_n)
  if(!rst_n)
    frame_start_extend <= 1'b0;
  else if (frame_start_in_dly)
    frame_start_extend <= 1'b1;
  else if (wri_cnt == 32*7-1)
    frame_start_extend <= 1'b0;

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    wri_cnt <= 0;
  else if(frame_start_in)
    wri_cnt <= 0;
  else if(enable&&(wri_idx==32*7-1))
    wri_cnt <= 0;
  else if(enable)
    wri_cnt <= wri_cnt + 1;

//write image sram
always@(posedge clk or negedge rst_n)
  if(!rst_n)
    pix_r <= 0;
  else begin
    pir_r[wri_cnt] <= ima;
    pir_r[wri_cnt+32*7] <= ima;
  end

//read image sram
genvar i;
for(i=0; i<7; i=i+1) begin
  always@(posedge clk or negedge rst_n)
    if(!rst_n) 
        img_to_con[i*56+55:i*56   ] <= 56'h0;
    else begin
        ima_to_con[i*56+7 :i*56   ] <= pri_r[wri_cnt+26+i*32];
        ima_to_con[i*56+15:i*56+ 8] <= pri_r[wri_cnt+27+i*32];
        ima_to_con[i*56+23:i*56+16] <= pri_r[wri_cnt+28+i*32];
        ima_to_con[i*56+31:i*56+24] <= pri_r[wri_cnt+29+i*32];
        ima_to_con[i*56+39:i*56+32] <= pri_r[wri_cnt+30+i*32];
        ima_to_con[i*56+47:i*56+40] <= pri_r[wri_cnt+31+i*32];
        ima_to_con[i*56+55:i*56+48] <= pri_r[wri_cnt+32+i*32];
    end
end

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    enable <= 1'b0;
  else begin
    if(wri_cnt[8:2]==7'd0)
      enable <= 1'b0;
    else if(frame_start_extend)
      enable <= 1'b0;
    else if(state_dly[1])
      enable <= 1'b1;
  end

con  con_inst(
    .clk      (clk),
    .rst_n    (rst_n),
    .wei      (), 
    .bias     (), 
    .ima      (ima_ton_con),
    .out_reg  ()
    .enable   (enable_r),
    .valid    (valid)
    );

single_port_rom con_b #(.DATA_WIDTH(8),
.ADDR_WIDTH(3920))

(
  .clk(clk),
  .ena(enable),
  .addr(addr),
  .q(q));

reg enable_r;

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    enable_r <= 0;
    else
      enable_r <= enable;

always@(posedge clk or negedge rst_n)
  if(!rst_n||frame_start_in)begin
    addr <= 0;
  end
    else if(enable)
      addr <= addr + 1;

single_port_rom con_w #(.DATA_WIDTH(8),
.ADDR_WIDTH(3920))

(
  .clk(clk),
  .ena(enable),
  .addr(addr),
  .q(q));
