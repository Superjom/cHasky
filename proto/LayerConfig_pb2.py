# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: LayerConfig.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='LayerConfig.proto',
  package='cHasky',
  serialized_pb=_b('\n\x11LayerConfig.proto\x12\x06\x63Hasky\"l\n\x0e\x41\x63tivateConfig\x12\x30\n\x04type\x18\x01 \x02(\x0e\x32\x1b.cHasky.ActivateConfig.Type:\x05\x45QUAL\"(\n\x04Type\x12\t\n\x05\x45QUAL\x10\x00\x12\x0b\n\x07SIGMOID\x10\x01\x12\x08\n\x04TANH\x10\x02\"L\n\x0bLayerConfig\x12\x0c\n\x04name\x18\x01 \x02(\t\x12/\n\x0f\x61\x63tivate_config\x18\x02 \x02(\x0b\x32\x16.cHasky.ActivateConfig')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)



_ACTIVATECONFIG_TYPE = _descriptor.EnumDescriptor(
  name='Type',
  full_name='cHasky.ActivateConfig.Type',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='EQUAL', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='SIGMOID', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TANH', index=2, number=2,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=97,
  serialized_end=137,
)
_sym_db.RegisterEnumDescriptor(_ACTIVATECONFIG_TYPE)


_ACTIVATECONFIG = _descriptor.Descriptor(
  name='ActivateConfig',
  full_name='cHasky.ActivateConfig',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='type', full_name='cHasky.ActivateConfig.type', index=0,
      number=1, type=14, cpp_type=8, label=2,
      has_default_value=True, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _ACTIVATECONFIG_TYPE,
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=29,
  serialized_end=137,
)


_LAYERCONFIG = _descriptor.Descriptor(
  name='LayerConfig',
  full_name='cHasky.LayerConfig',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='name', full_name='cHasky.LayerConfig.name', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='activate_config', full_name='cHasky.LayerConfig.activate_config', index=1,
      number=2, type=11, cpp_type=10, label=2,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=139,
  serialized_end=215,
)

_ACTIVATECONFIG.fields_by_name['type'].enum_type = _ACTIVATECONFIG_TYPE
_ACTIVATECONFIG_TYPE.containing_type = _ACTIVATECONFIG
_LAYERCONFIG.fields_by_name['activate_config'].message_type = _ACTIVATECONFIG
DESCRIPTOR.message_types_by_name['ActivateConfig'] = _ACTIVATECONFIG
DESCRIPTOR.message_types_by_name['LayerConfig'] = _LAYERCONFIG

ActivateConfig = _reflection.GeneratedProtocolMessageType('ActivateConfig', (_message.Message,), dict(
  DESCRIPTOR = _ACTIVATECONFIG,
  __module__ = 'LayerConfig_pb2'
  # @@protoc_insertion_point(class_scope:cHasky.ActivateConfig)
  ))
_sym_db.RegisterMessage(ActivateConfig)

LayerConfig = _reflection.GeneratedProtocolMessageType('LayerConfig', (_message.Message,), dict(
  DESCRIPTOR = _LAYERCONFIG,
  __module__ = 'LayerConfig_pb2'
  # @@protoc_insertion_point(class_scope:cHasky.LayerConfig)
  ))
_sym_db.RegisterMessage(LayerConfig)


# @@protoc_insertion_point(module_scope)
