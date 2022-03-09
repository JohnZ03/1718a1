module fix_ari_add_tb;

    parameter WIDTH = 16;
    
    logic   signed [WIDTH-1:0] data_in1;
    logic   signed [WIDTH-1:0] data_in2;

    logic   signed [WIDTH-1:0] data_out;
	logic   signed [WIDTH:0]   data_out_ref;
	logic   signed [WIDTH-1:0] data_out_ref_round;
	integer i;
	

    fix_ari_add#(.DATA(WIDTH))
	    dut(
        .data_in1(data_in1[WIDTH-1:0]),
		.data_in2(data_in2[WIDTH-1:0]),
		.data_out(data_out)
		);

    initial begin
	    data_in1 = -32768;
		data_in2 = -12384;
	    for(i=0;i<100;i=i+1) begin
		    #9
		    data_in1 = data_in1 + 480;
		    data_in2 = data_in2 + 480;
			data_out_ref = data_in1 + data_in2;
			data_out_ref_round = (data_out_ref< -32768) ? -32768 :
			                     (data_out_ref>  32767) ?  32767 : data_out_ref;
			#1
		    if(data_out_ref_round == data_out)
			    $display("Correct: in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);
			else
			    $error  ("Error:   in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);
			                                                         
		end

		//corner cases 0
        #9
		data_in1 = 16384;
		data_in2 = 16383;
		data_out_ref = data_in1 + data_in2;
		data_out_ref_round = (data_out_ref< -32768) ? -32768 :
		                     (data_out_ref>  32767) ?  32767 : data_out_ref;
		#1
	    if(data_out_ref_round == data_out)
		    $display("Correct: in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);
		else
		    $error  ("Error:   in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);

		//corner cases 1
        #9
		data_in1 = 16394;
		data_in2 = 16384;
		data_out_ref = data_in1 + data_in2;
		data_out_ref_round = (data_out_ref< -32768) ? -32768 :
		                     (data_out_ref>  32767) ?  32767 : data_out_ref;
		#1
	    if(data_out_ref_round == data_out)
		    $display("Correct: in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);
		else
		    $error  ("Error:   in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);

		//corner cases 2
        #9
		data_in1 = -16384;
		data_in2 = -16384;
		data_out_ref = data_in1 + data_in2;
		data_out_ref_round = (data_out_ref< -32768) ? -32768 :
		                     (data_out_ref>  32767) ?  32767 : data_out_ref;
		#1
	    if(data_out_ref_round == data_out)
		    $display("Correct: in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);
		else
		    $error  ("Error:   in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);

		//corner cases 3
        #9
		data_in1 = -16384;
		data_in2 = -16385;
		data_out_ref = data_in1 + data_in2;
		data_out_ref_round = (data_out_ref< -32768) ? -32768 :
		                     (data_out_ref>  32767) ?  32767 : data_out_ref;
		#1
	    if(data_out_ref_round == data_out)
		    $display("Correct: in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);
		else
		    $error  ("Error:   in1:%d, in2:%d, ref:%d, rounded ref:%d, output:%d", data_in1, data_in2, data_out_ref,data_out_ref_round,data_out);

	end

    initial begin
       $fsdbDumpfile("fix_ari_add.fsdb");
       $fsdbDumpvars;
    end		

    initial begin
    	#1100;
    	$finish();
    end

endmodule
