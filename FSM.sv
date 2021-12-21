module FSM(
	input CLK,RESET,M0,M1,
	output reg T,C0,C1);

	reg [1:0]state;
	reg [1:0]next_state;

	parameter E1 = 0;
	parameter E2 = 1;
	parameter E3 = 2;
	parameter E4 = 3;

	initial begin
		state = E1;
	end

	always @(posedge CLK, posedge RESET)
		begin
			if (RESET)
				state <= E1;
			else
				state <= next_state;
		end

//Next state logic
	always @(state,M0,M1)
		begin
			case(state)
				E1: 
					case({M0,M1})
						default: next_state = E1;
						2'b01: next_state = E2;
						2'b10: next_state = E4;
					endcase
				E2: 
					case({M0,M1})
						2'b00: next_state = E2;
						2'b01: next_state = E3;
						2'b10: next_state = E4;
						default: next_state = E1;
					endcase
				E3: 
					case({M0,M1})
						2'b00: next_state = E3;
						2'b01: next_state = E4;
						default: next_state = E1;
					endcase
				default: 
					case({M0,M1})
						2'b00: next_state = E4;
						default: next_state = E1;
					endcase
			endcase
		end

//Output Logic
	always @ (state,M0,M1)
		begin
			case(state)
				E1: 
					case({M0,M1})
						2'b00,
						2'b01,
						2'b10: {T,C0,C1} = 3'b000;
						default: {T,C0,C1} = 3'b100;
					endcase
				E2: 
					case({M0,M1})
						2'b00,
						2'b01,
						2'b10: {T,C0,C1} = 3'b000;
						default: {T,C0,C1} = 3'b101;
					endcase
				E3: 
					case({M0,M1})
						2'b00,
						2'b01: {T,C0,C1} = 3'b000;
						2'b10: {T,C0,C1} = 3'b100;
						default: {T,C0,C1} = 3'b110;
					endcase
				default: 
					case({M0,M1})

						2'b00: {T,C0,C1} = 3'b000;
						2'b01: {T,C0,C1} = 3'b100;
						2'b10: {T,C0,C1} = 3'b101;
						default: {T,C0,C1} = 3'b111;
					endcase
			endcase
		end
endmodule