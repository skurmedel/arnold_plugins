#include <string_view>

#include "tinyexpr/tinyexpr.h"

#include <ai.h>

namespace ParamNames {
constexpr char const *Expression = "expr";
constexpr char const *DomainMin = "min";
constexpr char const *DomainMax = "max";
} // namespace ParamNames

struct ExprImplicitNodeData {
    AtString expr;
    AtVector domain_min;
    AtVector domain_max;
};

auto tryCompile(AtString const &expr, double &x, double &y, double &z)
    -> te_expr * {
    te_variable vars[] = {{"x", &x}, {"y", &y}, {"z", &z}};
    int error = 0;
    te_expr *exprObj = te_compile(expr.c_str(), vars, 3, &error);

    if (!exprObj) {
        AiMsgFatal("Compilation of expression failed. Code %d.", error);
    }

    return exprObj;
}

AI_VOLUME_NODE_EXPORT_METHODS(ExprImplicitNodeMtd);

node_parameters {
    AiParameterStr(ParamNames::Expression, "x^2 + y^2 + z^2 - 1");
    // AiParameterVec(ParamNames::DomainMin, -2, -2, -2);
    // AiParameterVec(ParamNames::DomainMax, 2, 2, 2);
}

volume_create {
    ExprImplicitNodeData *nodeData = new ExprImplicitNodeData;
    if (nodeData == nullptr) {
        AiMsgFatal("Could not allocate node data structure. Buy more RAM.");
        return false;
    }

    nodeData->expr = AiNodeGetStr(node, ParamNames::Expression);
    nodeData->domain_min = AiNodeGetVec(node, ParamNames::DomainMin);
    nodeData->domain_max = AiNodeGetVec(node, ParamNames::DomainMax);

    double x, y, z;
    te_free(tryCompile(nodeData->expr, x, y, z));

    data->bbox = AtBBox(nodeData->domain_min, nodeData->domain_max);
    data->auto_step_size = 0.01f;
    data->private_info = nodeData;

    AiMsgInfo("so_expr_implicit: Created node data, expression is: %s",
              nodeData->expr.c_str());

    return true;
}

volume_update { return true; }

volume_cleanup {
    ExprImplicitNodeData *nodeData =
        static_cast<ExprImplicitNodeData *>(data->private_info);

    delete nodeData;

    return true;
}

volume_ray_extents {
    /* Let Arnold look along the whole ray. */
    AiVolumeAddIntersection(info, t0, t1);
}

volume_sample {
    ExprImplicitNodeData *nodeData =
        static_cast<ExprImplicitNodeData *>(data->private_info);
    if (channel != AtString("field"))
        return false;

    double x = sg->P.x, y = sg->P.y, z = sg->P.z;
    te_expr *exprObj = tryCompile(nodeData->expr, x, y, z);

    value->FLT() = te_eval(exprObj);
    *type = AI_TYPE_FLOAT;

    te_free(exprObj);

    return true;
}

volume_gradient {
    return false; // Let Arnold figure out the gradient by central differencing
                  // if it needs to.
}

node_loader {
    if (i > 0)
        return false;

    node->methods = ExprImplicitNodeMtd;
    node->name = "so_expr_implicit";
    node->output_type = AI_TYPE_FLOAT;
    node->node_type = AI_NODE_SHAPE_IMPLICIT;
    strcpy(node->version, AI_VERSION);
    return true;
}
