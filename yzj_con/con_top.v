module con_top(ima, clk , rst_n, frame_start_in,line_start_in,frame_end_in,frame_start_out,frame_end_out,line_start_out,enable,out_valid);
);

parameter IMA = 8;

input [IMA-1:0] ima;
input frame_start_in,frame_end_in,line_start_in;
output frame_start_out,frame_end_out,line_start_out;
input enable,clk,rst_n,out_valid;

reg [IMA-1:0] pix_r [32*7-1:0];
reg [32*7-1:0] frame_start_r;

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    frame_start_r <= 0;
  else
    frame_start_r <= {frame_start_r[32*7-1:0], frame_start_in};

reg [7:0] wri_idx;

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    wri_idx <= 0;
  else if(frame_start_in)
    wri_idx <= 0;
  else if(enable&&(wri_idx==32*7-1))
    wri_idx <= 0;
  else if(enable)
    wri_idx <= wri_idx + 1;

always@(posedge clk or negedge rst_n)
  if(!rst_n)
    pix_r <= 0;
  else if(enable)
    pir_r[wri_idx] <= ima;


