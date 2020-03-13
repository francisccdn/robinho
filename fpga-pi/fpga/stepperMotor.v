module stepperMotor(clk, dir, stepMotor); //dir - 0 up : 1 down : 2 stop
	input clk;
	input dir; 				
	output reg [0:3]stepMotor;
	
	parameter step_time = 250000;
	integer timer;
	
	always@(posedge clk)
	begin
		timer = timer + 1;
		
		case(dir)
		0:
		begin
			if(timer < step_time)
				stepMotor[0:3]=4'b0011;
			else if((timer > step_time) && (timer < step_time*2))
				stepMotor[0:3]=4'b1001;
			else if((timer > step_time*2) && (timer < step_time*3))
				stepMotor[0:3]=4'b1100;
			else if((timer > step_time*3) && (timer < step_time*4))
				stepMotor[0:3]=4'b0110;
			else if(timer > step_time*4)
				timer = 32'b0;
		end
		
		1:
		begin
			if(timer < step_time)
				stepMotor[0:3]=4'b0110;
			else if((timer > step_time) && (timer < step_time*2))
				stepMotor[0:3]=4'b1100;
			else if((timer > step_time*2) && (timer < step_time*3))
				stepMotor[0:3]=4'b1001;
			else if((timer > step_time*3) && (timer < step_time*4))
				stepMotor[0:3]=4'b0011;
			else if(timer > step_time*4)
				timer = 32'b0;
		end
		
		2:
		begin
			stepMotor[0:3] = 4'b1111;
		end
		
		endcase
	end
	
endmodule
