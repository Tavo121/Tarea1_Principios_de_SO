
module Tarea1_CPU (
	btn_export,
	buzzer_export,
	clk_clk,
	seg_0_export,
	seg_1_export,
	seg_2_export,
	seg_3_export,
	switches_export,
	leds_export);	

	input	[1:0]	btn_export;
	output		buzzer_export;
	input		clk_clk;
	output	[6:0]	seg_0_export;
	output	[6:0]	seg_1_export;
	output	[6:0]	seg_2_export;
	output	[6:0]	seg_3_export;
	input	[3:0]	switches_export;
	output	[4:0]	leds_export;
endmodule
