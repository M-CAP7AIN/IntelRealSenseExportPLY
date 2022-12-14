// Edited by M-CAP7AIN sm_sJabbari
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <librealsense2/hpp/rs_export.hpp>

int main(int argc, char* argv[]) try
{
    // Declare pointcloud object, for calculating pointclouds and texture mappings
    rs2::pointcloud pc;

    // We want the points object to be persistent so we can display the last cloud when a frame drops
    rs2::points points;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;

    // Start streaming with default recommended configuration
    pipe.start();

    int counter = 10;

    std::string inputArg = argv[1];

    while (counter != 0) // Application still alive?
    {
        // Wait for the next set of frames from the camera
        auto frames = pipe.wait_for_frames();

        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Tell pointcloud object to map to this color frame
        pc.map_to(color);

        auto depth = frames.get_depth_frame();

        // Generate the pointcloud and texture mappings
        points = pc.calculate(depth);

        //points.export_to_ply("test6.ply", color);
        if (counter == 1) {
            rs2::save_to_ply exporter(inputArg, pc);
            //exporter.set_option(rs2::save_to_ply::OPTION_PLY_MESH, 1.f);
            exporter.set_option(rs2::save_to_ply::OPTION_PLY_BINARY, 0.f);
            //exporter.set_option(rs2::save_to_ply::OPTION_IGNORE_COLOR, 0.f);
            exporter.set_option(rs2::save_to_ply::OPTION_PLY_NORMALS, 1.f);
            exporter.process(frames);

            std::cout << "Exported";
        }


        counter--;
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error& e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
