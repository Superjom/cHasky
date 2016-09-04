import sys
sys.path.append('../../build/protos/')
from chasky.core.framework.attr_value_pb2 import AttrValue
from chasky.core.framework.types_pb2 import DataType
from error import UnImplementedError

class AttrValueBuilder(object):
    def __init__(self, attr=None):
        self.attr_value_ = attr if attr else AttrValue()

    def Value(self, x, is_dtype=False):
        if isinstance(x, int) and not is_dtype:
            self.Int64Val(x)
        elif isinstance(x, float):
            self.FloatVal(x)
        elif isinstance(x, str):
            self.StringVal(x)
        elif isinstance(x, bool):
            self.BoolVal(x)
        elif isinstance(x, int) and is_dtype:
            self.DtypeVal(x)
        elif isinstance(x, AttrValue.ListValue):
            self.ListVal(x)
        else:
            raise UnImplementedError("unsupported type with value %s" % repr(x))

    def StringVal(self, x):
        self.attr_value_.string_val = x
        return self

    def Int64Val(self, x):
        self.attr_value_.int64_val = x
        return self

    def FloatVal(self, x):
        self.attr_value_.float_val = x
        return self

    def BoolVal(self, x):
        self.attr_value_.bool_val = x
        return self

    def DtypeVal(self, x):
        self.attr_value_.dtype_val = x
        return self

    def ListVal(self, x):
        self.attr_value_.list_val = x
        return self

    def Finalize(self):
        return self.attr_value_

if __name__ == '__main__':
    attr_value = AttrValueBuilder() \
                 .StringVal("value") \
                 .Finalize()

    print attr_value


