module sigmoid_tb;
logic clk;
logic rst_n;

logic signed [15:0] in_tr;

sigmoid_if sif(.clk(clk),.rst_n(rst_n));

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
            sif.ena = 1'b0;
            sif.sigmoid_in = 'd0;
            in_tr = 16'sb1111_1111_1111_1111;
            @(sif.cb);
            while(!rst_n)
                @(sif.cb);
            //generate a packet
            for(int i=0;i<1792;i=i+1) begin
                @(sif.cb);
                sif.ena = 1'b1;
                sif.sigmoid_in = i;
                repeat(12) begin
                    @(sif.cb);
                    sif.ena = 1'b0;
                end
            end
            repeat(500)
                @(sif.cb);
            for(int i=0;i<1792;i=i+1) begin
                sif.ena = 1'b1;
                sif.sigmoid_in = in_tr;
                in_tr = in_tr - 'sb1;
                @(sif.cb);
                sif.ena = 1'b0;
                @(sif.cb);
            end

        end
        begin
        end
    join
end

sigmoid dut(
    .clk  (clk),
    .rst_n(rst_n),
    .ena  (sif.ena),
    .sigmoid_in(sif.sigmoid_in),
    .valid (sif.valid),
    .sigmoid_out (sif.sigmoid_out)
    );

initial begin
   $fsdbDumpfile("sigmoid.fsdb");
   $fsdbDumpvars;
   $fsdbDumpMDA();
end		


initial begin
	#400000;
	$finish();
end


endmodule
