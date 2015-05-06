_isAtJunction = 0;
_randomInt = 1;
_collideState = 0;
_canWalk = {[1] = 0, [2] = 0, [3] = 0, [4] = 0};

function isAtJunction()
	return _isAtJunction;
end

function getRandomInt(_min, _max)
	return _randomInt;
end

function getCollideState()
	return _collideState;
end

function setDesire(desire)
	_desire = desire;
end

function canWalk(dir)
	return _canWalk[dir];
end
