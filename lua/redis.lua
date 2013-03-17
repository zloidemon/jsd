--[[
-- @file redis.lua
--
-- @brief Redis stats parser
--
]]-- 

local full = {
  cpu     = {},
  mem     = {},
  stats   = {},
  clients = {},
  server  = {},
  repl    = {},
  pers    = {}
}

function parse(string, section)
  if   section == "CPU"
    or section == "MEMORY"
    or section == "STATS"
    or section == "CLIENTS"
    or section == "SERVER"
    or section == "REPLICATION"
    or section == "PERSISTENCE"
    then

    for rstat, num in string.gmatch(string, "(%S*):(%w+)") do
      if section == "CPU" then
        full.cpu[rstat] = tonumber(num)
      elseif section == "MEMORY" then
        if rstat == "mem_allocator" then
          full.mem[rstat] = num
        else
          full.mem[rstat] = tonumber(num)
        end
      elseif section == "STATS" then
        full.stats[rstat] = tonumber(num)
      elseif section == "CLIENTS" then
        full.clients[rstat] = tonumber(num)
      elseif section == "SERVER" then
        full.server[rstat] = tonumber(num)
      elseif section == "REPLICATION" then
        if rstat == "role" then
          full.repl[rstat] = num
        else
          full.repl[rstat] = tonumber(num)
        end
      elseif section == "PERSISTENCE" then
        full.pers[rstat] = tonumber(num)
      end
    end
  else
      print("empty")
  end
end

function stat()
  return(cjson.encode(full))
end
