module fifo(input clock, input reset, input do_write, input di, output do, input valid_input, input ack_output);

reg read_pointer = 0;
reg write_pointer = 0;
reg we = 0;
wire has_remaining_bytes;

assign has_remaining_bytes = (

bram bram1(clock, we, addr, di, do);

always @(posedge clock)
begin
	if (valid_input & ~busy)
	begin
		if (do_write)
		begin
			addr <= write_pointer;
			write_pointer <= write_pointer + 1;
			we <= 1'b1;
			busy <= 1'b1;
		end
		else
		begin
			we <= 1'b0;
			addr <= read_pointer;
			read_pointer <= read_pointer + 1;
			busy <= 1'b1;
		end
	end
	else
	begin
		busy <= 1'b0;
		we <= 1'b0;
	end
	
end

endmodule
