module transceiver(input clock, input reset, inout data, output busy, output [7:0] rxfifo_di, input [7:0] txfifo_do);

reg receiving = 1'b0;

reg [18:0] timer = 19'b0;

parameter TIMER_5_MS = 19'd500000; // 5 ms @ 100 MHz
parameter RESET_STATE = 4'd0;

always @(posedge clock or posedge reset)
begin

	if (reset)
	begin
		state <= RESET_STATE;
	end
	else
	begin

		case (state)

		RESET_STATE:	begin
					if (~data)
					begin
						state <= recv_start_bit;
					end
					else

				end

		endcase

	end
end


always @(posedge data or posedge reset_counter)
begin

	if (reset_timer)
	begin
		
		reset_timer <= ~reset_timer;
	end
	else
	begin
		timer <= 19'b0;
	end

end

always @(posedge clock)
begin

	timer <= timer + 1;

end

endmodule

always @(negedge data)
begin
	if (negedge data)
end
