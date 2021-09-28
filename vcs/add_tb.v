`timescale 1 ns/1 ps

module add_tb();

reg [3:0] a;
reg [3:0] b;
wire [4:0] sum;

initial
begin
	#0 a=4'd1;
	#0 b=4'd2;
	#100;
	$stop;
end

always # 10
begin
	a=a+1;
	b=b+1;
end

add add_inst (
	.a(a),
	.b(b),
	.sum(sum)
);
endmodule
