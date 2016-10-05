#include "chasky/core/framework/graph.h"

namespace chasky {

Graph::Graph(GraphDef &def, PostBox *postbox)
    : data_provider_(
          DataProvider::Create(def.data_provider(), postbox, &edge_lib_)),
      node_lib_(postbox, &edge_lib_), postbox_(postbox), def_(def) {
  DLOG(INFO) << "graph's definition is " << def.DebugString();
  CH_CHECK_OK(CreateNodes());
  CH_CHECK_OK(ConnectNodes());
}

Status Graph::Compute(std::vector<ArgumentDef> &inputs) {
  Status status;
  CH_CHECK_OK(data_provider_->Fill(inputs));
  for (auto task_type :
       std::vector<TaskType>{TaskType::FORWARD, TaskType::BACKWARD}) {
    auto &nodes_cpt_order = task_type == TaskType::FORWARD
                                ? nodes_fwd_cpt_order_
                                : nodes_bwd_cpt_order_;
    // determine nodes computation order in the first pass
    if (nodes_cpt_order.empty()) {
      std::unordered_set<std::string> computed_nodes;

      for (int i = 0; i < node_lib_.Size(); i++) {
        for (auto &n : node_lib_.Nodes()) {
          auto &node = *n.second;
          if (computed_nodes.count(node.Name().tostring()) != 0)
            continue;
          auto status = node.Compute(task_type);
          DLOG(INFO) << "Node " << n.second->Name() << " compute "
                     << status.ok();
          if (status.ok()) {
            computed_nodes.emplace(node.Name().tostring());
            nodes_cpt_order.emplace_back(node.Name().tostring());
          }
        }
      }

      CHECK_EQ(nodes_cpt_order.size(), node_lib_.Size())
          << "some node compute failed";

    } else {
      for (const auto &node_name : nodes_cpt_order) {
        Node *node;
        CH_CHECK_OK(node_lib_.Retrive(node_name, &node));
        CH_CHECK_OK(node->Compute(task_type));
      }
    }
  }
  return status;
}

Status Graph::CreateNodes() {
  Status status;
  LOG(INFO) << "Creating Nodes ...";
  CHECK(node_lib_.IsEmpty()) << "duplicate creating nodes";
  LOG(INFO) << "node.size: " << def_.nodes().size();
  for (const auto &node_def : def_.nodes()) {
    DLOG(INFO) << strings::Printf("create node: %s[%s]",
                                  node_def.name().c_str(),
                                  node_def.signature().c_str());
    CH_CHECK_OK(node_lib_.RegisterCreate(node_def.name(), node_def));
  }
  DLOG(INFO) << "finish create nodes ...";
  return status;
}

Status Graph::ConnectNodes() {
  Status status;
  DLOG(INFO) << "begin to connect nodes ...";
  CH_STEST_RETURN2(!node_lib_.IsEmpty(), error::UNKNOWN,
                   "no nodes are found in graph, create edges failed.");
  CHECK(edge_lib_.IsEmpty()) << "duplicate create edges";

  for (auto &e : *def_.mutable_edges()) {
    auto sign = EdgeLib::CreateKey(e);
    DLOG(INFO) << "connect " << sign;
    e.set_signature_(sign);
    CH_CHECK_OK(edge_lib_.Register(sign));

    // reverse edge
    auto reverse_e = e;
    auto src_arg_key = PostBox::CreateArgKey(reverse_e.trg_node(),
                                             reverse_e.trg_arg(), BACKWARD);
    auto trg_arg_key = PostBox::CreateArgKey(reverse_e.src_node(),
                                             reverse_e.src_arg(), BACKWARD);
    auto reverse_key = EdgeLib::CreateKey(src_arg_key, trg_arg_key);
    CH_CHECK_OK(edge_lib_.Register(reverse_key));
  }

  DLOG(INFO) << "finish connecting nodes ...";
  return status;
}

// void Graph::ServiceThreadJoin() {
//   if (service_thread_ && service_thread_->joinable()) {
//     service_thread_->join();
//   }
// }

// Graph::~Graph() {
//   DLOG(INFO) << "graph desc";

//   ServiceThreadJoin();
// }

} // namespace chasky
