module fifo(input clock, input reset, input do_read, input do_write, input [7:0] di, output [7:0] do, output write_ack, output read_ack,  output busy);

reg [10:0] read_pointer = 11'd0;
reg [10:0] old_read_pointer = 11'd0;
reg [10:0] write_pointer = 11'd0;
reg [10:0] old_write_pointer = 11'd0;
reg [10:0] addr = 0;
reg we = 0;
wire has_remaining_bytes;
reg pointer_overflow = 0;
reg busy = 0;
reg write_ack;
reg read_ack;
wire can_read;

assign can_read = (~pointer_overflow && (read_pointer < write_pointer)) || (pointer_overflow && (read_pointer >= write_pointer));
assign can_write = (~pointer_overflow && (write_pointer >= read_pointer)) || (pointer_overflow && (read_pointer > write_pointer));

bram bram1(clock, we, addr, di, do);


always @(posedge clock)
begin
	if ( (old_read_pointer > read_pointer) || (old_write_pointer > write_pointer) )
	begin
		pointer_overflow <= ~pointer_overflow;
	end
end

always @(posedge clock)
begin
	if ( (do_write || do_read) & ~busy)
	begin
		if (do_write & can_write)
		begin
			write_ack <= 1'b1;
			addr <= write_pointer;
			old_write_pointer <= write_pointer;
			write_pointer <= write_pointer + 1;
			we <= 1'b1;
			busy <= 1'b1;
		end
		else
		begin
			if (do_read & can_read)
			begin
				we <= 1'b0;
				read_ack <= 1'b1;
				addr <= read_pointer;
				old_read_pointer <= read_pointer;
				read_pointer <= read_pointer + 1;
				busy <= 1'b1;
			end
		end
	end
	else
	begin
		read_ack <= 1'b0;
		write_ack <= 1'b0;
		busy <= 1'b0;
		we <= 1'b0;
	end
	
end

endmodule
