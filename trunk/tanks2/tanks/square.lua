turn_number = 0



function turn ()
	if turn_number == 0 then
		if not move(1, 0) then
			turn_number = 1
		end
	elseif turn_number == 1 then
		if not move(0, 1) then
			turn_number = 2
		end
	elseif turn_number == 2 then
		if not move(-1, 0) then
			turn_number = 3
		end
	elseif turn_number == 3 then
		if not move(0, -1) then
			turn_number = 0
		end
	end
	
end
