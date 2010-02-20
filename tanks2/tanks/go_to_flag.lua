require("tanks/astar")

function turn ()

	team = get_team_identifier()
	
	flags = get_flags()
	
	enemy_flag = nil
	
	for flag_team, location in pairs(flags) do
		if flag_team ~= team then	
			enemy_flag = location
			break
		end
	end
	
	next_move = astar(get_x(), get_y(), enemy_flag['x'], enemy_flag['y'])
	
	if next_move ~= false then
		x = next_move['x'] - get_x()
		y = next_move['y'] - get_y()
		move(x, y)
	end
	
end
