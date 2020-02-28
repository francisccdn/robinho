module robinho 
(
input rst,
input clk,
input rasp_on,
input obj_inSight,
input obj_isClose,
output reg [0:3]w_motor,
output reg [0:1]c_motor,
output reg [0:1]c_close;
)
integer n_turns;

//States
reg cur_state = init;
parameter init, stop, foward, turn, clawD, clawU, clawOpen, clawClose;

//Internal clock
reg timer = 0;
reg time_begin;
parameter claw_downTime, claw_upTime, claw_openTime, claw_closeTime, turn_time;

always @ (posedge clk)
begin
	timer <= timer + 1;
end

always @ (*)
begin
case(cur_state)
	init:
	begin
		w_motor <= 0;
		c_motor <= 0;
	end
	
	stop:
	begin
		w_motor <= 0;
		c_motor <= 0;
	end
	
	foward:
	begin
		if(timer >= time_begin + turn_time)
		begin
			time_begin <= timer
			n_turns <= n_turns - 1;
		end
			
		w_motor[0] <= 0;
		w_motor[1] <= 1;
		w_motor[2] <= 0;
		w_motor[3] <= 1;
		c_motor <= 0;
	end
	
	turn:
	begin
		if(timer >= time_begin + turn_time)
		begin
			time_begin <= timer
			n_turns <= n_turns + 1;
		end
		
		w_motor[0] <= 1;
		w_motor[1] <= 0;
		w_motor[2] <= 0;
		w_motor[3] <= 1;
		c_motor <= 0;
	end
	
	clawD:
	begin
		w_motor <= 0;
		c_motor[0] <= 1;
		c_motor[1] <= 0;	
	end
	
	clawU:
	begin
		w_motor <= 0;
		c_motor[0] <= 0;
		c_motor[1] <= 1;	
	end
	
	clawClose:
	begin
		w_motor <= 0;
		c_motor[0] <= 0;
		c_motor[1] <= 0;
		c_close <= 1;
	end
	
	clawOpen:
	begin
		w_motor <= 0;
		c_motor[0] <= 0;
		c_motor[1] <= 0;
		c_close <= 0;	
	end
	// Outputs temporarios (ate descobrir como o motor funciona)
	
endcase
end

always @ (posedge clk)
begin 

if(rst)
	cur_state <= clawOpen;

case(cur_state)
	clawOpen:
	begin
		time_begin = timer;
		if(timer > time_begin + claw_openTime)
			cur_state <= init;
	end
	
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
			cur_state <= clawClose;
	end
	
	clawClose:
	begin
		time_begin = timer;
		if(timer > time_begin + claw_closeTime)
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