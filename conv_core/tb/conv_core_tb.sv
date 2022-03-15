`include "tb/conv_core_input_tr.sv"
`include "tb/conv_core_output_tr.sv"
module conv_core_tb;

logic clk;
logic rst_n;

conv_core_if cif(.clk(clk),.rst_n(rst_n));

conv_core_input_tr  tr_i;
conv_core_output_tr tr_o;

con dut(
    .clk (clk), 
    .rst_n(rst_n), 
    .ima(cif.ima),
    .wei(cif.wei),
    .bias(cif.bias),
    .out_reg(cif.out_reg),
    .enable(cif.enable),
    .valid(cif.valid)
);

	initial begin 
	    clk = 1'b0;
		forever 
        #5 clk = !clk;
	end

	initial begin
	    rst_n = 1'b0;
        #15
		rst_n = 1'b1;
    end

    initial begin
       $fsdbDumpfile("conv_core_tb.fsdb");
       $fsdbDumpvars;
       $fsdbDumpMDA();
    end		


    initial begin
    	#200000;
    	$finish();
    end



    task driver_one_pkt(conv_core_input_tr tr_i);
        cif.enable <= 1'b0;
        @(cif.cb);
        for(int i=0;i<10;i=i+1) begin
            cif.enable = 1'b1;
		    cif.ima = tr_i.ima;
		    cif.wei = tr_i.wei;
		    cif.bias = tr_i.bias;
            @(cif.cb);
            cif.enable = 1'b0;
        end
    endtask

    

    initial begin
        tr_i=new();
        tr_i.randomize();
        fork
            begin
		        while(!rst_n) 
	                @(cif.cb);
                driver_one_pkt(tr_i);
            end
            begin
                @(cif.cb);
            end
        join
    end
endmodule
