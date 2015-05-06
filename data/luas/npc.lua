-- NPC Handler

states = {
	CRATE_BOMBING = 1,
	PWRUP_HUNTING = 2,
	ENEMY_KILLING = 3
};

desires = {
	NOTHING = 0,
	GOUP = 1,
	GODOWN = 2,
	GOLEFT = 3,
	GORIGHT = 4,
	BOMB = 5
};

directions = {
	NONE = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

oppositeDirections = {
	[directions.NONE] = directions.NONE,
	[directions.UP] = directions.DOWN,
	[directions.DOWN] = directions.UP,
	[directions.LEFT] = directions.RIGHT,
	[directions.RIGHT] = directions.LEFT
};
	

state = 0;
desire = 0;
direction = 0;

inited = false;
counter = 100;

function init()
	if(not inited) then
		print("Script initialization.");
		-- Further initing here
		state = states.CRATE_BOMBING;
		desire = desires.NOTHING;
		direction = getRandomInt(1, 4);
		inited = true;
	else
		print("Warning: Initialized attempted multiple times. Ignoring.");
	end
end

function reset()
	if(inited) then
		print("Script reset requested.")
		inited = false;
		init();
	else
		print("Warning: Reset requested but script never inited. Ignoring.");
	end
end

function tick()
	if(not inited) then
		print("Warning: Script ticked but never inited. Ignoring tick.");
		return 0;
	end
	
	if(isAtJunction() == 1) then
		dirs = {
			[directions.UP] = canWalk(directions.UP),
			[directions.DOWN] = canWalk(directions.DOWN),
			[directions.LEFT] = canWalk(directions.LEFT),
			[directions.RIGHT] = canWalk(directions.RIGHT)
		}
		dirs[oppositeDirections[direction]] = 0;
		
		repeat
			newDir = getRandomInt(1, 4);
		until dirs[newDir] ~= 0;
		
		direction = newDir;
	end
	
	if(getCollideState() == 1) then
		direction = getRandomInt(1, 4);
	end
	
	
	if(getRandomInt(0, 100) < 10) then
		setDesire(desires.BOMB);
	else
		setDesire(direction);
	end
end
	
	