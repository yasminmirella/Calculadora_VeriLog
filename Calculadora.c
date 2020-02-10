// 50MHz
module ULA_LCD(clk, rs, rw, en, dados, SOMA, SUBT, MULT, IGUAL, A, B, sinalA, sinalB);
    
	input [7:0] A;
	input [7:0] B;
	input sinalA;
	input sinalB;
	input SOMA;
	input SUBT;
	input MULT;
	input IGUAL;
	output en;
	output rw;
	output rs;
	output [7:0] dados;
	input clk;
	reg rs = 0;
	reg rw =0;
	reg en = 0;
	reg [7:0] dados;
	reg [21:0] c45;
	reg [21:0] c5;
	reg [21:0] c25;
	integer c1=0;
	integer iniciar=0;
	reg flag45;
	reg flag5;
	reg flag2meio;
	reg [9:0] est2;
	reg flagbt1;
	reg flagbt2;
	reg flagbt3;
	reg flagbt4;
	reg flag_rst;
	reg [31:0] estado;
	reg subest = 0;
	reg [5:0] charPos;
	reg [1:0] operacao;
	reg menos;
	reg [7:0] Alinha;
	reg [7:0] Blinha;
	reg menosA;
	reg menosB;
	reg menosR;
	
	
	initial flag_rst = 0;
	initial flag45 = 0;
	initial flag5 = 0;
	initial flag2meio = 0;
	initial est2 = 0;
	initial flagbt1 = 0;
	initial flagbt2 = 0;
	initial flagbt3 = 0;
	initial flagbt4 = 0;
	initial estado = 0;
	initial c45 = 0;
	initial c5 = 0;
	initial c25 = 0;
	initial menos = 0;
    
	parameter clear = 1, opSOMA=1, opSUBT=2, opMULT=3, opIGUAL=4, skipLn=192, WAIT=10, WRITE=11, DEFAULT=6;
    
    
	always @(posedge clk) begin
    	if(flag_rst) begin
        	c45 = 0;
        	c25 = 0;
        	c5 = 0;
        	flag5 = 0;
        	flag45 = 0;
        	flag2meio = 0;
    	end
    	if(c45 == 2250000) begin
            	c45 = 0;
            	flag45 = 1;
    	end
    	else if (c45 < 2250000) begin
        	c45 = c45+1;
    	end
    	if(c5 == 250000) begin
        	c5 = 0;
        	flag5 = 1;
    	end
    	else if (c5 < 250000) begin
        	c5 = c5 + 1;
    	end
    	if(c25 == 125000) begin
			c25 = 0;
			flag2meio = 1;
		end
		else if(c25 < 125000) begin
			c25 = c25 + 1;
		end
	end
    
    
	always @(posedge clk) begin
    	case(estado)
   		 
			0: begin
				if(flag45) begin
					estado <= 1;
					subest <= 0;
				end
				else begin
					rs <= 0;
					rw <= 0;
					en <= 0;
				end
			end
   		 
        	1: begin
   			 if(subest == 0) begin
   				 en <= 0;
   				 subest <= 1;
   				 flag_rst <= 1;
   			 end
   			 else if(subest == 1) begin
   				 if(flag5) begin
   					 c1 = c1+1;
   					 rs <= 0;
   					 rw <= 0;
   					 en <= 1;
   					 dados <= 8'b00111000;
   					 flag_rst <= 1;
   					 subest = 0;
   					 estado <= 20;
   					 est2 <= 1;
   				 end
   				 else begin
   					 en <= 0;
   					 flag_rst <= 0;
   				 end
   				 if(c1 == 4) begin
   					 flag_rst <= 1;
   					 estado <= 20;
   					 subest <= 0;
   					 est2 <= 2;
   				 end
   			 end
           	 
           	 
        	end
       	 
        	20: begin
				en <= 1;
				if(subest == 0) begin
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag2meio) begin
						estado <= est2;
						subest <= 0;
					end
					else begin
						flag_rst <= 0;
					end
				end
    	
			end
        	
        	2: begin
   			 if(subest == 0) begin
   				 en <= 0;
   				 subest <= 1;
   				 flag_rst <= 1;
   			 end
   			 
   			 else if(subest == 1) begin  	
				if(flag5) begin
					 rs <= 0;
					 rw <= 0;
					 en <= 1;
					 dados <= 8'b00001100;
					 flag_rst <= 1;
					 subest <= 0;
					 estado <= 20;
					 est2 <= 3;	
				end
				else flag_rst <= 0; 
   			 end
           	 
        	end
       	 
        	3: begin
   			 if(subest == 0) begin
   				 en <= 0;
   				 subest <= 1;
   				 flag_rst <= 1;
   			 end
   			 
   			 if(subest == 1) begin
   				 if(flag5) begin
   					 dados <= 1;
   					 flag_rst <= 1;
   					 subest <= 0;
   					 estado <= 20;
   					 est2 <= 4;
   				 end
   				 else begin
   					 en <= 0;
   					 flag_rst <= 0;
   				 end
   			 end
   			 
           	 
        	end
       	 
        	4: begin
				 if(subest == 0) begin
					 en <= 0;
					 subest <= 1;
					 flag_rst <= 1;
				 end
				 
				 if(subest == 1) begin
					 if(flag5) begin
						 rs <= 1'b0;
						 rw <= 1'b0;
						 en <= 1'b1;
						 dados <= 8'b0000110;
						 flag_rst <= 1;
						 subest <= 0;
						 estado <= 20;
						 est2 <= 5;
					 end
					 else begin
						 en <= 0;
						 flag_rst <= 0;
					 end
				 end
           	 
        	end
        	
        	5: begin
				if(subest == 0) begin
					dados <= 0;
					 en <= 0;
					 subest <= 1;
					 flag_rst <= 1;
				 end
				 
				 if(subest == 1) begin
					 if(flag5) begin
						 subest <= 0;
						 estado <= 6; 
					 end
					 else begin
						 en <= 0;
						 flag_rst <= 0;
					 end
				 end
			end
			
			6: begin
				en <= 0;
				if(!SOMA & !flagbt1) begin
					flagbt1 = 1;
				end
				else if(SOMA & flagbt1) begin
					subest <= 0;
					estado <= 9;
					charPos <= 20;
					flagbt1 <= 0;
					flag_rst <= 1;
					menosA <= 1;
					menosB <= 1;
					operacao = opSOMA;
				end
				 
				if(!SUBT & !flagbt2) begin
					flagbt2 = 1;
				end
				else if(SUBT & flagbt2) begin
					subest <= 0;
					estado <= 17;
					charPos <= 20;
					flag_rst <= 1;
					flagbt2 <= 0;
					menosA <= 1;
					menosB <= 1;
					operacao = opSUBT;
				end
				 
				if(!MULT & !flagbt3) begin
					flagbt3 = 1;
				end
				else if(MULT & flagbt3) begin
					subest <= 0;
					estado <= 25;
					flagbt3 = 0;
					flag_rst <= 1;
					operacao = opMULT;
					menosA <= 1;
					menosB <= 1;
					charPos <= 20;
				end
				 
				if(!IGUAL & !flagbt4) begin
					flagbt4 = 1;
				end
				else if(IGUAL & flagbt4) begin
					charPos <= 0;
					subest <= 0;
					estado <= 15;
					flagbt4 = 0;
				end
   			 
			end
        	
			9: begin
				if(subest == 0) begin
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						rs <= 0;
						rw <= 0;
						en <= 1;
						dados <= 1;
						subest <= 0;
						flag_rst <= 1;
						estado <= 20;
						est2 <= 10;
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end
			
			end
			
			
        	10: begin
				en <= 0;
				if(subest == 0) begin
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						subest <= 0;
						flag_rst <= 1;
						estado <= 11;
					end
					else begin
						flag_rst <= 0;
					end
				end
   			end
   			 
   			
   			11: begin
				if (charPos == 20) begin
					rs <= 0;
					rw <= 0;
					en <= 1;
					dados <= 1;
					charPos <= 0;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end	
				else if(charPos == 0) begin
					if(sinalA & menosA) begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= 45;
						estado <= 20;
						est2 <= 10;
						subest <= 0;
						menosA <= 0;
					end
					else begin 
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= ((A/100) + 48);
						charPos <= 1;
						estado <= 20;
						est2 <= 10;
						subest <= 0;
					end
				end
				else if(charPos == 1) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= (((A/10)%10) + 48);
					charPos <= 2;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 2) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((A%10) + 48);
					charPos <= 3;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 3) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 4;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 4) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 43;
					charPos <= 5;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 5) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 6;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 6) begin
					if(sinalB & menosB) begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= 45;
						estado <= 20;
						est2 <= 10;
						subest <= 0;
						menosB <= 0;
					end
					else begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= ((B/100) + 48);
						charPos <= 7;
						estado <= 20;
						est2 <= 10;
						subest <= 0;
					end
					
				end
				else if(charPos == 7) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((B/10)%10 + 48);
					charPos <= 8;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 8) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((B%10) + 48);
					charPos <= 9;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 9) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 10;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 10) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 11;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 11) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 12;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 12) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 13;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 13) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 14;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 14) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 15;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 15) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 17;
					estado <= 20;
					est2 <= 10;
					subest <= 0;
				end
				else if(charPos == 17) begin
					en <= 0;
					estado <= 20;
					est2 <= 10;
					charPos <= 18;
					subest <= 0;
				end
				else if(charPos == 18) begin
					estado <= 6;
					subest <= 0;
				end
   		 
			end
   			  
   			15: begin
				Alinha = A;
				Blinha = B;
				if(operacao == opSOMA) begin
					if(sinalA & !sinalB) begin
						operacao = opSUBT;
						if(A > B) begin
							menos = 1;
						end
						else begin
							menos = 0;
							Alinha = B;
							Blinha = A;
						end
					end
					else if(!sinalA & sinalB) begin
						operacao = opSUBT;
						
						if(B > A) begin
							menos = 1;
							Alinha = B;
							Blinha = A;
						end
						else menos = 0;
					end
					else if(sinalA & sinalB) begin
						menos = 1;
					end
					else menos = 0;
				end
				else if(operacao == opSUBT) begin
					if(sinalA & !sinalB) begin
						operacao = opSOMA;
						menos = 1;
					end
					else if(!sinalA & sinalB) begin
						operacao = opSOMA;
						menos = 0;
					end
					else if(sinalA & sinalB) begin
						if(A > B) begin
							menos = 1;
						end
						else begin
							menos = 0;
							Alinha = B;
							Blinha = A;
						end
					end
					else begin
						if(B > A) begin
							menos = 1;
							Alinha = B;
							Blinha = A;
						end
						else menos = 0;
					end
				end
				case(operacao) 
					opSOMA: begin
						if(subest == 0) begin
							en <= 0;
							subest <= 1;
							flag_rst <= 1;
						end
						else if(subest == 1) begin
							if(flag5) begin
								rs <= 0;
								rw <= 0;
								en <= 1;
								subest <= 0;
								estado <= 20;
								est2 <= 13;
								dados <= 8'b11000000;
							end
							else begin
								flag_rst <= 0;
								en <= 0;
							end
						end
					end
					
					opSUBT: begin
						if(subest == 0) begin
							en <= 0;
							subest <= 1;
							flag_rst <= 1;
						end
						else if(subest == 1) begin
							if(flag5) begin
								rs <= 0;
								rw <= 0;
								en <= 1;
								subest <= 0;
								estado <= 20;
								est2 <= 18;
								dados <= 8'b11000000;
							end
							else begin
								flag_rst <= 0;
								en <= 0;
							end
						end
					end
					
					opMULT: begin
						if(subest == 0) begin
							en <= 0;
							subest <= 1;
							flag_rst <= 1;
						end
						else if(subest == 1) begin
							if(flag5) begin
								rs <= 0;
								rw <= 0;
								en <= 1;
								subest <= 0;
								estado <= 20;
								est2 <= 21;
								dados <= 8'b11000000;
								if(sinalA & !sinalB || !sinalA & sinalB) begin
									menos = 1;
								end
								else begin
									menos = 0;
								end
							end
							else begin
								flag_rst <= 0;
								en <= 0;
							end
						end
					end
				endcase		
			end
			
			13: begin
				if(subest == 0) begin
					en <= 0;
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						subest <= 0;
						estado <= 14;
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end
			end
			
			14: begin
				if(subest == 0) begin
					en <= 0;
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						if(charPos == 0) begin
							subest <= 0;
							estado <= 20;
							est2 <= 13;
							charPos <= 1;
						end
						else if(charPos == 1) begin
							if(menos) begin
								menos = 0;
								rs <= 1;
								rw <= 0;
								en <= 1;
								dados <= 45;
								estado <= 20;
								est2 <= 13;
								subest <= 0;
							end
							else begin
								rs <= 1;
								rw <= 0;
								en <= 1;
								dados <= ((A+B)/100 + 48);
								subest <= 0;
								charPos <= 2;
								estado <= 20;
								est2 <= 13;
							end
						end
						else if(charPos == 2) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= ((((A+B)/10)%10) + 48);
							subest <= 0;
							charPos <= 3;
							estado <= 20;
							est2 <= 13;
						end
						else if(charPos == 3) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= (((A+B)%10) + 48);
							subest <= 0;
							charPos <= 4;
							estado <= 20;
							est2 <= 13;
						end
						else if(charPos == 4) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= 32;
							subest <= 0;
							charPos <= 5;
							estado <= 20;
							est2 <= 13;
						end
						else if(charPos == 5) begin
							subest <= 0;
							estado <= 20;
							est2 <= 6;
						end
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end
			end
			
			16: begin
				en <= 0;
				if(subest == 0) begin
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						subest <= 0;
						flag_rst <= 1;
						estado <= 17;
					end
					else begin
						flag_rst <= 0;
					end
				end
			end
			
			17: begin
				if (charPos == 20) begin
					rs <= 0;
					rw <= 0;
					en <= 1;
					dados <= 1;
					charPos <= 0;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end	
				else if(charPos == 0) begin
					if(sinalA & menosA) begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= 45;
						estado <= 20;
						est2 <= 16;
						subest <= 0;
						menosA <= 0;
					end
					else begin 
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= ((A/100) + 48);
						charPos <= 1;
						estado <= 20;
						est2 <= 16;
						subest <= 0;
					end
				end
				else if(charPos == 1) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= (((A/10)%10) + 48);
					charPos <= 2;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 2) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((A%10) + 48);
					charPos <= 3;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 3) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 4;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 4) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 45;
					charPos <= 5;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 5) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 6;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 6) begin
					if(sinalB & menosB) begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= 45;
						estado <= 20;
						est2 <= 16;
						subest <= 0;
						menosB <= 0;
					end
					else begin 
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= ((B/100) + 48);
						charPos <= 7;
						estado <= 20;
						est2 <= 16;
						subest <= 0;
					end
				end
				else if(charPos == 7) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((B/10)%10 + 48);
					charPos <= 8;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 8) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((B%10) + 48);
					charPos <= 9;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 9) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 10;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 10) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 11;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 11) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 12;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 12) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 13;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 13) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 14;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 14) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 15;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 15) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 16;
					estado <= 20;
					est2 <= 16;
					subest <= 0;
				end
				else if(charPos == 16) begin
					en <= 0;
					estado <= 20;
					est2 <= 16;
					charPos <= 17;
					subest <= 0;
				end
				else if(charPos == 17) begin
					estado <= 6;
					subest <= 0;
				end
			end
			
			18: begin
				if(subest == 0) begin
					en <= 0;
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						subest <= 0;
						estado <= 19;
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end
			end
			
			19: begin
				if(subest == 0) begin
					en <= 0;
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						if(charPos == 0) begin
							subest <= 0;
							estado <= 20;
							est2 <= 18;
							charPos <= 1;
						end
						else if(charPos == 1) begin
							if(menos) begin
								menos = 0;
								rs <= 1;
								rw <= 0;
								en <= 1;
								dados <= 45;
								estado <= 20;
								est2 <= 18;
								subest <= 0;
							end
							else begin
								rs <= 1;
								rw <= 0;
								en <= 1;
								dados <= ((Alinha-Blinha)/100 + 48);
								subest <= 0;
								charPos <= 2;
								estado <= 20;
								est2 <= 18;
							end
						end
						else if(charPos == 2) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= ((((Alinha-Blinha)/10)%10) + 48);
							subest <= 0;
							charPos <= 3;
							estado <= 20;
							est2 <= 18;
						end
						else if(charPos == 3) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= (((Alinha-Blinha)%10) + 48);
							subest <= 0;
							charPos <= 4;
							estado <= 20;
							est2 <= 18;
						end
						else if(charPos == 4) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= 32;
							subest <= 0;
							charPos <= 5;
							estado <= 20;
							est2 <= 18;
						end
						else if(charPos == 5) begin
							subest <= 0;
							estado <= 20;
							est2 <= 6;
						end
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end

			end
			
			21: begin
				if(subest == 0) begin
					en <= 0;
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						subest <= 0;
						estado <= 22;
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end
			end
			
			22: begin
				if(subest == 0) begin
					en <= 0;
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						if(charPos == 0) begin
							subest <= 0;
							estado <= 20;
							est2 <= 21;
							charPos <= 1;
						end
						else if(charPos == 1) begin
							if(menos) begin
								menos = 0;
								rs <= 1;
								rw <= 0;
								en <= 1;
								dados <= 45;
								estado <= 20;
								est2 <= 21;
								subest <= 0;
							end
							else begin
								rs <= 1;
								rw <= 0;
								en <= 1;
								dados <= ((A*B)/10000 + 48);
								subest <= 0;
								charPos <= 2;
								estado <= 20;
								est2 <= 21;
							end
							
						end
						else if(charPos == 2) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= ((((A*B)/1000)%10) + 48);
							subest <= 0;
							charPos <= 3;
							estado <= 20;
							est2 <= 21;
						end
						else if(charPos == 3) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= (((A*B/100)%10) + 48);
							subest <= 0;
							charPos <= 4;
							estado <= 20;
							est2 <= 21;
						end
						else if(charPos == 4) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= (((A*B/10)%10) + 48);
							subest <= 0;
							charPos <= 5;
							estado <= 20;
							est2 <= 21;
						end
						else if(charPos == 5) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= ((A*B)%10) + 48;
							subest <= 0;
							charPos <= 6;
							estado <= 20;
							est2 <= 21;
						end
						else if(charPos == 6) begin
							rs <= 1;
							rw <= 0;
							en <= 1;
							dados <= 32;
							subest <= 0;
							charPos <= 7;
							estado <= 20;
							est2 <= 21;
						end
						else if(charPos == 7) begin
							subest <= 0;
							estado <= 20;
							est2 <= 6;
						end
					end
					else begin
						flag_rst <= 0;
						en <= 0;
					end
				end

			end
   		 
			24: begin
				en <= 0;
				if(subest == 0) begin
					subest <= 1;
					flag_rst <= 1;
				end
				else if(subest == 1) begin
					if(flag5) begin
						subest <= 0;
						flag_rst <= 1;
						estado <= 25;
					end
					else begin
						flag_rst <= 0;
					end
				end
			end
			
			25: begin
				if (charPos == 20) begin
					rs <= 0;
					rw <= 0;
					en <= 1;
					dados <= 1;
					charPos <= 0;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end	
				else if(charPos == 0) begin
					if(sinalA & menosA) begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= 45;
						estado <= 20;
						est2 <= 24;
						subest <= 0;
						menosA <= 0;
					end
					else begin 
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= ((A/100) + 48);
						charPos <= 1;
						estado <= 20;
						est2 <= 24;
						subest <= 0;
					end
				end
				else if(charPos == 1) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= (((A/10)%10) + 48);
					charPos <= 2;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 2) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((A%10) + 48);
					charPos <= 3;
					estado <= 20;
					est2 <=24;
					subest <= 0;
				end
				else if(charPos == 3) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 4;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 4) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 42;
					charPos <= 5;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 5) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 6;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 6) begin
					if(sinalB & menosB) begin
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= 45;
						estado <= 20;
						est2 <= 24;
						subest <= 0;
						menosB <= 0;
					end
					else begin 
						rs <= 1;
						rw <= 0;
						en <= 1;
						dados <= ((B/100) + 48);
						charPos <= 7;
						estado <= 20;
						est2 <= 24;
						subest <= 0;
					end
				end
				else if(charPos == 7) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((B/10)%10 + 48);
					charPos <= 8;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 8) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= ((B%10) + 48);
					charPos <= 9;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 9) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 10;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 10) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 11;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 11) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 12;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 12) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 13;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 13) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 14;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 14) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 15;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 15) begin
					rs <= 1;
					rw <= 0;
					en <= 1;
					dados <= 32;
					charPos <= 16;
					estado <= 20;
					est2 <= 24;
					subest <= 0;
				end
				else if(charPos == 16) begin
					en <= 0;
					estado <= 20;
					est2 <= 24;
					charPos <= 17;
					subest <= 0;
				end
				else if(charPos == 17) begin
					estado <= 6;
					subest <= 0;
				end
			end
    	endcase
	end
    
    
	always @(iniciar)begin
   	 
	end
    
endmodule
