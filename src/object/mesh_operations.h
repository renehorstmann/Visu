#ifndef VISU_OBJECT_MESH_OPERATIONS_H
#define VISU_OBJECT_MESH_OPERATIONS_H

#include <limits.h>
#include <pbase/types/cloud.h>
#include <pbase/types/meshindices.h>

// THIS IS A COPY FROM Pointc/mesh/common.c!
// (with changed namespace and as static)
static pCloud vu_mesh_expand_cloud(pCloud mesh_cloud, pMeshIndices mesh_indices) {
    pCloud out_expanded = p_cloud_new_empty(mesh_indices.size * 3);

    for (int t = 0; t < mesh_indices.size; t++) {
        for (int abc = 0; abc < 3; abc++)
            out_expanded.data[t * 3 + abc] = mesh_cloud.data[mesh_indices.data[t].v[abc]];
    }

    return out_expanded;
}


// THIS IS A COPY FROM Pointc/mesh/features.c!
// (with changed namespace)
static pCloud vu_mesh_face_normals(pCloud points, pMeshIndices indices) {
    pCloud out_normals = p_cloud_new_empty(indices.size);

    for (int t = 0; t < indices.size; t++) {
        int index_a = indices.data[t].v0;
        int index_b = indices.data[t].v1;
        int index_c = indices.data[t].v2;
        vec3 ab = vec3_sub_vec(points.data[index_b].xyz, points.data[index_a].xyz);
        vec3 ac = vec3_sub_vec(points.data[index_c].xyz, points.data[index_a].xyz);
        out_normals.data[t].xyz = vec3_cross(ab, ac);
        out_normals.data[t].xyz = vec3_normalize(out_normals.data[t].xyz);
        out_normals.data[t].w = 0;
    }

    return out_normals;
}

// THIS IS A COPY FROM Pointc/mesh/features.c!
// (with changed namespace)
static void vu_mesh_expand_point_normals(pCloud *out_points, pCloud *out_normals,
                                         pCloud mesh_points, pMeshIndices mesh_indices) {
    pCloud triangle_normals = vu_mesh_face_normals(mesh_points, mesh_indices);

    // allocate out_ clouds
    *out_points = p_cloud_new_empty(mesh_indices.size * 3);

    *out_normals = p_cloud_new_empty(mesh_indices.size * 3);

    for (int t = 0; t < mesh_indices.size; t++) {
        for (int abc = 0; abc < 3; abc++) {
            out_points->data[t * 3 + abc] = mesh_points.data[mesh_indices.data[t].v[abc]];
            out_normals->data[t * 3 + abc] = triangle_normals.data[t];
        }
    }

    p_cloud_kill(&triangle_normals);
}


#endif //VISU_OBJECT_MESH_OPERATIONS_H
