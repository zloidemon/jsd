--[[
-- @file sphinx.lua
--
-- @brief Sphinx stats paarser 
--
]]--

local full = {
  dist  = {},
  ag   = {},
  comm = {},
  avg  = {}
}

function parse(string, num) 
	if string.find(string, "avg_") then
    full.avg[string] = tonumber(num)
  elseif string.find(string, "dist_") then
    full.dist[string] = tonumber(num)
  elseif string.find(string, "ag_") then
    full.ag[string] = tonumber(num)
  elseif string.find(string, "command_") then
    full.comm[string] = tonumber(num)
  else
    full[string] = tonumber(num)
  end
end

function stat()
  return(cjson.encode(full))
end
