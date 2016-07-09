#include <string>
#include <iostream>
#include "chasky/api/session.swig"
//#include "chasky/api/session.h"

using chasky::GraphDef;
using chasky::Status;

// Convert protobuf to string 
GraphDef Buf2Str(const std::string &buf);
// Convert string to protobuf
GraphDef Str2Buf(const std::string &buf);

bool ::session::CreateGraph(const std::string& buf) {
  GraphDef graphdef = Str2Buf(buf);
  return session_->CreateGraph(graphdef).ok();
}

bool ::session::StartExec() {
  return session_->StartExec().ok();
}

bool ::session::KillExec() {
  return session_->KillExec().ok();
}

bool ::session::DestroyGraph() {
  return session_->DestroyGraph().ok();
}

GraphDef Str2Buf(const std::string &buf) {
  GraphDef def;
  def.ParseFromString(buf);
  return def;
}
