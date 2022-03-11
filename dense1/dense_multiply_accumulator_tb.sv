module dense_multiply_accumulator_tb;
logic         clk;
logic         rst_n;
dense_if      dif(.clk(clk),.rst_n(rst_n));

dense_multiply_accumulator dut(
	.clk            ( clk            ), 
	.rst_n          ( rst_n          ),
    .ena            ( dif.ena            ),  
	.frame_start_in ( dif.frame_start_in ),              
	.frame_end_in   ( dif.frame_end_in   ),           
    .dense_input    ( dif.dense_input    ),
    .dense_sum_out  ( dif.dense_sum_out  ),
	.valid          ( dif.valid          ) 
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
	    fork
		    begin
			    dif.ena <= 1'b0;
		        dif.frame_start_in <= 1'b0;
		        dif.frame_end_in   <= 1'b0;
                @(dif.cb);
		        while(!rst_n) 
	                @(dif.cb);
                //generate a packet
	            dif.frame_start_in <= 1'b1;
				@(dif.cb);
	            dif.frame_start_in <= 1'b0;
                for(int u=0; u<5;  u=u+1) begin
		            for(int i=0; i<14; i=i+1) begin
				        for(int j=0; j<14; j=j+1) begin
                            dif.ena = 1'b1;
				    	    dif.dense_input = i+j;
                            fork
                                begin
                                    if(j==13&&i==13&&u==4) begin
				                        dif.frame_end_in <= 1'b1;
                                        @(dif.cb);
				                        dif.frame_end_in <= 1'b0;
                                    end
                                end
                                begin
                                    @(dif.cb);
                                    dif.ena = 1'b0;
                                    @(dif.cb);
                                end
                            join
				    	end
				    end
                end
		        dif.frame_end_in <=  1'b0;
                dif.ena <= 1'b0;
            end
			begin
		        while(!dif.ena) 
	                @(dif.cb);
		         
			end
		join
	end

    initial begin
       $fsdbDumpfile("dense_multiply_accumulator.fsdb");
       $fsdbDumpvars;
       $fsdbDumpMDA();
    end		


    initial begin
    	#20000;
    	$finish();
    end

endmodule
		        
		

