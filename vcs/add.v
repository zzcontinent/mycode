// add.v
`timescale 1 ns/1 ps
module add
(
	input [3:0] a,
	input [3:0] b,
	output [4:0] sum
);

fa fa_inst (
	.a(a),
	.b(b),
	.sum(sum)
);

endmodule
