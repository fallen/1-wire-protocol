`timescale 1 ns / 1 ns

module fifo_tb;

reg clk = 0;
reg reset = 1;
reg reset_sync = 1;

wire busy;
wire write_ack;
wire read_ack;
reg do_read = 1'b0;
reg do_write = 1'b0;
reg [7:0] di = 8'b0;
wire [7:0] do;

always #5 clk = ~clk;

always @(posedge clk)
begin
	reset_sync <= reset;
end

fifo fifo1(clk, reset_sync, do_read, do_write, di, do, write_ack, read_ack, busy);


initial begin
	$dumpfile("fifo_tb.vcd");
	$dumpvars(0, fifo_tb);

	reset = 1;
	#10
	reset = 0;
	#10

	di = 8'd42;
	do_write = 1;
	#20

	di = 8'd25;
	do_write = 1;
	#20

	di = 8'd32;
	do_write = 1;
	#20
	
	do_write = 0;
	do_read = 1;
	#20

	do_read = 1;
	#20

	do_read = 1;
	#20
		

	$stop;
	$finish;
end

endmodule
