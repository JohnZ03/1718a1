interface conv_if(
    input  clk,
    input  rst_n
    );
logic         frame_start_in ;
logic         frame_end_in   ;
logic         line_start_in    ;
logic         frame_start_out ;
logic         frame_end_out   ;
logic         line_start_out   ;
logic         ena_in;
logic signed [7:0]   ima;
logic signed [15:0]  sig_layer;
logic         valid          ;

clocking cb @(posedge clk);
    input  rst_n;
    input  frame_start_in,frame_end_in,ena_in,line_start_in,ima;
    output valid, frame_start_out, frame_end_out, line_start_out, sig_layer;
endclocking   
endinterface
