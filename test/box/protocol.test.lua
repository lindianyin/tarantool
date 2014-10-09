box.schema.user.grant('guest', 'read,write,execute', 'universe')

--------------------------------------------------------------------------------
-- Test case for #273: IPROTO_ITERATOR ignored in network protocol
--------------------------------------------------------------------------------

space = box.schema.create_space('tweedledum')
index = space:create_index('primary', { type = 'tree'})
for i=1,5 do space:insert{i} end

LISTEN = require('uri').parse(box.cfg.listen)
LISTEN ~= nil
conn = (require 'net.box'):new(LISTEN.host, LISTEN.service)
conn.space[space.id]:select(3, { iterator = 'GE' })
conn.space[space.id]:select(3, { iterator = 'LE' })
conn.space[space.id]:select(3, { iterator = 'GT' })
conn.space[space.id]:select(3, { iterator = 'LT' })
conn:close()

space:drop()
