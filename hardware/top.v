module top(input clock, input reset, inout data_line);

parameter DEBOUNCE_DELAY = 16'd50000; //0,5 ms @ 100 MHz

reg	[15:0] data_line_count = 16'd0;

reg reset_sync = 1'b1;
reg data = 1'b1;
reg data_line_inter = 1'b0;
reg RXbyte = 1'b0;
reg TXbyte = 1'b0;
reg busy = 1'b0;
reg done = 1'b0;

wire data_wire;

assign data_wire = data | reset_sync;


transceiver tr1 (clock, reset, data_wire, RXbyte, TXbyte, busy, done);
mac mac1(clock, reset, );



always @(posedge clock)
begin
	if (~busy)
	begin
		
	end
end


always @(posedge clock)
begin
	reset_sync <= reset;
end


always @(posedge clock)
begin
	if (data_line != data_line_inter)
	begin
		data_line_inter <= data_line;
		data_line_count <= 0;
	end
	else
	begin
		if (data_line_count == DEBOUNCE_DELAY)
		begin
			data <= data_line_inter;
		end
		else
		begin
			data_line_count <= data_line_count + 1;
		end
	end
end

endmodule
