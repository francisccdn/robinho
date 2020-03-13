module robinho 
(
input rst,
input clk,
input rasp_on,
input obj_inSight,
input obj_isClose,
output reg [0:3]w_motor,
output [0:3]c_motor
);
integer n_turns;

//States
reg cur_state = init;
parameter init = 0, stop = 1, foward = 2, turn = 3, clawD = 4, clawU = 5;

//Internal clock
integer timer = 0;
integer time_begin;
parameter claw_downTime = 2000, claw_upTime = 2000, turn_time = 2000;

//Claw control
reg dir = c_stop;
parameter c_up = 0, c_down = 1, c_stop = 2;
wire stepperMotor_out;
assign c_motor = stepperMotor_out;

stepperMotor claw(
	.clk(clk),
	.dir(dir),
	.stepMotor(stepperMotor_out)
);

always @ (posedge clk)
begin
	timer <= timer + 1;
end

always @ (*)
begin
case(cur_state)
	init:
	begin
		w_motor <= 4'b1111; // Motor 1 and 2 - Stopped 
		dir = c_stop;
	end
	
	stop:
	begin
		w_motor <= 4'b1111; // Motor 1 and 2 - Stopped
		dir = c_stop;
	end
	
	foward:
	begin
		if(timer >= time_begin + turn_time)
		begin
			time_begin = timer;
			n_turns = n_turns - 1;
		end
			
		w_motor[0:1] <= 2'b10; // Motor 1 - Clockwise
		w_motor[2:3] <= 2'b10; // Motor 2 - Clockwise
		dir = c_stop;
	end
	
	turn:
	begin
		if(timer >= time_begin + turn_time)
		begin
			time_begin = timer;
			n_turns = n_turns + 1;
		end
		
		w_motor[0:1] <= 2'b10; // Motor 1 - Clockwise
		w_motor[2:3] <= 2'b01; // Motor 2 - Counterclockwise
		dir = c_stop;
	end
	
	clawD:
	begin
		w_motor <= 4'b1111; // Motor 1 and 2 - Stopped
		dir = c_down;
	end
	
	clawU:
	begin
		w_motor <= 4'b1111; // Motor 1 and 2 - Stopped
		dir = c_up;
	end	
endcase
end

always @ (posedge clk)
begin 

if(rst)
	cur_state <= init;

case(cur_state)
	init:
	begin
		if(rasp_on)
			cur_state <= stop;
		else
			cur_state <= init;
	end
	
	stop:
	begin
		if(obj_isClose)
			cur_state <= clawD;
		else if(obj_inSight)
		begin
			time_begin = timer;
			cur_state <= foward;
		end
		else if(!obj_inSight)
		begin
			time_begin = timer;
			cur_state <= turn;
		end
	end
	
	foward:
	begin
		if(n_turns > 0)
		begin
			time_begin = timer;
			cur_state <= foward;
		end
		else if(!obj_inSight)
		begin
			time_begin = timer;
			cur_state <= turn;
		end
		else if(obj_isClose)
			cur_state <= stop;
	end
	
	turn:
	begin
		if(obj_inSight || n_turns > 3)
		begin
			time_begin = timer;
			cur_state <= foward;
		end
			
	end
	
	clawD:
	begin
		time_begin = timer;
		if(timer > time_begin + claw_downTime)
			cur_state <= clawU;
	end
	
	clawU:
	begin
		time_begin = timer;
		if(timer > time_begin + claw_upTime)
			cur_state <= stop;
	end
	
endcase
end


endmodule