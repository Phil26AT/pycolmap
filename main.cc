#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>

namespace py = pybind11;

#include "absolute_pose.cc"
#include "generalized_absolute_pose.cc"
#include "essential_matrix.cc"
#include "fundamental_matrix.cc"
#include "transformations.cc"
#include "sift.cc"
#include "pose_refinement.cc"

#include "reconstruction.cc"

void init_reconstruction(py::module &);

PYBIND11_MODULE(pycolmap, m) {
    m.doc() = "COLMAP plugin";

    // Absolute pose.
    m.def("absolute_pose_estimation", 
            &absolute_pose_estimation,
            py::arg("points2D"), py::arg("points3D"),
            py::arg("camera"),
            py::arg("max_error_px") = 12.0,
            "Absolute pose estimation with non-linear refinement.");
    
    // Absolute pose with camera dict.
    m.def("absolute_pose_estimation", 
            &absolute_pose_estimation_camera_dict,
            py::arg("points2D"), py::arg("points3D"),
            py::arg("camera_dict"),
            py::arg("max_error_px") = 12.0,
            "Absolute pose estimation with non-linear refinement.");

    m.def("rig_absolute_pose_estimation", &rig_absolute_pose_estimation,
          py::arg("points2D"), py::arg("points3D"),
          py::arg("cameras"),
          py::arg("rig_qvecs"), py::arg("rig_tvecs"),
          py::arg("max_error_px") = 12.0,
          "Absolute pose estimation of a multi-camera rig.");

    m.def("rig_absolute_pose_estimation", &rig_absolute_pose_estimation_camera_dicts,
          py::arg("points2D"), py::arg("points3D"),
          py::arg("camera_dicts"),
          py::arg("rig_qvecs"), py::arg("rig_tvecs"),
          py::arg("max_error_px") = 12.0,
          "Absolute pose estimation of a multi-camera rig.");

    // Essential matrix.
    m.def("essential_matrix_estimation", 
          &essential_matrix_estimation,
          py::arg("points2D1"), py::arg("points2D2"),
          py::arg("camera1"), py::arg("camera2"),
          py::arg("max_error_px") = 4.0,
          "LORANSAC + 5-point algorithm.");

    // Essential matrix with camera dict.
    m.def("essential_matrix_estimation", 
          &essential_matrix_estimation_camera_dict,
          py::arg("points2D1"), py::arg("points2D2"),
          py::arg("camera_dict1"), py::arg("camera_dict2"),
          py::arg("max_error_px") = 4.0,
          "LORANSAC + 5-point algorithm.");

    // Fundamental matrix.
    m.def("fundamental_matrix_estimation", 
          &fundamental_matrix_estimation,
          py::arg("points2D1"), py::arg("points2D2"),
          py::arg("max_error_px") = 4.0,
          "LORANSAC + 7-point algorithm.");

    // Image-to-world and world-to-image.
    m.def("image_to_world", &image_to_world, "Image to world transformation.");
    m.def("world_to_image", &world_to_image, "World to image transformation.");

    // SIFT.
    m.def("extract_sift", &extract_sift,
          py::arg("image"),
          py::arg("num_octaves") = 4, py::arg("octave_resolution") = 3, py::arg("first_octave") = 0,
          py::arg("edge_thresh") = 10.0, py::arg("peak_thresh") = 0.01, py::arg("upright") = false,
          "Extract SIFT features.");

    // Standalone Pose Refinement
    m.def("pose_refinement", &pose_refinement, 
          py::arg("tvec"), py::arg("qvec"),
          py::arg("points2D"), py::arg("points3D"),
          py::arg("inlier_mask"),
          py::arg("camera"),
          "Non-linear refinement.");

    // Standalone Pose Refinement
    m.def("pose_refinement", &pose_refinement_camera_dict, 
          py::arg("tvec"), py::arg("qvec"),
          py::arg("points2D"), py::arg("points3D"),
          py::arg("inlier_mask"),
          py::arg("camera_dict"),
          "Non-linear refinement.");

    //Reconstruction bindings
    init_reconstruction(m);
}
