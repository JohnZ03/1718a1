module rom_tb;
    parameter DATA_WIDTH = 8;
    parameter ADDR_WIDTH = 8;
    logic                  clk;
    logic                  ena;
    logic [ADDR_WIDTH-1:0] addr;
    logic [DATA_WIDTH-1:0] q;

single_port_rom dut
(
    .clk  (clk ),        
    .ena  (ena ),         
    .addr (addr),          
    .q    (q   )
);

initial begin 
    clk = 1'b0;
    forever 
        #5 clk = !clk;
end

initial begin
    ena = 1'b0;
    #25
    ena = 1'b1;
end

initial begin
    addr = 0;
    while(!ena)
        @(posedge clk);
    for(int i=0; i<20; i=i+1) begin
        addr = i;
        @(posedge clk);
    end
end


initial begin
   $fsdbDumpfile("rom.fsdb");
   $fsdbDumpvars;
   $fsdbDumpMDA();
end		


initial begin
	#2000;
	$finish();
end
endmodule
