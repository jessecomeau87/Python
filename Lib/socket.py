# Wrapper module for _socket, providing some additional facilities
# implemented in Python.

"""\
This module provides socket operations and some related functions.
On Unix, it supports IP (Internet Protocol) and Unix domain sockets.
On other systems, it only supports IP. Functions specific for a
socket are available as methods of the socket object.

Functions:

socket() -- create a new socket object
socketpair() -- create a pair of new socket objects [*]
fromfd() -- create a socket object from an open file descriptor [*]
gethostname() -- return the current hostname
gethostbyname() -- map a hostname to its IP number
gethostbyaddr() -- map an IP number or hostname to DNS info
getservbyname() -- map a service name and a protocol name to a port number
getprotobyname() -- mape a protocol name (e.g. 'tcp') to a number
ntohs(), ntohl() -- convert 16, 32 bit int from network to host byte order
htons(), htonl() -- convert 16, 32 bit int from host to network byte order
inet_aton() -- convert IP addr string (123.45.67.89) to 32-bit packed format
inet_ntoa() -- convert 32-bit packed format IP to string (123.45.67.89)
ssl() -- secure socket layer support (only available if configured)
socket.getdefaulttimeout() -- get the default timeout value
socket.setdefaulttimeout() -- set the default timeout value
create_connection() -- connects to an address, with an optional timeout

 [*] not available on all platforms!

Special objects:

SocketType -- type object for socket objects
error -- exception raised for I/O errors
has_ipv6 -- boolean value indicating if IPv6 is supported

Integer constants:

AF_INET, AF_UNIX -- socket domains (first argument to socket() call)
SOCK_STREAM, SOCK_DGRAM, SOCK_RAW -- socket types (second argument)

Many other constants may be defined; these may be used in calls to
the setsockopt() and getsockopt() methods.
"""

import _socket
from _socket import *

_have_ssl = False
try:
    import _ssl
    from _ssl import *
    _have_ssl = True
except ImportError:
    pass

import os, sys, io

try:
    from errno import EBADF
except ImportError:
    EBADF = 9

__all__ = ["getfqdn"]
__all__.extend(os._get_exports_list(_socket))
if _have_ssl:
    __all__.extend(os._get_exports_list(_ssl))

# WSA error codes
if sys.platform.lower().startswith("win"):
    errorTab = {}
    errorTab[10004] = "The operation was interrupted."
    errorTab[10009] = "A bad file handle was passed."
    errorTab[10013] = "Permission denied."
    errorTab[10014] = "A fault occurred on the network??" # WSAEFAULT
    errorTab[10022] = "An invalid operation was attempted."
    errorTab[10035] = "The socket operation would block"
    errorTab[10036] = "A blocking operation is already in progress."
    errorTab[10048] = "The network address is in use."
    errorTab[10054] = "The connection has been reset."
    errorTab[10058] = "The network has been shut down."
    errorTab[10060] = "The operation timed out."
    errorTab[10061] = "Connection refused."
    errorTab[10063] = "The name is too long."
    errorTab[10064] = "The host is down."
    errorTab[10065] = "The host is unreachable."
    __all__.append("errorTab")


# True if os.dup() can duplicate socket descriptors.
# (On Windows at least, os.dup only works on files)
_can_dup_socket = hasattr(_socket, "dup")

if _can_dup_socket:
    def fromfd(fd, family=AF_INET, type=SOCK_STREAM, proto=0):
        nfd = os.dup(fd)
        return socket(family, type, proto, fileno=nfd)


class socket(_socket.socket):

    """A subclass of _socket.socket adding the makefile() method."""

    __slots__ = ["__weakref__"]
    if not _can_dup_socket:
        __slots__.append("_base")

    def __repr__(self):
        """Wrap __repr__() to reveal the real class name."""
        s = _socket.socket.__repr__(self)
        if s.startswith("<socket object"):
            s = "<%s.%s%s" % (self.__class__.__module__,
                              self.__class__.__name__,
                              s[7:])
        return s

    def accept(self):
        """Wrap accept() to give the connection the right type."""
        conn, addr = _socket.socket.accept(self)
        fd = conn.fileno()
        nfd = fd
        if _can_dup_socket:
            nfd = os.dup(fd)
        wrapper = socket(self.family, self.type, self.proto, fileno=nfd)
        if fd == nfd:
            wrapper._base = conn  # Keep the base alive
        else:
            conn.close()
        return wrapper, addr

    if not _can_dup_socket:
        def close(self):
            """Wrap close() to close the _base as well."""
            _socket.socket.close(self)
            base = getattr(self, "_base", None)
            if base is not None:
                base.close()

    def makefile(self, mode="r", buffering=None, *,
                 encoding=None, newline=None):
        """Return an I/O stream connected to the socket.

        The arguments are as for io.open() after the filename,
        except the only mode characters supported are 'r', 'w' and 'b'.
        The semantics are similar too.  (XXX refactor to share code?)
        """
        for c in mode:
            if c not in {"r", "w", "b"}:
                raise ValueError("invalid mode %r (only r, w, b allowed)")
        writing = "w" in mode
        reading = "r" in mode or not writing
        assert reading or writing
        binary = "b" in mode
        rawmode = ""
        if reading:
            rawmode += "r"
        if writing:
            rawmode += "w"
        raw = io.SocketIO(self, rawmode)
        if buffering is None:
            buffering = -1
        if buffering < 0:
            buffering = io.DEFAULT_BUFFER_SIZE
        if buffering == 0:
            if not binary:
                raise ValueError("unbuffered streams must be binary")
            raw.name = self.fileno()
            raw.mode = mode
            return raw
        if reading and writing:
            buffer = io.BufferedRWPair(raw, raw, buffering)
        elif reading:
            buffer = io.BufferedReader(raw, buffering)
        else:
            assert writing
            buffer = io.BufferedWriter(raw, buffering)
        if binary:
            buffer.name = self.fileno()
            buffer.mode = mode
            return buffer
        text = io.TextIOWrapper(buffer, encoding, newline)
        text.name = self.fileno()
        text.mode = mode
        return text


def getfqdn(name=''):
    """Get fully qualified domain name from name.

    An empty argument is interpreted as meaning the local host.

    First the hostname returned by gethostbyaddr() is checked, then
    possibly existing aliases. In case no FQDN is available, hostname
    from gethostname() is returned.
    """
    name = name.strip()
    if not name or name == '0.0.0.0':
        name = gethostname()
    try:
        hostname, aliases, ipaddrs = gethostbyaddr(name)
    except error:
        pass
    else:
        aliases.insert(0, hostname)
        for name in aliases:
            if '.' in name:
                break
        else:
            name = hostname
    return name


def create_connection(address, timeout=None):
    """Connect to address (host, port) with an optional timeout.

    Provides access to socketobject timeout for higher-level
    protocols.  Passing a timeout will set the timeout on the
    socket instance (if not present, or passed as None, the
    default global timeout setting will be used).
    """

    msg = "getaddrinfo returns an empty list"
    host, port = address
    for res in getaddrinfo(host, port, 0, SOCK_STREAM):
        af, socktype, proto, canonname, sa = res
        sock = None
        try:
            sock = socket(af, socktype, proto)
            if timeout is not None:
                sock.settimeout(timeout)
            sock.connect(sa)
            return sock

        except error as err:
            msg = err
            if sock is not None:
                sock.close()

    raise error(msg)
