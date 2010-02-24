--[[
	This is an incomplete implementation of the A* Pathfinding
	algorithm
--]]




function min_from_open_set (open_set)

	min_score = 1000000000
	min_key = nil

	for key, value in pairs(open_set) do
		if value['score'] < min_score then
			min_score = value['score']
			min_key = key
		end
	end
	
	return min_key

end



--[[
	needle must be formatted at least as a table with x and y keys
--]]
function location_in_set (needle, haystack)
	
	for key, v in pairs(haystack) do
		if needle['x'] == v['x'] and needle['y'] == v['y'] then
			return key
		end
	end

	return false

end
	



function neighbor_locations (x, y)

	result = {}
	
	if location_free (x - 1, y) then
		row = {}
		row['x'] = x - 1
		row['y'] = y
		table.insert(result, row)
	end
	if location_free (x + 1, y) then
		row = {}
		row['x'] = x + 1
		row['y'] = y
		table.insert(result, row)
	end
	if location_free (x, y - 1) then
		row = {}
		row['x'] = x
		row['y'] = y - 1
		table.insert(result, row)
	end
	if location_free (x, y + 1) then
		row = {}
		row['x'] = x
		row['y'] = y + 1
		table.insert(result, row)
	end
	
	return result

end



--[[
	a location in open_set is
	location[x] = x location
	location[y] = y location
	location[score] = score/weight of this location
	
	closeness is how close we have to be to the target
--]]
function astar (source_x, source_y, dest_x, dest_y, closeness)

	closeness = closeness or 0

	closed_set = {}
	open_set = {}
	
	first_location = {}
	first_location['x'] = source_x
	first_location['y'] = source_y
	first_location['score'] = 0
	table.insert(open_set, first_location)

	while # open_set > 0 do
		
		location_to_test_key = min_from_open_set(open_set)
		
		location = open_set[location_to_test_key]
		if location['x'] >= dest_x - closeness and
		   location['x'] <= dest_x + closeness and 
		   location['y'] >= dest_y - closeness and
		   location['y'] <= dest_y + closeness then
			-- we keep tracing up the path until we find the parent who's
			-- x and y == source_x and source_y
			if location['x'] == source_x and location['y'] == source_y then
				return location
			end
			while location['parent']['x'] ~= source_x or location['parent']['y'] ~= source_y do
				location = location['parent']
			end
			return location
		end
		
		table.remove(open_set, location_to_test_key)
		table.insert(closed_set, location)
		
		for key, neighbor_location in pairs(neighbor_locations(location['x'], location['y'])) do
			-- insure this location does not exist in the closed set
			if location_in_set(neighbor_location, closed_set) == false then
				-- insure this location does not exist in the open set
				location_in_open_set = location_in_set(neighbor_location, open_set)
				if location_in_open_set == false then
					neighbor_location['score'] = location['score'] + 1
					neighbor_location['parent'] = location
					
					-- insert this location in to the open set
					table.insert(open_set, neighbor_location)
				end
				-- if it is in the open_set, and this is a better parent, set it as such
				-- maybe another day
			end
		end
	end
	
	-- If we get here, we've searched the entire map, and we can't reach the destination
	-- It's time for drastic measures
	
	
	return false
	
end
