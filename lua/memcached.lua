--[[
-- @file memcached.lua
--
-- @brief Memacached stats paarser 
--
]]--

full = {}

function parse(string, num) 
--[[ FIX ME
-- Need add rules for parsing version and another strings
]]--
  full[string] = tonumber(num)
end

function stat()
  return(cjson.encode(full))
end
