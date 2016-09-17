from attr_value_builder import AttrValueBuilder

g_batch_size = None


def NullFunc():
    return "NullFunc:11", dict()


def IdenticalFunc(dim, batch_size=None):
    global g_batch_size
    if not batch_size:
        batch_size = g_batch_size
    return "IdenticalFunc:11", dict(dim=dim, batch_size=batch_size)


def FullyConnectedFunc(dim, activation_type="sigmoid", batch_size=None):
    global g_batch_size
    if not batch_size:
        batch_size = g_batch_size
    return "FullyConnectedFunc:11", dict(dim=dim,
                                         batch_size=batch_size,
                                         activation_type=activation_type, )


def RMSElossFunc():
    return "RMSElossFunc:11", dict()
