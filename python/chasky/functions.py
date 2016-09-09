from attr_value_builder import AttrValueBuilder

g_batch_size = None


def NullFunc():
    return "NullFunc:11", dict()


def IdenticalFunc(dim, batch_size=None):
    global g_batch_size
    if not batch_size:
        batch_size = g_batch_size
    dim_attr = AttrValueBuilder().Value(dim).Finalize()
    batch_size_attr = AttrValueBuilder().Value(batch_size).Finalize()
    return "IdenticalFunc", dict(dim=dim_attr, batch_size=batch_size_attr)
