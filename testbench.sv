`timescale 1ns/1ns

module FSM_TB;
  reg CLK,RESET;
  reg M0_TB, M1_TB;
  wire T_TB, C0_TB, C1_TB;

  FSM DUT(.CLK(CLK), .RESET(RESET), .M0(M0_TB), .M1(M1_TB),
            .T(T_TB), .C0(C0_TB), .C1(C1_TB));

  initial begin
      CLK = 1'b0; 
      RESET = 1'b1;
  end

  always begin CLK=~CLK;#1; end

  initial begin
    $dumpfile("FSM.vcd");
    $dumpvars(0,FSM_TB);

    {M0_TB,M1_TB} = 2'b00; #1
    RESET = 1'b0; #2
    {M0_TB,M1_TB} = 2'b01; #2
    {M0_TB,M1_TB} = 2'b01; #2
    RESET = 1'b1; #2
    RESET = 1'b0; #2
    {M0_TB,M1_TB} = 2'b10; #2

    $finish;
  end
endmodule