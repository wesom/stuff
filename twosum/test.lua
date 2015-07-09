local tc = require "twosum.core"

function dump(tb)
	for k, v in ipairs(tb) do
		print(k, v)
	end
end

local tbl = {}

math.randomseed(os.time())
for i=1, 10 do
	table.insert(tbl, math.random(-10000, 10000))
end

dump(tbl)

local target = tbl[4] + tbl[7]
print("target : ", target)
local index1, index2 = tc.twosum(tbl, target)
print("index1 : ", index1, "index2 : ", index2)
