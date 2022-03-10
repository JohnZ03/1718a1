module maxpooling_tb;
logic         clk;
logic         rst_n;
logic         ena            ;
logic         frame_start_in ;
logic         line_start_in  ;
logic         frame_end_in   ;
logic [15:0]  sig_layer      ;
logic [15:0]  max_layer      ;
logic         frame_start_out;
logic         line_start_out ;
logic         frame_end_out  ;
logic         valid          ;




maxpooling dut(
	.clk            ( clk            ), 
	.rst_n          ( rst_n          ),
    .ena            ( ena            ),  
	.frame_start_in ( frame_start_in ),              
	.line_start_in  ( line_start_in  ),            
	.frame_end_in   ( frame_end_in   ),           
	.sig_layer      ( sig_layer      ),        
	.max_layer      ( max_layer      ),        
	.frame_start_out( frame_start_out),             
	.line_start_out ( line_start_out ),             
	.frame_end_out  ( frame_end_out  ),            
	.valid          ( valid          ) 
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
			    ena <= 1'b0;
		        frame_start_in <= 1'b0;
		        frame_end_in   <= 1'b0;
				line_start_in  <= 1'b0;
		        while(!rst_n) 
	                @(posedge clk);

                //generate a packet

	            frame_start_in <= 1'b1;
	            frame_end_in   <= 1'b0;
			    line_start_in  <= 1'b1;
	            ena <= 1'b0;
				@(posedge clk);

		        for(int i=0; i<28; i=i+1) begin
				    frame_start_in <= 1'b0;
				    line_start_in  <= 1'b0;
		            frame_end_in   <= 1'b0;
		            ena <= 1'b1;
				    for(int j=0; j<28; j=j+1) begin
					    sig_layer <= i+j+1;
						if(j==27&&i<27) 
						    line_start_in <= 1'b1;
                        else if(j==27&&i==27)
				            frame_end_in <= 1'b1;
						@(posedge clk);
						line_start_in <= 1'b0;
					end
				end
		        frame_end_in <=  1'b0;
                ena <= 1'b0;
            end
			begin
		        while(!ena) 
	                @(posedge clk);
		         
			end
		join
	end

    initial begin
       $fsdbDumpfile("maxpooling.fsdb");
       $fsdbDumpvars;
       $fsdbDumpMDA();
    end		


    initial begin
    	#20000;
    	$finish();
    end

endmodule
		        
		

