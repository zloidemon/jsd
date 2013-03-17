--[[
-- @file config.lua
--
-- @brief Simple configuration
]]-- 

function config(server)
  if server == "server" then
    port     = 3333
    host     = "127.0.0.1"
    handler  = "/admin.jsp"
  elseif server == "redis" then
    host     = "127.0.0.1"
    port     = 6379
    handler  = "lua/redis.lua"
  elseif server == "memc" then
    port     = 11211
    host     = "127.0.0.1"
    handler  = "lua/memcached.lua"
  elseif server == "mysql" then
    database = "mysql"
    password = "some_pass"
    host     = "127.0.0.1"
    port     = 3306
    user     = "admin"
    handler  = "lua/mysql.lua"
  elseif server == "sphinx" then
    port = 9306
    host = "127.0.0.1"
    handler = "lua/sphinx.lua"
  else
    port = 3333
    host = "127.0.0.1"
  end
end
