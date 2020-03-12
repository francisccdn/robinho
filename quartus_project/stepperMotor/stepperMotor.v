module stepperMotor(clk,dir,out);
	input dir,clk;
	output reg [0:3]out;
	
	integer count,i;
	
	always@(clk) begin
		
		if(dir==0)begin
			count=0;
			for(i=count;i<=4;i=i+1)
				count=count+1;
		end

	end
	always@(*) begin
		case (count)
			 0: begin out[0:3]=4'b0000; end
			 1: begin out[0:3]=4'b1010; end
			 2: begin out[0:3]=4'b1001; end
			 3: begin out[0:3]=4'b0101; end
			 4: begin out[0:3]=4'b0110; end
		endcase		
	end
endmodule
