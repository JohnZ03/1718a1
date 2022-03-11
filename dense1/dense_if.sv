interface dense_if(
    input  clk,
    input  rst_n
    );
logic         ena            ;
logic         frame_start_in ;
logic         frame_end_in   ;
logic signed [15:0]  dense_input    ;
logic signed [15:0]  dense_sum_out  ;
logic         valid          ;

clocking cb @(posedge clk);
    input  rst_n;
    input  frame_start_in,frame_end_in,dense_input;
    output dense_sum_out, valid;
endclocking   
endinterface
