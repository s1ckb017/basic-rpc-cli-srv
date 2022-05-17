from impacket.uuid import uuidtup_to_bin
from impacket.dcerpc.v5.ndr import NDRCALL
from impacket.dcerpc.v5.rpcrt import RPC_C_AUTHN_WINNT
from impacket.dcerpc.v5.rpcrt import RPC_C_AUTHN_LEVEL_CONNECT
from impacket.dcerpc.v5.transport import DCERPCTransportFactory
from impacket.dcerpc.v5.dtypes import STR
from impacket.dcerpc.v5.dtypes import UINT
from impacket.structure import Structure

import sys

class Hello_(NDRCALL):
    structure = (
        ('string', STR),
    )

protoseq = "ncacn_ip_tcp"
ip = sys.argv[1]
port = sys.argv[2]

# ncacn_ip_tcp
binding = protoseq+':' + ip + '[' + port + ']'
rpctransport = DCERPCTransportFactory(binding)
rpctransport.set_dport(port)
if hasattr(rpctransport, 'set_credentials'):
    # This method exists only for selected protocol sequences.
    rpctransport.set_credentials('John', 'Passw0rd!', '')
dce = rpctransport.get_dce_rpc()

dce.set_auth_type(RPC_C_AUTHN_WINNT)
dce.set_auth_level(RPC_C_AUTHN_LEVEL_CONNECT)
# in this way we can send 32 bytes of data + the usual 24 bytes dce rpc fixed fields
dce.set_max_fragment_size(32)
dce.connect()
dce.bind(uuidtup_to_bin(('a29b0684-dd1f-49fc-8d41-760c344ad111', '1.0')))
h = Hello_()
# expected three packets first frag set; ; first and last frag flags set
h['string'] = "H"*32+"B"*32+"C"*31+"\x00"
dce.call(0, h['string'])
dce.disconnect()
