# cHasky
Project cHasky is a tiny implementation of Computation Graph. It will include some basic models(layers) of Artifical Neural Networks.

To make it funny and different, we will forcus on data visualization for NNs.

## DNN demo

Project cHasky supports python languages to write graph configuration.

For example, to build a simple Neural Network, configurations like

    from chasky import functions
    import chasky.chasky_engine as engine
    from chasky.config_helper import Node, Edge, \
        CreateGraph, DestroyGraph, GraphStringBuffer, GraphDebugString, DataProvider, \
        Field

    batch_size = 30

    # create a demo graph
    CreateGraph("demo_graph")

    data_provider = DataProvider("data_provider",
                  './1.txt',
                  batch_size,
                  Field("feature", "float_mat",
                        dim=30, doc="first feature"),
                  Field("label", "float_mat",
                        dim=1, doc="prediction label"))
    Node("layer1", functions.FullyConnectedFunc(50, batch_size))
    Node("layer2", functions.FullyConnectedFunc(1, batch_size))
    Node("loss_layer", functions.RMSElossFunc(), is_end=True)
    Edge("data_provider", "feature", "layer1", "input")
    Edge("layer1", "output", "layer2", "input")
    Edge("layer2", "output", "loss_layer", "target")
    Edge("data_provider", "label", "loss_layer", "label")

    # init session
    session = engine.Session()
    session.CreateGraph(GraphStringBuffer())
    session.StartExec()

    # start training
    for batch in data_provider.batchs():
        session.Compute(batch)


In addition to python language, following C++ APIs are available:

    cHasky/chasky/core/framework/argument_def_builder.h
    cHasky/chasky/core/framework/data_provider_def_builder.h
    cHasky/chasky/core/framework/edge_def_builder.h
    cHasky/chasky/core/framework/function_def_builder.h
    cHasky/chasky/core/framework/graph_def_builder.h
    cHasky/chasky/core/framework/node_def_builder.h

and supports syntax like:

    auto graph_builder = GraphDefBuilder().Name("demo_graph");

    auto data_provider_def = DataProviderDefBuilder()
                                 .Name("data-provider")
                                 .Field(ArgumentDefBuilder()
                                            .Name("id")
                                            .Type("float_mat")
                                            .Shape(dim, batch_size)
                                            .Finalize())
                                 .Finalize();

    auto node_def_1 =
        NodeDefBuilder()
            .Name("node-1")
            .Signature("IdenticalFunc:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();
    auto node_def_2 =
        NodeDefBuilder()
            .Name("node-2")
            .Signature("IdenticalFunc:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();
    auto node_def_3 =
        NodeDefBuilder()
            .Name("node-3")
            .SetIsEnd(true)
            .Signature("IdenticalFunc:11")
            .Attr("dim", AttrValueBuilder().Value(dim).Finalize())
            .Attr("batch_size", AttrValueBuilder().Value(batch_size).Finalize())
            .Finalize();

    auto edge_def_1 = EdgeDefBuilder()
                          .SrcNode("node-1")
                          .SrcArg("output")
                          .TrgNode("node-2")
                          .TrgArg("input")
                          .Finalize();

    auto edge_def_2 = EdgeDefBuilder()
                          .SrcNode("node-2")
                          .SrcArg("output")
                          .TrgNode("node-3")
                          .TrgArg("input")
                          .Finalize();

    auto edge_def_3 = EdgeDefBuilder()
                          .SrcNode("data-provider")
                          .SrcArg("id")
                          .TrgNode("node-1")
                          .TrgArg("input")
                          .Finalize();

    graph_def = graph_builder.AddNode(node_def_1)
                    .SetDataProvider(data_provider_def)
                    .AddNode(node_def_2)
                    .AddNode(node_def_3)
                    .AddEdge(edge_def_1)
                    .AddEdge(edge_def_2)
                    .AddEdge(edge_def_3)
                    .Finalize();

# NOTE

This project is under development, and no release is ready now.

