/*
 * Author: Samuel Catania
 * Username: scatani9
 * Student Number: 251175283
 * Date: Wednesday, March 8, 2023
 *
 * Description of Doorbell.cpp:
 *
 */

#include <gtkmm/application.h>

#include "../include/Doorbell.hpp"
#include "../include/VideoRecorder.hpp"


Doorbell::Doorbell() {
    // Turn off OpenCV console logging output
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    camera_thread = std::thread(&Camera::detect_motion,
                                camera,
                                std::ref(recording),
                                std::ref(shared_queue),
                                std::ref(mutex_lock),
                                std::ref(recording_updated),
                                std::ref(queue_updated));

    //recorder_thread
    master_thread = std::thread(&Doorbell::thread_manager, this);

    camera_thread.join();
    master_thread.join();
}

Doorbell::~Doorbell() {
    cv::destroyAllWindows();
}

int Doorbell::open_window(int argc, char *argv[]) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    DisplayWindow display_window;

    // Shows the window and returns when it is closed
    return app->run(display_window);
}

void Doorbell::thread_manager() {
    cv::Mat frame;

    while (1) {
        // Critical Section
        std::unique_lock<std::mutex> unique_lock{mutex_lock};
        recording_updated.wait(unique_lock, [&] {
            return !recording;
        });

        std::cout << "RECORDING IS TRUEEEEE" << std::endl;

        create_video_recorder_thread();
    }
}

void Doorbell::create_video_recorder_thread() {
    VideoRecorder video_recorder(camera.get_lead_up_buffer(), camera.get_frame_rate());

    recorder_thread = std::thread(&VideoRecorder::write_frames,
                                video_recorder,
                                std::ref(recording),
                                std::ref(shared_queue),
                                std::ref(mutex_lock),
                                std::ref(recording_updated),
                                std::ref(queue_updated));

    recorder_thread.join();
}
