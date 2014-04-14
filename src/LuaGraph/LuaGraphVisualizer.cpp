#include "LuaGraph/LuaGraphVisualizer.h"
#include "LuaGraph/LuaGraph.h"
#include "Importer/GraphOperations.h"

Lua::LuaGraphVisualizer::LuaGraphVisualizer(Data::Graph *graph)
{
    currentGraph = graph;
    Importer::GraphOperations *operations = new Importer::GraphOperations(*currentGraph);
    operations->addDefaultTypes(edgeType, nodeType);
}

void Lua::LuaGraphVisualizer::visualize(bool incidence_as_node)
{
    Lua::LuaGraph *g = Lua::LuaGraph::loadGraph();
    g->printGraph();

    for (QMap<qlonglong, Lua::LuaNode *>::iterator i = g->getNodes()->begin(); i != g->getNodes()->end(); ++i){
         osg::ref_ptr<Data::Node> n = currentGraph->addNode(i.key() , i.value()->getLabel(), nodeType);
         setNodeParams(n, i.value());
    }

    for (QMap<qlonglong, Lua::LuaEdge *>::iterator i = g->getEdges()->begin(); i != g->getEdges()->end(); ++i){
         osg::ref_ptr<Data::Node> n = currentGraph->addNode(i.key() , i.value()->getLabel(), nodeType);
         setNodeParams(n, i.value());
    }

    if (incidence_as_node){
        for (QMap<qlonglong, Lua::LuaIncidence *>::iterator i = g->getIncidences()->begin(); i != g->getIncidences()->end(); ++i){
            osg::ref_ptr<Data::Node> incNode = currentGraph->addNode(i.key(), i.value()->getLabel(), nodeType);
            setNodeParams(incNode, i.value());

            osg::ref_ptr<Data::Node> srcNode = currentGraph->getNodes()->value(i.value()->getEdgeNodePair().first);
            osg::ref_ptr<Data::Node> dstNode = currentGraph->getNodes()->value(i.value()->getEdgeNodePair().second);
            if (i.value()->getOriented()){
                if (i.value()->getOutGoing()){
                    currentGraph->addEdge(i.value()->getLabel(), srcNode, incNode, edgeType, true);
                    currentGraph->addEdge(i.value()->getLabel(), incNode, dstNode, edgeType, true);
                } else {
                    currentGraph->addEdge(i.value()->getLabel(), dstNode, incNode, edgeType, true);
                    currentGraph->addEdge(i.value()->getLabel(), incNode, srcNode, edgeType, true);
                }
            } else {
                currentGraph->addEdge(i.value()->getLabel(), srcNode, incNode, edgeType, false);
                currentGraph->addEdge(i.value()->getLabel(), incNode, dstNode, edgeType, false);
            }
        }
    } else {
        for (QMap<qlonglong, Lua::LuaIncidence *>::iterator i = g->getIncidences()->begin(); i != g->getIncidences()->end(); ++i){
            osg::ref_ptr<Data::Node> srcNode = currentGraph->getNodes()->value(i.value()->getEdgeNodePair().first);
            osg::ref_ptr<Data::Node> dstNode = currentGraph->getNodes()->value(i.value()->getEdgeNodePair().second);
            if (i.value()->getOriented()){
                if (i.value()->getOutGoing())
                    currentGraph->addEdge(i.key(), i.value()->getLabel(), srcNode, dstNode, edgeType, true);
                else
                    currentGraph->addEdge(i.key(), i.value()->getLabel(), dstNode, srcNode, edgeType, true);
            } else
                currentGraph->addEdge(i.key(), i.value()->getLabel(), srcNode, dstNode, edgeType, false);
        }
    }
    g->setObserver(this);
}

void Lua::LuaGraphVisualizer::setNodeParams(osg::ref_ptr<Data::Node> node, Lua::LuaGraphObject *obj){
    float r = obj->getFloatParam("colorR", 1);
    float g = obj->getFloatParam("colorG", 1);
    float b = obj->getFloatParam("colorB", 1);
    float a = obj->getFloatParam("colorA", 1);
    node.get()->setColor(osg::Vec4 (r,g,b,a));
    float scale = obj->getFloatParam("size", 8);
    node->setScale(scale);
    node->reloadConfig();
}

void Lua::LuaGraphVisualizer::onUpdate()
{
    std::cout << "Graph update called" << std::endl;

    Lua::LuaGraph *g = Lua::LuaGraph::loadGraph();
    g->printGraph();
//    for (QMap<qlonglong, Lua::LuaNode *>::iterator i = g->getNodes()->begin(); i!= g->getNodes()->end(); i++){
//        if (!currentGraph->getNodes()->contains(i.key())){
//            cout << "Adding node " << i.key() << std::endl;
//            osg::ref_ptr<Data::Node> n = currentGraph->addNode(i.key() , i.value()->getLabel(), nodeType);
//            n.get()->setColor(osg::Vec4 (1,0,0,1));
//            n.get()->reloadConfig();
//        }
//    }
    for (QMap<qlonglong, osg::ref_ptr<Data::Edge> >::iterator i = currentGraph->getEdges()->begin(); i != currentGraph->getEdges()->end(); i++){
        if (!g->getIncidences()->contains(i.key())){
            std::cout << "Removing edge " << i.key() << std::endl;
            currentGraph->removeEdge(i.value());
        }
    }
//    for (QMap<qlonglong, osg::ref_ptr<Data::Node> >::iterator i = currentGraph->getNodes()->begin(); i != currentGraph->getNodes()->end(); i++){
//        if (!g->getNodes()->contains(i.key()) && !g->getEdges()->contains(i.key()) && !g->getIncidences()->contains(i.key())){
//            cout << "Removing node " << i.key() << std::endl;
//            currentGraph->removeNode(i.value());
//        }
//    }
//    osg::ref_ptr<Data::Node> n = (*currentGraph->getNodes())[8];
//    n.get()->setColor(osg::Vec4(1,1,1,1));
//    currentGraph->removeNode(n);
//    n->releaseGLObjects();
}
