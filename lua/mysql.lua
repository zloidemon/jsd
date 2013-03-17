--[[
-- @file mysql.lua
--
-- @brief MySQL stats parser
--
]]--

local full = {
  com  = {},
  inon = {},
  ssl  = {},
  hand = {},
  thr  = {},
  qcsh = {},
  perf = {},
  key  = {},
}

function parse( string, num )
	if string.find(string, "Com_") then
    full.com[string] = tonumber(num)
  elseif string.find(string, "Innodb_") then
    full.inon[string] = tonumber(num)
  elseif string.find(string, "Ssl_") then
    full.ssl[string] = tonumber(num)
  elseif string.find(string, "Handler_") then
    full.hand[string] = tonumber(num)
  elseif string.find(string, "Threads_") then
    full.thr[string] = tonumber(num)
  elseif string.find(string, "Qcache_") then
    full.qcsh[string] = tonumber(num)
  elseif string.find(string, "Performance_") then
    full.perf[string] = tonumber(num)
  elseif string.find(string, "Key_") then
    full.key[string] = tonumber(num)
  else
    full[string] = tonumber(num)
  end
end

function stat()
  return(cjson.encode(full))
end
