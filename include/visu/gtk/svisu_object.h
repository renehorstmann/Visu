#ifndef VISU_GTK_SVISU_OBJECT_H
#define VISU_GTK_SVISU_OBJECT_H



#include "svisu_base.h"
#include "visu/object/points.h"
#include "visu/object/lines.h"
#include "visu/object/mesh.h"


//
// Points
//

/**
 * Shows a point cloud with single color.
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuPoints *svisu_points(SVisu *self, pCloud points, vec4 color, float point_size);


/**
 * Shows a point cloud with a color for each point.
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuPointsColored *svisu_points_colored(SVisu *self, pCloud points, pCloud colors, float point_size);


/**
 * Shows a point cloud rendered with normals and a single color
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuPointNormals *svisu_point_normals(SVisu *self, pCloud points, pCloud normals,
                                    vec4 color, float point_size);


/**
 * Shows a point cloud rendered with normals and with a color for each point
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuPointNormalsColored *svisu_point_normals_colored(SVisu *self, pCloud points, pCloud normals,
                                                   pCloud colors, float point_size);




//
// Lines
//

/**
 * Shows lines with single color
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuLines *svisu_lines(SVisu *self, pCloud points, vec4 color, float line_size, enum vu_linemode line_mode);

/**
 * Shows lines with colors for each point
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuLinesColored *svisu_lines_colored(SVisu *self, pCloud points, pCloud colors,
                                    float line_size, enum vu_linemode line_mode);




//
// Mesh
//

/**
 * Shows a mesh with single color
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuMesh *svisu_mesh(SVisu *self, pCloud points, pMeshIndices indices, vec4 color);


/**
 * Shows a mesh with color for each point
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuMeshColored *svisu_mesh_colored(SVisu *self, pCloud points, pCloud colors, pMeshIndices indices);


/**
 * Shows a mesh with color for each point, without normal shining
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuMeshColoredMatt *svisu_mesh_colored_matt(SVisu *self, pCloud points, pCloud colors, pMeshIndices indices);


/**
 * Shows a mesh with expanded point normals and single color
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuRawMesh *svisu_raw_mesh(SVisu *self, pCloud points, pCloud normals, vec4 color);


/**
 * Shows a mesh with expanded point normal colors
 * @return: The reference of the render object (do not call vu_RenderObject_delete on it, its controlled by Visu)
 */
VuRawMeshColored *svisu_raw_mesh_colored(SVisu *self, pCloud points, pCloud normals, pCloud colors);



#endif //VISU_GTK_SVISU_OBJECT_H
